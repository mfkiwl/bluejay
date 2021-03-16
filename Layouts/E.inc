;**** **** **** **** ****
;
; Bluejay digital ESC firmware for controlling brushless motors in multirotors
;
; Copyright 2020, 2021 Mathias Rasmussen
; Copyright 2011, 2012 Steffen Skaug
;
; This file is part of Bluejay.
;
; Bluejay is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Bluejay is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with Bluejay.  If not, see <http://www.gnu.org/licenses/>.
;
;**** **** **** **** ****
;
; Hardware definition file "E". Equals "A", but with LED control. And with HIP2103/4 driver initialization
; L1 L0 RC X  MC MB MA CC    X  L2 Cc Cp Bc Bp Ac Ap
;
;**** **** **** **** ****

PWM_ACTIVE_HIGH	EQU	1		; Pwm non-inverted
COMPWM_ACTIVE_HIGH	EQU	1		; Damping inverted

COMPARATOR_PORT	EQU	0		; All comparator (mux) pins must be on the same port

IF FETON_DELAY == 0
	PCA0CPM_POWER	EQU	PCA0CPM0
	PCA0CPL_POWER	EQU	PCA0CPL0
	PCA0CPH_POWER	EQU	PCA0CPH0

	PCA0CPM_DAMP	EQU	PCA0CPM1
	PCA0CPL_DAMP	EQU	PCA0CPL1
	PCA0CPH_DAMP	EQU	PCA0CPH1
ELSE
	PCA0CPM_POWER	EQU	PCA0CPM0
	PCA0CPL_POWER	EQU	PCA0CPL0
	PCA0CPH_POWER	EQU	PCA0CPH0

	PCA0CPM_DAMP	EQU	PCA0CPM1
	PCA0CPL_DAMP	EQU	PCA0CPL1
	PCA0CPH_DAMP	EQU	PCA0CPH1
ENDIF

;*********************
; PORT 0 definitions *
;*********************
LED_1		EQU	7
LED_0		EQU	6
Rcp_In		EQU	5
;			EQU	4
C_Mux		EQU	3
B_Mux		EQU	2
A_Mux		EQU	1
V_Mux		EQU	0

P0_DIGITAL	EQU	NOT((1 SHL A_Mux) + (1 SHL B_Mux) + (1 SHL C_Mux) + (1 SHL V_Mux))
P0_INIT		EQU	NOT((1 SHL LED_0) + (1 SHL LED_1))
P0_PUSHPULL	EQU	(1 SHL LED_0) + (1 SHL LED_1)
P0_SKIP		EQU	0FFh


;*********************
; PORT 1 definitions *
;*********************
;			EQU	7
LED_2		EQU	6
C_com		EQU	5
C_pwm		EQU	4
B_com		EQU	3
B_pwm		EQU	2
A_com		EQU	1
A_pwm		EQU	0


P1_DIGITAL	EQU	(1 SHL A_pwm) + (1 SHL B_pwm) + (1 SHL C_pwm) + (1 SHL A_com) + (1 SHL B_com) + (1 SHL C_com) + (1 SHL LED_2)
P1_INIT		EQU	00h
P1_PUSHPULL	EQU	(1 SHL A_pwm) + (1 SHL B_pwm) + (1 SHL C_pwm) + (1 SHL A_com) + (1 SHL B_com) + (1 SHL C_com) + (1 SHL LED_2)
P1_SKIP		EQU	0FFh


;*********************
; PORT 2 definitions *
;*********************
DebugPin		EQU	0

P2_DIGITAL	EQU	(1 SHL DebugPin)
P2_PUSHPULL	EQU	(1 SHL DebugPin)
P2_SKIP		EQU	0FFh


;**** **** **** **** ****
; ESC specific
;**** **** **** **** ****
Initialize_Xbar MACRO
	mov	XBR2, #40h	;; Xbar enabled
	mov	XBR1, #02h	;; CEX0 and CEX1 routed to pins

	All_pwmFETs_off	;; For unlocking of HIP2103/4 driver circuits
	call	wait100ms
	AcomFET_on
	BcomFET_on
	CcomFET_on
	call	wait1ms
	All_comFETs_off
ENDM

Set_Pwm_Polarity MACRO
IF PCA0CPM_POWER == PCA0CPM0
	mov	PCA0POL, #((COMPWM_ACTIVE_HIGH SHL 1) + (1 - PWM_ACTIVE_HIGH))
ELSE
	mov	PCA0POL, #(((1 - PWM_ACTIVE_HIGH) SHL 1) + COMPWM_ACTIVE_HIGH)
ENDIF
ENDM


;**** **** **** **** ****
; LED configuration
;**** **** **** **** ****
Set_LED_0 MACRO
	setb	P0.LED_0
ENDM

Clear_LED_0 MACRO
	clr	P0.LED_0
ENDM

Set_LED_1 MACRO
	setb	P0.LED_1
ENDM

Clear_LED_1 MACRO
	clr	P0.LED_1
ENDM

Set_LED_2 MACRO
	setb	P1.LED_2
ENDM

Clear_LED_2 MACRO
	clr	P1.LED_2
ENDM

Set_LED_3 MACRO
ENDM

Clear_LED_3 MACRO
ENDM


;**** **** **** **** ****
; Inherit base layout
;**** **** **** **** ****
$set(CUSTOM_PWM_SETUP, CUSTOM_LED)
$include (Base.inc)