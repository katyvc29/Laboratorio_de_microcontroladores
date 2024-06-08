
#include <Arduino_HTS221.h> //sensores
#include <Arduino_APDS9960.h> //RGB

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!HTS.begin() || !APDS. begin () ) {
    Serial.println("No se inició correctamente");
    while (1);
  }
  Serial.println("Se inicio correcctamente");
  Serial.println("Rojo,Verde,Azul, Temperatura, Humedad");
}

void loop() {
  // read all the sensor values
  float temperatura = HTS.readTemperature();
  float humedad    = HTS.readHumidity();

  int rojo, verde, azul, lum, prox;
  float suma;

  // se espera hasta que existan datos de color y proximidad en el sensor
  while (!APDS.colorAvailable() || !APDS.proximityAvailable()) {}

  // Lee los valores de color y luminosidad del sensor
  APDS.readColor(rojo, verde, azul, lum, prox);
  
  prox = APDS.readProximity();  //Lee el valor de proximidad del sensor.
  sum = rojo + verde + azul + lum +  prox; //Lee el valor de proximidad del sensor.

  /* Si se dan las condiciones de proximidad mayor que 0, la suma de los colores mayor a cero y una luminosidad mayor a 10, 
  calcula las proporciones de los colores RGB, y luego los imprime junta a la humedad y 
  la temperatura que está detectando el sensor */
  if (if ( prox >= 0 && lum > 10 && suma > 0) {

    float redRatio = rojo / suma;
    float greenRatio = verde / suma;
    float blueRatio = azul / suma;

    Serial.print(redRatio, 3);
    Serial.print(',');
    Serial.print(greenRatio, 3);
    Serial.print(',');
    Serial.print(blueRatio, 3);
    Serial.print(',');
    Serial.print(temperatura, 3);
    Serial.print(',');
    Serial.print(humedad, 3);



  }

  // print an empty line
  //Serial.println();

  // wait 1 second to print again
  delay(1000);
}
