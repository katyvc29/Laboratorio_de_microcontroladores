#include <pic14/pic12f683.h>
//#include <pic14/pic12f675.h>
 
//To compile:
//sdcc -mpic14 -p16f675 blink.c
 
//To program the chip using picp:
//Assuming /dev/ttyUSB0 is the serial port.
 
//Erase the chip:
//picp /dev/ttyUSB0 16f887 -ef
 
//Write the program:
//picp /dev/ttyUSB0 16f887 -wp blink.hex
 
//Write the configuration words (optional):
//picp /dev/ttyUSB0 16f887 -wc 0x2ff4 0x3fff
 
//Doing it all at once: erasing, programming, and reading back config words:
//picp /dev/ttyUSB0 16f887 -ef -wp blink.hex -rc
 
//To program the chip using pk2cmd:
//pk2cmd -M -PPIC16f887 -Fblink.hex
 
void delay (unsigned int tiempo);
//unsigned char numero_aleatorio();
 
void main(void)
{
	TRISIO = 0b00001000; // ponemos pin 4 como entrada
    //TRISIO = 0b00000000; //Poner todos los pines como salidas
	GPIO = 0x00; //Poner pines en bajo
 
    unsigned int time = 1000000000000000, num;
 
    //Loop forever
    while ( 1 ){
	//GPIO = 0x00;
		if(GP3 == 0){
			num=6;
			//num = numero_aleatorio();

			switch (num){
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
			
			//GPIO = 0b00000111;
			//delay(1000000000000); //10 segundos
			//break;
			//GP0 = 0x01; Porque no sirve?
			//GP1 = 0x01;
			//GP2 = 0x01;
			
		}	
		//delay(500);
		//GPIO = 0x00; //Se apagan los LED
	delay(100000000); 
    }
	

}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}
