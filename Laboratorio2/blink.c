#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

volatile bool boton = false;

int main(void)
{
    //DDRB = 0x08; //Configuracion del puerto
    PORTB = 0b00000000; // Se configura puerto B en 0 (limpiamos)
    PIND = 0b00000000;  // Este no estoy segura
    PORTD = 0b00000000; // Puerto D se establece en 0 (limpiamos)

    // Configuracion de las entradas y salida
    //DDRB = 0b01111111;
    DDRB = 0b00001000; //Se configura B3 como salida
    //DDRD = 0b11110011;
    DDRD = 0b11111011; // Se configura D2 como entrada

    // Configuración de las interrupciones externas (para los botones)
    GIMSK |= (1 << INT0); // Botón 1
    MCUCR |= (1 << ISC00) | (1 << ISC01); // Se configura para que la iterrupción se de en flanco de bajada

    GIMSK |= (1 << INT1); // Botón 2
    MCUCR |= (1 << ISC10) | (1 << ISC11); // Se configura para que la iterrupción se de en flanco de bajada


    // Configuración del  Timer0 para las interrupciones (cada 1ms)
    TCCR0A = 0;
    TCCR0B = (1 << CS02) | (1 << CS00); // Preescalador   Timer0 contará a una velocidad de 1/1024 del reloj del microcontrolador.
    TIMSK |= (1 << TOIE0); // Habilitación de interrupción de desbordamiento del Timer0

    sei(); // Habilitación de interrupciones globales


  //Parpadear
  while (1) {

    if (boton == true) {
      PORTB |= (1 << PB3); // Encender LED en PB3
      //_delay_ms(5000);
      //break;

    }


    //PORTB = 0x00; //PORTB &= ~(1 << PB3); //Esto se puede hacer tambien asi
    //_delay_ms(500); 
    //PORTB = 0x08; //PORTB |=  (1 << PB3); //Esto se puede hacer tambien asi
    //_delay_ms(500); 
  }
  return 0;
}

//void maquina_estados(){}

// Manejadores de interrupcion
ISR(INT0_vect) {
    boton = true;
}

ISR(INT1_vect) {
    boton = true;
}

