;------------------------------------------------------------------
; colección de macros
;------------------------------------------------------------------
; Sintaxis:
; 	.macro NOMBRE_MACRO
; 		; cuerpo de la macro
;		; los parámetros de la macro se referencian como
;		; @0 (1er parámetro), @1 (2do parámetro), etc.
; 	.endm
;------------------------------------------------------------------
.def t0 = r16
.def t1 = r17
;------------------------------------------------------------------
; save: Salva los registros temporales t0,t1
;------------------------------------------------------------------
.macro	save
	push t0
	push t1
.endm
;------------------------------------------------------------------
; load: Recupera los registros temporales t0,t1
;------------------------------------------------------------------
.macro	load
	pop t1
	pop t0
.endm

;------------------------------------------------------------------
; input: usa "in"/"lds" según la dirección del registro que se lee.
;------------------------------------------------------------------
.macro	input ; @0= destino {r0, ... , r31}
              ; @1= fuente I/0 ($0000-$FFFF)
.if	@1<0x40
	in	@0,@1	; si dir del reg de E/S <0x40 uso "in"
.else
	lds	@0,@1	; sino uso "lds"
.endif
.endm

;------------------------------------------------------------------
; output: usa "out"/"sts" según la dirección del registro que se escribe.
;------------------------------------------------------------------
.macro	output		; @0= destino I/O ($0000-$FFFF)
			; @1= fuente, cte o r0..r31
.if	@0<0x40
		out	@0,@1	; si dir del reg de E/S <0x40 uso "out"
.else
		sts	@0,@1	; sino uso "sts"
.endif
.endm

.macro	ldiw		; carga puntero
		ldi	@0L, LOW(@1)
		ldi	@0H, HIGH(@1)
.endm

.macro	movi	; carga registro con constante
	save
		ldi	t0,@1
		mov	@0,t0
	load
.endm

.macro	addi	; carga registro con constante
	save
		ldi	t0,@1
		add	@0,t0
	load
.endm

.macro	outi	; envia a data/salida un constante
	save
		ldi		t0,@1
		output	@0,r16
	load
.endm

.macro	pushi	;@0		; Pone la constante @0 en la pila
	save
		ldi		t0,@0
		push	t0
	load
.endm

.macro	pushw	;@0		; Pone el puntero @0 de 16 bits en la pila
		push	@0L
		push	@0H
.endm


.macro	popw	;@0		; Saca el puntero @0 de 16 bits de la pila
		pop		@0H
		pop		@0L
.endm


.macro	pushio	;@0			; Pone en pila un registro de I/O
	save
		in		t0,@0	; usa la variable auxiliar t0
		push	t0
	load
.endm


.macro	popio	;@0			; Saca de pila un registro de I/O
	save
		pop		t0		; usa la variable auxiliar t0
		out		@0,t0
	load
.endm

.macro	sti		;@0,@1	; Guarda una constante de modo indirecto
	save
		ldi		t0,@1	; Usa: variable auxiliar "t0" y un puntero
		st		@0,t0	; [3 ciclos, 2 words] @0={X, Y, Z}
	load
.endm

.macro	stsi	;@0,@1	; Guarda una constante en SRAM
	save
		ldi		t0,@1	; Usa: variable auxiliar "t0".
		sts		@0,t0	; [3 ciclos, 2 words] @0={any SRAM ADDRESS}
	load
.endm

.macro	delay	;@0		; Inicia un delay de @0 ms
	save
		pushw Z
		ldiw Z, @0
		delay__delay:
		.if fosc==1000000
			ldi  t0, 2
			ldi  t1, 75
		L1: dec  t1
			brne L1
			dec  t0
			brne L1
			nop
		.elif fosc==8000000
			ldi  r18, 11
			ldi  r19, 99
		L1: dec  r19
			brne L1
			dec  r18
			brne L1
		.endif
		sbiw Z,1
		cpi ZL,0
		brne delay__delay
		cpi ZH,0
		brne delay__delay
		popw Z
	load
.endm