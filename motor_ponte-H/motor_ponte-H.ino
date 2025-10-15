// --- Definição dos Pinos ---
const int IN1 = 7; 
const int IN2 = 6; 
const int ENA = 5; // Pino PWM

// Variável global para a velocidade (0 a 255)
int vel = 0; // Renomeada de "velocidade" para "vel" para manter a sua lógica

void setup() {
  // Configura os pinos como SAÍDA
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // **CORREÇÃO: Define a Direção do Motor (Exemplo: Sentido Horário)**
  // O motor só gira se IN1 e IN2 tiverem estados opostos.
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  
  // Inicia a comunicação serial para monitoramento
  Serial.begin(9600);
  Serial.println("Controle de Motor DC com L298N (Q para + / A para -)");
  Serial.println("Velocidade inicial: 0");
}

void loop() {
  
  // A função var_vel() atualiza a variável global 'vel'
  var_vel();

  // Aplica a velocidade atualizada no pino ENA
  analogWrite(ENA, vel);
  
  Serial.print("Velocidade: ");
  Serial.println(vel);
  
  delay(100); 
}

// Função para ler comandos do Serial Monitor e ajustar a velocidade
void var_vel(){
  if (Serial.available() > 0) {
    char comando = Serial.read(); // Lê o caractere digitado

    if (comando == 'q') {
      vel += 5;
      // Garante que a velocidade não ultrapasse o máximo (255)
      if (vel > 255) {
        vel = 255;
      }
    } else if (comando == 'a') {
      vel -= 5;
      // Garante que a velocidade não seja menor que o mínimo (0)
      if (vel < 0) {
        vel = 0;
      }
    }
    // Para ver a nova velocidade imediatamente
    Serial.print("Comando recebido. Nova ");
  }
}