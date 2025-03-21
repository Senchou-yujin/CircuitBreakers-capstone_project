// Not ACCURATE


#include <Arduino.h>
#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

// Declare Fuzzy Inputs and Output globally
FuzzyInput *WaterQuality;
FuzzyInput *Tide;
FuzzyOutput *WaterGateAction;

void setup() {
    Serial.begin(115200);

    // Initialize FuzzyInput for Water Quality
    WaterQuality = new FuzzyInput(1);
    FuzzySet *toxic = new FuzzySet(0, 12.5, 25, 25);
    FuzzySet *poor = new FuzzySet(25, 38, 50, 50);
    FuzzySet *good = new FuzzySet(50, 63, 75, 75);
    FuzzySet *excellent = new FuzzySet(75, 88, 100, 100);
    
    WaterQuality->addFuzzySet(toxic);
    WaterQuality->addFuzzySet(poor);
    WaterQuality->addFuzzySet(good);
    WaterQuality->addFuzzySet(excellent);
    fuzzy->addFuzzyInput(WaterQuality);

    // Initialize FuzzyInput for Tide Level
    Tide = new FuzzyInput(2);
    FuzzySet *lowTide = new FuzzySet(0, 0, 0.79, 0.8);
    FuzzySet *highTide = new FuzzySet(0.8, 0.8, 1.4, 1.8);
    
    Tide->addFuzzySet(lowTide);
    Tide->addFuzzySet(highTide);
    fuzzy->addFuzzyInput(Tide);

    // Initialize FuzzyOutput for Water Gate Action
    WaterGateAction = new FuzzyOutput(1);
    FuzzySet *closeGate = new FuzzySet(0, 15, 30, 30);
    FuzzySet *conditional = new FuzzySet(30, 45, 60, 60);
    FuzzySet *openGate = new FuzzySet(55, 77.5, 100, 100);
    
    WaterGateAction->addFuzzySet(closeGate);
    WaterGateAction->addFuzzySet(conditional);
    WaterGateAction->addFuzzySet(openGate);
    fuzzy->addFuzzyOutput(WaterGateAction);

    // Define Fuzzy Rules
    
    // Rule 1: Toxic & Low Tide -> Open Gate
    FuzzyRuleAntecedent *toxic_low = new FuzzyRuleAntecedent();
    toxic_low->joinWithAND(toxic, lowTide);
    FuzzyRuleConsequent *open_output = new FuzzyRuleConsequent();
    open_output->addOutput(openGate);
    fuzzy->addFuzzyRule(new FuzzyRule(1, toxic_low, open_output));

    // Rule 2: Toxic & High Tide -> Open Gate
    FuzzyRuleAntecedent *toxic_high = new FuzzyRuleAntecedent();
    toxic_high->joinWithAND(toxic, highTide);
    fuzzy->addFuzzyRule(new FuzzyRule(2, toxic_high, open_output));

    // Rule 3: Poor & Low Tide -> Conditional
    FuzzyRuleAntecedent *poor_low = new FuzzyRuleAntecedent();
    poor_low->joinWithAND(poor, lowTide);
    FuzzyRuleConsequent *conditional_output = new FuzzyRuleConsequent();
    conditional_output->addOutput(conditional);
    fuzzy->addFuzzyRule(new FuzzyRule(3, poor_low, conditional_output));

    // Rule 4: Poor & High Tide -> Open Gate
    FuzzyRuleAntecedent *poor_high = new FuzzyRuleAntecedent();
    poor_high->joinWithAND(poor, highTide);
    fuzzy->addFuzzyRule(new FuzzyRule(4, poor_high, open_output));

    // Rule 5: Good & Low Tide -> Close Gate
    FuzzyRuleAntecedent *good_low = new FuzzyRuleAntecedent();
    good_low->joinWithAND(good, lowTide);
    FuzzyRuleConsequent *close_output = new FuzzyRuleConsequent();
    close_output->addOutput(closeGate);
    fuzzy->addFuzzyRule(new FuzzyRule(5, good_low, close_output));

    // Rule 6: Good & High Tide -> Close Gate
    FuzzyRuleAntecedent *good_high = new FuzzyRuleAntecedent();
    good_high->joinWithAND(good, highTide);
    fuzzy->addFuzzyRule(new FuzzyRule(6, good_high, close_output));

    // Rule 7: Excellent & Low Tide -> Close Gate
    FuzzyRuleAntecedent *excellent_low = new FuzzyRuleAntecedent();
    excellent_low->joinWithAND(excellent, lowTide);
    fuzzy->addFuzzyRule(new FuzzyRule(7, excellent_low, close_output));

    // Rule 8: Excellent & High Tide -> Close Gate
    FuzzyRuleAntecedent *excellent_high = new FuzzyRuleAntecedent();
    excellent_high->joinWithAND(excellent, highTide);
    fuzzy->addFuzzyRule(new FuzzyRule(8, excellent_high, close_output));

    Serial.println("Stage 2 Fuzzy System Initialized!");
}

void loop() {
    // Simulated input values (replace with actual sensor readings later)
    float waterQuality = 39.87; // Example value
    float tideLevel = 0.4; // Example value

    // Set Inputs
    fuzzy->setInput(1, waterQuality);
    fuzzy->setInput(2, tideLevel);

    // Evaluate Fuzzy Logic
    fuzzy->fuzzify();

    // Get Output
    float gateAction = fuzzy->defuzzify(1);
    
    Serial.print("Water Quality: "); Serial.print(waterQuality);
    Serial.print(" | Tide Level: "); Serial.print(tideLevel);
    Serial.print(" | Water Gate Action: "); Serial.println(gateAction);

    delay(2000);
}