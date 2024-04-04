#include <pic14/pic12f683.h> // Llamamos al archivo del PIC que estamos usando


typedef unsigned int word ;

word __at 0x2007 __CONFIG = ( _WDT_OFF & _MCLRE_OFF ); // Se desactivan las macros

 
void delay (unsigned int tiempo);
unsigned int lfsr(unsigned int seed, unsigned int taps);
 
void main(void)
{
	TRISIO = 0b00001000; // ponemos pin 4 como entrada para usar GP3 como entrada
	GPIO = 0x00; //Poner pines en bajo
 
    unsigned int time = 200; // 0.2 s
    unsigned int seed = 0xACE1u;  // Semilla inicial
    unsigned int taps = 0xB400u;  // Patrón de retroalimentación (polinomio)
    unsigned int bit;
 
    //Loop forever
    while ( 1 ){
		if(GP3 == 1){
			seed = lfsr(seed, taps);  // Generar el siguiente valor del LFSR
        	bit = (seed % 6) + 1;  // Obtener número entre 1 y 6

			switch (bit){
				case 1: //cuando num es 1 
				GPIO = 0b0000001;	//(Se prende 1 LED)
				delay(time);
				break;

				case 2: //cuando num es 3
				GPIO = 0b0000010;	//(Se prende 2 LED)
				delay(time);
				break;

				case 3: //cuando num es 3
				GPIO = 0b0000100;	//(Se prende 3 LED)
				delay(time);
				break;

				case 4: //cuando num es 4
				GPIO = 0b0000101;	//(Se prende 3 LED + 1 LED = 4 LED)
				delay(time);
				break;

				case 5: //cuando num es 3
				GPIO = 0b0000110;	//(Se prende 3 LED + 2 LED = 5 LED)
				delay(time);
				break;

				case 6: //cuando num es 3
				GPIO = 0b0000111;	//(Se prende 3 LED + 2 LED + 1 LED = 6 LED)
				delay(time);
				break;

			}
			

			
		}	

	GPIO = 0x00; //Se apagan todos los LED
    }
	

}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}



// Declaración de la función para el LFSR
unsigned int lfsr(unsigned int seed, unsigned int taps) {
    unsigned int lsb = seed & 1;
    seed >>= 1;
    if (lsb == 1) {
        seed ^= taps;
    }
    return seed;
}