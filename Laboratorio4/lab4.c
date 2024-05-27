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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaracion de funciones
void spi_transaction(uint16_t reg, uint16_t val);
static void spi_setup(void);
static void usart_setup(void);
uint8_t spi_communications(uint8_t command);
uint16_t leer_eje(uint8_t lsb_command, uint8_t msb_command);
giroscopio leer_ejes_xyz(void);
static void gpio_setup(void);
int print_decimal(int num); 	// basada en lcd-spi.c
//static void adc_setup(void); 	// basada en adc-dac-printf
//static uint16_t read_adc_naiive(uint8_t channel);	//basada en adc-dac-printf
void leds(float bateria_nivel);
void envio_datos(giroscopio lectura, float bateria_nivel);
void display_datos(giroscopio lectura, float bateria_nivel, bool enviar);
void inicializacion(void); 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//Funcion para que el giroscopio pueda hacer transacciones por SPI
void spi_transaction(uint16_t reg, uint16_t val){
    gpio_clear(GPIOC, GPIO1) //Pome en bajo el chip selectec para comenzar la transaccion SPI
    spi_send(SPI5, reg);       // Envia el registro al giroscopio
    spi_read(SPI5);            // Lee la respuesta del giroscopio
    spi_send(SPI5, val);       // Envia el valor al giroscopio
    spi_read(SPI5);            // Lee la respuesta del giroscopio
    gpio_set(GPIOC, GPIO1);    // Sube el chip selected para finalizar la transacción
}

///////////////////////////////////////////////////////////////////////////


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

///////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////////////////
//Funcion que se encarga de leer un eje del giroscopio usando los bits LSB Y MSB
uint16_t leer_eje(uint8_t lsb_command, uint8_t msb_command){
    int16_t result; //Crea una variable para guardar el resultado
    result = spi_communication(lsb_command); //Lee el byte menos significativo
    result |= spi_communication(msb_command) << 8; //Lee el byte más significativo y lo combina con el LSB
    return result; //Retorna el valor obtenido
}

///////////////////////////////////////////////////////////////////////////
//Funcion para leer los valores de los ejes X, Y, Z del giroscopio
giroscopio leer_ejes_xyz(void) {
	giroscopio medicion; //Crea una estructura para el giroscopio 

    spi_communication(GYR_WHO_AM_I | 0x80); //Lee el registro WHO_AM_I del giroscopio
    spi_communication(GYR_STATUS_REG | GYR_RNW); //Lee el registro STATUS_REG del giroscopio
    spi_communication(GYR_OUT_TEMP | GYR_RNW);  //Lee el registro OUT_TEMP del giroscopio

    //Obtiene los valores de los ejes y los escala segun la sensibilidad
    medicion.x = leer_eje(GYR_OUT_X_L | GYR_RNW, GYR_OUT_X_H | GYR_RNW) * L3GD20_SENSITIVITY_250DPS;
    medicion.y = leer_eje(GYR_OUT_Y_L | GYR_RNW, GYR_OUT_Y_H | GYR_RNW) * SENSITIVITY_250DPS;
    medicion.z = leer_eje(GYR_OUT_Z_L | GYR_RNW, GYR_OUT_Z_H | GYR_RNW) * SENSITIVITY_250DPS;

    return medicion; //Retorna la lectura con los valores de los 3 ejes

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Funcion que configura pines GPIO que se van a utilizar como entrada o salida, se basa en el codigo que esta en la libreria libopencm3 en el archivo spi.c 
static void gpio_setup(void){
    rcc_periph_clock_enable(RCC_GPIOG); //Habilita el reloj para el puerto GPIOG
    rcc_periph_clock_enable(RCC_GPIOA); //Habilita el reloj para el puerto GPIOA

    //Configura el pin GPIO0 de GPIOA como entrada
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
    //Configura los pines GPIO13 de GPIOG y GPIO14 de GPIOG como salida
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);

}


///////////////////////////////////////////////////////////////////////////
// Funcion para imprimir un entero como un numero decimal (tomada como base del archivo lcd-spi.c)
int print_decimal(int num){
	int	ndx = 0;
	char	buf[10]; 		//Se cambia a 12 para tener en cuenta el signo
	int	len = 0;			// Guarda la longitud de la cadena
	char	is_signed = 0;	// Va a indicar si el numero es negativo

	if (num < 0) {
		is_signed++;
		num = 0 - num;
	}
	buf[ndx++] = '\000';
	do {
		buf[ndx++] = (num % 10) + '0';
		num = num / 10;
	} while (num != 0);
	ndx--;
	if (is_signed != 0) {
		console_putc('-');
		len++;
	}
	while (buf[ndx] != '\000') {
		console_putc(buf[ndx--]);
		len++;
	}
	return len; /* number of characters printed */
}

// funcion que configura el conv analogico-digital para leer la bateria(basada en la funcion de archivo adc-dac-printf)
static void adc_setup(void){
	// Configuracion del pin  en modo analogico para la bateria
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3); 	// Establece en analogico y sin pull-up ni pull-down
    adc_power_off(ADC1);   												//  Apaga ADC1                    
    adc_disable_scan_mode(ADC1);										// Desactivan el modo escaneo                   
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);		// Establece el tiempo de muestreo
    adc_power_on(ADC1);                               					// Enciende ADC1
}

// funcion que configura el conv analogico-digital para leer la bateria(basada en la funcion de archivo adc-dac-printf)
static uint16_t read_adc_naiive(uint8_t channel){
    uint8_t channel_array[16];
    channel_array[0] = channel;                       // Establece el canal a leer en el array
    adc_set_regular_sequence(ADC1, 1, channel_array); // Configura la secuencia de conversión regular del ADC
    adc_start_conversion_regular(ADC1);               // Inicia la conversión
    while (!adc_eoc(ADC1));                           // Espera hasta que la conversión haya finalizado
    uint16_t reg16 = adc_read_regular(ADC1);          // Lee el valor convertido
    return reg16;                                     // Devuelve el valor
}


// funcion para los LED que indican bateria baja y deformacion mayor a 5 grados
void leds(float bateria_nivel) {

	// agregar lo de 5 grados 

    if (bateria_nivel < 7) {
        gpio_toggle(GPIOG, GPIO14);                   
    } else {
        gpio_clear(GPIOG, GPIO14);                   
    }
}

// 
void envio_datos(giroscopio lectura, float bateria_nivel) {
    print_decimal(lectura.x);                         // Imprime X del giroscopio
    console_puts("\t");                               
    print_decimal(lectura.y);                         // Imprime Y del giroscopio
    console_puts("\t");                               
    print_decimal(lectura.z);                         // Imprime Z del giroscopio
    console_puts("\t");                               
    print_decimal(bateria_nivel);                     // Imprime la tension de la batería
    console_puts("\n");                              

    leds(bateria_nivel);                   	 // llama a la funcion que controla los LEDs
}	 

void display_datos(giroscopio lectura, float bateria_nivel, bool enviar) {
    char display_str[50];
    
    // Pone la pantalla en blanco y configura tamano del texto
    gfx_fillScreen(LCD_WHITE);
    gfx_setTextSize(2);

    // Bateria en color negro
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    sprintf(display_str, "Nivel de bateria: \t: %.2f V", bateria_nivel);
    gfx_setCursor(5, 30);
    gfx_puts(display_str);


	// Normalizacion de los valores de la bateria 
    float battery_percentage = (bateria_nivel - 2.0) / (8.5 - 2.0); 


    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    gfx_setCursor(23, 90);
    gfx_puts("Giroscopio valores: ");		

	// Imprime en valor de x del giroscopio en color magenta
    gfx_setTextColor(LCD_MAGENTA, LCD_WHITE);
    sprintf(display_str, "X: %d", lectura.x);
    gfx_setCursor(20, 130);
    gfx_puts(display_str);

	// Imprime en valor de y del giroscopio en color azul
    gfx_setTextColor(LCD_BLUE , LCD_WHITE);
    sprintf(display_str, "Y: %d", lectura.y);
    gfx_setCursor(20, 170);
    gfx_puts(display_str);

	// Imprime en valor de z del giroscopio en color
    gfx_setTextColor(LCD_YELLOW, LCD_WHITE);
    sprintf(display_str, "Z: %d", lectura.z);
    gfx_setCursor(20, 210);
    gfx_puts(display_str);

	// Muestra el estado de la comunicacion
    gfx_setTextColor(LCD_BLACK, LCD_WHITE);
    gfx_setCursor(3, 250);
    gfx_puts("Comunicacion: ");
    gfx_setCursor(205, 250);
    gfx_puts(enviar ? "On" : "Off");
    
	// llama la funcion que controla los LEDs (nivel de bateria y cuando hay mas de 5 grados de deformacion)
    handle_leds(bateria_nivel);

    lcd_show_frame();
}

// Funcion que inicializa el sistema
void inicializacion(void) {
    console_setup(115200);
    clock_setup();
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_ADC1);
    gpio_setup();
    adc_setup();
    sdram_init();
    usart_setup();
    spi_setup();
    lcd_spi_init();
    gfx_init(lcd_draw_pixel, 240, 320);
}

// funcion de retardo sacada del archivo de miniblink.c
void delay(void) {
    for (int i = 0; i < 80000; i++) {
        __asm__("nop");
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////


//Funcion principal del programa 
int main(void) {
    giroscopio lectura;
    float bateria_nivel;
    uint16_t input_adc0;
    bool enviar = false;

	inicializacion();


	//Bucle principal del sistema
	while(1){
		lectura = read_xyz;
		gpio_set(GPIOC, GPIO1);
		input_adc0 = read_adc_naiive(3);
		bateria_nivel = (input_adc0 * 9.0f) / 4095.0f;

		display_datos(lectura, bateria_lvl, enviar);
		if (enviar) envio_datos(lectura, bateria_lvl);

		leds(bateria_nivel, enviar);  

		if (gpio_get(GPIOA, GPIO0)) {  
            enviar = !enviar;
        }

		delay(); 
	
	}
	return 0; 

}
