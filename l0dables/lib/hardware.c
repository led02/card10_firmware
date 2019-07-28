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

#include <stddef.h>
#include "epicardium.h"

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

// newlib syscall to allow printf to work.
long _write(int fd, const char *buf, size_t cnt)
{
	// Only print one line at a time.  Insert `\r` between lines so
	// they are properly displayed on the serial console.
	size_t i, last = 0;
	for (i = 0; i < cnt; i++) {
		if (buf[i] == '\n') {
			epic_uart_write_str(&buf[last], i - last);
			epic_uart_write_str("\r", 1);
			last = i;
		}
	}
	epic_uart_write_str(&buf[last], cnt - last);
	return cnt;
}

// newlib syscall to allow for a heap
extern uint32_t __heap_start;
uint32_t _sbrk(int incr)
{
	static char *brk = NULL;
	if (brk == NULL) {
		brk = (char *)&__heap_start;
	}

	// Ensure we don't overflow the heap by checking agsinst the current stack
	// pointer (the heap grows towards the stack, and vice-versa).
	//
	// This is only a last-ditch attempt at saving ourselves from memory
	// corruption. It doesn't prevent the stack from growing into the heap
	// first.

	void *sp;
	__asm__ __volatile__ ("mov %0, sp" : "=r"(sp));

	// Require a 'safe margin' of 4k between the heap and stack.
	uint32_t stack_bottom = (uint32_t)sp - 4096;

	if (((uint32_t)brk + incr) > stack_bottom) {
		errno = ENOMEM;
		return (uint32_t)-1;
	}

	char *prev_brk = brk;
	brk += incr;
	return (uint32_t)prev_brk;
}
