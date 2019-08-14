#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "fit/fit_api.h"
#include "hci_vs.h"

#include "ff.h"

#include "util/bstream.h"
#include "att_api.h"

#include "FreeRTOS.h"
#include "crc32.h"

#include "epicardium.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <machine/endian.h>

#define CARD10_UUID_SUFFIX                                                     \
	0x42, 0x23, 0x42, 0x23, 0x42, 0x23, 0x42, 0x23, 0x42, 0x23, 0x42, 0x23

/*!< \brief Service start handle. */
#define CARD10_START_HDL 0x920
/*!< \brief Service end handle. */
#define CARD10_END_HDL (CARD10_MAX_HDL - 1)

/*
 * This has to match in order and number of members to the functions
 * called in fileTransAddGroupDyn() otherwise the stack breaks.
 */
enum {
	/*!< \brief card10 service declaration */
	CARD10_SVC_HDL = CARD10_START_HDL,
	/*!< \brief vibra characteristic */
	CARD10_VIRBA_CH_HDL,
	CARD10_VIBRA_VAL_HDL,
	/*!< \brief rockets led characteristic */
	CARD10_ROCKETS_CH_HDL,
	CARD10_ROCKETS_VAL_HDL,
	/*!< \brief light sensor characteristic */
	CARD10_LIGHT_SENSOR_CH_HDL,
	CARD10_LIGHT_SENSOR_VAL_HDL,
	/*!< \brief Maximum handle. */
	CARD10_MAX_HDL
};

/* BLE UUID for card10 service*/
static const uint8_t UUID_svc[] = { CARD10_UUID_SUFFIX, 0x0, 0xf0, 0x0, 0x0 };

/* BLE UUID for card10 char vibra */
static const uint8_t UUID_char_vibra[] = { ATT_PROP_WRITE_NO_RSP,
					   UINT16_TO_BYTES(
						   CARD10_VIBRA_VAL_HDL),
					   CARD10_UUID_SUFFIX,
					   0xf,
					   0xf1,
					   0x0,
					   0x0 };

static const uint8_t UUID_attChar_vibra[] = {
	CARD10_UUID_SUFFIX, 0xf, 0xf1, 0x0, 0x0
};

/* BLE UUID for card10 char rockets */
static const uint8_t UUID_char_rockets[] = { ATT_PROP_WRITE_NO_RSP,
					     UINT16_TO_BYTES(
						     CARD10_ROCKETS_VAL_HDL),
					     CARD10_UUID_SUFFIX,
					     0x10,
					     0xf1,
					     0x0,
					     0x0 };

static const uint8_t UUID_attChar_rockets[] = {
	CARD10_UUID_SUFFIX, 0x10, 0xf1, 0x0, 0x0
};

/* BLE UUID for card10 char light sensor */
static const uint8_t UUID_char_light_sensor[] = {
	ATT_PROP_READ,
	UINT16_TO_BYTES(CARD10_LIGHT_SENSOR_VAL_HDL),
	CARD10_UUID_SUFFIX,
	0xf0,
	0xf0,
	0x0,
	0x0
};
static const uint8_t UUID_attChar_light_sensor[] = {
	CARD10_UUID_SUFFIX, 0xf0, 0xf0, 0x0, 0x0
};

/*
 * Create the BLE service description. 
 */
static void *addCard10GroupDyn(void)
{
	void *pSHdl;

	uint8_t initLightSensorValue[] = { UINT16_TO_BYTES(0) };

	/* Create the service */
	pSHdl = AttsDynCreateGroup(CARD10_START_HDL, CARD10_END_HDL);
	if (pSHdl != NULL) {
		/* Primary service */
		AttsDynAddAttrConst(
			pSHdl,
			attPrimSvcUuid,
			UUID_svc,
			sizeof(UUID_svc),
			0,
			ATTS_PERMIT_READ
		);

		// VIBRA

		AttsDynAddAttrConst(
			pSHdl,
			attChUuid,
			UUID_char_vibra,
			sizeof(UUID_char_vibra),
			0,
			ATTS_PERMIT_READ
		);

		AttsDynAddAttr(
			pSHdl,
			UUID_attChar_vibra,
			NULL,
			sizeof(uint16_t),
			sizeof(uint16_t),
			ATTS_SET_WRITE_CBACK,
			ATTS_PERMIT_WRITE
		);

		// ROCKETS

		AttsDynAddAttrConst(
			pSHdl,
			attChUuid,
			UUID_char_rockets,
			sizeof(UUID_char_rockets),
			0,
			ATTS_PERMIT_READ
		);

		AttsDynAddAttr(
			pSHdl,
			UUID_attChar_rockets,
			NULL,
			3 * sizeof(uint8_t),
			3 * sizeof(uint8_t),
			ATTS_SET_WRITE_CBACK,
			ATTS_PERMIT_WRITE
		);

		// LIGHT_SENSOR

		AttsDynAddAttrConst(
			pSHdl,
			attChUuid,
			UUID_char_light_sensor,
			sizeof(UUID_char_light_sensor),
			0,
			ATTS_PERMIT_READ
		);

		AttsDynAddAttr(
			pSHdl,
			UUID_attChar_light_sensor,
			initLightSensorValue,
			sizeof(uint8_t),
			sizeof(uint8_t),
			ATTS_SET_READ_CBACK,
			ATTS_PERMIT_READ
		);

		APP_TRACE_INFO0("ble-card10: services binded\n");
	}
	return pSHdl;
}

/*
 * BLE file transfer write callback.
 * 
 * This gets called when data is written by a BLE central to our BLE
 * peripheral. Here we take care of handling the received data.
 */

static uint8_t writeCard10CB(
	dmConnId_t connId,
	uint16_t handle,
	uint8_t operation,
	uint16_t offset,
	uint16_t len,
	uint8_t *pValue,
	attsAttr_t *pAttr
) {
	uint16_t ui16;

	switch (handle) {
	case CARD10_VIBRA_VAL_HDL:
		BYTES_TO_UINT16(ui16, pValue);
		epic_vibra_vibrate(ui16);
		APP_TRACE_INFO1("ble-card10: set vibra for %dms\n", ui16);
		return ATT_SUCCESS;
	case CARD10_ROCKETS_VAL_HDL:
		epic_leds_set_rocket(0, pValue[0]);
		epic_leds_set_rocket(1, pValue[1]);
		epic_leds_set_rocket(2, pValue[2]);
		APP_TRACE_INFO3(
			"ble-card10: set rockets 0:%d, 1:%d, 2:%d\n",
			pValue[0],
			pValue[1],
			pValue[2]
		);
		return ATT_SUCCESS;
	default:
		APP_TRACE_INFO1(
			"ble-card10: unsupported characteristic: %c\n", handle
		);
		return ATT_ERR_HANDLE;
	}
}

static uint8_t readCard10CB(
	dmConnId_t connId,
	uint16_t handle,
	uint8_t operation,
	uint16_t offset,
	attsAttr_t *pAttr
) {
	uint16_t ui16 = 0;

	switch (handle) {
	case CARD10_LIGHT_SENSOR_VAL_HDL:
		epic_light_sensor_run();
		epic_light_sensor_get(&ui16);
		// epic_light_sensor_stop(); stop it everywhere, also in py
		*pAttr->pValue = ui16;
		APP_TRACE_INFO1("ble-card10: read lightsensor: %d\n", ui16);
		return ATT_SUCCESS;
	default:
		APP_TRACE_INFO0("ble-card10: read no handler found\n");
		return ATT_ERR_HANDLE;
	}
}

/*
 * This registers and starts the BLE file transfer service.
 */

void bleCard10_init(void)
{
	void *pSHdl = addCard10GroupDyn();
	AttsDynRegister(pSHdl, readCard10CB, writeCard10CB);
}
