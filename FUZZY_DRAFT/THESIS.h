// Define analog input pins
#define DO_PIN A0   // DO Sensor connected to A0
#define PH_PIN A1   // pH Sensor connected to A1

// Calibration values for pH sensor
#define PH_7_VOLTAGE 1.2    // Voltage at pH 7 (Neutral)
#define PH_SLOPE -0.18      // Voltage change per pH unit
#define PH_SAMPLES 20       // Number of samples for averaging

// Calibration values for DO sensor
#define VREF 5.0            // Reference voltage
#define ADC_RESOLUTION 1023.0 // 10-bit ADC resolution
#define DO_SATURATION 25    // Maximum DO value in mg/L at calibration
#define DO_SAMPLES 20       // Number of samples for averaging

void setup() {
    Serial.begin(9600);  // Start serial communication
}

void loop() {
    // Get sensor readings
    float doValue = readDO();
    float pHValue = readPH();
    
    // Print sensor readings
    Serial.println("===== Averaged Sensor Readings =====");
    Serial.print("DO Level: ");
    Serial.print(doValue, 2);
    Serial.println(" mg/L");
    
    Serial.print("pH Level: ");
    Serial.print(pHValue, 2);
    Serial.println(" pH");
    Serial.println("-----------------------------------");
    delay(2000);  // Wait 2 seconds before next reading
}

float readDO() {
    float sum = 0;
    for(int i = 0; i < DO_SAMPLES; i++) {
        int raw = analogRead(DO_PIN);
        float voltage = raw * (VREF / ADC_RESOLUTION);
        sum += (voltage / VREF) * DO_SATURATION;
        delay(10);
    }
    return sum / DO_SAMPLES;
}

float readPH() {
    float sum = 0;
    for(int i = 0; i < PH_SAMPLES; i++) {
        int raw = analogRead(PH_PIN);
        float voltage = raw * (VREF / ADC_RESOLUTION);
        sum += 7.0 + (voltage - PH_7_VOLTAGE) / PH_SLOPE;
        delay(10);
    }
    return sum / PH_SAMPLES;
}