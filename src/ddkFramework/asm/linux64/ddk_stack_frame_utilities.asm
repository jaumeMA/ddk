
global set_curr_thread_stack
global get_curr_thread_stack
global consolidate_frame

section .text

set_curr_thread_stack:

	ret

get_curr_thread_stack:

	ret

frame_get_out:

    pop rdi
    pop rsi
    pop rbx
    pop rbp

    ret

__switch_frame:

    push rbp
    push rbx
    push rsi
    push rdi

    ret

consolidate_frame:

	push rbp

	mov rbp, rsp

	sub rsp, 028h

	call rdx

	add rsp, 028h

    pop rbp

	ret
