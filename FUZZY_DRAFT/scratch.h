#define trigPin 5
#define echoPin 6

#define MAX_HEIGHT 250.0 // from sensor to riverbed

String gateState = "Unknown";
long duration;
float distanceCM, distanceMeters;
float waterLevelCM, waterLevelMeters;
int waterSTAT = 0;


float getWaterLevel() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    
    distanceCM = (duration * 0.034 / 2); // Distance in cm
    distanceMeters = distanceCM / 100.0; // Convert cm to meters

    // Calculate actual water level
    waterLevelCM = MAX_HEIGHT - distanceCM;
    waterLevelMeters = waterLevelCM / 100.0;

    return waterLevelMeters;
}

// Function to determine tide status
String getTideStatus(float waterLevelMeters) {
    if (waterLevelMeters >= 0 && waterLevelMeters <= 0.8) return "Low";
    else if (waterLevelMeters >= 0.9 && waterLevelMeters <= 2) return "High";
    else return "Unknown";
}

// Function to classify water quality and set waterSTAT
String classifyWaterQuality(float waterQuality) {
    if (waterQuality >= 0 && waterQuality <= 25) {
        waterSTAT = 1;
        return "Toxic (T)";
    } else if (waterQuality > 25 && waterQuality <= 50) {
        waterSTAT = 2;
        return "Poor (P)";
    } else if (waterQuality > 50 && waterQuality <= 75) {
        waterSTAT = 3;
        return "Good (G)";
    } else if (waterQuality > 75 && waterQuality <= 100) {
        waterSTAT = 4;
        return "Excellent (E)";
    } else {
        waterSTAT = 0;
        return "Unknown";
    }
}

// Water Quality + Tide Status = Gate Decision
String getGateDecision(int waterSTAT, String tideStatus) {
    if ((waterSTAT == 1) && (tideStatus == "Low")) gateState = "Open";        // Toxic + Low  = Open 
    else if ((waterSTAT == 1) && (tideStatus == "High")) gateState = "Open";  // Toxic + High = Open 

    else if ((waterSTAT == 2) && (tideStatus == "Low")) gateState = "Conditional"; // Poor + Low   = Conditional 
    else if ((waterSTAT == 2) && (tideStatus == "High")) gateState = "Open";       // Poor + High  = Open 

    else if ((waterSTAT == 3) && (tideStatus == "Low")) gateState = "Close";       // Good + Low   = Close 
    else if ((waterSTAT == 3) && (tideStatus == "High")) gateState = "Conditional"; // Good + High = Conditional 

    else if ((waterSTAT == 4) && (tideStatus == "Low")) gateState = "Close";       // Excellent + Low  = Close 
    else if ((waterSTAT == 4) && (tideStatus == "High")) gateState = "Close";      // Excellent + High = Close 
    else gateState = "Unknown";

    return gateState;
}

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
    Serial.println("Fishpond Monitoring System - Gate Control Decision");
}

void loop() {
    // Simulated water quality input (Replace with actual sensor data)
    float waterQuality = 88;  // Change this value for testing

    // Get actual water level
    float waterLevelMeters = getWaterLevel();
    String tideStatus = getTideStatus(waterLevelMeters);
    String qualityLabel = classifyWaterQuality(waterQuality);
    String gateDecision = getGateDecision(waterSTAT, tideStatus);

    // Print results
    Serial.print("Measured Distance: "); Serial.print(distanceCM); Serial.print(" cm (");
    Serial.print(distanceMeters); Serial.print(" m) | ");

    Serial.print("Water Level: "); Serial.print(waterLevelCM); Serial.print(" cm (");
    Serial.print(waterLevelMeters); Serial.print(" m) | ");

    Serial.print("Tide Status: "); Serial.print(tideStatus); Serial.print(" | ");

    Serial.print("Water Quality: "); Serial.print(qualityLabel); Serial.print(" | ");

    Serial.print("Gate Decision: "); Serial.println(gateState);

    delay(2000);
}
