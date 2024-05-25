//Bibliotecas estandar de C
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>

//Librerias a utilizar 
#include "usart.h"
#include "spi.h"
#include "gpio.h"
#include "dac.h"
#include "adc.h"
#include "rcc.h"
#include "gfx.h"
#include "lcd-spi.h"
#include "sdram.h"
#include "console.h"
#include "clock.h"

//Librerias del micro
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>


//Definicion de variables relacionadas al giroscopio
#define GYR_RNW        (1 << 7) //Permite que se escriba cuando esta en 0
#define GYR_MNS        (1 << 6) //Permite varias lecturas cuando esta en 1
#define GYR_WHO_I_AM   0x0F //Identifica el dispositivo
#define GYR_OUT_TEMP   0x26 //Registra la temperatura de salida
#define GYR_STATUS_REG 0x27 //Registra el estado

//Definiciones para configurar el giroscopio
#define GYR_CTRL_REG1           0X20 //Configura el registro de control 1
#define GYR_CTRL_REG1_PD        (1 << 3) //Modo de encendido
#define GYR_CTRL_REG1_XEN       (1 << 1) //Habilita el eje X
#define GYR_CTRL_REG1_YEN       (1 << 0) //Habilita el eje Y
#define GYR_CTRL_REG1_ZEN       (1 << 2) //Habilita el eje Z
#define GYR_CTRL_REG1_BW_SHIFTH 4 //Cambios en el ancho de banda
#define GYR_CTRL_REG4           0x23 //Configura el registro de control 4
#define GYR_CTRL_REG4_FS_SHIFTH 4 //Cambios en la escala 

//Direcciones de los registros de datos del girocospio
#define GYR_OUT_X_L 0X28
#define GYR_OUT_X_H 0X29
#define GYR_OUT_Y_L 0X2A
#define GYR_OUT_Y_H 0X2B
#define GYR_OUT_Z_L 0X2C
#define GYR_OUT_Z_H 0X2D


//Funcion principal del programa 
int main(void) {
	float bateria_lvl;
	//Bucle principal del sistema
	while(1){
	
	
	}

}
