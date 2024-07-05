#include <DETECTOR_DE_INCENDIOS_inferencing.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h>

enum sensor_status {
    NOT_USED = -1,
    NOT_INIT,
    INIT,
    SAMPLED
};

typedef struct {
    const char *name;
    float *value;
    uint8_t (*poll_sensor)(void);
    bool (*init_sensor)(void);    
    sensor_status status;
} eiSensors;

#define MAX_ACCEPTED_RANGE 2.0f
#define N_SENSORS 5
//#define contador 0

static const bool debug_nn = false;
static float data[N_SENSORS];
static bool ei_connect_fusion_list(const char *input_list);
static int8_t fusion_sensors[N_SENSORS];
static int fusion_ix = 0;
static int contador = 0;


float ei_get_sign(float number);
bool init_BARO(void);
bool init_APDS(void);
uint8_t poll_BARO(void);
uint8_t poll_APDS_color(void);
uint8_t poll_APDS_proximity(void);
uint8_t poll_APDS_gesture(void);

eiSensors sensors[] = {

    {"rojo", &data[0], &poll_APDS_color, &init_APDS, NOT_USED},
    {"verde", &data[1], &poll_APDS_color, &init_APDS, NOT_USED},
    {"azul", &data[2], &poll_APDS_color, &init_APDS, NOT_USED},
    {"presion", &data[3], &poll_BARO, &init_BARO, NOT_USED},
    {"temperatura", &data[4], &poll_BARO, &init_BARO, NOT_USED}, /////////////////
};

void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode ( 2 , OUTPUT) ;
    Serial.println("Edge Impulse Sensor Fusion Inference\r\n");

    if(ei_connect_fusion_list(EI_CLASSIFIER_FUSION_AXES_STRING) == false) {
        ei_printf("ERR: Errors in sensor list detected\r\n");
        return;
    }

    for(int i = 0; i < fusion_ix; i++) {
        if (sensors[fusion_sensors[i]].status == NOT_INIT) {
            sensors[fusion_sensors[i]].status = (sensor_status)sensors[fusion_sensors[i]].init_sensor();
            if (!sensors[fusion_sensors[i]].status) {
              ei_printf("%s axis sensor initialization failed.\r\n", sensors[fusion_sensors[i]].name);             
            }
            else {
              ei_printf("%s axis sensor initialization successful.\r\n", sensors[fusion_sensors[i]].name);
            }
        }
    }
}

void loop() {
    ei_printf("\nStarting inferencing in 2 seconds...\r\n");
    delay(2000);

    //if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != fusion_ix) {
//
    //    ei_printf("ERR: Sensors don't match the sensors required in the model\r\n"
    //    "Following sensors are required: %s\r\n", EI_CLASSIFIER_FUSION_AXES_STRING);
    //    ei_printf("fusion %d\r\n",fusion_ix);
    //    return;
    //}

    ei_printf("Sampling...\r\n");
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME) {
        int64_t next_tick = (int64_t)micros() + ((int64_t)EI_CLASSIFIER_INTERVAL_MS * 1000);

        for(int i = 0; i < fusion_ix; i++) {
            if (sensors[fusion_sensors[i]].status == INIT) {
                sensors[fusion_sensors[i]].poll_sensor();
                sensors[fusion_sensors[i]].status = SAMPLED;
            }
            if (sensors[fusion_sensors[i]].status == SAMPLED) {
                buffer[ix + i] = *sensors[fusion_sensors[i]].value;
                sensors[fusion_sensors[i]].status = INIT;
            }
        }

        int64_t wait_time = next_tick - (int64_t)micros();

        if(wait_time > 0) {
            delayMicroseconds(wait_time);
        }
    }

    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("ERR:(%d)\r\n", err);
        return;
    }

    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR:(%d)\r\n", err);
        return;
    }

    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.):\r\n",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    //int contador = 0;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("%s: %.5f\r\n", result.classification[ix].label, result.classification[ix].value);

        if (result.classification[0].value >= 0.85) {
            contador= contador +1;
            result.classification[0].value =0;

        } else if(result.classification[0].value > 0 && result.classification[0].value < 0.85) {
            contador = 0; // Reinicia el contador si el valor no cumple la condición
        }
        if (contador >= 3) {
            tone(2, 1000, 2000); // Activa el tono si se reciben tres valores consecutivos >= 0.85
        } else {
            noTone(2); // Desactiva el tono si no se cumplen las condiciones
        }
    }


#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\r\n", result.anomaly);
#endif
}

static int8_t ei_find_axis(char *axis_name) {
    int ix;
    for(ix = 0; ix < N_SENSORS; ix++) {
        if(strstr(axis_name, sensors[ix].name)) {
            return ix;
        }
    }
    return -1;
}

static bool ei_connect_fusion_list(const char *input_list) {
    char *buff;
    bool is_fusion = false;

    char *input_string = (char *)ei_malloc(strlen(input_list) + 1);
    if (input_string == NULL) {
        return false;
    }
    memset(input_string, 0, strlen(input_list) + 1);
    strncpy(input_string, input_list, strlen(input_list));

    memset(fusion_sensors, 0, N_SENSORS);
    fusion_ix = 0;

    buff = strtok(input_string, "+");

    while (buff != NULL) {
        int8_t found_axis = 0;

        is_fusion = false;
        found_axis = ei_find_axis(buff);

        if(found_axis >= 0) {
            if(fusion_ix < N_SENSORS) {
                fusion_sensors[fusion_ix++] = found_axis;
                sensors[found_axis].status = NOT_INIT;
            }
            is_fusion = true;
        }

        buff = strtok(NULL, "+ ");
    }

    ei_free(input_string);

    return is_fusion;
}

float ei_get_sign(float number) {
    return (number >= 0.0) ? 1.0 : -1.0;
}

bool init_BARO(void) {
  static bool init_status = false;
  if (!init_status) {
    init_status = BARO.begin();
  }
  return init_status;
}

bool init_APDS(void) {
  static bool init_status = false;
  if (!init_status) {
    init_status = APDS.begin();
  }
  return init_status;
}

uint8_t poll_BARO(void) {
    data[3] = BARO.readPressure();
    data[4] = BARO.readTemperature(); // Assuming you have a method to read temperature
    return 0;
}
//uint8_t poll_APDS_color(void) {
//    int temp_data[4];
//    if (APDS.colorAvailable()) {
//        APDS.readColor(temp_data[0], temp_data[1], temp_data[2], temp_data[3]);
//        data[1] = temp_data[0];
//        data[2] = temp_data[1];
//        data[3] = temp_data[2];
//        data[4] = temp_data[3];
//    }
//    return 0;
//}

uint8_t poll_APDS_color(void) {
    int temp_data[3];
    if (APDS.colorAvailable()) {
        APDS.readColor(temp_data[0], temp_data[1], temp_data[2]);
        float suma = temp_data[0] + temp_data[1] + temp_data[2];
        data[0] = (temp_data[0]/suma)*100; // rojo
        data[1] = (temp_data[1]/suma)*100; // verde
        data[2] = (temp_data[2]/suma)*100; // azul
    }
    return 0;
}



//uint8_t poll_APDS_proximity(void) {
//    if (APDS.proximityAvailable()) {
//        data[5] = (float)APDS.readProximity();
//    }
//    return 0;
//}

//uint8_t poll_APDS_gesture(void) {
//    if (APDS.gestureAvailable()) {
//        data[6] = (float)APDS.readGesture();
//    }
//    return 0;
//}
