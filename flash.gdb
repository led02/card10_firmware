target remote localhost:3333

define reset
    mon mww 0x40000004 0x80000000
end
load build/bootloader/bootloader.elf
load build/epicardium/epicardium.elf
load build/pycardium/pycardium.elf
quit
