.dseg
dfase:		.byte 4
stepSize:	.byte 1
apos:		.byte 1
.cseg
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
	ldiw X,dfase
.endm

.macro cnctest
	cnctestloop:
		x_add
	brne cnctestloop
.endm

.macro parse

.endm

.macro cnc	;x=@0,y=@1;y=@2
	
.endm

.macro x_add
	lds t0,apos
	andi t0,0x3
	x_loop:
		delay 250
		ldiw X,dfase
		add XL,t0
		cbr t1,$FF
		adc XH,t1
		ld t1,X
		out portb,t1
		inc t0
		cpi t0,4
		brne PC+2
		ldi	t0,0
	rjmp x_loop
.endm

.macro x_sub

.endm

.macro y_add

.endm

.macro y_sub

.endm
