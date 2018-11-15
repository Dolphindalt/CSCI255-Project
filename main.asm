; File: main.asm
; Author: Dalton Caron
; Description: Hi

            .cdecls C,LIST,"msp430.h"       ; Include device header file
            .cdecls C,LIST,"controller.h"

nop1		.macro							; built NOPs using 3.4.6.33 of manual
			bic		#0,r5
			.endm
nop2		.macro
			jmp		$+2
			.endm
nop3		.macro
			nop1
			nop2
			.endm
nop4		.macro
			nop2
			nop2
			.endm
nop5		.macro
			mov		@r4,0(r4)
			.endm
nop6		.macro
			mov		0(r4),0(r4)
			.endm
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


; Main loop here obviously
MAIN

			mov.w	#CALBC1_16MHZ,&BCSCTL1	; 16 MHZ
			mov.w	#CALDCO_16MHZ,&DCOCTL

			call 	#controller_init

			;mov.w	#0x13,&P1DIR			; view these on scope
			;mov.w	#0x11,&P1SEL

			bis.w	#GIE,SR					; enable interrupts

forever		jmp 	forever

gbc_tx
			push.w	r10						; 3 cycles
			push.w	r11						; 3 cycles
			; size in bytes r13
			; buffer pointer r12
byte_loop
			mov.b	@r12+,r10				; 4 cycles	move byte to r10 and increment pointer
			mov.b	#08h,r11				; 4 cycle	count 8 bits in r11
bit_loop
			bic.b	#01h,&P1OUT				; 4 cycles	p1.0 low
			rla.b	r10						; 2 cycles	shift byte left 1
			jnc		low_signal				; 2 cycles 	jump if carry bit was reset

											; delay 1 cycle

			bis.b	#01h,&P1OUT				; 4 cycles	p1.0 high

			jmp 	finish_bit				; 2 cycles
low_signal
											; delay 6 cycles

			bis.b	#01h,&P1OUT				; 4 cycles	p1.0 high
finish_bit
			dec.b	r11						; 2 cycle	decrement the bit count
			jz		next_byte				; 2 cycles	jump if all bytes set

											; delay 6 cycles

			jmp		bit_loop				; 2 cycles
next_byte
			dec.b	r13						; 2 cycles	decrement length
			jz		exit					; 2 cycles	len is zero so we are done

											; delay 3 cycles

			jmp		byte_loop				; 2 cycles	continue to send bytes
exit										; 			need to send stop bit

											; delay 1 cycle

			bic.b	#01h,&P1OUT				; 4 cycle	p1.0 low

											; delay 6

			bis.b	#01h,&P1OUT				; 4 cycle	p1.0 high

			pop.w	r11						; 3 cycles
			pop.w	r10						; 3 cycles
			ret								; 5 cycles

gbc_rx
			; size in bytes r13
			; buffer pointer r12
			bic.b	#01h,P1DIR

			bis.b	#01h,P1DIR
			ret

edge_test
			bic.b	#01h,&P1OUT		; high
			nop1					; delay 1
			bis.b	#01h,&P1OUT
			nop6					; delay 6
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			ret

brute_force
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; high
			nop1					; delay 1
			bis.b	#01h,&P1OUT
			nop6					; delay 6
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1

			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1

			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1

			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; high
			nop1					; delay 1
			bis.b	#01h,&P1OUT
			nop6					; delay 6
			bic.b	#01h,&P1OUT		; high
			nop1					; delay 1
			bis.b	#01h,&P1OUT
			nop6					; delay 6

			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1

			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; low
			nop6					; delay 6
			bis.b	#01h,&P1OUT
			nop1					; delay 1
			bic.b	#01h,&P1OUT		; high
			nop1					; delay 1
			bis.b	#01h,&P1OUT
			nop6					; delay 6
			bic.b	#01h,&P1OUT		; high
			nop1					; delay 1
			bis.b	#01h,&P1OUT
			nop6					; delay 6

			bic.b	#01h,&P1OUT

			ret
; Extern stuff
			.global gbc_tx
			.global gbc_rx
			.global edge_test
			.global brute_force
; Stack Pointer definition
            .global __STACK_END
            .sect   .stack
            
; Interrupt Vectors
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
            ; PORT1_VECTOR            ".int02"        			  /* 0xFFE4 Port 1 */
            ; PORT2_VECTOR            ".int03"                    /* 0xFFE6 Port 2 */
            ; ADC10_VECTOR            ".int05"                    /* 0xFFEA ADC10 */
            ; USCIAB0TX_VECTOR        ".int06"                    /* 0xFFEC USCI A0/B0
            ; USCIAB0RX_VECTOR        ".int07"                    /* 0xFFEE USCI A0/B0 Receive */
            ; TIMER0_A1_VECTOR        ".int08"                    /* 0xFFF0 Timer0)A CC1, TA0 */
            ; TIMER0_A0_VECTOR        ".int09"                    /* 0xFFF2 Timer0_A CC0 */
            ; WDT_VECTOR              ".int10"                    /* 0xFFF4 Watchdog Timer */
            ; COMPARATORA_VECTOR      ".int11"                    /* 0xFFF6 Comparator A */
            ; TIMER1_A1_VECTOR        ".int12"                    /* 0xFFF8 Timer1_A CC1-4, TA1 */
            ; TIMER1_A0_VECTOR        ".int13"                    /* 0xFFFA Timer1_A CC0 */
            ; NMI_VECTOR              ".int14"                    /* 0xFFFC Non-maskable */
            ; RESET_VECTOR            ".reset"                    /* 0xFFFE Reset [Highest Priority] */
         	.sect ".data"




