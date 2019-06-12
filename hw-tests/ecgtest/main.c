/*******************************************************************************
 * License: TBD
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "mxc_config.h"
#include "led.h"
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "MAX30003.h"
#include "GUI_DEV/GUI_Paint.h"
#include "pmic.h"
#include "card10.h"
#include <stdbool.h>

/***** Definitions *****/

/***** Globals *****/
static bool ecg_switch;
static bool internal_pull;


/***** Functions *****/
static uint32_t ecg_read_reg(uint8_t reg)
{
    spi_req_t req;
    uint8_t tx_data[] = {(reg << 1) | 1, 0, 0, 0};
    uint8_t rx_data[] = {0, 0, 0, 0};
    req.tx_data = tx_data;
    req.rx_data = rx_data;
    req.len = 4;
    req.bits = 8;
    req.width = SPI17Y_WIDTH_1;
    req.ssel = 0;
    req.deass = 1;
    req.ssel_pol = SPI17Y_POL_LOW;
    req.tx_num = 0;
    req.rx_num = 0;

    SPI_MasterTrans(SPI0, &req);

    return (rx_data[1] << 16) | (rx_data[2] << 8) | rx_data[3];
}

static void ecg_write_reg(uint8_t reg, uint32_t data)
{
    printf("write %02x %06x\n", reg, data);
    spi_req_t req;
    uint8_t tx_data[] = {(reg << 1) | 0 , data >> 16, (data >> 8 ) & 0xFF, data & 0xFF};
    uint8_t rx_data[] = {0, 0, 0, 0};
    req.tx_data = tx_data;
    req.rx_data = rx_data;
    req.len = 4;
    req.bits = 8;
    req.width = SPI17Y_WIDTH_1;
    req.ssel = 0;
    req.deass = 1;
    req.ssel_pol = SPI17Y_POL_LOW;
    req.tx_num = 0;
    req.rx_num = 0;

    SPI_MasterTrans(SPI0, &req);
}

static void ecg_config(bool enable_internal_pull)
{
    // Reset ECG to clear registers
    ecg_write_reg(SW_RST , 0);

    // General config register setting
    union GeneralConfiguration_u CNFG_GEN_r;
    CNFG_GEN_r.bits.en_ecg = 1;     // Enable ECG channel
    if(enable_internal_pull) {
        CNFG_GEN_r.bits.rbiasn = 1;     // Enable resistive bias on negative input
        CNFG_GEN_r.bits.rbiasp = 1;     // Enable resistive bias on positive input
        CNFG_GEN_r.bits.en_rbias = 1;   // Enable resistive bias
    } else {
        CNFG_GEN_r.bits.rbiasn = 0;     // Enable resistive bias on negative input
        CNFG_GEN_r.bits.rbiasp = 0;     // Enable resistive bias on positive input
        CNFG_GEN_r.bits.en_rbias = 0;   // Enable resistive bias
    }

    CNFG_GEN_r.bits.imag = 2;       // Current magnitude = 10nA
    CNFG_GEN_r.bits.en_dcloff = 1;  // Enable DC lead-off detection
    ecg_write_reg(CNFG_GEN , CNFG_GEN_r.all);


    // ECG Config register setting
    union ECGConfiguration_u CNFG_ECG_r;
    CNFG_ECG_r.bits.dlpf = 1;       // Digital LPF cutoff = 40Hz
    CNFG_ECG_r.bits.dhpf = 1;       // Digital HPF cutoff = 0.5Hz
    //CNFG_ECG_r.bits.gain = 3;       // ECG gain = 160V/V
    CNFG_ECG_r.bits.gain = 0;
    CNFG_ECG_r.bits.rate = 2;       // Sample rate = 128 sps
    ecg_write_reg(CNFG_ECG , CNFG_ECG_r.all);


    //R-to-R configuration
    union RtoR1Configuration_u CNFG_RTOR_r;
    CNFG_RTOR_r.bits.en_rtor = 1;           // Enable R-to-R detection
    ecg_write_reg(CNFG_RTOR1 , CNFG_RTOR_r.all);


    //Manage interrupts register setting
    union ManageInterrupts_u MNG_INT_r;
    MNG_INT_r.bits.efit = 0b00011;          // Assert EINT w/ 4 unread samples
    MNG_INT_r.bits.clr_rrint = 0b01;        // Clear R-to-R on RTOR reg. read back
    ecg_write_reg(MNGR_INT , MNG_INT_r.all);


    //Enable interrupts register setting
    union EnableInterrupts_u EN_INT_r;
    EN_INT_r.all = 0;
    EN_INT_r.bits.en_eint = 1;              // Enable EINT interrupt
    EN_INT_r.bits.en_rrint = 0;             // Disable R-to-R interrupt
    EN_INT_r.bits.intb_type = 3;            // Open-drain NMOS with internal pullup
    ecg_write_reg(EN_INT , EN_INT_r.all);


    //Dyanmic modes config
    union ManageDynamicModes_u MNG_DYN_r;
    MNG_DYN_r.bits.fast = 0;                // Fast recovery mode disabled
    ecg_write_reg(MNGR_DYN , MNG_DYN_r.all);

    // MUX Config
    union MuxConfiguration_u CNFG_MUX_r;
    CNFG_MUX_r.bits.openn = 0;          // Connect ECGN to AFE channel
    CNFG_MUX_r.bits.openp = 0;          // Connect ECGP to AFE channel
    ecg_write_reg(CNFG_EMUX , CNFG_MUX_r.all);

    return;
}

#define SIZE_X 160
#define SIZE_Y 80

static uint8_t prev;

static void clear(void)
{
    Paint_Clear(BLACK);
    prev = 32;
}


static void set(uint8_t index, int8_t val)
{

    if(val < -31) val = -31;
    if(val > 32) val = 32;

    int8_t pos = 32 + val;

    int min, max;
    if(prev < pos) {
        min = prev;
        max = pos;
    } else {
        min = pos;
        max = prev;
    }

    for(int i = min; i < max + 1; i++) {
        LCD_SetUWORD(SIZE_X - index - 1, i, RED);
    }

    prev = pos;
}

static int16_t samples[SIZE_X];
void update(void)
{
    clear();

    char buf[128];
    sprintf(buf, "Switch: %d  Pull: %d", ecg_switch, internal_pull);
    Paint_DrawString_EN(0, 0, buf, &Font8, 0x0000, 0xffff);


    int16_t max = 0;
    for(int i=0; i<SIZE_X; i++) {
        if(abs(samples[i]) > max) {
            max = abs(samples[i]);
        }
    }

    int16_t scale = max / 32;

    for(int i=0; i<SIZE_X; i++) {
        set(i, (samples[i] / scale));
        //set(i, ((samples[i*2] + samples[i*2 + 1]) / scale) / 2);
    }

    LCD_Update();
}

static uint8_t sample_count = 0;

static void add_sample(int16_t sample)
{
    memmove(samples, samples + 1, sizeof(*samples) * (SIZE_X-1));
    samples[SIZE_X-1] = sample;
    sample_count++;

    if(sample_count == 5) {
        sample_count = 0;
        update();
    }
}

static volatile bool ecgFIFOIntFlag = false;
static void ecgFIFO_callback(void *data) {
    ecgFIFOIntFlag = true;
}

// *****************************************************************************
int main(void)
{

    card10_init();
    card10_diag();

    const gpio_cfg_t interrupt_pin = {PORT_1, PIN_12, GPIO_FUNC_IN, GPIO_PAD_PULL_UP};
    GPIO_Config(&interrupt_pin);
    GPIO_RegisterCallback(&interrupt_pin, ecgFIFO_callback, NULL);
    GPIO_IntConfig(&interrupt_pin, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(&interrupt_pin);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(PORT_1));

    const gpio_cfg_t analog_switch = {PORT_0, PIN_31, GPIO_FUNC_OUT, GPIO_PAD_NONE};
    GPIO_Config(&analog_switch);


    ecg_switch = false;
    GPIO_OutClr(&analog_switch); // Wrist

    internal_pull = true;
    ecg_config(internal_pull);

    for(int i=0; i<0x20; i++) {
        uint32_t val = ecg_read_reg(i);
        printf("%02x: 0x%06x\n", i, val);
    }

    ecg_write_reg(SYNCH, 0);

    uint32_t ecgFIFO, readECGSamples, idx, ETAG[32], status;
    int16_t ecgSample[32];
    const int EINT_STATUS_MASK =  1 << 23;
    const int FIFO_OVF_MASK =  0x7;
    const int FIFO_VALID_SAMPLE_MASK =  0x0;
    const int FIFO_FAST_SAMPLE_MASK =  0x1;
    const int ETAG_BITS_MASK = 0x7;


    while(1) {
        // Read back ECG samples from the FIFO
        if( ecgFIFOIntFlag ) {
            ecgFIFOIntFlag = false;

            if(PB_Get(0)) {
                ecg_switch = !ecg_switch;
                while(PB_Get(0));
                if(ecg_switch) {
                    GPIO_OutSet(&analog_switch); // USB
                } else {
                    GPIO_OutClr(&analog_switch); // Wrist
                }
            }

            if(PB_Get(2)) {
                internal_pull =! internal_pull;
                while(PB_Get(2));
                ecg_config(internal_pull);
            }

            //printf("Int\n");
            status = ecg_read_reg( STATUS );      // Read the STATUS register

            // Check if EINT interrupt asserted
            if ( ( status & EINT_STATUS_MASK ) == EINT_STATUS_MASK ) {

                readECGSamples = 0;                        // Reset sample counter

                do {
                    ecgFIFO = ecg_read_reg(ECG_FIFO );       // Read FIFO
                    ecgSample[readECGSamples] = ecgFIFO >> 8;                  // Isolate voltage data
                    ETAG[readECGSamples] = ( ecgFIFO >> 3 ) & ETAG_BITS_MASK;  // Isolate ETAG
                    readECGSamples++;                                          // Increment sample counter

                // Check that sample is not last sample in FIFO
                } while ( ETAG[readECGSamples-1] == FIFO_VALID_SAMPLE_MASK ||
                          ETAG[readECGSamples-1] == FIFO_FAST_SAMPLE_MASK );

                // Check if FIFO has overflowed
                if( ETAG[readECGSamples - 1] == FIFO_OVF_MASK ){
                    ecg_write_reg(FIFO_RST , 0); // Reset FIFO
                    //printf("OV\n");
                    // notifies the user that an over flow occured
                    //LED_On(0);
                    pmic_set_led(0, 31);
                }

                // Print results
                for( idx = 0; idx < readECGSamples; idx++ ) {
                    //printf("%6d\r\n", ecgSample[idx]);
                    add_sample(ecgSample[idx]);
                }
            }
        }
    }
}
