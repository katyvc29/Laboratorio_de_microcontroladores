#include <Arduino_LPS22HB.h>

void setup() {
  Serial.begin(115200); // Cambiado a 115200
  while (!Serial);

  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
}

void loop() {
  float pressure = BARO.readPressure(); // LPS22HB usa hPa por defecto

  float temperature = BARO.readTemperature();

  // Enviar los datos en formato CSV
  Serial.print(pressure);
  Serial.print(",");
  Serial.println(temperature);
}

