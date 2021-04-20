
global get_curr_thread_stack
global set_curr_thread_stack
global consolidate_frame

struc CONTEXT

  .REG_GS: resd 1
  .REG_FS: resd 1
  .REG_ES: resd 1
  .REG_DS: resd 1
  .REG_EDI: resd 1
  .REG_ESI: resd 1
  .REG_EBP: resd 1
  .REG_ESP: resd 1
  .REG_EBX: resd 1
  .REG_EDX: resd 1
  .REG_ECX: resd 1
  .REG_EAX: resd 1
  .REG_TRAPNO: resd 1
  .REG_ERR: resd 1
  .REG_EIP: resd 1
  .REG_CS: resd 1
  .REG_EFL: resd 1
  .REG_UESP: resd 1
  .REG_SS: resd 1

endstruc

section .text

get_curr_thread_stack:

	ret

set_curr_thread_stack:

	ret

frame_get_out:

	pop edi
	pop esi
	pop ebx
	pop ebp

	ret

__switch_frame:

	push ebp
	push ebx
	push esi
	push edi

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
