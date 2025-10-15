#include "HX711.h"
#define escala 916.169982
#define off 57904

// Define os pinos de comunicação
const int PINO_DT = 6;   // Pino de Dados (Data)
const int PINO_SCK = 7;  // Pino de Clock (Serial Clock)

HX711 massa; // Cria o objeto da célula de carga

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando a célula de carga...");

  // Inicializa a comunicação, passando os pinos DT e SCK
  massa.begin(PINO_DT, PINO_SCK);

  massa.set_offset(off);
  massa.set_scale(escala);

  // ... Resto da configuração (Tara e Calibração)
}

void loop() {
  // O clock (SCK) é acionado automaticamente pela função read()
  float peso_lido = massa.get_units(10);
  Serial.print("Peso: ");
  Serial.println(peso_lido);
}