#include <SPI.h>

// Eingangsspannung Variablen
const float ue_r1 = 10000;
const float ue_r2 = 1000;
const float ue = 5 * (ue_r1 / (ue_r1 + ue_r2));

// Schritte für digitalen Potentiometer einstellen
const int max_step_resistance = 30;
const int min_step_resistance = 5;

// Werte für poti
float max_resistance = 9850;
float wiper_resistance = 75;
float step_resistance = max_resistance/256;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();   
}

float get_voltage(){
  return  analogRead(A0) * (5/1023.0);
}

void calculate_resistance()
{
  int current_step = max_step_resistance;
  float ua = 0.0;

  while ( current_step > min_step_resistance){
    // Digitalen Potentiometer verändern
    SPI.transfer(0x00);
    SPI.transfer(current_step);
    // Ausgangsspannung einlesen
    ua = get_voltage();
    if (ua > 3.5) break;
    current_step -= 1;
  }
  float digi_res = current_step * step_resistance + wiper_resistance;
  float factor_k = ua / ue;
  // Anhand von Faktor k und dem Widerstand resistance2 den unbekannten Widerstand ermitteln. 
  Serial.print("Widerstand  ");
  Serial.println((factor_k-1) * digi_res);
}

void loop() {
  calculate_resistance();
}