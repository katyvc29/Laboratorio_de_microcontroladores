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
; config word(s)
;--------------------------------------------------------
	__config 0x3fd7
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	__moduint
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_delay
	global	_lfsr

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
r0x1010	res	1
r0x1011	res	1
r0x1012	res	1
r0x1013	res	1
r0x1009	res	1
r0x1008	res	1
r0x100A	res	1
r0x100B	res	1
r0x100C	res	1
r0x100D	res	1
r0x100E	res	1
r0x100F	res	1
r0x1001	res	1
r0x1000	res	1
r0x1003	res	1
r0x1002	res	1
r0x1004	res	1
r0x1005	res	1
r0x1007	res	1
r0x1006	res	1
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
;   _lfsr
;   __moduint
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _lfsr
;   __moduint
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;7 compiler assigned registers:
;   r0x1010
;   r0x1011
;   r0x1012
;   STK02
;   STK01
;   STK00
;   r0x1013
;; Starting pCode block
S_labo1_dado__main	code
_main:
; 2 exit points
;	.line	15; "labo1_dado.c"	TRISIO = 0b00001000; // ponemos pin 4 como entrada para usar GP3 como entrada
	MOVLW	0x08
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	16; "labo1_dado.c"	GPIO = 0x00; //Poner pines en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
;	.line	20; "labo1_dado.c"	unsigned int seed = 0xACE1u;  // Semilla inicial
	MOVLW	0xe1
	BANKSEL	r0x1010
	MOVWF	r0x1010
	MOVLW	0xac
	MOVWF	r0x1011
_00115_DS_:
;	.line	26; "labo1_dado.c"	if(GP3 == 1){
	BANKSEL	r0x1012
	CLRF	r0x1012
	BANKSEL	_GPIObits
	BTFSS	_GPIObits,3
	GOTO	_00001_DS_
	BANKSEL	r0x1012
	INCF	r0x1012,F
_00001_DS_:
	BANKSEL	r0x1012
	MOVF	r0x1012,W
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00113_DS_
;	.line	27; "labo1_dado.c"	seed = lfsr(seed, taps);  // Generar el siguiente valor del LFSR
	MOVLW	0x00
	MOVWF	STK02
	MOVLW	0xb4
	MOVWF	STK01
	MOVF	r0x1010,W
	MOVWF	STK00
	MOVF	r0x1011,W
	PAGESEL	_lfsr
	CALL	_lfsr
	PAGESEL	$
	BANKSEL	r0x1011
	MOVWF	r0x1011
	MOVF	STK00,W
	MOVWF	r0x1010
;	.line	28; "labo1_dado.c"	bit = (seed % 6) + 1;  // Obtener número entre 1 y 6
	MOVLW	0x06
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
	MOVF	r0x1010,W
	MOVWF	STK00
	MOVF	r0x1011,W
	PAGESEL	__moduint
	CALL	__moduint
	PAGESEL	$
	BANKSEL	r0x1013
	MOVWF	r0x1013
	MOVF	STK00,W
	MOVWF	r0x1012
	INCF	r0x1012,F
	BTFSC	STATUS,2
	INCF	r0x1013,F
;;unsigned compare: left < lit(0x1=1), size=2
;	.line	30; "labo1_dado.c"	switch (bit){
	MOVLW	0x00
	SUBWF	r0x1013,W
	BTFSS	STATUS,2
	GOTO	_00135_DS_
	MOVLW	0x01
	SUBWF	r0x1012,W
_00135_DS_:
	BTFSS	STATUS,0
	GOTO	_00113_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit(0x7=7), size=2
	MOVLW	0x00
	BANKSEL	r0x1013
	SUBWF	r0x1013,W
	BTFSS	STATUS,2
	GOTO	_00136_DS_
	MOVLW	0x07
	SUBWF	r0x1012,W
_00136_DS_:
	BTFSC	STATUS,0
	GOTO	_00113_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
	BANKSEL	r0x1012
	DECF	r0x1012,F
	MOVLW	HIGH(_00137_DS_)
	BANKSEL	PCLATH
	MOVWF	PCLATH
	MOVLW	_00137_DS_
	BANKSEL	r0x1012
	ADDWF	r0x1012,W
	BTFSS	STATUS,0
	GOTO	_00002_DS_
	BANKSEL	PCLATH
	INCF	PCLATH,F
_00002_DS_:
	MOVWF	PCL
_00137_DS_:
	GOTO	_00105_DS_
	GOTO	_00106_DS_
	GOTO	_00107_DS_
	GOTO	_00108_DS_
	GOTO	_00109_DS_
	GOTO	_00110_DS_
_00105_DS_:
;	.line	32; "labo1_dado.c"	GPIO = 0b0000001;	//(Se prende 1 LED)
	MOVLW	0x01
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	33; "labo1_dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	34; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00106_DS_:
;	.line	37; "labo1_dado.c"	GPIO = 0b0000010;	//(Se prende 2 LED)
	MOVLW	0x02
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	38; "labo1_dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	39; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00107_DS_:
;	.line	42; "labo1_dado.c"	GPIO = 0b0000100;	//(Se prende 3 LED)
	MOVLW	0x04
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	43; "labo1_dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	44; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00108_DS_:
;	.line	47; "labo1_dado.c"	GPIO = 0b0000101;	//(Se prende 3 LED + 1 LED = 4 LED)
	MOVLW	0x05
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	48; "labo1_dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	49; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00109_DS_:
;	.line	52; "labo1_dado.c"	GPIO = 0b0000110;	//(Se prende 3 LED + 2 LED = 5 LED)
	MOVLW	0x06
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	53; "labo1_dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	54; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00110_DS_:
;	.line	57; "labo1_dado.c"	GPIO = 0b0000111;	//(Se prende 3 LED + 2 LED + 1 LED = 6 LED)
	MOVLW	0x07
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	58; "labo1_dado.c"	delay(time);
	MOVLW	0xc8
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
_00113_DS_:
;	.line	67; "labo1_dado.c"	GPIO = 0x00; //Se apagan todos los LED
	BANKSEL	_GPIO
	CLRF	_GPIO
	GOTO	_00115_DS_
;	.line	71; "labo1_dado.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;11 compiler assigned registers:
;   r0x1000
;   STK00
;   r0x1001
;   STK01
;   r0x1002
;   STK02
;   r0x1003
;   r0x1004
;   r0x1005
;   r0x1006
;   r0x1007
;; Starting pCode block
S_labo1_dado__lfsr	code
_lfsr:
; 2 exit points
;	.line	85; "labo1_dado.c"	unsigned int lfsr(unsigned int seed, unsigned int taps) {
	BANKSEL	r0x1000
	MOVWF	r0x1000
	MOVF	STK00,W
	MOVWF	r0x1001
	MOVF	STK01,W
	MOVWF	r0x1002
	MOVF	STK02,W
	MOVWF	r0x1003
;	.line	86; "labo1_dado.c"	unsigned int lsb = seed & 1;
	MOVLW	0x01
	ANDWF	r0x1001,W
	MOVWF	r0x1004
	CLRF	r0x1005
;;shiftRight_Left2ResultLit:5474: shCount=1, size=2, sign=0, same=0, offr=0
;	.line	87; "labo1_dado.c"	seed >>= 1;
	BCF	STATUS,0
	RRF	r0x1000,W
	MOVWF	r0x1006
	RRF	r0x1001,W
	MOVWF	r0x1007
;	.line	88; "labo1_dado.c"	if (lsb == 1) {
	MOVF	r0x1004,W
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00175_DS_
	MOVF	r0x1005,W
	XORLW	0x00
	BTFSS	STATUS,2
	GOTO	_00175_DS_
;	.line	89; "labo1_dado.c"	seed ^= taps;
	MOVF	r0x1003,W
	XORWF	r0x1007,F
	MOVF	r0x1002,W
	XORWF	r0x1006,F
_00175_DS_:
;	.line	91; "labo1_dado.c"	return seed;
	BANKSEL	r0x1007
	MOVF	r0x1007,W
	MOVWF	STK00
	MOVF	r0x1006,W
;	.line	92; "labo1_dado.c"	}
	RETURN	
; exit point of _lfsr

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x1008
;   STK00
;   r0x1009
;   r0x100A
;   r0x100B
;   r0x100C
;   r0x100D
;   r0x100E
;   r0x100F
;; Starting pCode block
S_labo1_dado__delay	code
_delay:
; 2 exit points
;	.line	73; "labo1_dado.c"	void delay(unsigned int tiempo)
	BANKSEL	r0x1008
	MOVWF	r0x1008
	MOVF	STK00,W
	MOVWF	r0x1009
;	.line	78; "labo1_dado.c"	for(i=0;i<tiempo;i++)
	CLRF	r0x100A
	CLRF	r0x100B
_00148_DS_:
	BANKSEL	r0x1008
	MOVF	r0x1008,W
	SUBWF	r0x100B,W
	BTFSS	STATUS,2
	GOTO	_00169_DS_
	MOVF	r0x1009,W
	SUBWF	r0x100A,W
_00169_DS_:
	BTFSC	STATUS,0
	GOTO	_00150_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;	.line	79; "labo1_dado.c"	for(j=0;j<1275;j++);
	MOVLW	0xfb
	BANKSEL	r0x100C
	MOVWF	r0x100C
	MOVLW	0x04
	MOVWF	r0x100D
_00146_DS_:
	MOVLW	0xff
	BANKSEL	r0x100C
	ADDWF	r0x100C,W
	MOVWF	r0x100E
	MOVLW	0xff
	MOVWF	r0x100F
	MOVF	r0x100D,W
	BTFSC	STATUS,0
	INCFSZ	r0x100D,W
	ADDWF	r0x100F,F
	MOVF	r0x100E,W
	MOVWF	r0x100C
	MOVF	r0x100F,W
	MOVWF	r0x100D
	MOVF	r0x100F,W
	IORWF	r0x100E,W
	BTFSS	STATUS,2
	GOTO	_00146_DS_
;	.line	78; "labo1_dado.c"	for(i=0;i<tiempo;i++)
	INCF	r0x100A,F
	BTFSC	STATUS,2
	INCF	r0x100B,F
	GOTO	_00148_DS_
_00150_DS_:
;	.line	80; "labo1_dado.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  186+   43 =   229 instructions (  544 byte)

	end
