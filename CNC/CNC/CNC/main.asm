.include "m328pdef.inc"		; definición de registros, bits y constantes del micro 
.include "avrmacros.inc"	; macros
.include "cncControl.inc"	; manejo de la cnc
.include "div.inc"			; division [No es totalmente mio]
.include "serial.inc"		; manejo de la conexion serial
;recordatorio t0,t1 es temporal r16,r17
;==========================================
;Inicializacion de variables
.cseg
msg_config: .db "Inserta la cantidad de pasos que representa un punto(1-8): ",'$'
msg_error_config: .db "Valor invalido intenta otra vez",13,10,'$'
msg_welcome: .db "Inserte su codigo G debe finalizar con el codigo M00:",13,10,'$'
.equ   baud=4800      ;baudios
.equ   fosc=1000000   ;frecuencia
;.equ   fosc=8000000   ;frecuencia
start:
;==========================================
;Inicializacion del stack
	ldi	t0,HIGH(RAMEND)
	out	SPH,t0
	ldi	t0,LOW(RAMEND)
	out	SPL,t0
;==========================================
;Inicializacion de puertos
	outi DDRB,0xFF
	outi DDRC,0x0F
	outi PORTB,0xFF
	outi PORTC,0x0F
;==========================================
	initSerial
	call clear
config_loop:
	/*puts msg_config,0
	getn t0,1
	cpi t0,0
	breq error
	cpi t0,9
	breq error*/
	initCNC t0
	cnctest
	call clear
	puts msg_welcome,0
	call loop
error:
	call println
	puts msg_error_config,0
	rjmp config_loop

loop:
	putci '>'
	call gets
	call println
	delay 1000
rjmp loop
ret

gets:
	fgets
ret
println:
	putci 13
	putci 10
ret
 
clear:
	save
		putci 033
		putci '['
		putci '2'
		putci 'J'
		putci 033
		putci '['
		putci '0'
		putci ';'
		putci '0'
		putci 'H'
	load
ret
ldi t0,60
		clear_loop:
			call println
			dec t0
			cpi t0,0
		brne clear_loop