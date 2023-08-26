#include <SPI.h>
// OPV mittlere Spannung 
float mid_voltage = 1.94;
// Werte für poti
float max_resistance = 9850;
float wiper_resistance = 75;
float step_resistance = max_resistance/256;

// Zweite Widerstand im Verstärkungsfaktor
float resistance2 = 1000;
float resistance3 = 1000;

int base_step= int((resistance3 * (mid_voltage / (5- mid_voltage)) - wiper_resistance) / step_resistance);


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
  int current_step = 0;
  float u_a = 0;

  while (current_step < 60 && u_a){
    // Widerstand einstellen
    SPI.transfer(0x00);
    SPI.transfer(int(current_step + base_step));
    // Ausgangsspannung einlesen 
    u_a = get_voltage() - mid_voltage;
    if (u_a > mid_voltage * 0.75) break;
    current_step += 1;
  }

  // Digitaler Potentiometer Widerstand ermitteln
  float digi_res = current_step * step_resistance + wiper_resistance;
  // Eingangsspannung berechnen
  float u_e = 5 *( digi_res / (digi_res + resistance3) ) - mid_voltage;
  // Faktor k ermitteln
  float factor_k = u_a / u_e;

  // Anhand von Faktor k und dem Widerstand resistance2 den unbekannten Widerstand ermitteln. 
  Serial.print("Widerstand  ");
  Serial.println((factor_k-1) * resistance2);

}
void loop() {
  calculate_resistance();
}