target remote localhost:3333

define reset
    mon mww 0x40000004 0x80000000
end
