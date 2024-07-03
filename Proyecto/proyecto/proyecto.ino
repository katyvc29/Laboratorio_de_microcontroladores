#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h> //RGB

void setup() {
  Serial.begin(115200); // Cambiado a 115200
  while (!Serial);

  if (!BARO.begin() || !APDS.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
}

void loop() {
  //Calculo del RGB 
  int rojo, verde, azul, lum, prox;
  float sum;
  // se espera hasta que existan datos de color y proximidad en el sensor
  while (!APDS.colorAvailable() || !APDS.proximityAvailable()) {}

  // Lee los valores de color y luminosidad del sensor
  APDS.readColor(rojo, verde, azul, lum);
  
  prox = APDS.readProximity();  //Lee el valor de proximidad del sensor

  sum = rojo + verde + azul;
  float redRatio = rojo / sum;
  float greenRatio = verde / sum;
  float blueRatio = azul / sum;

  //Calculo de la presion y la temperatura 
  float pressure = BARO.readPressure(); // LPS22HB usa hPa por defecto

  float temperature = BARO.readTemperature();

  // Enviar los datos en formato CSV
  Serial.print(redRatio, 3);
  Serial.print(',');
  Serial.print(greenRatio, 3);
  Serial.print(',');
  Serial.print(blueRatio, 3);
  Serial.print(',');
  Serial.print(pressure);
  Serial.print(",");
  Serial.println(temperature);
}

