

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

	ret

set_curr_thread_stack_limit ENDP

set_curr_thread_stack_dealloc PROC

	mov gs:[01478h], rcx

	ret

set_curr_thread_stack_dealloc ENDP

get_curr_thread_stack_base PROC

	mov rax, gs:[008h]

	ret

get_curr_thread_stack_base ENDP

get_curr_thread_stack_limit PROC

	mov rax, gs:[010h]

	ret

get_curr_thread_stack_limit ENDP

get_curr_thread_stack_dealloc PROC

	mov rax, gs:[01478h]

	ret

get_curr_thread_stack_dealloc ENDP

switch_frame PROC

	mov (Context PTR [rcx]).reg_Rcx, rcx
	mov (Context PTR [rcx]).reg_Rdx, rdx 
	mov (Context PTR [rcx]).reg_Rbx, rbx 
	mov (Context PTR [rcx]).reg_Rsp, rsp 
	mov (Context PTR [rcx]).reg_Rbp, rbp 
	mov (Context PTR [rcx]).reg_Rdi, rdi 
	mov (Context PTR [rcx]).reg_Rsi, rsi 
	mov (Context PTR [rcx]).reg_Rax, rax
	mov (Context PTR [rcx]).reg_R8, r8
	mov (Context PTR [rcx]).reg_R9, r9
	mov (Context PTR [rcx]).reg_R10, r10 
	mov (Context PTR [rcx]).reg_R11, r11 
	mov (Context PTR [rcx]).reg_R12, r12 
	mov (Context PTR [rcx]).reg_R13, r13 
	mov (Context PTR [rcx]).reg_R14, r14
	mov (Context PTR [rcx]).reg_R15, r15 
	lea r10, [get_out]
	mov (Context PTR [rcx]).reg_Rip, r10

	mov r10, rdx

	mov rcx, (Context PTR [r10]).reg_Rcx
	mov rdx, (Context PTR [r10]).reg_Rdx
	mov rbx, (Context PTR [r10]).reg_Rbx
	mov rsp, (Context PTR [r10]).reg_Rsp
	mov rbp, (Context PTR [r10]).reg_Rbp
	mov rdi, (Context PTR [r10]).reg_Rdi
	mov rsi, (Context PTR [r10]).reg_Rsi
	mov rax, (Context PTR [r10]).reg_Rax
	mov r8, (Context PTR [r10]).reg_R8
	mov r9, (Context PTR [r10]).reg_R9
	mov r11, (Context PTR [r10]).reg_R11
	mov r12, (Context PTR [r10]).reg_R12
	mov r13, (Context PTR [r10]).reg_R13
	mov r14, (Context PTR [r10]).reg_R14
	mov r15, (Context PTR [r10]).reg_R15

	jmp (Context PTR [r10]).reg_Rip

get_out:
	ret

switch_frame ENDP

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

	pop r8

	cmp (Context PTR [r8]).reg_Rip, 00h
	je assert_point

	mov rcx, (Context PTR [r8]).reg_Rcx
	mov rdx, (Context PTR [r8]).reg_Rdx
	mov rbx, (Context PTR [r8]).reg_Rbx
	mov rsp, (Context PTR [r8]).reg_Rsp
	mov rbp, (Context PTR [r8]).reg_Rbp
	mov rdi, (Context PTR [r8]).reg_Rdi
	mov rsi, (Context PTR [r8]).reg_Rsi
	mov rax, (Context PTR [r8]).reg_Rax
	mov r9, (Context PTR [r8]).reg_R9
	mov r10, (Context PTR [r8]).reg_R10
	mov r11, (Context PTR [r8]).reg_R11
	mov r12, (Context PTR [r8]).reg_R12
	mov r13, (Context PTR [r8]).reg_R13
	mov r14, (Context PTR [r8]).reg_R14
	mov r15, (Context PTR [r8]).reg_R15

	jmp (Context PTR [r8]).reg_Rip

assert_point:
	int 3

	ret

consolidate_frame ENDP

END