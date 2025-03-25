// Not ACCURATE

#include <Fuzzy.h>

// Create fuzzy logic object
Fuzzy *fuzzy = new Fuzzy();

// Inputs
FuzzyInput *pH_Level = new FuzzyInput(1);
FuzzyInput *DO_Level = new FuzzyInput(2);
FuzzyInput *Tide_Level = new FuzzyInput(3);

// Outputs
FuzzyOutput *Water_Gate_Action = new FuzzyOutput(1);

// Membership Functions
void setupFuzzy() {
    // **pH Level Membership Functions**
    FuzzySet *acidic = new FuzzySet(-1, 2, 5.4, 6.3); 
    FuzzySet *normal = new FuzzySet(6.2, 7, 8.3, 8.5); 
    FuzzySet *alkaline = new FuzzySet(8.4, 9.5, 14, 15); 
    pH_Level->addFuzzySet(acidic);
    pH_Level->addFuzzySet(normal_pH);
    pH_Level->addFuzzySet(alkaline);
    fuzzy->addFuzzyInput(pH_Level);

    // **DO Level Membership Functions**
    FuzzySet *veryLow = new FuzzySet(-1, 1, 2.4, 4.2); 
    FuzzySet *low = new FuzzySet(3.8, 5, 7.8, 8.2); 
    FuzzySet *normalDO = new FuzzySet(7.8, 9, 18, 21);
    DO_Level->addFuzzySet(very_low);
    DO_Level->addFuzzySet(low);
    DO_Level->addFuzzySet(normal_do);
    fuzzy->addFuzzyInput(DO_Level);

    // **Tide Level Membership Functions**
    FuzzySet *low_tide = new FuzzySet(0, 0, 0.79, 0.8);
    FuzzySet *high_tide = new FuzzySet(0.8, 0.8, 1.4, 1.8);
    Tide_Level->addFuzzySet(low_tide);
    Tide_Level->addFuzzySet(high_tide);
    fuzzy->addFuzzyInput(Tide_Level);
   
    // Water Gate Action
    FuzzySet *close = new FuzzySet(0, 0, 15, 30);
    FuzzySet *conditional = new FuzzySet(30, 30, 45, 60);
    FuzzySet *open = new FuzzySet(55, 55, 77.5, 100);
    Water_Gate_Action->addFuzzySet(close);
    Water_Gate_Action->addFuzzySet(conditional);
    Water_Gate_Action->addFuzzySet(open);
    fuzzy->addFuzzyOutput(Water_Gate_Action);

    // Define fuzzy rule consequents
    FuzzyRuleConsequent *then_open = new FuzzyRuleConsequent();
    then_open->addOutput(open);

    FuzzyRuleConsequent *then_conditional = new FuzzyRuleConsequent();
    then_conditional->addOutput(conditional);

    FuzzyRuleConsequent *then_close = new FuzzyRuleConsequent();
    then_close->addOutput(close);


    // Rule 1: IF pH is acidic AND DO is very low AND Tide is low THEN Gate is Open
    FuzzyRuleAntecedent *ph_acidic_and_do_very_low = new FuzzyRuleAntecedent();
    ph_acidic_and_do_very_low->joinWithAND(acidic, very_low);

    FuzzyRuleAntecedent *if_pH_acidic_and_DO_very_low_and_tide_low = new FuzzyRuleAntecedent();
    if_pH_acidic_and_DO_very_low_and_tide_low->joinWithAND(ph_acidic_and_do_very_low, low_tide);

    FuzzyRule *rule1 = new FuzzyRule(1, if_pH_acidic_and_DO_very_low_and_tide_low, then_open);
    fuzzy->addFuzzyRule(rule1);

    // Rule 2: IF pH is acidic AND DO is very low AND Tide is high THEN Gate is Conditional
    FuzzyRuleAntecedent *if_pH_acidic_and_DO_very_low_and_tide_high = new FuzzyRuleAntecedent();
    if_pH_acidic_and_DO_very_low_and_tide_high->joinWithAND(ph_acidic_and_do_very_low, high_tide);

    FuzzyRule *rule2 = new FuzzyRule(2, if_pH_acidic_and_DO_very_low_and_tide_high, then_conditional);
    fuzzy->addFuzzyRule(rule2);

    // Rule 3: IF pH is acidic AND DO is low AND Tide is low THEN Gate is Open
    FuzzyRuleAntecedent *ph_acidic_and_do_low = new FuzzyRuleAntecedent();
    ph_acidic_and_do_low->joinWithAND(acidic, low);

    FuzzyRuleAntecedent *if_pH_acidic_and_DO_low_and_tide_low = new FuzzyRuleAntecedent();
    if_pH_acidic_and_DO_low_and_tide_low->joinWithAND(ph_acidic_and_do_low, low_tide);

    FuzzyRule *rule3 = new FuzzyRule(3, if_pH_acidic_and_DO_low_and_tide_low, then_open);
    fuzzy->addFuzzyRule(rule3);

    // Rule 4: IF pH is acidic AND DO is low AND Tide is high THEN Gate is Conditional
    FuzzyRuleAntecedent *if_pH_acidic_and_DO_low_and_tide_high = new FuzzyRuleAntecedent();
    if_pH_acidic_and_DO_low_and_tide_high->joinWithAND(ph_acidic_and_do_low, high_tide);

    FuzzyRule *rule4 = new FuzzyRule(4, if_pH_acidic_and_DO_low_and_tide_high, then_conditional);
    fuzzy->addFuzzyRule(rule4);

    // Rule 5: IF pH is acidic AND DO is normal AND Tide is low THEN Gate is Open
    FuzzyRuleAntecedent *ph_acidic_and_do_normal = new FuzzyRuleAntecedent();
    ph_acidic_and_do_normal->joinWithAND(acidic, normal_do);

    FuzzyRuleAntecedent *if_pH_acidic_and_DO_normal_and_tide_low = new FuzzyRuleAntecedent();
    if_pH_acidic_and_DO_normal_and_tide_low->joinWithAND(ph_acidic_and_do_normal, low_tide);

    FuzzyRule *rule5 = new FuzzyRule(5, if_pH_acidic_and_DO_normal_and_tide_low, then_open);
    fuzzy->addFuzzyRule(rule5);

    // Rule 6: IF pH is acidic AND DO is normal AND Tide is high THEN Gate is Conditional
    FuzzyRuleAntecedent *if_pH_acidic_and_DO_normal_and_tide_high = new FuzzyRuleAntecedent();
    if_pH_acidic_and_DO_normal_and_tide_high->joinWithAND(ph_acidic_and_do_normal, high_tide);

    FuzzyRule *rule6 = new FuzzyRule(6, if_pH_acidic_and_DO_normal_and_tide_high, then_conditional);
    fuzzy->addFuzzyRule(rule6);

    // Rule 7: IF pH is normal AND DO is very low AND Tide is low THEN Gate is Open
    FuzzyRuleAntecedent *ph_normal_and_do_very_low = new FuzzyRuleAntecedent();
    ph_normal_and_do_very_low->joinWithAND(normal_pH, very_low);

    FuzzyRuleAntecedent *if_pH_normal_and_DO_very_low_and_tide_low = new FuzzyRuleAntecedent();
    if_pH_normal_and_DO_very_low_and_tide_low->joinWithAND(ph_normal_and_do_very_low, low_tide);

    FuzzyRule *rule7 = new FuzzyRule(7, if_pH_normal_and_DO_very_low_and_tide_low, then_open);
    fuzzy->addFuzzyRule(rule7);

    // Rule 8: IF pH is normal AND DO is very low AND Tide is high THEN Gate is Conditional
    FuzzyRuleAntecedent *if_pH_normal_and_DO_very_low_and_tide_high = new FuzzyRuleAntecedent();
    if_pH_normal_and_DO_very_low_and_tide_high->joinWithAND(ph_normal_and_do_very_low, high_tide);

    FuzzyRule *rule8 = new FuzzyRule(8, if_pH_normal_and_DO_very_low_and_tide_high, then_conditional);
    fuzzy->addFuzzyRule(rule8);

}

void setup() {
    Serial.begin(9600);
    setupFuzzy();
}

void loop() {
    float pH = 6;
    float DO = 3.5;
    float Tide = 0.2;

    fuzzy->setInput(1, pH);
    fuzzy->setInput(2, DO);
    fuzzy->setInput(3, Tide);

    fuzzy->fuzzify();

    float gate_action = fuzzy->defuzzify(1);
    Serial.print("Water Gate Action: ");
    Serial.println(gate_action);

    delay(2000);
}
