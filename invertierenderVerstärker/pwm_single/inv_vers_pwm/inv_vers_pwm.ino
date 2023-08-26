// OPV mittlere Spannung 
float mid_voltage = 1.94;
// Step variable und Base step berechnen
int steps = 0;
const int base_step = int((mid_voltage / 5)  * 256);

// Zweite Widerstand im Verstärkungsfaktor
int resistance2 = 1000;

bool increase = true;
long start_time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9,OUTPUT);
  analogWrite(9,180);
}

float get_voltage(){
  return  analogRead(A0) * (5/1023.0);
}

void configure_voltage(){
  if (!increase) return;
  analogWrite(9,base_step + steps);
  start_time = millis();
  increase = false;
}

void calculate_resistance(){
  // Bedingung ueberpruefen
  if (start_time > 0) return; 
  if (millis() - start_time < 10)return;
  
  float ua = get_voltage() - mid_voltage;

  // Ueberpruefen ob maximale Spannung erreicht wurde
  if (ua > mid_voltage * 0.75 || steps < 80) {
    // Widerstand ermitteln
    float ue = ((base_step + steps)/256 *5) - mid_voltage;
    float factor_k  = ua / ue;
    Serial.print("Widerstand  ");
    Serial.println((factor_k-1) * resistance2);
    steps = 0;
    }
  else steps += 1;
  
  increase = true;
  start_time = 0;
}

void loop() {
  configure_voltage();

  calculate_resistance();
}