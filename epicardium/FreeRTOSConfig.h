#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "max32665.h"

/* CMSIS keeps a global updated with current system clock in Hz */
#define configCPU_CLOCK_HZ          ((unsigned long)120000000)

/* TODO: Adjust this for tickless idle */
#define configTICK_RATE_HZ          ((portTickType)1000)

/* Memory */
#define configTOTAL_HEAP_SIZE       ((size_t)(26 * 1024))
#define configMINIMAL_STACK_SIZE    ((unsigned short)128)

#define configMAX_PRIORITIES        5
/* # of priority bits (configured in hardware) is provided by CMSIS */
#define configPRIO_BITS             __NVIC_PRIO_BITS
/* Priority 7, or 255 as only the top three bits are implemented.  This is the lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY       ( ( unsigned char ) 7 << ( 8 - configPRIO_BITS) )
/* Priority 5, or 160 as only the top three bits are implemented. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  ( ( unsigned char ) 5 << ( 8 - configPRIO_BITS) )  

/* We want to use preemption to easier integrate components */
#define configUSE_PREEMPTION        1

/* TODO: Adjust */
#define configUSE_IDLE_HOOK         0
#define configUSE_TICK_HOOK         0
#define configUSE_CO_ROUTINES       0
#define configUSE_16_BIT_TICKS      0
#define configUSE_MUTEXES           1

#define INCLUDE_vTaskSuspend        1
#define INCLUDE_vTaskDelay          1

/* Allow static allocation of data structures */
#define configSUPPORT_STATIC_ALLOCATION 1

/* Alias the default handler names to match CMSIS weak symbols */
#define vPortSVCHandler       SVC_Handler
#define xPortPendSVHandler    PendSV_Handler
#define xPortSysTickHandler   SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
