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
#include "pb.h"
#include "MAX30003.h"
#include "GUI_DEV/GUI_Paint.h"
#include "pmic.h"
#include "card10.h"
#include <stdbool.h>

/***** Definitions *****/

/***** Globals *****/
static bool ecg_switch;
static bool internal_pull;

#define Y_OFFSET    36
#define Y_SCALE     35

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
    printf("write %02x %06lx\n", reg, data);
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
    //CNFG_ECG_r.bits.rate = 1;       // Sample rate = 256 sps
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
    prev = Y_OFFSET;
}


static void set(uint8_t index, int8_t val)
{

    if(val < -Y_SCALE) val = -Y_SCALE;
    if(val > Y_SCALE) val = Y_SCALE;

    int8_t pos = Y_OFFSET + val;

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

    int16_t max = 0;
    for(int i=0; i<SIZE_X; i++) {
        if(abs(samples[i]) > max) {
            max = abs(samples[i]);
        }
    }

    int16_t scale = max / Y_SCALE + 1;

    char buf[128];
    sprintf(buf, "Switch: %d  Pull: %d Scale: %d", ecg_switch, internal_pull, scale);
    Paint_DrawString_EN(0, 0, buf, &Font8, 0x0000, 0xffff);


    for(int i=0; i<SIZE_X; i++) {
        set(i, (samples[i] / scale));
        //set(i, ((samples[i*2] + samples[i*2 + 1]) / scale) / 2);
    }

    LCD_Update();
}

#if 0
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 200 Hz

* 0 Hz - 40 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 4.1393894966071585 dB

* 50 Hz - 100 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -40.07355419274887 dB

*/

#define FILTER_TAP_NUM 21

static double filter_taps[FILTER_TAP_NUM] = {
  -0.02010411882885732,
  -0.05842798004352509,
  -0.061178403647821976,
  -0.010939393385338943,
  0.05125096443534972,
  0.033220867678947885,
  -0.05655276971833928,
  -0.08565500737264514,
  0.0633795996605449,
  0.310854403656636,
  0.4344309124179415,
  0.310854403656636,
  0.0633795996605449,
  -0.08565500737264514,
  -0.05655276971833928,
  0.033220867678947885,
  0.05125096443534972,
  -0.010939393385338943,
  -0.061178403647821976,
  -0.05842798004352509,
  -0.02010411882885732
};
#endif
#if 1
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 200 Hz

* 0 Hz - 40 Hz
  gain = 1
  desired ripple = 0.5 dB
  actual ripple = 0.3562617633495587 dB

* 45 Hz - 100 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -40.37207584888854 dB

*/

#define FILTER_TAP_NUM 71

static double filter_taps[FILTER_TAP_NUM] = {
  0.0057445089465822975,
  0.007451288673767406,
  -0.0011523652638272097,
  -0.0030609271459832005,
  -0.002596310763437956,
  0.004452951934981218,
  0.003740429140762826,
  -0.002351310203707235,
  -0.00638990322759006,
  0.000013108391204023357,
  0.007226767366250225,
  0.003994033360879168,
  -0.00665189327484351,
  -0.008058131222070393,
  0.0035814528837470965,
  0.011450459869389184,
  0.0016796020911726648,
  -0.012657785603199918,
  -0.008611714082779583,
  0.010587105712461298,
  0.01581354994840992,
  -0.0044174953741535905,
  -0.021419548904285074,
  -0.005927897725436821,
  0.023159558089340265,
  0.01973520678092361,
  -0.018604692334579287,
  -0.035495574517452874,
  0.004958680854279203,
  0.05116349477789529,
  0.02244341207607204,
  -0.06449371846227892,
  -0.0790927656214483,
  0.07342794634140555,
  0.3089023630319309,
  0.42341742792869774,
  0.3089023630319309,
  0.07342794634140555,
  -0.0790927656214483,
  -0.06449371846227892,
  0.02244341207607204,
  0.05116349477789529,
  0.004958680854279203,
  -0.035495574517452874,
  -0.018604692334579287,
  0.01973520678092361,
  0.023159558089340265,
  -0.005927897725436821,
  -0.021419548904285074,
  -0.0044174953741535905,
  0.01581354994840992,
  0.010587105712461298,
  -0.008611714082779583,
  -0.012657785603199918,
  0.0016796020911726648,
  0.011450459869389184,
  0.0035814528837470965,
  -0.008058131222070393,
  -0.00665189327484351,
  0.003994033360879168,
  0.007226767366250225,
  0.000013108391204023357,
  -0.00638990322759006,
  -0.002351310203707235,
  0.003740429140762826,
  0.004452951934981218,
  -0.002596310763437956,
  -0.0030609271459832005,
  -0.0011523652638272097,
  0.007451288673767406,
  0.0057445089465822975
};
#endif
int32_t fir(int32_t a)
{
    static int32_t x[FILTER_TAP_NUM];
    memmove(x + 1, x, sizeof(*x) * (FILTER_TAP_NUM-1));
    x[0] = a;

    int32_t y = 0;
    int i;
    for(i = 0; i < FILTER_TAP_NUM; i++) {
        y += filter_taps[i] * x[i];
    }
    return y;
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

    #if 1
    ecg_config(internal_pull);

    for(int i=0; i<0x20; i++) {
        uint32_t val = ecg_read_reg(i);
        printf("%02x: 0x%06lx\n", i, val);
    }

    ecg_write_reg(SYNCH, 0);
    #else
    max86150_begin();
    max86150_setup(0x1F, 4, 3, 400, 411, 4096);
    #endif


    uint32_t ecgFIFO, readECGSamples, idx, ETAG[32], status;
    int16_t ecgSample[32];
    const int EINT_STATUS_MASK =  1 << 23;
    const int FIFO_OVF_MASK =  0x7;
    const int FIFO_VALID_SAMPLE_MASK =  0x0;
    const int FIFO_FAST_SAMPLE_MASK =  0x1;
    const int ETAG_BITS_MASK = 0x7;



    while(1) {
        #if 1
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
        #else
        static float y1, y2, y3;

        static const float alpha = 0.001;

        if(max86150_check()>0) {
            while(max86150_available()) {
                uint32_t a = max86150_getFIFORed();
                uint32_t b = max86150_getFIFOIR();
                //if( a & (0x1 << 17) ) {
                //    a |= 0xFFFC0000;
                //}

                //int32_t b = (int32_t)a;
                //int32_t b = (int16_t)max86150_getFIFOEcg();
                int16_t c = (int16_t)max86150_getFIFOECG();
                c = fir(c);

                y1 = (1. - alpha) * y1 + alpha * a;
                y2 = (1. - alpha) * y2 + alpha * b;
                y3 = (1. - alpha) * y3 + alpha * c;

                //printf("%d,%d,%d\n", a - y1, b - y2, c - y3);
                //printf("%d,%d,%d\n", a, b, c);

                add_sample(c - y3);
                max86150_nextSample();
            }
        }
        #endif
    }
}
