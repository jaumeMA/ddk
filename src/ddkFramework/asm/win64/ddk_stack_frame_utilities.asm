
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

	mov r8, (Stack PTR [rcx]).init_s
	mov gs:[008h], r8
	mov r8, (Stack PTR [rcx]).end_s
	mov gs:[010h], r8
	mov r8, (Stack PTR [rcx]).dealloc_s
	mov gs:[01478h], r8

	ret

set_curr_thread_stack ENDP

get_curr_thread_stack PROC

	mov r8, gs:[008h]
	mov (Stack PTR [rcx]).init_s, r8
	mov r8, gs:[010h]
	mov (Stack PTR [rcx]).end_s, r8
	mov r8, gs:[01478h]
	mov (Stack PTR [rcx]).dealloc_s, r8

	ret

get_curr_thread_stack ENDP

frame_get_out PROC

	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbx
	pop rbp

	ret

frame_get_out ENDP

__switch_frame PROC

	push rbp
	push rbx
	push rsi
	push rdi
	push r12
	push r13
	push r14
	push r15
	mov (Context PTR [rcx]).reg_Rsp, rsp

	mov rsp, (Context PTR [rdx]).reg_Rsp
	jmp (Context PTR [rdx]).reg_Rip

	int 3

__switch_frame ENDP

consolidate_frame PROC FRAME
	pop r8
	pop rdx
	pop rcx
	lea r9, [frame_get_out]
	pop r10
	mov (Context PTR [r10]).reg_Rip, r9
	pop r11
	mov (Context PTR [r11]).reg_Rip, r9
	push r10
	push r11
	push rbp
.pushreg rbp
	mov rbp, rsp
.setframe rbp,00h
.endprolog

	sub rsp, 18h

	call r8

	add rsp, 20h

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
