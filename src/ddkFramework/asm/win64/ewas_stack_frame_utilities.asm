

.code

set_curr_thread_stack_base PROC

	mov gs:[008h], rcx

	ret

set_curr_thread_stack_base ENDP

set_curr_thread_stack_limit PROC

	mov gs:[010h], rcx
	mov gs:[01478h], rcx

	ret

set_curr_thread_stack_limit ENDP

get_curr_thread_stack_base PROC

	mov rax, gs:[008h]

	ret

get_curr_thread_stack_base ENDP

get_curr_thread_stack_limit PROC

	mov rax, gs:[010h]

	ret

get_curr_thread_stack_limit ENDP

consolidate_frame PROC FRAME
	push rbp
.pushreg rbp
	mov rbp, rsp
.setframe rbp,00h
.endprolog

	sub rsp, 020h

	call r8

	add rsp, 028h

	pop rbp

	pop rsi

	pop rdi

	mov rsp, [rsi]

	mov rsi, [rdi]

	jmp rsi

	ret

consolidate_frame ENDP

END