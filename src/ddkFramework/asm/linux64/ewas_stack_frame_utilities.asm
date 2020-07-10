
global set_curr_thread_stack_base
global set_curr_thread_stack_limit
global get_curr_thread_stack_base
global get_curr_thread_stack_limit
global consolidate_frame

section .text

set_curr_thread_stack_base:

	ret

set_curr_thread_stack_limit:

	ret

get_curr_thread_stack_base:

	ret

get_curr_thread_stack_limit:

	ret

consolidate_frame:

	push rbp

	mov rbp, rsp

	sub rsp, 028h

	call rdx

	add rsp, 028h

    pop rbp

	ret
