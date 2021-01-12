
global get_curr_thread_stack
global set_curr_thread_stack
global consolidate_frame

section .text

get_curr_thread_stack:

	ret

set_curr_thread_stack:

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
