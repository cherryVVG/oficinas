#define PIN_SENSOR_OPTICO 18
#define pinoDirecaoA 12 // IN1 do L298N
#define pinoDirecaoB 13 // IN2 do L298N

// Constantes de Temporização (Ajustáveis)
const unsigned long DURACAO_PARADA = 2000;       // 2 segundos parado
const unsigned long ATRASO_RODAR_MS = 1200;       // 100ms de giro após a detecção (Ajuste aqui)

// Variáveis Globais de Estado (volatile para uso na ISR)
volatile bool motorParadoPorInterrupcao = false; // True: Motor está na pausa de 2 segundos.
volatile bool motorParadaComAtraso = false;      // True: Motor está no curto período de giro após detecção.
volatile unsigned long tempoParada = 0;          // Tempo em que o motor PAROU (para a pausa de 2s)
volatile unsigned long tempoDisparoAtraso = 0;   // Tempo em que o sensor foi DISPARADO (para o atraso de giro)

//============================================= FUNÇÕES DE CONTROLE ================================================
void girarMotor() {
  digitalWrite(pinoDirecaoA, HIGH);
  digitalWrite(pinoDirecaoB, LOW);
}

void pararMotor() {
  digitalWrite(pinoDirecaoA, LOW);
  digitalWrite(pinoDirecaoB, LOW);
}

//============================================= ROTINA DE INTERRUPÇÃO ================================================
// A ISR agora só inicia o processo de "atrasar a parada".
void parar_motor_ISR() {
  // Se o sistema JÁ NÃO estiver processando um atraso de parada ou pausa,
  // inicie um novo ciclo.
  if (!motorParadoPorInterrupcao && !motorParadaComAtraso) {
    tempoDisparoAtraso = millis();
    motorParadaComAtraso = true;
  }
  // IMPORTANTE: O motor NÃO É PARADO AQUI para que ele continue rodando no loop principal.
}

//============================================= SETUP ================================================
void setup() {
  Serial.begin(9600);

  pinMode(pinoDirecaoA, OUTPUT);
  pinMode(pinoDirecaoB, OUTPUT);

  // Configura a interrupção. Recomendado o INPUT_PULLUP para sensores ópticos.
  pinMode(PIN_SENSOR_OPTICO, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_OPTICO), parar_motor_ISR, FALLING);
  
  // Inicia o motor para o estado inicial
  girarMotor();
}

//============================================= LOOP PRINCIPAL (Máquina de Estados) ================================================
void loop() {
  
  // 1. ESTADO DE PAUSA (MAIOR PRIORIDADE - Checando os 2 segundos)
  if (motorParadoPorInterrupcao) {
    
    pararMotor(); // Garantimos que o motor está parado
    if (millis() - tempoParada >= DURACAO_PARADA) {
      
      // Fim da pausa: Volta ao estado de giro normal.
      girarMotor();
      motorParadoPorInterrupcao = false;
      Serial.println("Motor reativado apos 2 segundos.");
    }
  } 
  
  // 2. ESTADO DE ATRASO (Prioridade Média - Rodando o pouco extra)
  else if (motorParadaComAtraso) {
    
    girarMotor(); // Mantemos o motor girando durante o atraso
    if (millis() - tempoDisparoAtraso >= ATRASO_RODAR_MS) {
      
      // Fim do atraso: Fazemos a parada e iniciamos o estado de pausa.
      pararMotor();
      motorParadaComAtraso = false;      // Sai do estado de atraso
      motorParadoPorInterrupcao = true;  // Entra no estado de pausa
      tempoParada = millis();            // Inicia o timer da pausa de 2 segundos
      Serial.println("Atraso de giro concluido. Motor parado.");
    }
  } 
  
  // 3. ESTADO NORMAL (MENOR PRIORIDADE - Motor em giro livre)
  else { 
    // Se não estamos em pausa e nem em atraso, o motor deve estar girando.
    girarMotor();
  }

}