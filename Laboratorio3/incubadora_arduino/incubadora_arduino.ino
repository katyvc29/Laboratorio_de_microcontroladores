#include <PID_v1.h>
#include <PCD8544.h>

//Leds azul, verde y rojo en ese orden
#define LED_AZUL 8
#define LED_VERDE 12
#define LED_ROJO 13

//Pulsadores y potenciometro
#define PUL1 A5 //Impresion pantalla LCD
#define PUL2 A4 //Comunicacion PC
#define POT A0

// Variable par manejo de la pantalla
PCD8544 LCD;

//Variables de control para el PID
double SetPoint, Input, Output;

//Variable del PID
PID myPID(&Input, &Output, &SetPoint, 0.2, 0.1, 0.1, DIRECT); // ganancias escogidas arbitrariamente


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
  static uint32_t last = 0;
  uint32_t interval = 100; // ms
    
  if ( millis () - last >= interval) {
      last += interval;
      // 0−transferencia de calor
      T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) - area - h;
  }

  return T;
    }

  int Estado = 0;
  int operacion = 0;
  double Temperatura = 0;




void setup() {
  
  Serial.begin(9600); // Inicializa la comunicación serial

  //Inicializacion de la pantalla LCD
  LCD.begin(84, 48);
  LCD.setContrast(60);

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
  operacion = map(analogRead(POT), 0, 1023, 0, 80);
  SetPoint = map(operacion, 0, 80, 0, 255);
  float TempWatts = (int)Output * 20.0 / 255;
  Temperatura =  simPlanta(TempWatts);
  Input = map(Temperatura, 0, 1023, 0, 255);

 //Habilitar PID
  myPID.SetMode(AUTOMATIC);
}


void loop() {
  //Como el valor del potenciometro cambia se debe mapear el punto de operacion
  operacion = (map(analogRead(POT), 0, 1023, 0, 80));
  switch(Estado){
    case 0:
      float TempWatts = (int)Output * 20.0 / 255;
      Temperatura =  simPlanta(TempWatts);
      Estado = 1;
      break;
    
    case 1:
      Input = map(analogRead(POT), 0, 1023, 0, 80); //map(Temperatura, 0, 1023, 0, 255);
      myPID.Compute();

      if(Temperatura > operacion){
        Temperatura = Temperatura - 1.5;
      }
      else if (Temperatura < operacion){
        Temperatura = Temperatura + 1.5;
      }
      //margen de error
      if (abs(Temperatura-operacion) < 0.3){
        Temperatura = map(operacion, 0, 80, 0, 255);
        Estado = 0;
      }
    break;
  }      

  //Condicional para encender los leds
  if (Temperatura >= 42){
    digitalWrite(LED_AZUL, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
  }
  else if(Temperatura <= 30){
    digitalWrite(LED_AZUL, HIGH);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, LOW);    
  }
  else{
    digitalWrite(LED_AZUL, LOW);
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
  }

  //Impresion en pantalla LCD
  if (digitalRead(PUL1) == HIGH){
    LCD.setCursor(0,0);
    LCD.print("T.op: ");
    LCD.print(operacion);
    LCD.setCursor(0,1);
    LCD.print("T.sen: ");
    LCD.print(Temperatura);
    LCD.setCursor(0,3);
    LCD.print("S control: ");
    LCD.print(Output);

  }
  //Comunicacion serial
  if (digitalRead(PUL2) == HIGH){
    Serial.print("Temperatura: ");
    Serial.print(Temperatura);
  }
}


