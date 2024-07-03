//#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h> //RGB
#include <Arduino_HTS221.h>

void setup() {
  Serial.begin(115200); // Cambiado a 115200
  while (!Serial);

  //if ((!BARO.begin()) && (!APDS.begin())) {
  if (!APDS.begin()) {
    Serial.println("Failed to initialize pressure sensor and rgb!");
    while (1);
  }
}

void loop() {
  //float pressure = BARO.readPressure(); // LPS22HB usa hPa por defecto
  //float temperature = BARO.readTemperature();

  int rojo, verde, azul, c, p;
  float suma;

  while (!APDS.colorAvailable() || !APDS.proximityAvailable()) {}

  APDS.readColor(rojo, verde, azul, c);
  suma = rojo + verde + azul;
  p = APDS.readProximity();

  if (p >= 0 && c > 10 && suma > 0) {
    float redRatio = rojo / suma;
    float greenRatio = verde / suma;
    float blueRatio = azul / suma;

  Serial.print(redRatio, 3);
  Serial.print(",");
  Serial.print(greenRatio, 3);
  Serial.print(",");
  Serial.print(blueRatio, 3);}
  //Serial.print(',');
  //Serial.print(pressure);
  //Serial.print(',');
  //Serial.println(temperature);}
    // Enviar los datos en formato CSV

}

