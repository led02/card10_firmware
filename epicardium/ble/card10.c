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
#define CARD10_UUID_PREFIX 0x02, 0x23, 0x42

/*!< \brief Service start handle. */
#define CARD10_START_HDL 0x920
/*!< \brief Service end handle. */
#define CARD10_END_HDL (CARD10_MAX_HDL - 1)

/*
 * This has to match in order and number of members to the functions
 * called in addCard10GroupDyn() otherwise the stack breaks.
 */
enum {
	/*!< \brief card10 service declaration */
	CARD10_SVC_HDL = CARD10_START_HDL,
	/*!< \brief time update characteristic */
	CARD10_TIME_UPDATE_CH_HDL,
	CARD10_TIME_UPDATE_VAL_HDL,
	/*!< \brief vibra characteristic */
	CARD10_VIRBA_CH_HDL,
	CARD10_VIBRA_VAL_HDL,
	/*!< \brief rockets led characteristic */
	CARD10_ROCKETS_CH_HDL,
	CARD10_ROCKETS_VAL_HDL,
	/*!< \brief single led characteristic */
	CARD10_LED_S_CH_HDL,
	CARD10_LED_S_VAL_HDL,
	/*!< \brief light sensor characteristic */
	CARD10_LIGHT_SENSOR_CH_HDL,
	CARD10_LIGHT_SENSOR_VAL_HDL,
	/*!< \brief Maximum handle. */
	CARD10_MAX_HDL
};

/* clang-format off */

/* BLE UUID for card10 service*/
static const uint8_t UUID_svc[] = { CARD10_UUID_SUFFIX, 0x0, CARD10_UUID_PREFIX };


// starting at 0x01 with write (non visual) charateristics

/* BLE UUID for card10 time update */
static const uint8_t UUID_char_time[] = {
	ATT_PROP_WRITE,
	UINT16_TO_BYTES(CARD10_TIME_UPDATE_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x01, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_time[] = {
	CARD10_UUID_SUFFIX, 0x01, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 char vibra */
static const uint8_t UUID_char_vibra[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_VIBRA_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x0f, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_vibra[] = {
	CARD10_UUID_SUFFIX, 0x0f, CARD10_UUID_PREFIX
};


// starting at 0x10 with write of leds (visual output)

/* BLE UUID for card10 char rockets */
static const uint8_t UUID_char_rockets[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_ROCKETS_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x10, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_rockets[] = {
	CARD10_UUID_SUFFIX, 0x10, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 char led single (debugging) */
static const uint8_t UUID_char_led_s[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LED_S_VAL_HDL),
	CARD10_UUID_SUFFIX, 0xef, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_led_s[] = {
	CARD10_UUID_SUFFIX, 0xef, CARD10_UUID_PREFIX
};
// starting at 0xf0 with read only characteristics

/* BLE UUID for card10 char light sensor */
static const uint8_t UUID_char_light_sensor[] = {
	ATT_PROP_READ,
	UINT16_TO_BYTES(CARD10_LIGHT_SENSOR_VAL_HDL),
	CARD10_UUID_SUFFIX, 0xf0, CARD10_UUID_PREFIX
};
static const uint8_t UUID_attChar_light_sensor[] = {
	CARD10_UUID_SUFFIX, 0xf0, CARD10_UUID_PREFIX
};
/* clang-format on */

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

		// TIME UPDTAE

		AttsDynAddAttrConst(
			pSHdl,
			attChUuid,
			UUID_char_time,
			sizeof(UUID_char_time),
			0,
			ATTS_PERMIT_READ
		);

		AttsDynAddAttr(
			pSHdl,
			UUID_attChar_time,
			NULL,
			0,
			sizeof(uint64_t),
			ATTS_SET_WRITE_CBACK,
			ATTS_PERMIT_WRITE
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
			0,
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
			0,
			3 * sizeof(uint8_t),
			ATTS_SET_WRITE_CBACK,
			ATTS_PERMIT_WRITE
		);

		// SINGLE LED

		AttsDynAddAttrConst(
			pSHdl,
			attChUuid,
			UUID_char_led_s,
			sizeof(UUID_char_led_s),
			0,
			ATTS_PERMIT_READ
		);

		AttsDynAddAttr(
			pSHdl,
			UUID_attChar_led_s,
			NULL,
			0,
			sizeof(uint16_t) + 3 * sizeof(uint8_t),
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

		APP_TRACE_INFO0("ble-card10: services bound\n");
	}
	return pSHdl;
}

/*
 * Set the time given in milliseconds since 1.1.1970 as 64 bit integer.
 */
static uint8_t setTime(uint8_t *pValue, uint16_t len)
{
	uint64_t timeNet;
	uint64_t time;

	if (len < sizeof(uint64_t)) {
		return ATT_ERR_LENGTH;
	}
	memcpy(&timeNet, pValue, sizeof(timeNet));

	time = __bswap64(timeNet);
	epic_rtc_set_milliseconds(time);

	return ATT_SUCCESS;
}

/*
 * BLE card10 write callback.
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
	case CARD10_TIME_UPDATE_VAL_HDL:
		if (operation == ATT_PDU_PREP_WRITE_REQ) {
			if (len < sizeof(uint64_t)) {
				return ATT_ERR_LENGTH;
			}
			return ATT_SUCCESS;
		}
		return setTime(pValue, len);
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
	case CARD10_LED_S_VAL_HDL:
		BYTES_TO_UINT16(ui16, pValue);
		epic_leds_set(ui16, pValue[2], pValue[3], pValue[4]);
		APP_TRACE_INFO4(
			"ble-card10: set single led %ld to #%x%x%x\n",
			ui16,
			pValue[2],
			pValue[3],
			pValue[4]
		);
		return ATT_SUCCESS;
	default:
		APP_TRACE_INFO1(
			"ble-card10: unsupported characteristic: %c\n", handle
		);
		return ATT_ERR_HANDLE;
	}
}

/*
 * BLE card10 read callback.
 */

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
		epic_light_sensor_get(&ui16);
		*pAttr->pValue = ui16;
		APP_TRACE_INFO1("ble-card10: read lightsensor: %d\n", ui16);
		return ATT_SUCCESS;
	default:
		APP_TRACE_INFO0("ble-card10: read no handler found\n");
		return ATT_ERR_HANDLE;
	}
}

/*
 * This registers and starts the BLE card10 service.
 */

void bleCard10_init(void)
{
	void *pSHdl = addCard10GroupDyn();
	AttsDynRegister(pSHdl, readCard10CB, writeCard10CB);
}
