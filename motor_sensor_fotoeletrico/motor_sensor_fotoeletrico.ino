#define PIN_SENSOR_OPTICO 18
#define pinoDirecaoA 12 // IN1 do L298N
#define pinoDirecaoB 13 // IN2 do L298N

// Variáveis Globais de Estado
volatile bool motorParadoPorInterrupcao = false; // Flag para indicar estado
volatile unsigned long tempoParada = 0;          // Variável para armazenar o tempo da interrupção
const unsigned long DURACAO_PARADA = 2000;       // 2000 milissegundos = 2 segundos

void girarMotor() {
  digitalWrite(pinoDirecaoA, HIGH);
  digitalWrite(pinoDirecaoB, LOW);
}

void pararMotor() {
  digitalWrite(pinoDirecaoA, LOW);
  digitalWrite(pinoDirecaoB, LOW);
}

// =================================== ISR ===================================
// A ISR deve ser a mais curta possível. Apenas registra o tempo e o estado.
void parar_motor_ISR() {
  // Apenas registra o tempo e o estado na ISR
  tempoParada = millis();
  motorParadoPorInterrupcao = true;
  pararMotor(); // Para o motor imediatamente
}

void setup() {
  Serial.begin(9600);

  // Define pinos de direção como SAÍDA
  pinMode(pinoDirecaoA, OUTPUT);
  pinMode(pinoDirecaoB, OUTPUT);
  
  // Configura a interrupção
  pinMode(PIN_SENSOR_OPTICO, INPUT_PULLUP); // Geralmente recomendado para sensores ópticos com pull-up
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_OPTICO), parar_motor_ISR, FALLING);
  
  // Inicia o motor
  girarMotor();
}

void loop() {
  // 1. Lógica de reativação do motor (Não-Bloqueante)
  if (motorParadoPorInterrupcao) {
    // Se a parada foi ativada, verifica se o tempo de 2 segundos já passou
    if (millis() - tempoParada >= DURACAO_PARADA) {
      girarMotor(); // Reativa o motor
      motorParadoPorInterrupcao = false; // Reseta a flag
      Serial.println("Motor reativado apos 2 segundos.");
    }
  } else {
      // 2. Opcional: Monitoramento do sensor
      // Não é estritamente necessário no loop com interrupção,
      // mas pode ser útil para debug.
      // A leitura original: sinal_dig = digitalRead(PIN_SENSOR_OPTICO);
      // Serial.println(digitalRead(PIN_SENSOR_OPTICO));
  }
}