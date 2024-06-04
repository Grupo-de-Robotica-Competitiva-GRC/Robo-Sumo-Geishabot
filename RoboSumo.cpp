#include <IRremote.h>

// Definições de pinos
#define pinoTrigger 7         // Pino Trigger do sensor ultrassônico
#define pinoEcho 8            // Pino Echo do sensor ultrassônico
#define pinoMotorEsquerdo1 3  // Pino 1 do motor esquerdo
#define pinoMotorEsquerdo2 5  // Pino 2 do motor esquerdo
#define pinoMotorDireito1 6   // Pino 1 do motor direito
#define pinoMotorDireito2 9   // Pino 2 do motor direito
#define pinoSensorEsquerdo 11 // Pino do sensor de linha esquerdo
#define pinoSensorDireito 12  // Pino do sensor de linha direito
#define receptor 10           // Receptor Infravermelho

// Variáveis de controle
bool ligado = false;
bool trava = true;

// Variáveis para controle do sensor ultrassônico
long duracao;
int distancia;

// Variáveis para controle dos sensores de linha
bool sensorEsquerdoDetectado = false;
bool sensorDireitoDetectado = false;

// Variável para controle da velocidade
int velocidade = 200; // Velocidade inicial

// Inicializa o receptor IR
IRrecv irrecv(receptor);
decode_results results;

// Lista de códigos válidos para ativar e desativar o robô
unsigned long codigoAtivacao =  0xFC03EF00;
unsigned long codigoDesativacao = 0xFD02EF00;

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
  pinMode(receptor, INPUT);

  // Inicia o receptor IR
  irrecv.enableIRIn();
}

void loop() {
  // Recebe sinal infravermelho
  if (irrecv.decode()) {
    unsigned long codigoRecebido = irrecv.decodedIRData.decodedRawData; 
    
    // Verifica se o código é de ativação
    if (codigoRecebido == codigoAtivacao) {
      ligado = true;
      Serial.println("Ativando...");
    }
    // Verifica se o código é de desativação
    if (codigoRecebido == codigoDesativacao) {
      parar();
      ligado = false;
      Serial.println("Desativando...");
    }
    irrecv.resume(); 
  }
  
  // Operação principal
  if (ligado) {
    // Lê os sensores de linha
    sensorEsquerdoDetectado = digitalRead(pinoSensorEsquerdo);
    sensorDireitoDetectado = digitalRead(pinoSensorDireito);

    // Realiza manobra para voltar à arena
    if (!sensorEsquerdoDetectado) {
      realizarManobraParaVoltar(true);
    } else if (!sensorDireitoDetectado) {
      realizarManobraParaVoltar(false);
    } else {
      // Lê a distância do sensor ultrassônico
      distancia = lerDistancia();

      // Avança se detectar o oponente
      if (distancia < 20) {
        frente();
      } else {
        // Procura o oponente
        procurarOponente();
        delay(100);
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

// Funções de movimento do robô
void frente() {
  analogWrite(pinoMotorEsquerdo1, velocidade);
  digitalWrite(pinoMotorEsquerdo2, LOW);
  analogWrite(pinoMotorDireito1, velocidade);
  digitalWrite(pinoMotorDireito2, LOW);
}

void re() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  analogWrite(pinoMotorEsquerdo2, velocidade);
  digitalWrite(pinoMotorDireito1, LOW);
  analogWrite(pinoMotorDireito2, velocidade);
}

void virarEsquerda() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  analogWrite(pinoMotorEsquerdo2, velocidade);
  analogWrite(pinoMotorDireito1, velocidade);
  digitalWrite(pinoMotorDireito2, LOW);
}

void virarDireita() {
  analogWrite(pinoMotorEsquerdo1, velocidade);
  digitalWrite(pinoMotorEsquerdo2, LOW);
  digitalWrite(pinoMotorDireito1, LOW);
  analogWrite(pinoMotorDireito2, velocidade);
}

void girar() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  analogWrite(pinoMotorEsquerdo2, velocidade);
  analogWrite(pinoMotorDireito1, velocidade);
  digitalWrite(pinoMotorDireito2, LOW);
}

void parar() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  digitalWrite(pinoMotorEsquerdo2, LOW);
  digitalWrite(pinoMotorDireito1, LOW);
  digitalWrite(pinoMotorDireito2, LOW);
}

void procurarOponente() {
  // Gira em torno do próprio eixo
  girar();
  delay(1000); // Ajustar tempo de giro
  parar();
  delay(1);
}

void realizarManobraParaVoltar(bool esquerdo) {
  parar();
  delay(1);
  re();
  delay(300);
  parar();
  delay(1);
  if (esquerdo) {
    virarDireita();
  } else {
    virarEsquerda();
  }
  delay(600);
  parar();
  delay(1);
  frente();
}
