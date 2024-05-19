#define pinoTrigger 12        // Pino Trigger do sensor ultrassônico
#define pinoEcho 11           // Pino Echo do sensor ultrassônico
#define pinoMotorEsquerdo1 14 // Pino 1 do motor esquerdo
#define pinoMotorEsquerdo2 15 // Pino 2 do motor esquerdo
#define pinoMotorDireito1 16  // Pino 1 do motor direito
#define pinoMotorDireito2 17  // Pino 2 do motor direito
#define pinoSensorEsquerdo 4  // Pino do sensor de linha esquerdo
#define pinoSensorDireito 3   // Pino do sensor de linha direito
#define botao 2               // Botao de inicio

// Variaveis de controle
int ligado = false;
int trava = true;

// Variáveis para controle do sensor ultrassônico
long duracao;
int distancia;

// Variáveis para controle dos sensores de linha
bool sensorEsquerdoDetectado = false;
bool sensorDireitoDetectado = false;

// Variável para controle da velocidade
int velocidade = 200; // Velocidade inicial

void setup() {
  // Inicializa comunicação serial para debug
  Serial.begin(9600);

  // Define os pinos como entrada ou saída
  pinMode(pinoTrigger, OUTPUT);
  pinMode(pinoEcho, INPUT);
  pinMode(pinoMotorEsquerdo1, OUTPUT);
  pinMode(pinoMotorEsquerdo2, OUTPUT);
  pinMode(pinoMotorDireito1, OUTPUT);
  pinMode(pinoMotorDireito2, OUTPUT);
  pinMode(pinoSensorEsquerdo, INPUT);
  pinMode(pinoSensorDireito, INPUT);
}

void loop() {
  int botao_ativador = digitalRead(botao);
  if (botao_ativador == 0) {
    trava = false;
  }
  if (botao_ativador == 1 && trava == false) {
    Serial.print("iniciando");
    delay(5000);
    frente();
    ligado = true;
    trava = true;
  }
  if (ligado == true) {
    // Lê os sensores de linha
    sensorEsquerdoDetectado = digitalRead(pinoSensorEsquerdo);
    sensorDireitoDetectado = digitalRead(pinoSensorDireito);

    // Se detectar a linha, realiza manobra para voltar à arena
    if (sensorEsquerdoDetectado) {
      parar();
      delay(100);
      re();
      delay(300);
      virarDireita();
    } else if (sensorDireitoDetectado) {
      parar();
      delay(100);
      re();
      delay(300);
      virarEsquerda();
    } else {
      // Lê a distância do sensor ultrassônico
      distancia = lerDistancia();

      // Se detectar o oponente, avança
      if (distancia < 20) {
        frente();
      } else {
        // Se não detectar o oponente, procura em volta
        procurarOponente();
      }
    }
  }
}

// Função para ler a distância do sensor ultrassônico
int lerDistancia() {
  digitalWrite(pinoTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrigger, LOW);

  duracao = pulseIn(pinoEcho, HIGH);
  return duracao * 0.034 / 2;
}
