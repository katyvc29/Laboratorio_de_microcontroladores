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
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_delay
	global	_numero_aleatorio

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
r0x100C	res	1
r0x1005	res	1
r0x1004	res	1
r0x1006	res	1
r0x1007	res	1
r0x1008	res	1
r0x1009	res	1
r0x100A	res	1
r0x100B	res	1
r0x1001	res	1
r0x1002	res	1
r0x1003	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

IDD_labo1_dado_0	idata
_numero_aleatorio_lfsr_65536_12
	db	0x08	; 8

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
;   _numero_aleatorio
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _numero_aleatorio
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;   _delay
;2 compiler assigned registers:
;   r0x100C
;   STK00
;; Starting pCode block
S_labo1_dado__main	code
_main:
; 2 exit points
;	.line	13; "labo1_dado.c"	TRISIO = 0b00001000; // ponemos pin 4 como entrada para usar GP3 como entrada
	MOVLW	0x08
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	14; "labo1_dado.c"	GPIO = 0x00; //Poner pines en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
_00115_DS_:
;	.line	22; "labo1_dado.c"	if(GP3 == 1){
	BANKSEL	r0x100C
	CLRF	r0x100C
	BANKSEL	_GPIObits
	BTFSS	_GPIObits,3
	GOTO	_00001_DS_
	BANKSEL	r0x100C
	INCF	r0x100C,F
_00001_DS_:
	BANKSEL	r0x100C
	MOVF	r0x100C,W
	XORLW	0x01
	BTFSS	STATUS,2
	GOTO	_00113_DS_
;	.line	24; "labo1_dado.c"	num = numero_aleatorio();
	PAGESEL	_numero_aleatorio
	CALL	_numero_aleatorio
	PAGESEL	$
	BANKSEL	r0x100C
	MOVWF	r0x100C
;;unsigned compare: left < lit(0x1=1), size=1
;	.line	26; "labo1_dado.c"	switch (num){
	MOVLW	0x01
	SUBWF	r0x100C,W
	BTFSS	STATUS,0
	GOTO	_00113_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit(0x7=7), size=1
	MOVLW	0x07
	SUBWF	r0x100C,W
	BTFSC	STATUS,0
	GOTO	_00113_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
	DECF	r0x100C,F
	MOVLW	HIGH(_00137_DS_)
	BANKSEL	PCLATH
	MOVWF	PCLATH
	MOVLW	_00137_DS_
	BANKSEL	r0x100C
	ADDWF	r0x100C,W
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
;	.line	28; "labo1_dado.c"	GPIO = 0b0000001;	//(Se prende 1 LED)
	MOVLW	0x01
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	29; "labo1_dado.c"	delay(time);
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	30; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00106_DS_:
;	.line	33; "labo1_dado.c"	GPIO = 0b0000010;	//(Se prende 2 LED)
	MOVLW	0x02
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	34; "labo1_dado.c"	delay(time);
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	35; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00107_DS_:
;	.line	38; "labo1_dado.c"	GPIO = 0b0000100;	//(Se prende 3 LED)
	MOVLW	0x04
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	39; "labo1_dado.c"	delay(time);
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	40; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00108_DS_:
;	.line	43; "labo1_dado.c"	GPIO = 0b0000101;	//(Se prende 3 LED + 1 LED = 4 LED)
	MOVLW	0x05
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	44; "labo1_dado.c"	delay(time);
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	45; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00109_DS_:
;	.line	48; "labo1_dado.c"	GPIO = 0b0000110;	//(Se prende 3 LED + 2 LED = 5 LED)
	MOVLW	0x06
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	49; "labo1_dado.c"	delay(time);
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	50; "labo1_dado.c"	break;
	GOTO	_00113_DS_
_00110_DS_:
;	.line	53; "labo1_dado.c"	GPIO = 0b0000111;	//(Se prende 3 LED + 2 LED + 1 LED = 6 LED)
	MOVLW	0x07
	BANKSEL	_GPIO
	MOVWF	_GPIO
;	.line	54; "labo1_dado.c"	delay(time);
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
_00113_DS_:
;	.line	63; "labo1_dado.c"	GPIO = 0x00; //Se apagan todos los LED
	BANKSEL	_GPIO
	CLRF	_GPIO
	GOTO	_00115_DS_
;	.line	67; "labo1_dado.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;3 compiler assigned registers:
;   r0x1001
;   r0x1002
;   r0x1003
;; Starting pCode block
S_labo1_dado__numero_aleatorio	code
_numero_aleatorio:
; 2 exit points
_00174_DS_:
;	.line	83; "labo1_dado.c"	bit = ((lfsr >> 0) ^ (lfsr >> 1)) & 1; 
	BANKSEL	_numero_aleatorio_lfsr_65536_12
	MOVF	_numero_aleatorio_lfsr_65536_12,W
	BANKSEL	r0x1001
	MOVWF	r0x1001
;;shiftRight_Left2ResultLit:5474: shCount=1, size=1, sign=0, same=0, offr=0
	BCF	STATUS,0
	BANKSEL	_numero_aleatorio_lfsr_65536_12
	RRF	_numero_aleatorio_lfsr_65536_12,W
	BANKSEL	r0x1002
	MOVWF	r0x1002
	XORWF	r0x1001,F
	MOVLW	0x01
	ANDWF	r0x1001,F
;	.line	84; "labo1_dado.c"	lfsr = (lfsr >> 1) | (bit << 2);
	MOVF	r0x1001,W
	MOVWF	r0x1003
	BCF	STATUS,0
	RLF	r0x1003,W
	MOVWF	r0x1001
	BCF	STATUS,0
	RLF	r0x1001,F
	MOVF	r0x1001,W
	MOVWF	r0x1003
	IORWF	r0x1002,W
	BANKSEL	_numero_aleatorio_lfsr_65536_12
	MOVWF	_numero_aleatorio_lfsr_65536_12
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit(0x7=7), size=1
;	.line	85; "labo1_dado.c"	} while (lfsr > 6); 
	MOVLW	0x07
	SUBWF	_numero_aleatorio_lfsr_65536_12,W
	BTFSC	STATUS,0
	GOTO	_00174_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;	.line	87; "labo1_dado.c"	return lfsr;  
	MOVF	_numero_aleatorio_lfsr_65536_12,W
;	.line	88; "labo1_dado.c"	}
	RETURN	
; exit point of _numero_aleatorio

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x1004
;   STK00
;   r0x1005
;   r0x1006
;   r0x1007
;   r0x1008
;   r0x1009
;   r0x100A
;   r0x100B
;; Starting pCode block
S_labo1_dado__delay	code
_delay:
; 2 exit points
;	.line	69; "labo1_dado.c"	void delay(unsigned int tiempo)
	BANKSEL	r0x1004
	MOVWF	r0x1004
	MOVF	STK00,W
	MOVWF	r0x1005
;	.line	74; "labo1_dado.c"	for(i=0;i<tiempo;i++)
	CLRF	r0x1006
	CLRF	r0x1007
_00148_DS_:
	BANKSEL	r0x1004
	MOVF	r0x1004,W
	SUBWF	r0x1007,W
	BTFSS	STATUS,2
	GOTO	_00169_DS_
	MOVF	r0x1005,W
	SUBWF	r0x1006,W
_00169_DS_:
	BTFSC	STATUS,0
	GOTO	_00150_DS_
;;genSkipc:3307: created from rifx:00000000047A5780
;	.line	75; "labo1_dado.c"	for(j=0;j<1275;j++);
	MOVLW	0xfb
	BANKSEL	r0x1008
	MOVWF	r0x1008
	MOVLW	0x04
	MOVWF	r0x1009
_00146_DS_:
	MOVLW	0xff
	BANKSEL	r0x1008
	ADDWF	r0x1008,W
	MOVWF	r0x100A
	MOVLW	0xff
	MOVWF	r0x100B
	MOVF	r0x1009,W
	BTFSC	STATUS,0
	INCFSZ	r0x1009,W
	ADDWF	r0x100B,F
	MOVF	r0x100A,W
	MOVWF	r0x1008
	MOVF	r0x100B,W
	MOVWF	r0x1009
	MOVF	r0x100B,W
	IORWF	r0x100A,W
	BTFSS	STATUS,2
	GOTO	_00146_DS_
;	.line	74; "labo1_dado.c"	for(i=0;i<tiempo;i++)
	INCF	r0x1006,F
	BTFSC	STATUS,2
	INCF	r0x1007,F
	GOTO	_00148_DS_
_00150_DS_:
;	.line	76; "labo1_dado.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  144+   40 =   184 instructions (  448 byte)

	end
