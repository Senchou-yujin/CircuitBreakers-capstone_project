#include <Arduino.h>
#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

// Declare Fuzzy Inputs and Output globally
FuzzyInput *pH;
FuzzyInput *DO;
FuzzyOutput *WaterQuality;

void setup() {
    Serial.begin(115200);

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

    // Initialize FuzzyOutput for Water Quality
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

    // Acidic pH Rules
    FuzzyRuleAntecedent *acidic_veryLow = new FuzzyRuleAntecedent();
    acidic_veryLow->joinWithAND(acidic, veryLow);
    FuzzyRuleConsequent *toxic_output = new FuzzyRuleConsequent();
    toxic_output->addOutput(toxic);
    fuzzy->addFuzzyRule(new FuzzyRule(1, acidic_veryLow, toxic_output));

    FuzzyRuleAntecedent *acidic_low = new FuzzyRuleAntecedent();
    acidic_low->joinWithAND(acidic, low);
    FuzzyRuleConsequent *poor_output = new FuzzyRuleConsequent();
    poor_output->addOutput(poor);
    fuzzy->addFuzzyRule(new FuzzyRule(2, acidic_low, poor_output));

    FuzzyRuleAntecedent *acidic_normal = new FuzzyRuleAntecedent();
    acidic_normal->joinWithAND(acidic, normalDO);
    FuzzyRuleConsequent *good_output = new FuzzyRuleConsequent();
    good_output->addOutput(good);
    fuzzy->addFuzzyRule(new FuzzyRule(3, acidic_normal, good_output));

    // Normal pH Rules
    FuzzyRuleAntecedent *normal_veryLow = new FuzzyRuleAntecedent();
    normal_veryLow->joinWithAND(normal, veryLow);
    fuzzy->addFuzzyRule(new FuzzyRule(4, normal_veryLow, poor_output));

    FuzzyRuleAntecedent *normal_low = new FuzzyRuleAntecedent();
    normal_low->joinWithAND(normal, low);
    FuzzyRuleConsequent *excellent_output = new FuzzyRuleConsequent();
    excellent_output->addOutput(excellent);
    fuzzy->addFuzzyRule(new FuzzyRule(5, normal_low, excellent_output));

    FuzzyRuleAntecedent *normal_normal = new FuzzyRuleAntecedent();
    normal_normal->joinWithAND(normal, normalDO);
    fuzzy->addFuzzyRule(new FuzzyRule(6, normal_normal, excellent_output));

    // Alkaline pH Rules
    FuzzyRuleAntecedent *alkaline_veryLow = new FuzzyRuleAntecedent();
    alkaline_veryLow->joinWithAND(alkaline, veryLow);
    fuzzy->addFuzzyRule(new FuzzyRule(7, alkaline_veryLow, toxic_output));

    FuzzyRuleAntecedent *alkaline_low = new FuzzyRuleAntecedent();
    alkaline_low->joinWithAND(alkaline, low);
    fuzzy->addFuzzyRule(new FuzzyRule(8, alkaline_low, poor_output));

    FuzzyRuleAntecedent *alkaline_normal = new FuzzyRuleAntecedent();
    alkaline_normal->joinWithAND(alkaline, normalDO);
    fuzzy->addFuzzyRule(new FuzzyRule(9, alkaline_normal, good_output));

    Serial.println("Fuzzy system initialized!");
}

void loop() {
  // Palitan ng Actual Sensor Reading Later 
  float pH_value = 7.0;
  float DO_value = 3.5;

  // Set Inputs
  fuzzy->setInput(1, pH_value);
  fuzzy->setInput(2, DO_value);

  // Evaluate Fuzzy Logic
  fuzzy->fuzzify();

  // Get Output
  float waterQuality = fuzzy->defuzzify(1);
  
  Serial.print("pH: "); Serial.print(pH_value);
  Serial.print(" | DO: "); Serial.print(DO_value);
  Serial.print(" | Water Quality: "); Serial.println(waterQuality);

  delay(2000);
}
