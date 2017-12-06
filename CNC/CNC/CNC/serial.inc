.dseg
inbuf: .byte 100
outbuf: .byte 100
.cseg
;------------------------------------------------------------------
; initSerial: Inicializa el serial
;------------------------------------------------------------------
.macro initSerial
	save
		ldi t1, high(fosc/(16*baud)-1)
		ldi t0, low(fosc/(16*baud)-1)
		output UBRR0H, t1
		output UBRR0L, t0 
		;habilitar rx y tx
		ldi t0, (1<<RXEN0)|(1<<TXEN0) 
		output UCSR0B,t0
		;formato: 8data, 1stop bit 
		ldi t0, (1<<UCSZ01) | (1<<UCSZ00) 
		output UCSR0C,t0
	load
.endm
.macro testSerial
	testSerialloop:
	putci 0
	rjmp testSerialloop
.endm
;------------------------------------------------------------------
; getc: Obtiene un caracter desde serial
;------------------------------------------------------------------
.macro getc ; @0= destino {r0, ... , r31} @1-echo mode 
	save
		getc_RX:
			lds t0,UCSR0A 
			sbrs t0,RXC0 
		rjmp getc_RX 
	load
		lds @0, UDR0
		.if	@1==0x1
			putc @0			
		.endif
.endm

;------------------------------------------------------------------
; getn: Obtiene un numero desde serial
;------------------------------------------------------------------
.macro getn ; @0= destino {r0, ... , r31} @1-echo mode 
	save
		getn_RX:
			lds t0,UCSR0A 
			sbrs t0,RXC0 
		rjmp getn_RX 
		lds t0, UDR0
		mov t1,t0
		subi t0,0x30
		brmi PC+4
		cpi t0,10
		brge PC+2
		rjmp PC+2
		rjmp getn_RX
		putc t1
	load
		lds @0,UDR0
		subi @0,0x30
.endm

;------------------------------------------------------------------
; gets: Obtiene una serie de caracteres hasta llegar a un salto de linea
;------------------------------------------------------------------
.macro fgets
	save
		pushw Z
		ldiw Z, inbuf
		clr t1
		gets_RX:
			getc t0,0
			cpi t0,13
			breq gets_out
			cpi t0,8;detectar retroceso
			brne gets_continue
				cpi t1,0
				breq gets_RX
				dec t1
				sbiw Z,1
				putc t0
				rjmp gets_RX
			gets_continue:
			putc t0
			st Z,t0
			inc t1
			adiw Z,1
			cpi t1,99
			brne gets_RX
			gets_overflow:
				rjmp gets_out
		gets_out:
		sti Z,'$'
		popw Z
	load
.endm

;------------------------------------------------------------------
; puts: Imprime una cadena a serial | cuidado no es fail-proof la cadena debe terminar con un signo de $
;------------------------------------------------------------------
.macro puts ; @0 = $0000-$ffff
	save
		pushw Z
		ldiw Z, (@0<<1)
		puts_ciclo:
			lpm t0,Z+
			cpi t0,'$'
			breq puts_out
			putc t0
		rjmp puts_ciclo
		puts_out:
		.if @1==1
			putci 13
			putci 10
		.endif
		popw Z
	load
.endm

;------------------------------------------------------------------
; putc: Imprime un caracter a serial
;------------------------------------------------------------------
.macro putc ; @0 = registro
	save
		putc_TX: 
			lds t0,UCSR0A
			sbrs t0,UDRE0
		rjmp putc_TX
	load
		output UDR0,@0
.endm

;------------------------------------------------------------------
; putci: Imprime un caracter a serial desde constante
;------------------------------------------------------------------
.macro putci ; @0 = constante
	save
		putci_TX: 
			lds t0,UCSR0A
			sbrs t0,UDRE0
		rjmp putci_TX
		ldi t0,@0
		output UDR0,t0
	load
.endm

;------------------------------------------------------------------
; debug: Imprime el valor de un registro
;------------------------------------------------------------------
.macro debug; @0 = registro
	save
		push r18
		push r19
		putci 13
		putci 10
		mov t0,@0
		ldi t1,10
		call debug_ciclo
		pop r19
		pop r18
	load
	rjmp debug_out
		debug_ciclo:
			cpi t0,0
			breq debug_ciclo_out
			div8
			mov t0,r18
			addi r19,0x30
			push r19
			call debug_ciclo
			pop r19
			putc r19
			debug_ciclo_out:
		ret
	debug_out:
.endm