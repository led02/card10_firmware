#TARGET = max32665
#COMPILER = GCC

#export TARGET
#export COMPILER

all: sdk subdirs

travis:
	$(MAKE) ADDFLAGS=-Werror lib subdirs

subdirs:
	$(MAKE) -C bootloader

sdk/Libraries/MAX32665PeriphDriver/Build/PeriphDriver.a:
#	$(MAKE) -C sdk/Libraries/MAX32665PeriphDriver

sdk/Libraries/MAXUSB/Build/maxusb.a:
#	$(MAKE) -C sdk/Libraries/MAXUSB

sdk: sdk/Libraries/MAX32665PeriphDriver/Build/PeriphDriver.a sdk/Libraries/MAXUSB/Build/maxusb.a

clean:
	$(MAKE) -C bootloader clean
#	$(MAKE) -C sdk/Libraries/MAX32665PeriphDriver clean
#	$(MAKE) -C sdk/Libraries/MAXUSB clean

