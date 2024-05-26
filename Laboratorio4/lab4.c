//Bibliotecas estandar de C
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>

//Librerias a utilizar 
//#include "usart.h"
//#include "spi.h"
//#include "gpio.h"
//#include "dac.h"
//#include "adc.h"
//#include "rcc.h"

// Archivos del codigo del ejemplo lcd-serial
#include "clock.h"
#include "console.h"
#include "gfx.h"
#include "lcd-spi.h"
#include "sdram.h"



//Librerias del micro
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/dac.h>
#include <libopencm3/stm32/adc.h>


// Definiciones de macros  para interacturar por medio spi sacadas del archivo spi.c
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
#define GYR_CTRL_REG4           0x23 //Configura el registro de control 4 **escala de medicion
#define GYR_CTRL_REG4_FS_SHIFTH 4 //Cambios en la escala 

//Direcciones de los registros de datos del girocospio
#define GYR_OUT_X_L 0X28
#define GYR_OUT_X_H 0X29
#define GYR_OUT_Y_L 0X2A
#define GYR_OUT_Y_H 0X2B
#define GYR_OUT_Z_L 0X2C
#define GYR_OUT_Z_H 0X2D

//Sensibielidad del giroscopio
#define SENSITIVITY_250DPS (0.00875F) //@

typedef struct Giroscopio {
  int16_t x;
  int16_t y;
  int16_t z;
} giroscopio;

//Declaracion de funciones
void spi_transaction(uint16_t reg, uint16_t val);
static void spi_setup(void);
static void usart_setup(void);
uint8_t spi_communications(uint8_t command);

//Funcion para que el giroscopio pueda hacer transacciones por SPI
void spi_transaction(uint16_t reg, uint16_t val){
    gpio_clear(GPIOC, GPIO1) //Pome en bajo el chip selectec para comenzar la transaccion SPI
    spi_send(SPI5, reg);       // Envia el registro al giroscopio
    spi_read(SPI5);            // Lee la respuesta del giroscopio
    spi_send(SPI5, val);       // Envia el valor al giroscopio
    spi_read(SPI5);            // Lee la respuesta del giroscopio
    gpio_set(GPIOC, GPIO1);    // Sube el chip selected para finalizar la transacción
}

//Función para configurar el módulo SPI5 y GPIOs relacionados, se basa en el codigo que esta en la libreria libopencm3 en el archivo spi.c
static void spi_setup(void){
    rcc_periph_clock_enable(RCC_SPI5); //Habilita el reloj para SPI5
    rcc_periph_clock_enable(RCC_GPIOC); //Habilita el reloj para el puerto GPIOC
    rcc_periph_clock_enable(RCC_GPIOF); //Habilita el reloj para el puerto GPIOF

    //Configura el pin GPIO1 de GPIOC como salida
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO1); //Establece modo de operacion/ NO pull-up ni pull-down / número específico del pin
    gpio_set(GPIOC, GPIO1); //ambos en alto

    //Configura los pines GPIO7, GPIO8 y GPIO9 de GPIOF para funciones alternas 
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE,GPIO7 | GPIO8 | GPIO9);   
    gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);
    
    //Configuración de SPI5
    spi_set_master_mode(SPI5);   //Establece SPI5 en modo maestro
    spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);  //Configura la velocidad de baudios de SPI5
    spi_set_clock_polarity_0(SPI5); //Configura la polaridad del reloj a 0
    spi_set_clock_phase_0(SPI5); //Configura la fase del reloj a 0
    spi_set_full_duplex_mode(SPI5); //Establece SPI5 en modo full duplex
    spi_set_unidirectional_mode(SPI5); //Establece SPI5 en modo unidireccional
    spi_enable_software_slave_management(SPI5); //Habilita la gestión de esclavo por software
    spi_send_msb_first(SPI5); //Establece la transmisión de bits empezando por el más significativo
    spi_set_nss_high(SPI5); //Establece el pin NSS en alto
    SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD; //Configuración adicional para SPI5
    spi_enable(SPI5); //Habilita SPI5


    //Se llama la funcion que realiza las transacciones SPI para configurar el giroscopio
    spi_transaction(GYR_CTRL_REG1, GYR_CTRL_REG1_PD | GYR_CTRL_REG1_XEN | GYR_CTRL_REG1_YEN | GYR_CTRL_REG1_ZEN | (3 << GYR_CTRL_REG1_BW_SHIFT));
    spi_transaction(GYR_CTRL_REG4, (1 << GYR_CTRL_REG4_FS_SHIFT));
}

//Funcion que configura el USART1 y los Gpios necesarios, se basa en el codigo que esta en la libreria libopencm3 en el archivo spi.c
static void usart_setup(void){
    //Configura el pin GPIO9 de GPIOA para transmitir en USART1
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);	
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
	
    // Configuración de USART1
    usart_set_baudrate(USART1, 115200); //Establece la velocidad de baudios en 115200
    usart_set_databits(USART1, 8); //Establece 8 bits de datos
    usart_set_stopbits(USART1, USART_STOPBITS_1); //Establece 1 bit de parada
    usart_set_mode(USART1, USART_MODE_TX); //Establece USART1 en modo de transmisión
    usart_set_parity(USART1, USART_PARITY_NONE); //Sin paridad
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE); //Sin control de flujo

    //Habilita USART1
    usart_enable(USART1);
}

//Funcion para realizar una configuracion SPI con el giroscopio y obtener una respuesta
uint8_t spi_communications(uint8_t command){
     gpio_clear(GPIOC, GPIO1); //Desactiva el pin GPIO1 de GPIOC, osea el  chip select, para iniciar la comunicacion
    spi_send(SPI5, command);// Envia el comando por SPI5
    spi_read(SPI5); //Lee una respuesta desde SPI5
    spi_send(SPI5, 0);//Envia un byte en 0 por SPI5
    uint8_t result = spi_read(SPI5); //Lee el resultado desde SPI5
    gpio_set(GPIOC, GPIO1); //Activa el pin GPIO1 de GPIOC (termina la comunicacion con el dispositivo)
    return result; // Devuelve el resultado leido


}



//Funcion principal del programa 
int main(void) {
	float bateria_lvl;
	//Bucle principal del sistema
	while(1){
	
	
	}

}
