.dseg
inbuf: .byte 100
outbuf: .byte 100
pointer: .byte 1
.cseg
;------------------------------------------------------------------
; initSerial: Inicializa el serial
;------------------------------------------------------------------
.macro initSerial
	save
			sts pointer,0
	load
.endm

;------------------------------------------------------------------
; getc: Obtiene un caracter desde serial
;------------------------------------------------------------------
.macro getc ; @0= destino {r0, ... , r31} @1-echo mode 
pushw Z
	save
		lds t0,pointer
		ldiw Z, (GCODE<<1)
		addwb Z,t0
		inc t0
		sts pointer,t0
	load
		lpm @0,Z
popw Z
.endm
;------------------------------------------------------------------
; touch: Obtiene un caracter desde serial no lo quita del buffer
;------------------------------------------------------------------
.macro touch ; @0= destino {r0, ... , r31} @1-echo mode 
pushw Z
	save
		lds t0,pointer
		ldiw Z, (GCODE<<1)
		addwb Z,t0
	load
		lpm @0,Z
popw Z
.endm
;------------------------------------------------------------------
; getn: Obtiene un numero desde serial
;------------------------------------------------------------------
.macro getn ; @0= destino {r0, ... , r31} retorna $ff si error
	save
		touch t0
		mov t1,t0
		subi t0,0x30
		brmi PC+4
		cpi t0,10
		brge PC+2
		rjmp PC+2
		rjmp getn_out
	load
		getc @0
		subi @0,0x30
	rjmp getn_out
		getn_error:
			ldi @0,$FF
	getn_out:	
.endm