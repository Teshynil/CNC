.dseg
dfase:		.byte 4
apos:		.byte 1
.cseg
.def xi	= r16
.def yi	= r17
.def zi	= r18
.def xpos = r23
.def ypos = r24
.def zpos = r25
.def objx = r20
.def objy = r21
.def objz = r22
.equ limit = 50
.macro initCNC ;
	stsi dfase,0x5
	stsi dfase+1,0x6
	stsi dfase+2,0xa
	stsi dfase+3,0x9
	stsi apos,0x00
.endm

.macro parse
	getc t0
	cpi t0,'G'
	breq PC+2
call error
	getn t0
	cpi t0,0
	breq PC+2
call error
	getn t0
	cpi t0,0
	brne PC+2
call G0
	cpi t0,1
	brne PC+2
call G0
call error
.endm

.macro FG0
	getc t0
	cpi t0,'X'
	getn t0
	getn t1
	cpi t1,FF
	breq 
	
.endm

.macro x_add
	lds xi,apos
	andi xi,0x3; mascara de 2 bits para recuperar la posicion de rotacion en el array
	inc xi
	cpi xi,4
	brne PC+2
	ldi xi,0
	ldiw X, dfase
	addwb X,xi
	lds t1,apos
	andi t1,0xFC; mascara inversa para guardar
	or xi,t1
	sts apos,xi
	ld t0,X
	in t1,portb
	andi t1,$F0
	or t0,t1
	out portb,t0;UUUUXXXX
	inc xpos
.endm

.macro x_sub
	lds xi,apos
	andi xi,0x3; mascara de 2 bits para recuperar la posicion de rotacion en el array
	dec xi
	cpi xi,0xFF
	brne PC+2
	ldi xi,3
	ldiw X, dfase
	addwb X,xi
	lds t1,apos
	andi t1,0xFC; mascara inversa para guardar
	or xi,t1
	sts apos,xi
	ld t0,X
	in t1,portb
	andi t1,$F0
	or t0,t1
	out portb,t0;UUUUXXXX
	dec xpos
.endm

.macro y_add
	lds yi,apos
	andi yi,0x18; mascara de 2 bits para recuperar la posicion de rotacion en el array
	lsr yi lsr yi lsr yi
	inc yi
	cpi yi,4
	brne PC+2
	ldi yi,0
	ldiw Y, dfase
	addwb Y,yi
	lsl yi lsl yi lsl yi
	lds t0,apos
	andi t0,0xE7; mascara inversa para guardar
	or yi,t0
	sts apos,yi
	ld t0,Y
	swap t0
	in t1,portb
	andi t1,$0F
	or t0,t1
	out portb,t0;YYYYUUUU
	inc ypos
.endm

.macro y_sub
	lds yi,apos
	andi yi,0x18; mascara de 2 bits para recuperar la posicion de rotacion en el array
	lsr yi lsr yi lsr yi
	dec yi
	cpi yi,0xFF
	brne PC+2
	ldi yi,3
	ldiw Y, dfase
	addwb Y,yi
	lsl yi lsl yi lsl yi
	lds t0,apos
	andi t0,0xE7; mascara inversa para guardar
	or yi,t0
	sts apos,yi
	ld t0,Y
	swap t0
	in t1,portb
	andi t1,$0F
	or t0,t1
	out portb,t0;YYYYUUUU
	dec ypos
.endm

.macro z_add
	lds zi,apos
	andi zi,0xC0; mascara de 2 bits para recuperar la posicion de rotacion en el array
	lsr zi lsr zi lsr zi lsr zi lsr zi lsr zi
	inc zi
	cpi zi,4
	brne PC+2
	ldi zi,0
	ldiw Z, dfase
	addwb Z,zi
	lds t0,apos
	lsl zi lsl zi lsl zi lsl zi lsl zi lsl zi
	andi t0,0x3F; mascara inversa para guardar
	or zi,t0
	sts apos,zi
	ld t0,Z
	in t1,portc
	andi t1,$F0
	or t0,t1
	out portc,t0;UUUZZZZ
	inc zpos
.endm

.macro z_sub
	lds zi,apos
	andi zi,0xC0; mascara de 2 bits para recuperar la posicion de rotacion en el array
	lsr zi lsr zi lsr zi lsr zi lsr zi lsr zi
	dec zi
	cpi zi,0xFF
	brne PC+2
	ldi zi,3
	ldiw Z, dfase
	addwb Z,zi
	lds t0,apos
	lsl zi lsl zi lsl zi lsl zi lsl zi lsl zi
	andi t0,0x3F; mascara inversa para guardar
	or zi,t0
	sts apos,zi
	ld t0,Z
	in t1,portc
	andi t1,$F0
	or t0,t1
	out portc,t0;UUUZZZZ
	dec zpos
.endm

;======================TESTS;==========================
.macro cnctestz2
	startcncz2test:
	ldi t0,50
		cnct2z1:
		call zsub
			delay 50
			dec t0
		brne cnct2z1
	ldi t0,50
		cnct2z2:
		call zadd
			delay 50
			dec t0
		brne cnct2z2
	rjmp startcncz2test
.endm
.macro cnctestx2
	startcncx2test:
	ldi t0,50
		cnct2x1:
		call xsub
			delay 100
			dec t0
		brne cnct2x1
	ldi t0,50
		cnct2x2:
		call xadd
			delay 100
			dec t0
		brne cnct2x2
	rjmp startcncx2test
.endm
.macro cnctestz
	startcncztest:
	ldi t0,50
		cnctz:
		call zsub
			delay 50
			dec t0
		brne cnctz
	rjmp startcncztest
.endm

.macro cnctestmax
	startcncmaxtest:
	ldi t0,100
		cnctmax1:
		call xadd
		call yadd
		//call zadd
			delay 50
			dec t0
		brne cnctmax1
	ldi t0,100
		cnctmax3:
		call xsub
		//call yadd
		//call zadd
			delay 50
			dec t0
		brne cnctmax3
	ldi t0,100
		cnctmax2:
		call xadd
		call ysub
		//call zsub
			delay 50
			dec t0
		brne cnctmax2
	ldi t0,100
		cnctmax4:
		call xsub
		//call zsub
			delay 50
			dec t0
		brne cnctmax4
	
.endm
.macro cnctestcube
starttest:
	ldi t0,50
	cnl1:
	call xadd
		delay 100
		dec t0
	brne cnl1
	ldi t0,50
	cnl2:
	call yadd
		delay 100
		dec t0
	brne cnl2
	ldi t0,50
	cnl3:
	call zadd
		delay 100
		dec t0
	brne cnl3
	ldi t0,50
	cnl4:
	call xsub
		delay 100
		dec t0
	brne cnl4
	ldi t0,50
	cnl5:
	call ysub
		delay 100
		dec t0
	brne cnl5
	ldi t0,50
	cnl6:
	call zsub
		delay 100
		dec t0
	brne cnl6
.endm