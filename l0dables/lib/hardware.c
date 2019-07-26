/*
 * Hardware routines for l0dables.
 *
 * You shouldn't have to do much here. SystemInit/SystemCoreClockUpdate are
 * called automatically before main(), and provide you with a sensible execution
 * environment.
 *
 * However, if you wish, you can define your own SystemInit and take over the
 * initialization before main() gets called.
 */

#include "max32665.h"
#include "mxc_sys.h"
#include "gcr_regs.h"
#include "icc_regs.h"
#include "pwrseq_regs.h"

uint32_t SystemCoreClock = HIRC_FREQ >> 1; 

void SystemCoreClockUpdate(void)
{
	uint32_t base_freq, div, clk_src;

	// Determine the clock source and frequency
	clk_src = (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL);
	switch (clk_src)
	{
		case MXC_S_GCR_CLKCN_CLKSEL_HIRC:
			base_freq = HIRC_FREQ;
			break;
		case MXC_S_GCR_CLKCN_CLKSEL_XTAL32M:
			base_freq = XTAL32M_FREQ;
			break;
		case MXC_S_GCR_CLKCN_CLKSEL_LIRC8:
			base_freq = LIRC8_FREQ;
			break;
		case MXC_S_GCR_CLKCN_CLKSEL_HIRC96:
			base_freq = HIRC96_FREQ;
			break;
		case MXC_S_GCR_CLKCN_CLKSEL_HIRC8:
			base_freq = HIRC8_FREQ;
			break;
		case MXC_S_GCR_CLKCN_CLKSEL_XTAL32K:
			base_freq = XTAL32K_FREQ;
			break;
		default:
		// Values 001 and 111 are reserved, and should never be encountered.
		base_freq = HIRC_FREQ;
			break;
	}
	// Clock divider is retrieved to compute system clock
	div = (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC) >> MXC_F_GCR_CLKCN_PSC_POS;

	SystemCoreClock = base_freq >> div;
}

__weak void SystemInit() {
	// Enable FPU.
	SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
	__DSB();
	__ISB();

	// Enable ICache1 Clock
	MXC_GCR->perckcn1 &= ~(1 << 22);

	// Invalidate cache and wait until ready
	MXC_ICC1->invalidate = 1;
	while (!(MXC_ICC1->cache_ctrl & MXC_F_ICC_CACHE_CTRL_CACHE_RDY));

	// Enable Cache
	MXC_ICC1->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_CACHE_EN;

	SystemCoreClockUpdate();
}
