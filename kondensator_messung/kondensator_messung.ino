#define charge 7         // Kondensator lade Pin über einen 10kOhm Widerstand
#define discharge 6      // Kondensator entlade Pin über einen 220 Ohm Widerstand
#define ad_pin A0           // Analog Messeingang
#define resistance 10000  // 10 kOhm > gemessen 9,996 kOhm

long start_time = 0;
long elapsed_time = 0;
float farad = 0;


void setup() {
  pinMode(charge, OUTPUT);     
  digitalWrite(charge, LOW);  

  pinMode(ad_pin, INPUT);

  Serial.begin(9600);           
}


void discharge() {
  // entladen
  digitalWrite(charge, LOW); 
  pinMode(discharge, OUTPUT);
  digitalWrite(discharge, LOW); 
}

void calculate_condenstaor(){
  // Überprüfen ob Kondensator entladen
  if (analogRead(ad_pin) > 0) 
  {
    discharge();
    return;
  }
  pinMode(discharge, INPUT);

  float condensator = 0;

    // Kondensator laden.
  digitalWrite(charge, HIGH);  
  
  // Startzeit nehmen.          
  start_time = micros();

  // Messung durchführen.                  
  while(analogRead(ad_pin) < 648){} 

  // Zeit berechnen.    
  elapsed_time= micros() - start_time ;
  if(elapsed_time > 2147483647) elapsed_time = 0;  


  // Farad berechnen.
  farad = ((float)elapsed_time / resistance); 

  condensator += farad;

  // Kondensator entladen 
  discharge();     
          
  Serial.print(condensator,5);         
  Serial.println(" uF");
}
void loop() {
  
  calculate_condenstaor();
}