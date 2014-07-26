; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"


.cseg
; Zacatek programu - po resetu
.org 0
jmp start



; preruseni - doimplementuj
.org 0x000A
	jmp interrupt






; Zacatek programu - hlavni program
.org 0x100
start:
    ; Inicializace zasobniku
	ldi r16, 0x00 ;FF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16

    ; Inicializace displeje
	call init_disp

	; *** ZDE muzeme psat nase instrukce

	ldi r25, 0 ; 1. cislo
	ldi r26, 0 ; 2. cislo
	ldi r27, 0 ; operand / mode
	ldi r31, 0 ; blink
	


/* PRERUSENI */

cli ; globální zakázání pøerušení
ldi r16, 0b00001000
sts ASSR, r16 ; vyber hodiny od 
; krystalového 
; oscilátoru 32768 Hz
ldi r16, 0b00000001
sts TIMSK2, r16 ; povolení pøerušení
; od èasovaèe 2
ldi r16, 0b00000100											// 110 = pul sekunda, 101 = sekunda
sts TCCR2A, r16 ; spuštìní èítaèe s 
; dìlicím pomìrem 128
clr r16 ; zákaz pøerušení od
out EIMSK, r16 ; joysticku
//sei ; globální povolení pøerušení

	ldi r25, 255 ; 1 cislo
	ldi r26, 255 ; 2 cislo
	ldi r29, 1  ; pozice

	; init joysticku
	in r19, DDRE
	andi r19, 0b11110011
	in r18, PORTE
	ori r18, 0b00001100
	out DDRE, r19
	out PORTE, r18 
	ldi r18, 0b00000000
	sts DIDR1, r18
	in r19, DDRB
	andi r19, 0b00101111
	in r18, PORTB
	ori r18, 0b11010000
	out DDRB, r19
	out PORTB, r18



	everloop:
	
	in r24, PINB ; D0 - NONE, 90 - UP, C0 - CENTER, 50 - DOWN
	andi r24, 0b11010000

	in r21, PINE ; 0C - NONE, 08 - LEFT, 04 - RIGHT
	andi r21, 0b00001100

	; cekajici loop
sei

	ldi r22, 255
cycle2:
	ldi r28, 255
cycle:
	NOP
	dec r28
	brne cycle
	dec r22
	brne cycle2
	
cli

	in r22, PINB
	andi r22, 0b11010000
	in r23, PINE
	andi r23, 0b00001100

	cp r24, r22
	brne everloop

	cp r21, r23
	brne everloop

	jmp noteq
	
noteq:
	
	cpi r24, 0xC0 ; CENTER
	breq center

	cpi r24, 0x90 ; UP
	breq up

	cpi r24, 0x50 ; DOWN
	breq down

	cpi r21, 0x08 ; LEFT
	breq left

	cpi r21, 0x04 ; RIGHT
	breq right

	//ldi r29, 0
		/*jmp continue*/
	jmp everloop

center: // zmena modu
	cpi r29, 0
	breq moresult

	ldi r29, 0
	jmp everloop //continue

up:
	//ldi r29, 1
	
	cpi r29, 1
	breq afst1

	cpi r29, 2
	breq afst2

	cpi r29, 3
	breq aoper

	cpi r29, 4
	breq asnd1

	cpi r29, 5
	breq asnd2

	jmp everloop //continue

down:
	//ldi r29, 2

	cpi r29, 1
	breq sfst1

	cpi r29, 2
	breq sfst2

	cpi r29, 3
	breq soper

	cpi r29, 4
	breq ssnd1

	cpi r29, 5
	breq ssnd2

	jmp everloop //continue

left:
	//ldi r29, 3
	cpi r29, 2
	brlo jmpever //continue

	dec r29
	jmp everloop //continue

right:
	//ldi r29, 4
	cpi r29, 5
	brge jmpever //continue

	cpi r29, 0	
	breq jmpever //continue

	inc r29
	jmp everloop //continue

jmpever:
	jmp everloop

moresult:
	ldi r29, 1
	jmp everloop //continue
/******************/
afst1:
	ldi r28, 16
	add r25, r28
	jmp continue

afst2:
	inc r25
	jmp continue

aoper:
	cpi r27, 2
	breq aopoflow

	inc r27				// add looping
	jmp continue

aopoflow:
	ldi r27, 0
	jmp continue

asnd1:
	ldi r28, 16
	add r26, r28
	jmp continue

asnd2:
	inc r26
	jmp continue
/*********************/
sfst1:
	ldi r28, 16
	sub r25, r28
	jmp continue

sfst2:
	dec r25
	jmp continue

soper:
	cpi r27, 0
	breq sopoflow

	dec r27				// add looping
	jmp continue

sopoflow:
	ldi r27, 2
	jmp continue

ssnd1:
	ldi r28, 16
	sub r26, r28
	jmp continue

ssnd2:
	dec r26
	jmp continue
/**********************/







/* R29 =  ovladani */

continue:

	; prevedeni do jednoho registru
	cpi r29, 0
	breq display22


/*	PRVNI CISLO		*/
	; osmibitove cislo
	MOV r19, r25

	MOV r20, r19
	ANDI r20,$0F
	CPI r20, 10
	BRLO mensi
	JMP vetsi

return:
	ldi r16, 0	; znak
	MOV R16, R20
	ldi r17, 3      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak


	ANDI r19,$F0
	SWAP r19
	CPI r19, 10
	BRLO mensi2
	JMP vetsi2

return2:
	MOV R16, R19
	ldi r17, 2      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak

/*	OPERAND		*/
	cpi r27, 0
	breq opadd

	cpi r27, 1
	breq opsub

	cpi r27, 2
	breq opmul

return5:

	/* do r16 loadni znak*/
	ldi r17, 4      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak



/*	DRUHE CISLO 	*/
; osmibitove cislo
	MOV r19, r26

	MOV r20, r19
	ANDI r20,$0F
	CPI r20, 10
	BRLO mensi3
	JMP vetsi3

return3:
	ldi r16, 0	; znak
	MOV R16, R20
	ldi r17, 6      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak



	ANDI r19,$F0
	SWAP r19
	CPI r19, 10
	BRLO mensi4
	JMP vetsi4

return4:
	MOV R16, R19
	ldi r17, 5      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak


	cpi r31, 1
	breq blink

	reti
	JMP everloop
	; Zastavime program - nekonecna smycka
	rjmp PC

display22:
	jmp display2

jmpcont:
	jmp continue

mensi:
	LDI r30, 48
	ADD r20, r30
	JMP return

vetsi:
	LDI r30, 55
	ADD r20, r30
	JMP return

mensi2:
	LDI r30, 48
	ADD r19, r30
	JMP return2

vetsi2:
	LDI r30, 55
	ADD r19, r30
	JMP return2

opadd:
	ldi r16, 43
	jmp return5

opsub:
	ldi r16, 45
	jmp return5

opmul:
	ldi r16, 42
	jmp return5

mensi3:
	LDI r30, 48
	ADD r20, r30
	JMP return3

vetsi3:
	LDI r30, 55
	ADD r20, r30
	JMP return3

mensi4:
	LDI r30, 48
	ADD r19, r30
	JMP return4

vetsi4:
	LDI r30, 55
	ADD r19, r30
	JMP return4

interrupt:
	cpi r31, 0
	breq inter1
	jmp inter0

inter1:
	cpi r29, 0
	breq jmpcont

	ldi r31, 1
	jmp continue

inter0:
	cpi r29, 0
	breq jmpcont

	ldi r31, 0
	jmp continue

blink:
	LDI R16, 32
	mov r17, r29      ; pozice (zacinaji od 2)
	inc r17
	call show_char  ; zobraz znak
	jmp everloop

scitani:
	ADD zl, R25
	ADC zl, R26
	ADC zh, zh
	jmp vratse

odcitani:
	mov zl, r25
	sub zl, r26
	sbc zh, zh
	jmp vratse

nasobeni:
	mul r26, r25
	mov zl, r0
	mov zh, r1
	jmp vratse

display2:
/* POCITANI & ZOBRAZOVANI VYSLEDKU 	R30+R31*/
	LDI R30, 0
	LDI R31, 0

	cpi r27, 0
	breq scitani

	cpi r27, 1
	breq odcitani

	jmp nasobeni

vratse:

/*	PRVNI CISLO		*/
	; osmibitove cislo
	MOV r19, r31

	MOV r20, r19
	ANDI r20,$0F
	CPI r20, 10
	BRLO mensiv1
	JMP vetsiv1

returnv1:
	MOV r16, R20	; znak
	ldi r17, 3      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak


	ANDI r19,$F0
	SWAP r19
	CPI r19, 10
	BRLO mensiv2
	JMP vetsiv2

returnv2:
	MOV R16, R19
	ldi r17, 2      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak

/*	DRUHE CISLO		*/
	; osmibitove cislo
	MOV r19, r30

	MOV r20, r19
	ANDI r20,$0F
	CPI r20, 10
	BRLO mensiv3
	JMP vetsiv3

returnv3:
	MOV r16, R20	; znak
	ldi r17, 5      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak


	ANDI r19,$F0
	SWAP r19
	CPI r19, 10
	BRLO mensiv4
	JMP vetsiv4

returnv4:
	MOV R16, R19
	ldi r17, 4      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak

	ldi R16, 32
	ldi r17, 6      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak

	jmp everloop


mensiv1:
	LDI r18, 48
	ADD r20, r18
	JMP returnv1

mensiv2:
	LDI r18, 48
	ADD r19, r18
	JMP returnv2

vetsiv1:
	LDI r18, 55
	ADD r20, r18
	JMP returnv1

vetsiv2:
	LDI r18, 55
	ADD r19, r18
	JMP returnv2

mensiv3:
	LDI r18, 48
	ADD r20, r18
	JMP returnv3

mensiv4:
	LDI r18, 48
	ADD r19, r18
	JMP returnv4

vetsiv3:
	LDI r18, 55
	ADD r20, r18
	JMP returnv3

vetsiv4:
	LDI r18, 55
	ADD r19, r18
	JMP returnv4
