#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "card10.h"
#include "tmr_utils.h"

int main(void)
{
    int count = 0;

    card10_init();
    card10_diag();

    printf("API Test.\n");

    while(1) {
        printf("count = %d\n", count++);

        TMR_Delay(MXC_TMR0, SEC(1), 0);
    }
}
