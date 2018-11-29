; File: main.asm
; Author: Dalton Caron, Kaleb Bausch, Jackson Jenkins
; Code: SUPA DUPA ROVA'LDESTROYA BOT
; Description: This little dude will blast music and destroy
; eletronics like no one has ever seen before.

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

			mov.w	#CALBC1_1MHZ,&BCSCTL1	; 16 MHZ
			mov.w	#CALDCO_1MHZ,&DCOCTL	; what the fuck
											; this cost me days
			call 	#controller_init

			bis.w	#GIE,SR					; enable interrupts

forever
			jmp 	forever

gbc_rw		; abandon all hope, ye who dare enter here
			push.w	r10						; 3 cycles
			push.w	r11						; 3 cycles
			push.w	r7
			push.w	r8
			push.w	r9
			push.w  r6
			push.w	r5
			bic.w	#GIE,SR					; disable interrupts
			; out buffer size in bytes r15
			; out buffer pointer r14
			; in buffer size in bytes r13
			; in buffer pointer r12

			; timeout counter r9
			; input state r8
			; clobber timeout r7
			; temp pin state r6
			; bytes read r5
			xor r7,r7
			xor	r8,r8
			xor	r9,r9
			xor r5,r5
			xor r6,r6
byte_loop
			mov.b	@r12+,r10				; 4 cycles	move byte to r10 and increment pointer
			mov.b	#08h,r11				; 4 cycle	count 8 bits in r11
bit_loop
			bic.b	#01h,&P1OUT				; 4 cycles	p1.0 low
			rla.b	r10						; 2 cycles	shift byte left 1
			jnc		low_signal				; 2 cycles 	jump if carry bit was reset

			nop6							; delay 12 cycles
			nop6

			bis.b	#01h,&P1OUT				; 4 cycles	p1.0 high

			nop6							; delay 30 cycles
			nop6
			nop6
			nop6
			nop6

			jmp 	finish_bit				; 2 cycles
low_signal
			nop6							; delay 43 cycles
			nop6
			nop6
			nop6
			nop6
			nop6
			nop6
			nop1

			bis.b	#01h,&P1OUT				; 4 cycles	p1.0 high
finish_bit
			dec.b	r11						; 2 cycle	decrement the bit count
			jz		next_byte				; 2 cycles	jump if all bytes set

			nop6							; delay 10 cycles
			nop4

			jmp		bit_loop				; 2 cycles
next_byte
			dec.b	r13						; 2 cycles	decrement length
			jz		exit					; 2 cycles	len is zero so we are done

			nop4							; delay 3 cycles

			jmp		byte_loop				; 2 cycles	continue to send bytes
exit										; 			need to send stop bit

			nop6							; delay 8 cycles
			nop2

			bic.b	#01h,&P1OUT				; 4 cycle	p1.0 low

			nop6							; delay 6
			nop6
			nop2

			bis.b	#01h,&P1OUT				; 4 cycle	p1.0 high

			nop6

			mov.b	#08h,r11				; bit counter
			mov.b	#00h,r10				; bytes got
											; r9 timeout counter is 0 still
			bic.b	#01h,&P1DIR				; p1.0 to input
wait_low_extra
			xor.b	r9,r9
wait_low_extra_loop_1
			mov.b	#P1IN,r8				; read pin
			and.b	#01h,r8					; compare with mask
			jz		wait_for_measure		; measure if low
											; or we are high still
			dec.b	r9						; remember r9 is a timeout counter
			jnz		wait_low_extra_loop_1	; huge ass name, loop if counter not zero
											; 2us extra loop
			xor.b	r9,r9					; timeout to zero
wait_low_extra_loop_2
			mov.b	P1IN,r8					; read pin
			and.b	#01h,r8					; compare with mask
			jz		wait_for_measure		; measure if low
											; or we are high still
			dec.b	r9						; remember r9 is a timeout counter
			jnz		wait_low_extra_loop_2	; why so long, loop if counter not zero

wait_for_low
			mov.b	#80h,r9					; 128 chances to get this right
wait_for_low_loop
			mov.b	P1IN,r8					; read pin
			and.b	#01h,r8					; mask pin
			jz		wait_for_measure		; measure if low
											; or we are still high
			dec.b	r9						; decrement timeout counter
			jnz		wait_for_low_loop		; loop again if not timed out

			dec.b	r7						; decrease clobber timeout
			jnz		wait_low_extra			; if not zero, wait extra
			jmp 	end						; we timed out
wait_for_measure
			nop6							; delay 23 cycles
			nop6
			nop6
			nop2
			nop3

			mov.b	#01h,r7					; timeout disabled

			rla.b	r6						; shift left current byte in out buffer
			mov.b 	P1IN,r8					; read pin
			and.b	#01h,r8					; bitmask pin
			jz		check_bit_count			; skip set if low
			bis.b	#01h,r6					; set buffer high if pin high
check_bit_count
			dec.b	r11						; dec bit counter register
			jnz		wait_for_high			; check if we got a whole byte
											; else we got a whole byte
			; actually moving the read bit into the buffer here
			mov.b	r6,0(r14)				; save read data and increment mem ptr
			inc.w	r14
			inc.b	r5						; return type inc
			mov.b	#08h,r11				; bit counter back to 8
			cmp.b	r15,r5					; compare bytes read vs sizeof buffer
			jz		end						; if 0, we got all the bytes

wait_for_high
			mov.b	#80h,r9					; 128 chances to get this right
wait_for_high_loop
			mov.b	P1IN,r8					; read pin
			and.b	#01h,r8					; mask value
			jnz		wait_for_low			; with line high, next
											; or still low?
			dec.b	r9						; dec timeout counter
			jnz		wait_for_high_loop		; loop if not time
											; we timed out
end
			mov.b	r5,r12					; move bytes read to r12 so it will return
			bis.b	#01h,&P1DIR				; p1.0 to output
			pop.w	r5						; too many god damn registers
			pop.w	r6
			pop.w	r9
			pop.w	r8
			pop.w	r7
			pop.w	r11						; 3 cycles
			pop.w	r10						; 3 cycles
			bis.w	#GIE,SR					; enable interrupts
			ret								; 5 cycles
; Holy shit, it is finally over
; Extern stuff
			.global gbc_rw
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




