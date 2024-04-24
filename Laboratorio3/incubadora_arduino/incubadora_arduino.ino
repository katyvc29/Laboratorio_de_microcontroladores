#include <PID_v1.h>
#include <PCD8544.h>

//Leds azul, verde y rojo en ese orden
#define LED1 8
#define LED2 12
#define LED3 13

// Conexiones del PCD8544
#define CLK 3
#define DIN 4
#define DC 5
#define CS 6
#define RST 7

//Pulsadores y potenciometro
#define PUL1 A5
#define PUL2 A4
#define POT A0

void setup() {
  
  Serial.begin(9600); // Inicializa la comunicación serial
  //Inicializacion de los pines
  pinMode(RST, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DC, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CLK, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  pinMode(POT, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

 
//Esta funcion simula la planta/proceso
//@param Q: Entrada de calor en Watts (o J/s). Para convertir la salida en temperatura del control PID(Output) puede utilizar:
// float TempWatts = (int)Output ∗ 20.0 / 255;
// @return T: Temperatura de salida en la planta

float simPlanta(float Q) {
  //simula un bloque de aluminio de 1x1x2cm con un calentador y con enfreamiento pasivo
  float C = 237; // W/mK coeficiente de conductividad termica para el Aluminio
  float h = 5; // W/m2K coeficiente de conveccion termica para el Aluminio
  float Cps = 0.89; // J/gC
  float area = 1e-4; // m2 area por conveccion
  float masa = 10 ; // g
  float Tamb = 25; // Temperatura ambiente en C
  static float T = Tamb; // Temperatura en C
  static uint32 t last = 0;
  uint32 t interval = 100; // ms
    
  if ( millis () - last >= interval) {
      last += interval;
      // 0−transferencia de calor
      T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) - area - h;
  }

  return T;
    }
