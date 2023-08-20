#define inc 7
#define up_down 6   // high + inc -> wiper to high
#define sensor0 9   // anliegender Sensor0.
#define sensor1 10  // anliegender Sensor1.
#define sensor2 11  // anliegender Sensor2.
#define reference 8


void setup() {
  Serial.begin(9600);
  // increment und up/down auf Output stellen um den Abgriff zu steueren.
  pinMode(inc, OUTPUT);
  pinMode(up_down, OUTPUT);

  // Alle Sensoren auf Input setzen es soll kein Strom fließen.
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  // Referenz als Input setzen, dient zur Berechnung des eingestellten Widerstands.
  pinMode(reference, INPUT);

  // A/D-Wandler als Input setzen.
  pinMode(A0, INPUT);
}

// Gesamtwiderstand des Potentiometers.
float poti_resistance = 9650;



float get_voltage() {
  // Einlesen der Spannung.
  long read_value = 0;
  for (int i = 0; i < 5; i++) { read_value += analogRead(A0); }
  int wert = trunc(read_value / 5);
  return wert * (5 / 1023.0);
}


void set_inputs() {
  // Pins als Input setzen damit kein Spannung anliegt.
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(reference, INPUT);
}

void change_ground(int ground_pin) {
  // pinMode aktualisieren ground_pin als ground setzen.
  set_inputs();
  pinMode(ground_pin, OUTPUT);
}

float calculate_resistance() {
  float u2 = get_voltage();
  return poti_resistance * (5 - u2) / 5;
}


float update_ds_resistance(float dest) {
  change_ground(reference);
  float current_resistance = calculate_resistance();
  while (abs(current_resistance - dest) > 100) {
    // Ermitteln in welche Richtung der Widerstand gehen sollte.
    int upd = (dest < current_resistance) ? 0 : 1;
    digitalWrite(up_down, (dest < current_resistance) ? 0 : 1);

    // Pin für Increment auf High dann auf low setzen um eine fallende Flanke zu erzeugen.
    digitalWrite(inc, 1);
    digitalWrite(inc, 0);
    current_resistance = calculate_resistance();
  }

  // Eingestellten Widerstand ausgeben
  Serial.print("R1: ");
  Serial.println(current_resistance);
  return current_resistance;
}

float calculate_sensor(float current_resistance) {
  // berechnen des Widerstands eines Sensors.
  float u2 = get_voltage();
  float u1 = 5 - u2;
  if (u1 == 0) return 0;
  return current_resistance * u2 / u1;
}

void get_sensor(float use_resistance, int number_pin) {
  // Referenzwiderstand einstellen
  float current_resistance = update_ds_resistance(use_resistance);
  // Pin ändern um Stromkreis über den Sensor zu schließen
  change_ground(number_pin);
  // Widerstandssensor ermitteln
  float sensor = calculate_sensor(current_resistance);
  // Gemessenen Widerstand ausgeben
  Serial.print("Sensor ");
  Serial.print(number_pin);
  Serial.print(": ");
  Serial.println(sensor);
}

void loop() {
  // Aufrufen der Funktion um einen Sensor einzulesen.
  get_sensor(4000, sensor0);
  get_sensor(7000, sensor1);
  get_sensor(8000, sensor2);
  delay(5000);
}
