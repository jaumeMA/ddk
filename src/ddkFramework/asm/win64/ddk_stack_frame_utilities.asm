
Stack			STRUCT
init_s			QWORD	?
end_s			QWORD	?
dealloc_s		QWORD	?
alloc_s			QWORD	?
Stack			ENDS


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

set_curr_thread_stack PROC

	mov rbx, (Stack PTR [rcx]).init_s
	mov gs:[008h], rbx
	mov rbx, (Stack PTR [rcx]).end_s
	mov gs:[010h], rbx
	mov rbx, (Stack PTR [rcx]).dealloc_s
	mov gs:[01478h], rbx

	ret

set_curr_thread_stack ENDP

get_curr_thread_stack PROC

	mov rbx, gs:[008h]
	mov (Stack PTR [rcx]).init_s, rbx
	mov rbx, gs:[010h]
	mov (Stack PTR [rcx]).end_s, rbx
	mov rbx, gs:[01478h]
	mov (Stack PTR [rcx]).dealloc_s, rbx

	ret

get_curr_thread_stack ENDP

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
	mov (Context PTR [rcx]).reg_Rbp, rbp
	mov (Context PTR [rcx]).reg_Rsp, rsp 
	mov (Context PTR [rcx]).reg_R8, r8
	mov (Context PTR [rcx]).reg_R9, r9
	lea r10, [get_out]
	mov (Context PTR [rcx]).reg_Rip, r10

	mov r10, rdx

	mov rcx, (Context PTR [r10]).reg_Rcx
	mov rdx, (Context PTR [r10]).reg_Rdx
	mov rbp, (Context PTR [r10]).reg_Rbp
	mov rsp, (Context PTR [r10]).reg_Rsp
	mov r8, (Context PTR [r10]).reg_R8
	mov r9, (Context PTR [r10]).reg_R9

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

	mov rbp, (Context PTR [r8]).reg_Rbp
	mov rsp, (Context PTR [r8]).reg_Rsp
	jmp (Context PTR [r8]).reg_Rip

assert_point:
	int 3

	ret

consolidate_frame ENDP

END