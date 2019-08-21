source init.gdb

set confirm off

mon max32xxx mass_erase 0
mon max32xxx mass_erase 1

echo #### BOOTLOADER ####\n
load build/bootloader/bootloader.elf
echo #### EPICARDIUM ####\n
load build/epicardium/epicardium.elf
echo #### PYCARDIUM ####\n
load build/pycardium/pycardium.elf
reset
quit
