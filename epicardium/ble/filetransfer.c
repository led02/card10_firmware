/*
 * BLE (Bluetooth Low energy) file transfer service.
 *
 * This file provides a BLE service and a characteristic which allows to 
 * write the content of the mytest.py file over BLE into the file system. 
 * We haven't found any existing BLE service which provides such a 
 * functionality so we implemented our own service.

 * The service uses the UUID in fileTransSvc
 * The characteristic uses the UUID in attTxChConfigUuid

 * BLE point to point links use a pretty small MTU (min 23 bytes, max 244
 * bytes) which is negotiated between the central and the peripheral (card10).
 * 
 * The first byte of each message is the type of message being send,
 * different types are supported and based on this type the next bytes
 * have different meaning.
 * 
 * TODOs:
 * * File deletion
 * * File read
 * * Directory creation
 * * Directory listing
 */

#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "fit/fit_api.h"
#include "hci_vs.h"

#include <epicardium.h>
#include "modules/log.h"

#include "util/bstream.h"
#include "att_api.h"

#include "FreeRTOS.h"
#include "crc32.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <machine/endian.h>

/*!< \brief Service start handle. */
#define FILE_TRANS_START_HDL 0x820
/*!< \brief Service end handle. */
#define FILE_TRANS_END_HDL (FILE_TRANS_MAX_HDL - 1)

#define FILE_TRANS_UUID_SUFFIX                                                 \
	0x42, 0x23, 0x42, 0x23, 0x42, 0x23, 0x42, 0x23, 0x42, 0x23, 0x42, 0x23
#define FILE_TRANS_UUID_PREFIX 0x01, 0x23, 0x42

/*
 * This has to match in order and number of members to the functions
 * called in fileTransAddGroupDyn() otherwise the stack breaks.
 */
enum {
	/*!< \brief File transfer service declaration */
	FILE_TRANS_SVC_HDL = FILE_TRANS_START_HDL,
	/*!< \brief File transfer Central tx characteristic */
	FILE_TRANS_CENTRAL_TX_CH_HDL,
	/*!< \brief File transfer Central tx value */
	FILE_TRANS_CENTRAL_TX_VAL_HDL,
	/*!< \brief File transfer Central rx characteristic */
	FILE_TRANS_CENTRAL_RX_CH_HDL,
	/*!< \brief File transfer Central rx value */
	FILE_TRANS_CENTRAL_RX_VAL_HDL,
	/*!< \brief File transfer Central rx notification characteristic */
	FILE_TRANS_CENTRAL_RX_CCC_HDL,
	/*!< \brief Maximum handle. */
	FILE_TRANS_MAX_HDL
};

/* BLE File transfer Service UUID */
static const uint8_t fileTransSvc[]    = { FILE_TRANS_UUID_SUFFIX,
                                        0x00,
                                        FILE_TRANS_UUID_PREFIX };
static const uint16_t fileTransSvc_len = sizeof(fileTransSvc);

/* BLE File transfer Central TX configuration */
static const uint8_t txChConfig[]    = { ATT_PROP_WRITE_NO_RSP,
                                      UINT16_TO_BYTES(
                                              FILE_TRANS_CENTRAL_TX_VAL_HDL),
                                      FILE_TRANS_UUID_SUFFIX,
                                      0x01,
                                      FILE_TRANS_UUID_PREFIX };
static const uint16_t txChConfig_len = sizeof(txChConfig);

/* BLE File transfer Central TX UUID */
static const uint8_t attTxChConfigUuid[] = { FILE_TRANS_UUID_SUFFIX,
					     0x01,
					     FILE_TRANS_UUID_PREFIX };

/* BLE File transfer Central RX configuration */
static const uint8_t rxChConfig[]    = { ATT_PROP_READ | ATT_PROP_NOTIFY,
                                      UINT16_TO_BYTES(
                                              FILE_TRANS_CENTRAL_RX_VAL_HDL),
                                      FILE_TRANS_UUID_SUFFIX,
                                      0x02,
                                      FILE_TRANS_UUID_PREFIX };
static const uint16_t rxChConfig_len = sizeof(rxChConfig);

/* BLE File transfer Central RX UUID */
static const uint8_t attRxChConfigUuid[] = { FILE_TRANS_UUID_SUFFIX,
					     0x02,
					     FILE_TRANS_UUID_PREFIX };
static uint8_t attRxChConfigValue[64];
static uint16_t attRxChConfigValue_len = 0;

/* File descriptor of the currently transferred file */
static int file_fd = -1;

/* Attribute list for uriCfg group */
static const attsAttr_t fileTransCfgList[] = {
	/* Service declaration */
	{
		.pUuid       = attPrimSvcUuid,
		.pValue      = (uint8_t *)fileTransSvc,
		.pLen        = (uint16_t *)&fileTransSvc_len,
		.maxLen      = sizeof(fileTransSvc),
		.settings    = 0,
		.permissions = ATTS_PERMIT_READ,
	},
	/* File transfer Central TX characteristic */
	{
		.pUuid       = attChUuid,
		.pValue      = (uint8_t *)txChConfig,
		.pLen        = (uint16_t *)&txChConfig_len,
		.maxLen      = sizeof(txChConfig),
		.settings    = 0,
		.permissions = ATTS_PERMIT_READ,
	},
	/* File transfer Central TX, this contains information about the real data */
	{
		.pUuid       = attTxChConfigUuid,
		.pValue      = NULL,
		.pLen        = NULL,
		.maxLen      = 128,
		.settings    = ATTS_SET_WRITE_CBACK | ATTS_SET_VARIABLE_LEN,
		.permissions = ATTS_PERMIT_WRITE,
	},
	/* File transfer Central RX characteristic */
	{
		.pUuid       = attChUuid,
		.pValue      = (uint8_t *)rxChConfig,
		.pLen        = (uint16_t *)&rxChConfig_len,
		.maxLen      = sizeof(rxChConfig),
		.settings    = 0,
		.permissions = ATTS_PERMIT_READ,
	},
	/* File transfer Central RX, this contains information about the real data */
	{
		.pUuid       = attRxChConfigUuid,
		.pValue      = attRxChConfigValue,
		.pLen        = &attRxChConfigValue_len,
		.maxLen      = sizeof(attRxChConfigValue),
		.settings    = ATTS_SET_VARIABLE_LEN,
		.permissions = ATTS_PERMIT_READ,
	},
	/* File transfer Central RX notification channel */
	{
		.pUuid       = attCliChCfgUuid,
		.pValue      = attRxChConfigValue,
		.pLen        = &attRxChConfigValue_len,
		.maxLen      = sizeof(attRxChConfigValue),
		.settings    = ATTS_SET_CCC,
		.permissions = ATTS_PERMIT_READ | ATTS_PERMIT_WRITE,
	},
};

/**
 * Send a repose with an optional CRC.
 * 
 * The Central RX characteristic is filled and a notification is send in addition.
 * If msg is given message is added in addition.
 */
static void sendCrcResponse(
	dmConnId_t connId,
	char code,
	uint16_t crcLen,
	uint8_t *crcData,
	const char *msg
) {
	uint32_t crc;
	uint16_t len = 1;
	uint8_t answer[24];

	answer[0] = code;

	if (crcData) {
		crc = __htonl(CalcCrc32(0xffffffff, crcLen, crcData));
		memcpy(answer + len, &crc, sizeof(crc));
		len += 4;
	}

	if (msg) {
		if (strlen(msg) < sizeof(answer) - len) {
			strncpy((char *)answer + len,
				msg,
				sizeof(answer) - len);
			len += strlen(msg);
			LOG_ERR("filetrans", "%s\n", msg);
		} else {
			LOG_ERR("filetrans",
				"error message \"%s\" too long\n",
				msg);
		}
	}

	AttsSetAttr(FILE_TRANS_CENTRAL_RX_VAL_HDL, len, answer);
	AttsHandleValueNtf(connId, FILE_TRANS_CENTRAL_RX_VAL_HDL, len, answer);
}

/*
 * This function splits the path into the folders and the file name and 
 * creates all the missing folders.
 */
static int bleFileCreateOrOpen(char *filepath)
{
	char *pathEnd;
	int pos = 0;
	int ret;

	while (true) {
		pathEnd = strchr(filepath + pos, '/');
		if (!pathEnd)
			return epic_file_open(filepath, "w");

		pathEnd[0] = '\00';
		pos        = pathEnd - filepath + 1;

		if (strlen(filepath)) {
			ret = epic_file_stat(filepath, NULL);
			if (ret == -ENOENT) {
				ret = epic_file_mkdir(filepath);
				if (ret) {
					LOG_ERR("filetrans",
						"mkdir failed: %s, ret: %i\n",
						filepath,
						ret);
					return ret;
				}
			}
		}
		pathEnd[0] = '/';
	}
}

static uint8_t bleFileOpen(dmConnId_t connId, uint8_t *pValue, uint16_t len)
{
	char filepath[100];

	if (len > sizeof(filepath)) {
		sendCrcResponse(connId, 'e', 0, NULL, "path too long");
		return ATT_ERR_RESOURCES;
	}

	/* Copy only file path and not type, make sure this is NULL terminated */
	strncpy(filepath, (char *)pValue + 1, len - 1);
	filepath[len - 1] = 0;

	if (file_fd != -1)
		epic_file_close(file_fd);

	file_fd = bleFileCreateOrOpen(filepath);
	if (file_fd < 0) {
		sendCrcResponse(connId, 'e', 0, NULL, "open failed");
		return ATT_ERR_RESOURCES;
	}
	sendCrcResponse(connId, 'S', len, pValue, NULL);
	return ATT_SUCCESS;
}

static uint8_t bleFileWrite(dmConnId_t connId, uint8_t *pValue, uint16_t len)
{
	uint32_t fileOffsetNet;
	uint32_t fileOffset;
	int res;

	if (len < 6) {
		sendCrcResponse(connId, 'e', 0, NULL, "msg too short");
		return ATT_ERR_LENGTH;
	}

	memcpy(&fileOffsetNet, pValue + 1, sizeof(fileOffsetNet));
	fileOffset = __ntohl(fileOffsetNet);

	res = epic_file_seek(file_fd, fileOffset, SEEK_SET);
	if (res) {
		sendCrcResponse(connId, 'e', 0, NULL, "seek failed");
		return ATT_ERR_RESOURCES;
	}

	res = epic_file_write(file_fd, pValue + 5, len - 5);
	if (res != (len - 5)) {
		sendCrcResponse(connId, 'e', 0, NULL, "write failed");
		return ATT_ERR_RESOURCES;
	}

	sendCrcResponse(connId, 'C', len, pValue, NULL);
	return ATT_SUCCESS;
}

/*
 * Handle the Central TX characteristic which is used by the central to
 * issues file transfer commands on this peripheral.
 */
static uint8_t handleCentralTX(
	dmConnId_t connId,
	uint16_t handle,
	uint8_t operation,
	uint16_t offset,
	uint16_t len,
	uint8_t *pValue,
	attsAttr_t *pAttr
) {
	if (len < 1) {
		sendCrcResponse(connId, 'e', 0, NULL, "msg too short");
		return ATT_ERR_LENGTH;
	}

	if (operation == ATT_PDU_PREP_WRITE_REQ) {
		/* In case of relaiable write, just return success and wait for exec. */
		return ATT_SUCCESS;
	} else if (
		operation != ATT_PDU_EXEC_WRITE_REQ &&
		operation != ATT_PDU_WRITE_CMD) {
		LOG_ERR("filetrans",
			"operation 0x%x not supported, try normal write\n",
			operation);
		return ATT_ERR_INVALID_PDU;
	}

	switch (pValue[0]) {
	case 's':
		return bleFileOpen(connId, pValue, len);

	case 'c':
		return bleFileWrite(connId, pValue, len);

	case 'f':
		if (file_fd != -1) {
			epic_file_close(file_fd);
			file_fd = -1;
		}
		sendCrcResponse(connId, 'F', 0, NULL, NULL);
		return ATT_SUCCESS;

	case 'e':
		if (file_fd != -1) {
			epic_file_close(file_fd);
			file_fd = -1;
		}
		sendCrcResponse(connId, 'E', 0, NULL, NULL);
		return ATT_SUCCESS;

	case 'E':
		LOG_ERR("filetrans", "Error was acked");
		return ATT_SUCCESS;

	default:
		sendCrcResponse(connId, 'e', 0, NULL, "unkown command");
		return ATT_ERR_NOT_SUP;
	}
	return ATT_ERR_NOT_SUP;
}

/*
 * BLE file transfer write callback.
 * 
 * This gets called when data is written by a BLE central to our BLE
 * peripheral. Here we take care of handling the received data.
 */
static uint8_t writeCallback(
	dmConnId_t connId,
	uint16_t handle,
	uint8_t operation,
	uint16_t offset,
	uint16_t len,
	uint8_t *pValue,
	attsAttr_t *pAttr
) {
	switch (handle) {
	case FILE_TRANS_CENTRAL_TX_VAL_HDL:
		return handleCentralTX(
			connId, handle, operation, offset, len, pValue, pAttr
		);
	default:
		LOG_ERR("filetrans",
			"unsupported characteristic: %c\n",
			handle);
		return ATT_ERR_HANDLE;
	}
}

static attsGroup_t fileTransCfgGroup = {
	.pAttr       = (attsAttr_t *)fileTransCfgList,
	.writeCback  = writeCallback,
	.startHandle = FILE_TRANS_START_HDL,
	.endHandle   = FILE_TRANS_END_HDL,
};

/*
 * This registers and starts the BLE file transfer service.
 */
void bleFileTransfer_init(void)
{
	AttsAddGroup(&fileTransCfgGroup);
}
