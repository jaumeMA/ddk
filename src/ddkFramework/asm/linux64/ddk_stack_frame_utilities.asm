
global set_curr_thread_stack
global get_curr_thread_stack
global consolidate_frame

section .text

set_curr_thread_stack:

	ret

get_curr_thread_stack:

	ret

consolidate_frame:

	push rbp

	mov rbp, rsp

	sub rsp, 020h

	call rdx

	add rsp, 020h

    pop rbp

	ret
