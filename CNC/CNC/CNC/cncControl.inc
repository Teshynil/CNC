.dseg
dfase:		.byte 4
stepSize:	.byte 1
apos:		.byte 1
.cseg
.def xi	= r16
.def yi	= r17
.def zi	= r18
.def posx = r23
.def posy = r24
.def posz = r25
.def objx = r20
.def objy = r21
.def objz = r22
.equ limit = 50
.macro initCNC ;@0=Numero de pasos por 1 pt en x,y,z
	stsi dfase,0x5
	stsi dfase+1,0x6
	stsi dfase+2,0xa
	stsi dfase+3,0x9
	sts stepSize,@0
	stsi apos,0x0
.endm

.macro cnctest
	ldi t0,50
	cnl1:
	call xadd
		dec t0
		delay 100
	brne cnl1
	ldi t0,50
	cnl2:
	call yadd
		dec t0
		delay 100
	brne cnl2
	ldi t0,50
	cnl3:
	call xsub
		dec t0
		delay 100
	brne cnl3
	ldi t0,50
	cnl4:
	call ysub
		dec t0
		delay 100
	brne cnl4
.endm

.macro parse

.endm

.macro cnc	;x=@0,y=@1;y=@2
	
.endm

.macro x_add
	lds xi,apos
	andi xi,0x3; mascara de 2 bits para recuperar la posicion de rotacion en el array
	inc xi
	cpi xi,4
	brne PC+2
	ldi xi,0
	lds t1,apos
	andi t1,0xFC; mascara inversa para guardar
	or xi,t1
	sts apos,xi
	ldiw X, dfase
	addwb X,xi
	ld t0,X
	out portb,t0;UUUUXXXX
.endm

.macro x_sub
	lds xi,apos
	andi xi,0x3; mascara de 2 bits para recuperar la posicion de rotacion en el array
	dec xi
	cpi xi,0xFF
	brne PC+2
	ldi xi,3
	lds t1,apos
	andi t1,0xFC; mascara inversa para guardar
	or xi,t1
	sts apos,xi
	ldiw X, dfase
	addwb X,xi
	ld t0,X
	out portb,t0;UUUUXXXX
.endm

.macro y_add
	lds yi,apos
	andi yi,0x18; mascara de 2 bits para recuperar la posicion de rotacion en el array
	inc yi
	cpi yi,4
	brne PC+2
	ldi yi,0
	lds t0,apos
	andi t0,0xE7; mascara inversa para guardar
	or yi,t0
	sts apos,yi
	ldiw Y, dfase
	addwb Y,yi
	ld t0,Y
	swap t0
	out portb,t0;YYYYUUUU
.endm

.macro y_sub
	lds yi,apos
	andi yi,0x18; mascara de 2 bits para recuperar la posicion de rotacion en el array
	dec yi
	cpi yi,0xFF
	brne PC+2
	ldi yi,3
	lds t0,apos
	andi t0,0xE7; mascara inversa para guardar
	or yi,t0
	sts apos,yi
	ldiw Y, dfase
	addwb Y,yi
	ld t0,Y
	swap t0
	out portb,t0;YYYYUUUU
.endm

.macro z_add
	lds zi,apos
	andi zi,0xC0; mascara de 2 bits para recuperar la posicion de rotacion en el array
	inc zi
	cpi zi,4
	brne PC+2
	ldi zi,0
	lds t0,apos
	andi t0,0x3F; mascara inversa para guardar
	or zi,t0
	sts apos,zi
	ldiw Z, dfase
	addwb Z,zi
	ld t0,Z
	swap t0
	out portc,t0;UUUZZZZ
.endm

.macro z_sub
	lds zi,apos
	andi zi,0xC0; mascara de 2 bits para recuperar la posicion de rotacion en el array
	dec zi
	cpi zi,0xFF
	brne PC+2
	ldi zi,3
	lds t0,apos
	andi t0,0x3F; mascara inversa para guardar
	or zi,t0
	sts apos,zi
	ldiw Z, dfase
	addwb Z,zi
	ld t0,Z
	swap t0
	out portc,t0;UUUZZZZ
.endm
