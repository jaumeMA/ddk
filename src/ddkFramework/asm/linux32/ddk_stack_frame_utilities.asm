
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

    push ebp

	mov ebp, esp

    sub esp, 016h

    push dword [ebp + 0ch]

    push dword [ebp + 08h]

    mov eax, dword [ebp + 010h]

	call eax

	add esp, 010h

    nop

    leave

	ret
