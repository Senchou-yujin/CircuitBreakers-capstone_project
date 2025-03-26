#define trigPin 5
#define echoPin 6

// Define riverbed distance (adjust this based on actual measurement)
#define MAX_HEIGHT 250.0 // Example: 200 cm (2 meters) from sensor to riverbed

long duration;
float distanceCM, distanceMeters;
float waterLevelCM, waterLevelMeters;

// Function to get the actual water level
float getWaterLevel() {
    // Trigger the sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echoPin. pulseIn() returns the duration in microseconds:
    duration = pulseIn(echoPin, HIGH);
    
    // Convert duration to distance (cm and meters)
    distanceCM = (duration * 0.034 / 2); // Distance in cm
    distanceMeters = distanceCM / 100.0; // Convert cm to meters

    // Calculate actual water level
    waterLevelCM = MAX_HEIGHT - distanceCM;
    waterLevelMeters = waterLevelCM / 100.0;

    return waterLevelMeters; // Returning meters, but we print both
}

// Function to determine tide status
String getTideStatus(float waterLevelMeters) {
    if (waterLevelMeters >= 0 && waterLevelMeters <= 0.8) return "Low";
    else if (waterLevelMeters >= 0.9 && waterLevelMeters <= 1.8) return "High";
    else return "Unknown"; // Handle out-of-range cases
}

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
    Serial.println("Ultrasonic Sensor - Water Level Calculation");
}

void loop() {
    // Get actual water level
    float waterLevelMeters = getWaterLevel();
    String tideStatus = getTideStatus(waterLevelMeters);

    // Print results
    Serial.print("Measured Distance: "); Serial.print(distanceCM); Serial.print(" cm (");
    Serial.print(distanceMeters); Serial.print(" m) | ");

    Serial.print("Water Level: "); Serial.print(waterLevelCM); Serial.print(" cm (");
    Serial.print(waterLevelMeters); Serial.print(" m) | ");

    Serial.print("Tide Status: "); Serial.println(tideStatus);

    delay(1000); // Wait before next reading
}
