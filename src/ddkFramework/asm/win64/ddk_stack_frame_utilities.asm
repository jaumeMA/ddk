

Context         STRUCT
reg_P1Home		QWORD	?
reg_P2Home		QWORD	?
reg_P3Home		QWORD	?
reg_P4Home		QWORD	?
reg_P5Home		QWORD	?
reg_P6Home		QWORD	?
reg_ContextFlags DWORD	?
reg_MxCsr		DWORD	?
reg_SegCs		WORD	?
reg_SegDs		WORD	?
reg_SegEs		WORD	?
reg_SegFs		WORD	?
reg_SegGs		WORD	?
reg_SegSs		WORD	?
reg_EFlags		DWORD	?
reg_Dr0			QWORD	?
reg_Dr1			QWORD	?
reg_Dr2			QWORD	?
reg_Dr3			QWORD	?
reg_Dr6			QWORD	?
reg_Dr7			QWORD	?
reg_Rax			QWORD   ?
reg_Rcx			QWORD   ?
reg_Rdx			QWORD   ?
reg_Rbx			QWORD   ?
reg_Rsp			QWORD   ?
reg_Rbp			QWORD   ?
reg_Rsi			QWORD   ?
reg_Rdi			QWORD   ?
reg_R8 			QWORD   ?
reg_R9 			QWORD   ?
reg_R10			QWORD   ?
reg_R11			QWORD   ?
reg_R12			QWORD   ?
reg_R13			QWORD   ?
reg_R14			QWORD   ?
reg_R15			QWORD   ?
reg_Rip			QWORD	?
Context         ENDS

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

	sub rsp, 08h

	call r8

	add rsp, 010h

	pop rbp

	pop rax

	mov rcx, (Context PTR [rax]).reg_Rcx
	mov rdx, (Context PTR [rax]).reg_Rdx
	mov rbx, (Context PTR [rax]).reg_Rbx
	mov rsp, (Context PTR [rax]).reg_Rsp
	mov rbp, (Context PTR [rax]).reg_Rbp
	mov rdi, (Context PTR [rax]).reg_Rdi
	mov rsi, (Context PTR [rax]).reg_Rsi
	mov r8, (Context PTR [rax]).reg_R8
	mov r9, (Context PTR [rax]).reg_R9
	mov r10, (Context PTR [rax]).reg_R10
	mov r11, (Context PTR [rax]).reg_R11
	mov r12, (Context PTR [rax]).reg_R12
	mov r13, (Context PTR [rax]).reg_R13
	mov r14, (Context PTR [rax]).reg_R14
	mov r15, (Context PTR [rax]).reg_R15

	jmp (Context PTR [rax]).reg_Rip

	ret

consolidate_frame ENDP

END