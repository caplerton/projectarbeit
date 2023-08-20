
int steps = 30;
int factor_k = 2;
int resistance = 1000;
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
  // Widerstand ermitteln
  float res = 0;
  float u2 = get_voltage();
  res = resistance * ((factor_k * 5 * current_step/ 255.0) - u2)/u2;

  // Überprüfen ob Ergebnis erreicht wurde
  if (u2 > 4 || current_step == 4) {
    Serial.print("Widerstand  ");
    Serial.println(res);
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
