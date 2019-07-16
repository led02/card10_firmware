source ../../init.gdb

set confirm off

echo Loading core1 image ...
file ../../build/hw-tests/api-demo/api-demo-core1.elf
load
echo Loading core0 image ...
file ../../build/hw-tests/api-demo/api-demo-core0.elf
load

reset
quit
