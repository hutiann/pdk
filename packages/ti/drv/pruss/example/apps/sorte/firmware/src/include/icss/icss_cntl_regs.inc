; Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
;
; Redistributions of source code must retain the above copyright
; notice, this list of conditions and the following disclaimer.
;
; Redistributions in binary form must reproduce the above copyright
; notice, this list of conditions and the following disclaimer in the
; documentation and/or other materials provided with the
; distribution.
;
; Neither the name of Texas Instruments Incorporated nor the names of
; its contributors may be used to endorse or promote products derived
; from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
; "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
; LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
; OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
; SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
; LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
; file:   icss_cntl_regs.inc
;
; brief:  {ICSS Control Module Registers}
;
;  author     {Vijaya Sarathy}
;
;  version    0.1     Created
;  			  0.2	  Ported to PRU C Compiler 		{Thomas Mauer}
;
	.if ! $defined( __icss_cntl_regs_inc)
	.define "1", __icss_cntl_regs_inc
; ==== PRU Control Module Registers Offsets (from PRUx_CNTL$..._BASE) ====
PRUx_CNTL_CONTROL_OFFSET			.set	0x0000
PRUx_CNTL_STATUS_OFFSET				.set	0x0004
PRUx_CNTL_WKUP_OFFSET				.set	0x0008
PRUx_CNTL_CYCLE_COUNT_OFFSET 		.set	0x000C	; #of cycles PRU was enabled
PRUx_CNTL_STALL_COUNT_OFFSET 		.set	0x0010	; #of cycles PRU was stalled
PRUx_CNTL_CONST_IDX0_OFFSET			.set	0x0020	; Constant Table Block Index Reg 0
PRUx_CNTL_CONST_IDX1_OFFSET			.set	0x0024	; Constant Table Block Index Reg 1
PRUx_CNTL_CONST_PTR0_OFFSET			.set	0x0028	; Constant Table Pointer Reg 0
PRUx_CNTL_CONST_PTR1_OFFSET			.set	0x002C	; Constant Table Pointer Reg 1


ICSS_PRU0_CNTL_BASE				.set	0x22000
ICSS_PRU1_CNTL_BASE				.set	0x24000

; ==== PRU0 Control Module Registers ====
PRU0_CNTL_CONTROL_REG			.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_CONTROL_OFFSET)
PRU0_CNTL_STATUS_REG			.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_STATUS_OFFSET)
PRU0_CNTL_WKUP_REG				.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_WKUP_OFFSET)
PRU0_CNTL_CYCLE_COUNT_REG		.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_CYCLE_COUNT_OFFSET)
PRU0_CNTL_STALL_COUNT_REG		.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_STALL_COUNT_OFFSET)
PRU0_CNTL_CONST_IDX0_REG 		.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_CONST_IDX0_OFFSET)
PRU0_CNTL_CONST_IDX1_REG		.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_CONST_IDX1_OFFSET)
PRU0_CNTL_CONST_PTR0_REG		.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_CONST_PTR0_OFFSET)
PRU0_CNTL_CONST_PTR1_REg		.set	(ICSS_PRU0_CNTL_BASE + PRUx_CNTL_CONST_PTR1_OFFSET)

; ==== Peer PRU's Control Module Registers ====
PRUx_CNTLPEER_CONTROL_REG			.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_CONTROL_OFFSET)
PRUx_CNTLPEER_STATUS_REG			.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_STATUS_OFFSET)
PRUx_CNTLPEER_WKUP_REG				.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_WKUP_OFFSET)
PRUx_CNTLPEER_CYCLE_COUNT_REG		.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_CYCLE_COUNT_OFFSET)
PRUx_CNTLPEER_STALL_COUNT_REG		.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_STALL_COUNT_OFFSET)
PRUx_CNTLPEER_CONST_IDX0_REG 		.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_CONST_IDX0_OFFSET)
PRUx_CNTLPEER_CONST_IDX1_REG		.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_CONST_IDX1_OFFSET)
PRUx_CNTLPEER_CONST_PTR0_REG		.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_CONST_PTR0_OFFSET)
PRUx_CNTLPEER_CONST_PTR1_REg		.set	(ICSS_PRU1_CNTL_BASE + PRUx_CNTL_CONST_PTR1_OFFSET)
	.endif