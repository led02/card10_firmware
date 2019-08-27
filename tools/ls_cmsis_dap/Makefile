.PHONY: all clean

all: ls_cmsis_dap-hidraw ls_cmsis_dap-libusb

clean:
	-rm *.o ls_cmsis_dap-hidraw ls_cmsis_dap-libusb

ls_cmsis_dap.o: ls_cmsis_dap.c

ls_cmsis_dap-hidraw: LDFLAGS=-lhidapi-hidraw
ls_cmsis_dap-hidraw: ls_cmsis_dap.o
	$(CC) $(LDFLAGS) -o $@ $<

ls_cmsis_dap-libusb: LDFLAGS=-lhidapi-libusb
ls_cmsis_dap-libusb: ls_cmsis_dap.o
	$(CC) $(LDFLAGS) -o $@ $<
