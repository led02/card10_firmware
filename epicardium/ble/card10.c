#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_assert.h"
#include "att_api.h"

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
	CARD10_TIME_CH_HDL,
	CARD10_TIME_VAL_HDL,
	/*!< \brief vibra characteristic */
	CARD10_VIRBA_CH_HDL,
	CARD10_VIBRA_VAL_HDL,
	/*!< \brief rockets led characteristic */
	CARD10_ROCKETS_CH_HDL,
	CARD10_ROCKETS_VAL_HDL,
	/*!< \brief led for background on bottom left characteristic */
	CARD10_LED_BG_BOTTOM_LEFT_CH_HDL,
	CARD10_LED_BG_BOTTOM_LEFT_VAL_HDL,
	/*!< \brief led for background on bottom right characteristic */
	CARD10_LED_BG_BOTTOM_RIGHT_CH_HDL,
	CARD10_LED_BG_BOTTOM_RIGHT_VAL_HDL,
	/*!< \brief led for background on top right characteristic */
	CARD10_LED_BG_TOP_RIGHT_CH_HDL,
	CARD10_LED_BG_TOP_RIGHT_VAL_HDL,
	/*!< \brief led for background on top left characteristic */
	CARD10_LED_BG_TOP_LEFT_CH_HDL,
	CARD10_LED_BG_TOP_LEFT_VAL_HDL,
	/*!< \brief dim leds on bottom characteristic */
	CARD10_LEDS_BOTTOM_DIM_CH_HDL,
	CARD10_LEDS_BOTTOM_DIM_VAL_HDL,
	/*!< \brief dim leds on top characteristic */
	CARD10_LEDS_TOP_DIM_CH_HDL,
	CARD10_LEDS_TOP_DIM_VAL_HDL,
	/*!< \brief led powersafe characteristic */
	CARD10_LED_POWERSAFE_CH_HDL,
	CARD10_LED_POWERSAFE_VAL_HDL,
	/*!< \brief flashlight characteristic */
	CARD10_FLASHLIGHT_CH_HDL,
	CARD10_FLASHLIGHT_VAL_HDL,
	/*!< \brief flashlight characteristic */
	CARD10_PERSONAL_STATE_CH_HDL,
	CARD10_PERSONAL_STATE_VAL_HDL,
	/*!< \brief leds above characteristic */
	CARD10_LEDS_ABOVE_CH_HDL,
	CARD10_LEDS_ABOVE_VAL_HDL,
	/*!< \brief light sensor characteristic */
	CARD10_LIGHT_SENSOR_CH_HDL,
	CARD10_LIGHT_SENSOR_VAL_HDL,
	/*!< \brief Maximum handle. */
	CARD10_MAX_HDL
};

/* clang-format off */

/* BLE UUID for card10 service*/
static const uint8_t UUID_svc[] = { CARD10_UUID_SUFFIX, 0x0, CARD10_UUID_PREFIX };
// works vor everyone?
static const uint16_t UUID_len = sizeof(UUID_svc);

// starting at 0x01 with write (non visual) charateristics

/* BLE UUID for card10 time */
static const uint8_t UUID_char_time[] = {
	(ATT_PROP_READ | ATT_PROP_WRITE_NO_RSP),
	UINT16_TO_BYTES(CARD10_TIME_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x01, CARD10_UUID_PREFIX
};

static uint8_t timeValue[] = { UINT32_TO_BYTES(0), UINT32_TO_BYTES(0) };
static uint16_t timeLen = sizeof(timeValue);

// works vor everyone?
static const uint16_t UUID_char_len = sizeof(UUID_char_time);

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

/* BLE UUID for card10 led background bottom left */
static const uint8_t UUID_char_led_bg_bottom_left[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LED_BG_BOTTOM_LEFT_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x11, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_led_bg_bottom_left[] = {
	CARD10_UUID_SUFFIX, 0x11, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 led background bottom right */
static const uint8_t UUID_char_led_bg_bottom_right[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LED_BG_BOTTOM_RIGHT_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x12, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_led_bg_bottom_right[] = {
	CARD10_UUID_SUFFIX, 0x12, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 led background top right */
static const uint8_t UUID_char_led_bg_top_right[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LED_BG_TOP_RIGHT_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x13, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_led_bg_top_right[] = {
	CARD10_UUID_SUFFIX, 0x13, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 led background top left */
static const uint8_t UUID_char_led_bg_top_left[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LED_BG_TOP_LEFT_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x14, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_led_bg_top_left[] = {
	CARD10_UUID_SUFFIX, 0x14, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 dim leds on bottom */
static const uint8_t UUID_char_leds_bottom_dim[] = {
	ATT_PROP_WRITE,
	UINT16_TO_BYTES(CARD10_LEDS_BOTTOM_DIM_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x15, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_leds_bottom_dim[] = {
	CARD10_UUID_SUFFIX, 0x15, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 dim leds on top */
static const uint8_t UUID_char_leds_top_dim[] = {
	ATT_PROP_WRITE,
	UINT16_TO_BYTES(CARD10_LEDS_TOP_DIM_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x16, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_leds_top_dim[] = {
	CARD10_UUID_SUFFIX, 0x16, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 powersafe */
static const uint8_t UUID_char_led_powersafe[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LED_POWERSAFE_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x17, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_led_powersafe[] = {
	CARD10_UUID_SUFFIX, 0x17, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 flashlight */
static const uint8_t UUID_char_flashlight[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_FLASHLIGHT_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x18, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_flashlight[] = {
	CARD10_UUID_SUFFIX, 0x18, CARD10_UUID_PREFIX
};

/* BLE UUID for card10 personal state */
static const uint8_t UUID_char_personal_state[] = {
	ATT_PROP_READ | ATT_PROP_WRITE,
	UINT16_TO_BYTES(CARD10_PERSONAL_STATE_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x19, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_personal_state[] = {
	CARD10_UUID_SUFFIX, 0x19, CARD10_UUID_PREFIX
};

static uint8_t personalStateValue = 0; 
static uint16_t personalStateLen = sizeof(personalStateValue);

/* BLE UUID for card10 above leds */
static const uint8_t UUID_char_leds_above[] = {
	ATT_PROP_WRITE_NO_RSP,
	UINT16_TO_BYTES(CARD10_LEDS_ABOVE_VAL_HDL),
	CARD10_UUID_SUFFIX, 0x20, CARD10_UUID_PREFIX
};

static const uint8_t UUID_attChar_leds_above[] = {
	CARD10_UUID_SUFFIX, 0x20, CARD10_UUID_PREFIX
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

static uint8_t initLightSensorValue[] = { UINT16_TO_BYTES(0) };
static uint16_t initLightSensorLen = sizeof(initLightSensorValue);

/*
 * Create the BLE service description. 
 */

static const attsAttr_t card10SvcAttrList[] =
{
	{
		.pUuid = attPrimSvcUuid,
		.pValue = (uint8_t *) UUID_svc,
		.pLen = (uint16_t *) &UUID_len,
		.maxLen = sizeof(UUID_svc),
		.permissions = ATTS_PERMIT_READ
	},

	// TIME

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_time,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_time),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_time,
		.pValue = timeValue,
		.pLen = &timeLen,
		.maxLen = sizeof(uint64_t),
		.settings = (ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK),
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH | 
				ATTS_PERMIT_READ | ATTS_PERMIT_READ_ENC |
				ATTS_PERMIT_READ_AUTH)
	},

	// VIBRA

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_vibra,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_vibra),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_vibra,
		.pValue = NULL,
		.maxLen = sizeof(uint16_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// ROCKETS

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_rockets,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_rockets),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_rockets,
		.pValue = NULL,
		.maxLen = 3 * sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// BG LED Bottom left

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_led_bg_bottom_left,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_led_bg_bottom_left),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_led_bg_bottom_left,
		.pValue = NULL,
		.maxLen = 3 * sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// BG LED Bottom right

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_led_bg_bottom_right,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_led_bg_bottom_right),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_led_bg_bottom_right,
		.pValue = NULL,
		.maxLen = 3 * sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// BG LED top right

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_led_bg_top_right,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_led_bg_top_right),
		.settings = 0,
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_led_bg_top_right,
		.pValue = NULL,
		.maxLen = 3 * sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// BG LED top left

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_led_bg_top_left,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_led_bg_top_left),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_led_bg_top_left,
		.pValue = NULL,
		.maxLen = 3 * sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// Dim bottom module

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_leds_bottom_dim,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_leds_bottom_dim),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_leds_bottom_dim,
		.pValue = NULL,
		.pLen = 0,
		.maxLen = sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// Dim top module

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_leds_top_dim,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_leds_top_dim),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_leds_top_dim,
		.pValue = NULL,
		.maxLen = sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// led powersafe

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_led_powersafe,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_led_powersafe),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_led_powersafe,
		.pValue = NULL,
		.maxLen = sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// flashlight

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_flashlight,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_flashlight),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_flashlight,
		.pValue = NULL,
		.maxLen = sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// personal state

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_personal_state,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_personal_state),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_personal_state,
		.pValue = &personalStateValue,
		.pLen = &personalStateLen,
		.maxLen = sizeof(uint16_t),
		.settings = (ATTS_SET_WRITE_CBACK | ATTS_SET_READ_CBACK),
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH |
				ATTS_PERMIT_READ | ATTS_PERMIT_READ_ENC |
				ATTS_PERMIT_READ_AUTH)
	},

	// ABOVE LEDS

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_leds_above,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_leds_above),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_leds_above,
		.pValue = NULL,
		.maxLen = 11 * 3 * sizeof(uint8_t),
		.settings = ATTS_SET_WRITE_CBACK,
		.permissions = (ATTS_PERMIT_WRITE | ATTS_PERMIT_WRITE_ENC |
				ATTS_PERMIT_WRITE_AUTH)
	},

	// Light sensor

	{
		.pUuid = attChUuid,
		.pValue = (uint8_t *) UUID_char_light_sensor,
		.pLen = (uint16_t *) &UUID_char_len,
		.maxLen = sizeof(UUID_char_light_sensor),
		.permissions = ATTS_PERMIT_READ
	},
	{
		.pUuid = UUID_attChar_light_sensor,
		.pValue = initLightSensorValue,
		.pLen = &initLightSensorLen,
		.maxLen = sizeof(uint8_t),
		.settings = ATTS_SET_READ_CBACK,
		.permissions = (ATTS_PERMIT_READ | ATTS_PERMIT_READ_ENC |
				ATTS_PERMIT_READ_AUTH)
	},
};
/* clang-format on */

// validating, that the service really get all charateristics
WSF_CT_ASSERT(
	((sizeof(card10SvcAttrList) / sizeof(card10SvcAttrList[0])) ==
	 CARD10_END_HDL - CARD10_START_HDL + 1));
/*
 * Set the time given in milliseconds since 1.1.1970 as 64 bit integer.
 */
static uint8_t setTime(uint8_t *pValue)
{
	uint64_t timeNet;
	uint64_t time;

	memcpy(&timeNet, pValue, sizeof(timeNet));
	time = __bswap64(timeNet);
	epic_rtc_set_milliseconds(time);

	APP_TRACE_INFO0("ble-card10: set time");
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
	uint16_t ui16 = 0;
	uint8_t ui8   = 0;

	switch (handle) {
	// time
	case CARD10_TIME_VAL_HDL:
		return setTime(pValue);
	// vibra
	case CARD10_VIBRA_VAL_HDL:
		BYTES_TO_UINT16(ui16, pValue);
		epic_vibra_vibrate(ui16);
		APP_TRACE_INFO1("ble-card10: set vibra for %dms\n", ui16);
		return ATT_SUCCESS;
	// rockets
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
	// bg leds
	case CARD10_LED_BG_BOTTOM_LEFT_VAL_HDL:
		epic_leds_set(11, pValue[0], pValue[1], pValue[2]);
		APP_TRACE_INFO3(
			"ble-card10: set bg bottom left: #%02x%02x%02x\n",
			pValue[0],
			pValue[1],
			pValue[2]
		);
		return ATT_SUCCESS;
	case CARD10_LED_BG_BOTTOM_RIGHT_VAL_HDL:
		epic_leds_set(12, pValue[0], pValue[1], pValue[2]);
		APP_TRACE_INFO3(
			"ble-card10: set bg bottom right: #%02x%02x%02x\n",
			pValue[0],
			pValue[1],
			pValue[2]
		);
		return ATT_SUCCESS;
	case CARD10_LED_BG_TOP_RIGHT_VAL_HDL:
		epic_leds_set(13, pValue[0], pValue[1], pValue[2]);
		APP_TRACE_INFO3(
			"ble-card10: set bg top right: #%02x%02x%02x\n",
			pValue[0],
			pValue[1],
			pValue[2]
		);
		return ATT_SUCCESS;
	case CARD10_LED_BG_TOP_LEFT_VAL_HDL:
		epic_leds_set(14, pValue[0], pValue[1], pValue[2]);
		APP_TRACE_INFO3(
			"ble-card10: set bg top left: #%02x%02x%02x\n",
			pValue[0],
			pValue[1],
			pValue[2]
		);
		return ATT_SUCCESS;
	// dim
	case CARD10_LEDS_BOTTOM_DIM_VAL_HDL:
		ui8 = pValue[0];
		if (ui8 >= 1 && ui8 <= 8) {
			if (operation == ATT_PDU_WRITE_CMD ||
			    operation == ATT_PDU_SIGNED_WRITE_CMD ||
			    operation == ATT_PDU_WRITE_REQ ||
			    operation == ATT_PDU_EXEC_WRITE_REQ) {
				epic_leds_dim_bottom(pValue[0]);
				APP_TRACE_INFO1(
					"ble-card10: dim bottom to: %d\n",
					pValue[0]
				);
				return ATT_SUCCESS;
			} else if (operation == ATT_PDU_PREP_WRITE_REQ) {
				APP_TRACE_INFO1(
					"ble_card10: value for dim bottom would be okay: %d\n",
					pValue[0]
				);
				return ATT_SUCCESS;
			} else {
				APP_TRACE_INFO1(
					"ble-card10: dim bottom with unknown operation: %d\n",
					operation
				);
				return ATT_ERR_INVALID_PDU;
			}
		} else {
			APP_TRACE_INFO1(
				"ble-card: prep dim bottom invalid value (1-8): %d\n",
				ui8
			);
			return ATT_ERR_RANGE;
		}
	case CARD10_LEDS_TOP_DIM_VAL_HDL:
		ui8 = pValue[0];
		if (ui8 >= 1 && ui8 <= 8) {
			if (operation == ATT_PDU_WRITE_CMD ||
			    operation == ATT_PDU_SIGNED_WRITE_CMD ||
			    operation == ATT_PDU_WRITE_REQ ||
			    operation == ATT_PDU_EXEC_WRITE_REQ) {
				epic_leds_dim_top(pValue[0]);
				APP_TRACE_INFO1(
					"ble-card10: dim top to: %d\n",
					pValue[0]
				);
				return ATT_SUCCESS;
			} else if (operation == ATT_PDU_PREP_WRITE_REQ) {
				APP_TRACE_INFO1(
					"ble_card10: value for dim top would be okay: %d\n",
					pValue[0]
				);
				return ATT_SUCCESS;
			} else {
				APP_TRACE_INFO1(
					"ble-card10: dim top with unknown operation: %d\n",
					operation
				);
				return ATT_ERR_INVALID_PDU;
			}
		} else {
			APP_TRACE_INFO1(
				"ble-card: prep dim top invalid value (1-8): %d\n",
				ui8
			);
			return ATT_ERR_RANGE;
		}
	// led powersafe
	case CARD10_LED_POWERSAFE_VAL_HDL:
		epic_leds_set_powersave(pValue[0]);
		APP_TRACE_INFO1("set powersafe to: %d\n", pValue[0]);
		return ATT_SUCCESS;
	// flashlight
	case CARD10_FLASHLIGHT_VAL_HDL:
		epic_set_flashlight(pValue[0]);
		APP_TRACE_INFO1("set flashlight to: %d\n", pValue[0]);
		return ATT_SUCCESS;
	// personal state
	case CARD10_PERSONAL_STATE_VAL_HDL:
		BYTES_TO_UINT16(ui16, pValue);
		if (ui16 <= STATE_MAX) {
			if (operation == ATT_PDU_WRITE_CMD ||
			    operation == ATT_PDU_SIGNED_WRITE_CMD ||
			    operation == ATT_PDU_WRITE_REQ ||
			    operation == ATT_PDU_EXEC_WRITE_REQ) {
				epic_personal_state_set(ui16, true);
				APP_TRACE_INFO1(
					"ble-card10: set personal state to: %d\n",
					ui16
				);
				return ATT_SUCCESS;
			} else if (operation == ATT_PDU_PREP_WRITE_REQ) {
				APP_TRACE_INFO1(
					"ble_card10: personal state would be okay: %d\n",
					ui16
				);
				return ATT_SUCCESS;
			} else {
				APP_TRACE_INFO1(
					"ble-card10: personal state with unknown operation: %d\n",
					operation
				);
				return ATT_ERR_INVALID_PDU;
			}
		} else {
			APP_TRACE_INFO2(
				"ble-card: personal state invalid value (0-%d): %d\n",
				STATE_MAX - 1,
				ui16
			);
			return ATT_ERR_RANGE;
		}
	// leds above
	case CARD10_LEDS_ABOVE_VAL_HDL:
		for (ui16 = 0; ui16 < 11; ui16++) {
			epic_leds_set(
				ui16,
				pValue[ui16 * 3],
				pValue[ui16 * 3 + 1],
				pValue[ui16 * 3 + 2]
			);
			APP_TRACE_INFO4(
				"ble-card10: set led %ld above to #%02x%02x%02x\n",
				ui16,
				pValue[ui16 * 3],
				pValue[ui16 * 3 + 1],
				pValue[ui16 * 3 + 2]
			);
		}
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
	uint64_t ui64 = 0;

	switch (handle) {
	case CARD10_TIME_VAL_HDL:
		ui64 = epic_rtc_get_milliseconds();
		uint64_t time;

		time = __bswap64(ui64);
		memcpy(pAttr->pValue, &time, sizeof(time));

		APP_TRACE_INFO0("ble-card10: read time\n");
		return ATT_SUCCESS;
	case CARD10_PERSONAL_STATE_VAL_HDL:
		ui16           = epic_personal_state_get();
		*pAttr->pValue = ui16;
		APP_TRACE_INFO1("ble-card10: read personal state: %d\n", ui16);
		return ATT_SUCCESS;
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

static attsGroup_t svcCard10Group = {
	.pNext       = NULL,
	.pAttr       = (attsAttr_t *)card10SvcAttrList,
	.readCback   = readCard10CB,
	.writeCback  = writeCard10CB,
	.startHandle = CARD10_START_HDL,
	.endHandle   = CARD10_END_HDL,
};

/*
 * This registers and starts the BLE card10 service.
 */

void bleCard10_init(void)
{
	AttsAddGroup(&svcCard10Group);
}
