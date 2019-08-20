#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <hidapi/hidapi.h>

int main(int argc, char *argv[])
{
	int rc = 0;
	if( (rc = hid_init()) ){
		fprintf(stderr, "hid_init: %d\n", rc);
		goto done;
	}

	struct hid_device_info *hid_devs = hid_enumerate(0x0d28, 0x0204);
	if( !hid_devs ){
		fprintf(stderr, "hid_enumerate: NULL\n");
		rc = 1; goto done;
	}

	for( struct hid_device_info *dev = hid_devs; dev ; dev = dev->next ){
		fprintf(stdout, "%s\n", dev->path);
	}

done:
	if( hid_devs ){ hid_free_enumeration(hid_devs); }
	hid_exit();
	return -1;
}
