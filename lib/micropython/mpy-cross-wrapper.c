#include <stdio.h>
#include <unistd.h>

#ifndef MPY_CROSS_PATH
#error "You need to define MPY_CROSS_PATH to compile this wrapper."
#endif

int main(int argc, char**argv)
{
	char path[] = MPY_CROSS_PATH "/mpy-cross";
	argv[0] = "mpy-cross";
	execv(path, argv);
	fprintf(stderr, "Failed to run '%s'!\n", path);
}
