#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
//#include <util/delay.h>

//Estados
#define BOTON_NO_PRESIONADO         1
#define BOTON_PRESIONADO            2
#define MOMENTO_INTERMEDIO          3
#define PASO_PEATONES               4
#define PEATONES_BLINKING           5
#define PASO_PEATONAL_DESHABILITADO 6


// Variables Globales
volatile int timer = 0;
volatile bool boton = false;
volatile int blink_counter = 0;
int estado;
int siguiente_estado;

void maquina_estados();


int main(void)
{
    // Configuración de los puertos
    PORTB = 0b00000000; // Se configura puerto B en 0 (limpiamos)
    PIND = 0b00000000;  // Este no estoy segura
    PORTD = 0b00000000; // Puerto D se establece en 0 (limpiamos)

    // Configuracion de las entradas y salida
    DDRB = 0b01111111; //Se configura B3 como salida
    DDRD = 0b11110011; // Se configura D2 como entrada

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


    siguiente_estado = BOTON_NO_PRESIONADO; //estado inicial
    estado = siguiente_estado; //cambiar de estado
  while(1){
    maquina_estados();
  }

  return 0;
}

void maquina_estados(){
      switch (estado) {
        case BOTON_NO_PRESIONADO:
          PORTB = 0b00110010;  // Estado inicial donde la luz verde de los carros esta encendida siempre
          if (timer >= 680 && boton) { //Despues de 10s 
              //PORTB &= ~(1 << PB0);  // Asegurarse que B0 esté apagado, ya que es la luz verde del semaforo peatonal
              //PORTB &= ~(1 << PB6);  // Asegurarse que B6 esté apagado, ya que es la luz verde del semaforo peatonal
              //PORTB &= ~(1 << PB4); // Se apaga el LED verde del semaforo vehicular
              siguiente_estado = BOTON_PRESIONADO;
              boton = false;
              timer = 0;
          }
          
          break;
        case BOTON_PRESIONADO:
            if (timer < 204) { // Despues de 3 segundos 
            // Parpadea cada segundo para que sea visible
            if (blink_counter % 2 == 0) {
                PORTB |= (1 << PB1);  // Encender LED B1
                PORTB |= (1 << PB5);  // Encender LED B5
                PORTB |= (1 << PB3);  // Se prende el LED amarillo del semaforo vehicular
            } else {
                PORTB &= ~(1 << PB1);  // Apagar LED B1
                PORTB &= ~(1 << PB5);  // Encender LED B5
                PORTB &= ~(1 << PB3);  // Se prende el LED amarillo del semaforo vehicular
            }

            // Incrementar el contador de parpadeo cada segundo
            if (timer != blink_counter) {
                blink_counter = timer;
            }
        } else {
            PORTB &= ~(1 << PB3);  // Apagar LED B3 después de 3 segundos, led amarilla
            PORTB &= ~(1 << PB4); // Se apaga el LED verde del semaforo vehicular
            siguiente_estado = MOMENTO_INTERMEDIO;
            timer = 0;  // Resetear el contador
        }
        break;

        case MOMENTO_INTERMEDIO:
            PORTB &= ~(1 << PB3);  // Asegurarse que B3 esté apagado
            PORTB |= (1 << PB2);  // Encender LED B2 rojo, vahiculos detenidos
            PORTB |= (1 << PB1);  // Encender LED rojo (peatonal)
            PORTB |= (1 << PB5);  // Encender LED rojo (peatonal)
            if (timer >= 80) {  // Pasar al siguiente estado después de 1 segundo
                siguiente_estado = PASO_PEATONES;
                timer = 0;  // Resetear el contador
            }
          break;
        case PASO_PEATONES:
            //Se enciende la luz roja del semaforo vehicular
            PORTB |= (1 << PB2);  // Asegurarse que B2 rojo (vehicular) este encendido

            // Se asegura que los siugientes LED esten apagados
            PORTB &= ~(1 << PB1);  // Apagar LED rojo (peatonal)
            PORTB &= ~(1 << PB5);  // Apagar LED rojo (peatonal)
            //PORTB &= ~(1 << PB3);  // Apagar LED amarillo (vehicular)
            PORTB &= ~(1 << PB4);  // Apagar LED verde (vehicular)

            //Se encienden los led verdes de los semaforos peatonales
            PORTB |= (1 << PB0);
            PORTB |= (1 << PB6);

        
            if (timer >= 680) { //10 segundos 
                  PORTB &= ~(1 << PB4);  // Asegurarse que B4 verde (vehicular) esté apagado
                  siguiente_estado = PEATONES_BLINKING;
                  timer = 0;
                }
                break;
        case PEATONES_BLINKING:
              if (timer < 204) { // Después de 3s 204
                  // Parpadea cada segundo para que sea visible
                  if (blink_counter % 2 == 0) {
                      PORTB |= (1 << PB0) | (1 << PB6);  // Encender LED verdes B0 y B6, para el paso de peatones
                  } else {
                      PORTB &= ~((1 << PB0) | (1 << PB6));  // Sino apagar LED verdes B0 y B6
                  }

                  // Incrementar el contador de parpadeo cada segundo
                  if (timer != blink_counter) {
                      blink_counter = timer;
                  }
              } else {
                  PORTB &= ~((1 << PB0) | (1 << PB6));  // Apagar LED B0 y B6 después de 3 segundos, para detener los peatones
                  PORTB |= (1 << PB1) | (1 << PB5);  // Encender LED rojos peatonales B1 y B5, indicar que ya no hay paso peatonal
                  siguiente_estado = PASO_PEATONAL_DESHABILITADO;
                  timer = 0;  // Resetear el contador
              }
              break;
        case PASO_PEATONAL_DESHABILITADO:
            if (timer >= 68) {
                siguiente_estado = BOTON_NO_PRESIONADO;
            }

  }
  estado = siguiente_estado;

}


// Manejadores de interrupcion
ISR(INT0_vect) {
    boton = true;
}

ISR(INT1_vect) {
    boton = true;
}

ISR(TIMER0_OVF_vect) {
    timer++;
}

