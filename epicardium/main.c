#include <stdio.h>
#include "card10.h"

int main(void)
{
	card10_init();
	card10_diag();

	printf("Staring core1 payload ...\n");
	core1_start();
}
