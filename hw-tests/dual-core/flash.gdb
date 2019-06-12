source ../../.gdbinit

set confirm off

echo Loading core1 image ...
file ../../build/hw-tests/dual-core/dual-core1.elf
load
echo Loading core0 image ...
file ../../build/hw-tests/dual-core/dual-core0.elf
load

reset
quit
