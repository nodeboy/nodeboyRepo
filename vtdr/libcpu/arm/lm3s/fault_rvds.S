;/*
; * File      : context.S
; * This file is part of RT-Thread RTOS
; * COPYRIGHT (C) 2006, RT-Thread Development Team
; *
; * The license and distribution terms for this file may be
; * found in the file LICENSE in this distribution or at
; * http://www.rt-thread.org/license/LICENSE
; *
; * Change Logs:
; * Date           Author       Notes
; * 2009-01-17     Bernard      first version
; */

	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8

	IMPORT rt_hw_hard_fault_exception

rt_hw_hard_fault	PROC
	EXPORT rt_hw_hard_fault

	; get current context 
	MRS     r0, psp                 ; get fault thread stack pointer
	PUSH    {lr}
	BL		rt_hw_hard_fault_exception
	POP     {lr}

	ORR     lr, lr, #0x04
	BX		lr
	ENDP
	
	END