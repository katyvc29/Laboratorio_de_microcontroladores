#include <Arduino_APDS9960.h>
#include <Arduino_LPS22HB.h>

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!APDS.begin() || !BARO.begin()) {
        Serial.println("No se inició correctamente");
        while (1);
    }
}

void loop() {
   
        int rojo, verde, azul, lum, prox;
        float suma;

        // Espera hasta que existan datos de color y proximidad en el sensor
        while (!APDS.colorAvailable() || !APDS.proximityAvailable()) {}

        // Lee los valores de color y luminosidad del sensor
        APDS.readColor(rojo, verde, azul, lum);
        prox = APDS.readProximity();

        suma = rojo + verde + azul;

        if (prox >= 0 && lum > 10 && suma > 0) {
            float redRatio = (float)rojo / suma;
            float greenRatio = (float)verde / suma;
            float blueRatio = (float)azul / suma;

            float pressure = BARO.readPressure();
            float temperature = BARO.readTemperature();

            // Enviar los datos en formato CSV
            Serial.print(redRatio, 3);
            Serial.print(',');
            Serial.print(greenRatio, 3);
            Serial.print(',');
            Serial.print(blueRatio, 3);
            Serial.print(',');
            Serial.print(pressure, 6);
            Serial.print(',');
            Serial.println(temperature, 6);
        }
}
