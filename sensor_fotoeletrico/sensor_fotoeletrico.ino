#define sensor 7

int sinal_dig = 0;   

void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(9600);

}

void loop() {

  sinal_dig = digitalRead(sensor);

  Serial.println(sinal_dig);

}

//obs.: antihorário diminui a distancia
