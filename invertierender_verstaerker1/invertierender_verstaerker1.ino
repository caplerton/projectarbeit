
int steps = 30;
int factor_k = 2;
int resistance2 = 1000;
int current_step = 0;
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
  analogWrite(9,120+ steps *current_step);
  start_time = millis();
  increase = false;
}


void calculate_resistance(){
  // Bedingung überprüfen
  if (start_time > 0) return; 
  if (millis() - start_time < 2000)return;
  
  float u2 = get_voltage();

  // Überprüfen ob maximale Spannung erreicht wurde
  if (u2 > 4 || current_step == 4) {
    // Widerstand ermitteln
    float u1 = (120+ steps *current_step)/256 *5;
    factor_k  = u2 / u1;
    Serial.print("Widerstand  ");
    Serial.println((factor_k-1) * resistance2);
    current_step = 0;
    }
  else current_step += 1;
  
  increase = true;
  start_time = 0;
}


void loop() {
  configure_voltage();

  calculate_resistance();
}
