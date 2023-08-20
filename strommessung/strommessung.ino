// Wire Bibliothek initialisieren für Kommunikation
#include <Wire.h>
// Adafruit implementieren und initalisieren
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

void setup() {
  // Kommunikation initialisieren
  Serial.begin(9600);
  uint32_t currentFrequency;
  
  // Initialisieren des INA219 Chips
  // Überprüft ob eine Kommunikation zum Chip aufgebaut werden kann.
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

}

void loop() {
  // Messungen durchführen
  // Spannungen messen
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);
  // Messung der Stromstärke
  float current_mA = ina219.getCurrent_mA();
  // Elektrische Leistung messen und berechnen
  float power_mW = ina219.getPower_mW();

  // Ausgabe der gemessenen Werte
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");
}
