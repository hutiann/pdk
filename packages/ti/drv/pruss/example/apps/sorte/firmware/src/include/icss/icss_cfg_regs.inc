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

; file:   icss_cfg_regs.inc
;
; brief:  {ICSS Global Configuration Module Registers}
;
;  author     {Vijaya Sarathy}
;
;  version    0.1     Created
;  			  0.2	  Ported to PRU C Compiler 		{Thomas Mauer}
;
	.if ! $defined( __icss_cfg_regs_inc)
	.define "1", __icss_cfg_regs_inc
ICSS_CFG_REVID		.set	0x0000	; Module Revision ID
ICSS_CFG_SYSCFG		.set	0x0004	; System Idle/Standby Configuration
ICSS_CFG_GPCFG0		.set	0x0008	; GP IO Configuration Register 0
ICSS_CFG_GPCFG1		.set	0x000C	; GP IO Configuration Register 1
ICSS_CFG_CGR		.set	0x0010	; Clock Gating Register
ICSS_CFG_ISRP		.set	0x0014	; IRQ Status RAW Parity
ICSS_CFG_ISP		.set	0x0018	; IRQ Status Parity (Masked)
ICSS_CFG_IESP		.set	0x001C	; IRQ Enable Set Parity
ICSS_CFG_IECP		.set	0x0020	; IRQ Enable Clear Parity (ie., Disable)
ICSS_CFG_SCRP		.set	0x0024	; SCR Interconnect Arbitration Priority
ICSS_CFG_PMAO		.set	0x0028	; PRU Master OCP Address Offset
ICSS_CFG_MIIRTEN	.set	0x002C	; MII RealTime Module Enable
ICSS_CFG_IEPCLK		.set	0x0030	; Industrial Ethernet Peripheral Clocking
ICSS_CFG_SPPC		.set	0x0034	; Scratch PAD priority and config
ICSS_CFG_MUXSEL		.set	0x0040	; Mux sel for SA
	.endif
