#include <PID_v1.h>
#include <PCD8544.h>

//Leds azul, verde y rojo en ese orden
#define LED_AZUL 8;
#define LED_VERDE 12;
#define LED_ROJO 13;

// Conexiones del PCD8544
#define CLK 3;
#define DIN 4;
#define DC 5;
#define CS 6;
#define RST 7;

//Pulsadores y potenciometro
#define PUL1 A5; //Impresion pantalla LCD
#define PUL2 A4; //Comunicacion PC
#define POT A0;

// Variable par manejo de la pantalla
PCD8544 LCD;

//Variables de control para el PID
double Setpoint, Input, Output;

//Variable del PID
PID myPID(&Input, &Output, &Setpoint, 2, 5, 1 , DIRECT); // ganancias escogidas arbitrariamente


//Funcion de la planta para simular lazo cerrado
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

void setup() {
  
  Serial.begin(9600); // Inicializa la comunicación serial

  //Inicializacion de la pantalla LCD
  LCD.begin(84, 48);

  //Inicializacion de los pines
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);

  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  pinMode(POT, INPUT);

  //Estado para el lazo cerrado
    Estado = 0;

  //Control PID
  //Inicializar variables
  //Se mapea el punto de operacion del potenciometro
  operacion = (map(analogRead(POT), 0, 1023, 0, 80));
  SetPoint = map(operacion, 0, 80, 0, 255);
  float TempWatts = (int)Output * 20.0 / 255;
  Temperatura =  SimPlanta(TempWatts);
  Input = map(Temperatura, 0, 1023, 0, 255);

 //Habilitar PID
  myPID.SetMode(AUTOMATIC);

}


void loop() {
  //Como el valor del potenciometro cambia se debe mapear el punto de operacion
  operacion = (map(analogRead(POT), 0, 1023, 0, 80));

  switch(estado){
    case 0:
      float TempWatts = (int)Output * 20.0 / 255;
      Temperatura =  SimPlanta(TempWatts);
      estado = 1;
    
    case 1:
      Input = map(Temperatura, 0, 1023, 0, 255);
      myPID.Compute();
      //analogic.write()

      if(Temperatura > operacion){
        Temperatura = Temperatura - 1.5;
      }
      else if (Temperatura < operacion){
        Temperatura = Temperatura + 1.5;
      }
      if (abs(T-operacion) < 0.3){
        Temperatura = map(operacion, 0, 80, 0, 255);
        estado = 0;
      }      

  //Condicional para encender los leds
  if (Temperatura >= 42){
    digitalwrite(LED_AZUL, LOW);
    digitalwrite(LED_VERDE, LOW);
    digitalwrite(LED_ROJO, HIGH);
  }
  else if(Temperatura <= 30){
    digitalwrite(LED_AZUL, HIGH);
    digitalwrite(LED_VERDE, LOW);
    digitalwrite(LED_ROJO, LOW);    
  }
  else{
    digitalwrite(LED_AZUL, LOW);
    digitalwrite(LED_VERDE, HIGH);
    digitalwrite(LED_ROJO, LOW);
  }

  //Impresion en pantalla LCD
  if (digitalRead(PULL1) == HIGH){
    LCD.setCursor(0,0);
    LCD.print("Temp operacion: ");
    LCD.print(operacion);
    LCD.setCursor(0,1);
    LCD.print("Temp sensada: ");
    LCD.print(Temperatura);
    LCD.setCursor(0,3);
    LCD.print("Señal de control: ");
    LCD.print(Output);

  }
  //Comunicacion serial
  if (digitalRead(PULL2) == HIGH){
    Serial.print("Temperatura: ");
    Serial.print(Temperatura);
  }
  


}


