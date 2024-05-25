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
#define GYR_MNS        (1 << 6) //Permite lecturas multiples cuando esta en 1
#define GYR_WHO_I_AM   0x0F //Identifica el dispositivo
#define GYR_OUT_TEMP   0x26 //Registra la temperatura de salida
#define GYR_STATUS_REG 0x27 //Registra el estado









//Funcion principal del programa 
int main(void) {
	float bateria_lvl;
	//Bucle principal del sistema
	while(1){
	
	
	}

}
