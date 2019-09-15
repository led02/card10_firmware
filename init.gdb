target remote localhost:3333

define reset
    mon mww 0x40000004 0x80000000
end


# usage: task_backtrace <tskTCB*>
define task_backtrace
    set $taskbt_task_ptr = $arg0
    set $taskbt_stack_ptr = $taskbt_task_ptr->pxTopOfStack

    set $taskbt_frame_offset = 9
    if ((*(uint32_t*)($taskbt_stack_ptr + 8)) & 0x10 == 0)
        echo FPU is on\n
        set $taskbt_frame_offset += 16
    else
        echo FPU is off\n
    end

    set $taskbt_reg_lr = $lr
    set $taskbt_reg_pc = $pc
    set $taskbt_reg_sp = $sp

    set $lr = *($taskbt_stack_ptr + $taskbt_frame_offset + 5)
    set $pc = *($taskbt_stack_ptr + $taskbt_frame_offset + 6)
    set $sp = $taskbt_stack_ptr + $taskbt_frame_offset + 8

    bt

    set $lr = $taskbt_reg_lr
    set $pc = $taskbt_reg_pc
    set $sp = $taskbt_reg_sp
end
alias tbt = task_backtrace
