;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (MINGW64)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"labo1_dado.c"
	list	p=12f683
	radix dec
	include "p12f683.inc"
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_delay

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0070
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_labo1_dado_0	udata
r0x1008	res	1
r0x1001	res	1
r0x1000	res	1
r0x1002	res	1
r0x1003	res	1
r0x1004	res	1
r0x1005	res	1
r0x1006	res	1
r0x1007	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------
;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_labo1_dado	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;2 compiler assigned registers:
;   r0x1008
;   STK00
;; Starting pCode block
S_labo1_dado__main	code
_main:
; 2 exit points
;	.line	30; "labo1_dado.c"	TRISIO = 0b00001000; // ponemos pin 4 como entrada
	MOVLW	0x08
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	32; "labo1_dado.c"	GPIO = 0x00; //Poner pines en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
_00115_DS_:
;	.line	39; "labo1_dado.c"	if(GP3 == 0){
	BANKSEL	_GPIObits
	BTFSC	_GPIObits,3
	GOTO	_00113_DS_
;	.line	43; "labo1_dado.c"	switch (num){
	MOVLW	0x05
	BANKSEL	r0x1008
	MOVWF	r0x1008
	MOVLW	HIGH(_00129_DS_)
	BANKSEL	PCLATH
	MOVWF	PCLATH
	MOVLW	_00129_DS_
	BANKSEL	r0x1008
	ADDWF	r0x1008,W
	BTFSS	STATUS,0
	GOTO	_00001_DS_
	BANKSEL	PCLATH
	INCF	PCLATH,F
_00001_DS_:
	MOVWF	PCL
_00129_DS_:
	GOTO	_00105_DS_
	GOTO	_00106_DS_
	GOTO	_00107_DS_
	GOTO	_00108_DS_
	GOTO	_00109_DS_
	GOTO	_00110_DS_
_00105_DS_:
;	.line	45; "labo1_dado.c"	GPIO = 0b0000001;	//(Se prende 1 LED)
	MOVLW	0x01
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	46; "labo1_dado.c"	delay(time);
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	47; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00106_DS_:
;	.line	50; "labo1_dado.c"	GPIO = 0b0000010;	//(Se prende 2 LED)
	MOVLW	0x02
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	51; "labo1_dado.c"	delay(time);
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	52; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00107_DS_:
;	.line	55; "labo1_dado.c"	GPIO = 0b0000100;	//(Se prende 3 LED)
	MOVLW	0x04
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	56; "labo1_dado.c"	delay(time);
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	57; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00108_DS_:
;	.line	60; "labo1_dado.c"	GPIO = 0b0000101;	//(Se prende 3 LED + 1 LED = 4 LED)
	MOVLW	0x05
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	61; "labo1_dado.c"	delay(time);
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	62; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00109_DS_:
;	.line	65; "labo1_dado.c"	GPIO = 0b0000110;	//(Se prende 3 LED + 2 LED = 5 LED)
	MOVLW	0x06
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	66; "labo1_dado.c"	delay(time);
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	67; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00110_DS_:
;	.line	70; "labo1_dado.c"	GPIO = 0b0000111;	//(Se prende 3 LED + 2 LED + 1 LED = 6 LED)
	MOVLW	0x07
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	71; "labo1_dado.c"	delay(time);
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0x80
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
_00113_DS_:
;	.line	86; "labo1_dado.c"	delay(100000000); 
	MOVLW	0x00
	MOVWF	STK00
	MOVLW	0xe1
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
	GOTO	_00115_DS_
;	.line	90; "labo1_dado.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x1000
;   STK00
;   r0x1001
;   r0x1002
;   r0x1003
;   r0x1004
;   r0x1005
;   r0x1006
;   r0x1007
;; Starting pCode block
S_labo1_dado__delay	code
_delay:
; 2 exit points
;	.line	92; "labo1_dado.c"	void delay(unsigned int tiempo)
	BANKSEL	r0x1000
	MOVWF	r0x1000
	MOVF	STK00,W
	MOVWF	r0x1001
;	.line	97; "labo1_dado.c"	for(i=0;i<tiempo;i++)
	CLRF	r0x1002
	CLRF	r0x1003
_00140_DS_:
	BANKSEL	r0x1000
	MOVF	r0x1000,W
	SUBWF	r0x1003,W
	BTFSS	STATUS,2
	GOTO	_00161_DS_
	MOVF	r0x1001,W
	SUBWF	r0x1002,W
_00161_DS_:
	BTFSC	STATUS,0
	GOTO	_00142_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;	.line	98; "labo1_dado.c"	for(j=0;j<1275;j++);
	MOVLW	0xfb
	BANKSEL	r0x1004
	MOVWF	r0x1004
	MOVLW	0x04
	MOVWF	r0x1005
_00138_DS_:
	MOVLW	0xff
	BANKSEL	r0x1004
	ADDWF	r0x1004,W
	MOVWF	r0x1006
	MOVLW	0xff
	MOVWF	r0x1007
	MOVF	r0x1005,W
	BTFSC	STATUS,0
	INCFSZ	r0x1005,W
	ADDWF	r0x1007,F
	MOVF	r0x1006,W
	MOVWF	r0x1004
	MOVF	r0x1007,W
	MOVWF	r0x1005
	MOVF	r0x1007,W
	IORWF	r0x1006,W
	BTFSS	STATUS,2
	GOTO	_00138_DS_
;	.line	97; "labo1_dado.c"	for(i=0;i<tiempo;i++)
	INCF	r0x1002,F
	BTFSC	STATUS,2
	INCF	r0x1003,F
	GOTO	_00140_DS_
_00142_DS_:
;	.line	99; "labo1_dado.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  107+   31 =   138 instructions (  338 byte)

	end
