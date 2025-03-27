#include <Arduino.h>
#include <Fuzzy.h>

//PINS DESIGNATIONS
#define trigPin 5
#define echoPin 6

// VARIABLES na pwede nyong baguhin based sa locale nyo.
#define MAX_HEIGHT 250.0 // from sensor to riverbed

String gateState = "Unknown";
long duration;
float distanceCM, distanceMeters;
float waterLevelCM, waterLevelMeters;
int waterSTAT = 0;


Fuzzy *fuzzy = new Fuzzy();

// Declare Fuzzy Inputs and Output globally
FuzzyInput *pH;
FuzzyInput *DO;
FuzzyOutput *WaterQuality;

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
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Initialize FuzzyInput for pH level
    pH = new FuzzyInput(1);
    FuzzySet *acidic = new FuzzySet(-1, 2, 5.4, 6.3); 
    FuzzySet *normal = new FuzzySet(6.2, 7, 8.3, 8.5); 
    FuzzySet *alkaline = new FuzzySet(8.4, 9.5, 14, 15); 


    pH->addFuzzySet(acidic);
    pH->addFuzzySet(normal);
    pH->addFuzzySet(alkaline);
    fuzzy->addFuzzyInput(pH);

    // Initialize FuzzyInput for DO (Dissolved Oxygen)
    DO = new FuzzyInput(2);
    FuzzySet *veryLow = new FuzzySet(-1, 1, 2.4, 4.2); 
    FuzzySet *low = new FuzzySet(3.8, 5, 7.8, 8.2); 
    FuzzySet *normalDO = new FuzzySet(7.8, 9, 18, 21);
    
    DO->addFuzzySet(veryLow);
    DO->addFuzzySet(low);
    DO->addFuzzySet(normalDO);
    fuzzy->addFuzzyInput(DO);

    // Initialize FuzzyOutput for Water Quality based on table
    WaterQuality = new FuzzyOutput(1);
    FuzzySet *toxic = new FuzzySet(0, 12.5, 12.5, 25);
    FuzzySet *poor = new FuzzySet(26, 38, 38, 50);
    FuzzySet *good = new FuzzySet(51, 63, 63, 75);
    FuzzySet *excellent = new FuzzySet(76, 88, 88, 100);
    
    WaterQuality->addFuzzySet(toxic);
    WaterQuality->addFuzzySet(poor);
    WaterQuality->addFuzzySet(good);
    WaterQuality->addFuzzySet(excellent);
    fuzzy->addFuzzyOutput(WaterQuality);

    // Define Fuzzy Rules

    // (1) Acidic + Very Low DO → Toxic 
    FuzzyRuleAntecedent *acidic_veryLow = new FuzzyRuleAntecedent();
    acidic_veryLow->joinWithAND(acidic, veryLow);
    FuzzyRuleConsequent *toxic_output = new FuzzyRuleConsequent();
    toxic_output->addOutput(toxic);
    fuzzy->addFuzzyRule(new FuzzyRule(1, acidic_veryLow, toxic_output));

    // (2) Acidic + Low DO → Poor
    FuzzyRuleAntecedent *acidic_low = new FuzzyRuleAntecedent();
    acidic_low->joinWithAND(acidic, low);
    FuzzyRuleConsequent *poor_output = new FuzzyRuleConsequent();
    poor_output->addOutput(poor);
    fuzzy->addFuzzyRule(new FuzzyRule(2, acidic_low, poor_output));

    // (3) Acidic + Normal DO → Good
    FuzzyRuleAntecedent *acidic_normal = new FuzzyRuleAntecedent();
    acidic_normal->joinWithAND(acidic, normalDO);
    FuzzyRuleConsequent *good_output = new FuzzyRuleConsequent();
    good_output->addOutput(good);
    fuzzy->addFuzzyRule(new FuzzyRule(3, acidic_normal, good_output));

    // (4) Normal pH + Very Low DO → Poor 
    FuzzyRuleAntecedent *normal_veryLow = new FuzzyRuleAntecedent();
    normal_veryLow->joinWithAND(normal, veryLow);
    fuzzy->addFuzzyRule(new FuzzyRule(4, normal_veryLow, poor_output));

    // (5) Normal pH + Low DO → Excellent
    FuzzyRuleAntecedent *normal_low = new FuzzyRuleAntecedent();
    normal_low->joinWithAND(normal, low);
    FuzzyRuleConsequent *excellent_output = new FuzzyRuleConsequent();
    excellent_output->addOutput(excellent);
    fuzzy->addFuzzyRule(new FuzzyRule(5, normal_low, excellent_output));

    // (6) Normal pH + Normal DO → Excellent
    FuzzyRuleAntecedent *normal_normal = new FuzzyRuleAntecedent();
    normal_normal->joinWithAND(normal, normalDO);
    fuzzy->addFuzzyRule(new FuzzyRule(6, normal_normal, excellent_output));

    // (7) Alkaline + Very Low DO → Toxic
    FuzzyRuleAntecedent *alkaline_veryLow = new FuzzyRuleAntecedent();
    alkaline_veryLow->joinWithAND(alkaline, veryLow);
    fuzzy->addFuzzyRule(new FuzzyRule(7, alkaline_veryLow, toxic_output));

    // (8) Alkaline + Low DO → Poor 
    FuzzyRuleAntecedent *alkaline_low = new FuzzyRuleAntecedent();
    alkaline_low->joinWithAND(alkaline, low);
    fuzzy->addFuzzyRule(new FuzzyRule(8, alkaline_low, poor_output));

    // (9) Alkaline + Normal DO → Good 
    FuzzyRuleAntecedent *alkaline_normal = new FuzzyRuleAntecedent();
    alkaline_normal->joinWithAND(alkaline, normalDO);
    fuzzy->addFuzzyRule(new FuzzyRule(9, alkaline_normal, good_output)); 


    Serial.println("Fuzzy system initialized!");
}

void loop() {
    Serial.println("Simulating all possible outputs...\n");

    for (float pH_value = 0.0; pH_value <= 14.0; pH_value += 0.5) {  
        for (float DO_value = 0.0; DO_value <= 20.0; DO_value += 0.5) { 
            
            // Set Inputs
            fuzzy->setInput(1, pH_value);
            fuzzy->setInput(2, DO_value);

            // Evaluate Fuzzy Logic
            fuzzy->fuzzify();

            // Get Fuzzy Output
            float waterQuality = fuzzy->defuzzify(1);
            String qualityLabel = classifyWaterQuality(waterQuality);

            // Simulate Tide Data
            float waterLevelMeters = getWaterLevel();
            String tideStatus = getTideStatus(waterLevelMeters);
            String gateDecision = getGateDecision(waterSTAT, tideStatus);

            // Debug Output
            Serial.println("------------------------------------------------------");
            Serial.print("pH: "); Serial.print(pH_value, 1);
            Serial.print(" | DO: "); Serial.print(DO_value, 1);
            Serial.print(" | Water Quality: "); Serial.print(waterQuality);
            Serial.print(" | Classification: "); Serial.println(qualityLabel);
            
            Serial.print("Tide Height: "); Serial.print(waterLevelMeters, 2); Serial.print(" m");
            Serial.print(" | Tide Status: "); Serial.println(tideStatus);
            
            Serial.print("Gate Decision: "); Serial.println(gateDecision);
            Serial.println("------------------------------------------------------");

            delay(200); 
        }
    }

    Serial.println("Simulation complete!");
    while (1); 
}