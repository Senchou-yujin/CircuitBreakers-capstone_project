#include <Arduino.h>
#include <Fuzzy.h>

Fuzzy *fuzzy = new Fuzzy();

// Declare Fuzzy Inputs and Output globally
FuzzyInput *pH;
FuzzyInput *DO;
FuzzyOutput *WaterQuality;

    String classifyWaterQuality(float defuzzifiedValue) {
      if (defuzzifiedValue >= 0 && defuzzifiedValue <= 25) {
          return "Toxic";
      } else if (defuzzifiedValue > 25 && defuzzifiedValue <= 50) {
          return "Poor";
      } else if (defuzzifiedValue > 50 && defuzzifiedValue <= 75) {
          return "Good";
      } else {
          return "Excellent";
      }
    }


void setup() {
    Serial.begin(115200);

    // Initialize FuzzyInput for pH level
    pH = new FuzzyInput(1);
    FuzzySet *acidic = new FuzzySet(1, 3.5, 6, 6.4);
    FuzzySet *normal = new FuzzySet(6, 7.55, 7.55, 9);
    FuzzySet *alkaline = new FuzzySet(8, 9, 14.58, 19.25);
    
    pH->addFuzzySet(acidic);
    pH->addFuzzySet(normal);
    pH->addFuzzySet(alkaline);
    fuzzy->addFuzzyInput(pH);

    // Initialize FuzzyInput for DO (Dissolved Oxygen)
    DO = new FuzzyInput(2);
    FuzzySet *veryLow = new FuzzySet(0, 0, 1.5, 2.5);
    FuzzySet *low = new FuzzySet(1, 2.5, 2.5, 4);
    FuzzySet *normalDO = new FuzzySet(3, 4, 10.42, 13.75);
    
    DO->addFuzzySet(veryLow);
    DO->addFuzzySet(low);
    DO->addFuzzySet(normalDO);
    fuzzy->addFuzzyInput(DO);

    // Initialize FuzzyOutput for Water Quality
    WaterQuality = new FuzzyOutput(1);
    FuzzySet *toxic = new FuzzySet(0, 12.5, 25, 25);
    FuzzySet *poor = new FuzzySet(25, 38, 50, 50);
    FuzzySet *good = new FuzzySet(50, 63, 75, 75);
    FuzzySet *excellent = new FuzzySet(75, 88, 100, 100);
    
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
  float pH_value = 8.9;
  float DO_value = 6;

  // Set Inputs
  fuzzy->setInput(1, pH_value);
  fuzzy->setInput(2, DO_value);

  // Evaluate Fuzzy Logic
  fuzzy->fuzzify();

  // Get Output
  // float waterQuality = fuzzy->defuzzify(1);
  
  // Serial.print("pH: "); Serial.print(pH_value);
  // Serial.print(" | DO: "); Serial.print(DO_value);
  // Serial.print(" | Water Quality: "); Serial.println(waterQuality);

  float waterQuality = fuzzy->defuzzify(1);
  String category = classifyWaterQuality(waterQuality);

  Serial.print("Status ng Fish Pond: "); Serial.print("Insert Date Here");
  Serial.print(" | Water Quality: "); Serial.print(waterQuality);
  Serial.print(" | Category: "); Serial.print(category);
  Serial.print(" | Tide Status: "); Serial.println("Insert Low/Hi-Tide");

  delay(2000);
}
