source init.gdb

set confirm off

echo #### BOOTLOADER ####\n
load build/bootloader/bootloader.elf
reset
quit
