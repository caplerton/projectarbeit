#include<Wire.h>

// Definieren von Variablen
#define Addr 0x54
#define max_resistance 9850
#define start_resistance 100
#define r1 7
#define r2 6  
#define r3 5 

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

float get_voltage(){
  // Einlesen der Spannung.
  long read_value = 0;
   // 10 mal einlesen und Durchschnit verwenden.
  for( int i = 0; i < 10; i++) {read_value += analogRead(A0);}
  float wert = read_value / 10.;
  return wert * (5/1023.0);
}

float change_resistance(int wiper_position){
  if (wiper_position > 0 || wiper_position > 256) return -1;
  // I2C Kommunikation um Abgriff einzustellen
  Wire.beginTransmission(Addr);
  char c = Wire.read();
  Wire.write(0x00);
  Wire.write(wiper_position);
  Wire.endTransmission();

  // Widerstand des digitalen Potentiometers berechnen
  return (256 - wiper_position)/256 * max_resistance - start_resistance;
}

void set_inputs(){
  // Pins als Input setzen damit kein Spannung anliegt.
  pinMode(r1, INPUT);
  pinMode(r2,INPUT);
  pinMode(r3,INPUT);
}

float calculate_sensor(float current_resistance){
  // berechnen des Widerstands eines Sensors.
  float u2 = get_voltage();
  float u1= 5- u2;
  if (u1 == 0) return 0;
  return current_resistance  * u2/u1;
}


void sensor(int step,int pin){
  // Pins einstellen nur den angegebenen Pin einlesen.
  set_inputs();
  pinMode(pin, OUTPUT);
  
  // Widerstand berechnen und Spannungsteiler R2 einlesen.
  float current_resistance = change_resistance(step);
  Serial.print(pin);
  Serial.print(": ");
  Serial.println(calculate_sensor(current_resistance));
}


void loop() {
  sensor(25,7);
  sensor(20,6);
  sensor(28, 5);
}
