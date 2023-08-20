#define charge_pin 7         // Kondensator lade Pin ueber einen 10kOhm Widerstand
#define discharge_pin 6      // Kondensator entlade Pin ueber einen 220 Ohm Widerstand
#define analog_pin A0           // Analog Messeingang
#define resistance  10000  // 10 kOhm > gemessen 9,996 kOhm
long start_time, used_time;
float micro_farad, nano_farad;

void setup() {
  pinMode(charge_pin, OUTPUT);     
  digitalWrite(charge_pin, LOW);  
  Serial.begin(9600);           
}


void start_discharge(){
  // Pins auf low setzen, um ueber Pins zu entladen
  digitalWrite(charge_pin, LOW);     
  pinMode(discharge_pin, OUTPUT);           
  digitalWrite(discharge_pin, LOW);       
}

bool check_discharged( ){
  // Ueberpruefen ob Kondensator vollstaendig entladen
  bool discharged = bool(analogRead(analog_pin) == 0);
  if (discharged) pinMode(discharge_pin, INPUT);     
  return discharged;
}

void print_value(float tmp_micro_farad, float tmp_used_time)
{
  // Ausgabe der Zeit
  Serial.print(tmp_used_time);    
  Serial.println(" nS    ");
  
  // Ausgabe des Werts
  if (tmp_micro_farad > 1) Serial.print(tmp_micro_farad);
  else Serial.print(long(micro_farad * 1000.0));

  // Einheit ausgeben
  if (tmp_micro_farad > 1) Serial.println(" uF");
  else Serial.println(" nF");
}

void charge_condensator(){
  if (check_discharged()) return;

  // Starte Ladevorgang
  digitalWrite(charge_pin, HIGH);            
  start_time = micros();   

  // Laden bis Kondensator zu 63% geladen ist               
  while(analogRead(analog_pin) < 648){}    

  // Ermitteln der Kapazitaet
  used_time= micros() - start_time - 114; 
  if(used_time > 2147483647) used_time = 0;  
  micro_farad = ((float)used_time / resistance); 

  print_value(micro_farad, used_time);
  start_discharge();
}

void loop() {
  charge_condensator();
}
