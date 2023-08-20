#include <SPI.h>
#define r1 7
#define r2 6  
#define r3 5 

void setup() {
  pinMode(10, OUTPUT); 
  SPI.begin();         
  Serial.begin(9600);
}

// Maximale mögliche Widerstand 
float rab = 96600;
// Widerstandsänderungen pro Stufe
float rs = rab / 256;

float get_voltage(){
  // Einlesen der Spannung.
  long read_value = 0;
   // 10 mal einlesen und Durchschnit verwenden.
  for( int i = 0; i < 10; i++) {read_value += analogRead(A0);}
  float wert = read_value / 10.;
  return wert * (5/1023.0);
}

float calculate_sensor(float current_resistance){
  // berechnen des Widerstands eines Sensors.
  float u2 = get_voltage();
  float u1= 5- u2;
  if (u1 == 0) return 0;
  return current_resistance  * u2/u1;
}



void set_inputs(){
  // Pins als Input setzen damit kein Spannung anliegt.
  pinMode(r1, INPUT);
  pinMode(r2,INPUT);
  pinMode(r3,INPUT);
}


void sensor(int step,int pin){
  // Pins einstellen nur den angegebenen Pin einlesen.
  set_inputs();
  pinMode(pin, OUTPUT);
  
  // Abgriff einstellen Position über  Spi an digitalen Potentiometer senden.
  SPI.transfer(0x00);            
  SPI.transfer(step); 
  
  // Widerstand berechnen und Spannungsteiler R2 einlesen.
  float current_resistance = rs *step +106;
  Serial.print("Sensor");
  Serial.print(pin);
  Serial.print(": ");
  Serial.println(calculate_sensor(current_resistance));
}

void loop() {
  sensor(25,7);
  sensor(20,6);
  sensor(28, 5);
}