#include <IRremote.h>

// Definições de pinos
#define pinoTrigger 7         // Pino Trigger do sensor ultrassônico
#define pinoEcho 8            // Pino Echo do sensor ultrassônico
#define pinoMotorEsquerdo1 5  // Pino 1 do motor esquerdo
#define pinoMotorEsquerdo2 3  // Pino 2 do motor esquerdo
#define pinoMotorDireito1 9   // Pino 1 do motor direito
#define pinoMotorDireito2 6   // Pino 2 do motor direito
#define pinoSensorEsquerdo 11 // Pino do sensor de linha esquerdo
#define pinoSensorDireito 12  // Pino do sensor de linha direito
#define receptor 10           // Receptor Infravermelho

// Variáveis de controle
bool ligado = false;

// Variáveis para controle do sensor ultrassônico
long duracao;
int distancia;

// Variáveis para controle dos sensores de linha
bool sensorEsquerdoDetectado = false;
bool sensorDireitoDetectado = false;

// Lista de códigos válidos para ativar e desativar o robô
unsigned long codigoAtivacao = 0xFC03EF00;
unsigned long codigoDesativacao = 0xFD02EF00;

// Funções de movimento do robô
void frente() {
  digitalWrite(pinoMotorEsquerdo1, HIGH);
  analogWrite(pinoMotorEsquerdo2, LOW);
  digitalWrite(pinoMotorDireito1, HIGH);
  digitalWrite(pinoMotorDireito2, LOW);
}

void re() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  digitalWrite(pinoMotorEsquerdo2, HIGH);
  digitalWrite(pinoMotorDireito1, LOW);
  digitalWrite(pinoMotorDireito2, HIGH);
}

void virarEsquerda() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  digitalWrite(pinoMotorEsquerdo2, HIGH);
  digitalWrite(pinoMotorDireito1, HIGH);
  digitalWrite(pinoMotorDireito2, LOW);
}

void virarDireita() {
  digitalWrite(pinoMotorEsquerdo1, HIGH);
  digitalWrite(pinoMotorEsquerdo2, LOW);
  digitalWrite(pinoMotorDireito1, LOW);
  digitalWrite(pinoMotorDireito2, HIGH);
}

void parar() {
  digitalWrite(pinoMotorEsquerdo1, LOW);
  digitalWrite(pinoMotorEsquerdo2, LOW);
  digitalWrite(pinoMotorDireito1, LOW);
  digitalWrite(pinoMotorDireito2, LOW);
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

  // Inicia o receptor IR
  IrReceiver.begin(receptor, ENABLE_LED_FEEDBACK); 
}

void loop() {
  // Recebe sinal infravermelho
  if (IrReceiver.decode()) {
    unsigned long codigoRecebido = IrReceiver.decodedIRData.decodedRawData; 
    
    // Verifica se o código é de ativação
    if (codigoRecebido == codigoAtivacao) {
      ligado = true;
      Serial.println("Ativando...");
    } else if (codigoRecebido == codigoDesativacao) {
      ligado = false;
      Serial.println("Desativando...");
    }
    IrReceiver.resume(); 
  }
  
  // Operação principal
  if (ligado) {
  // Lê os sensores de linha
      sensorEsquerdoDetectado = digitalRead(pinoSensorEsquerdo);
      sensorDireitoDetectado = digitalRead(pinoSensorDireito);
      Serial.println(sensorEsquerdoDetectado);
      Serial.println(distancia);
      // Realiza manobra para voltar à arena
      if (sensorEsquerdoDetectado) {
        parar();
        delay(100);
        re(); //volta
        delay(300);
      } else if (sensorDireitoDetectado) { 
        parar();
        delay(100);
        re();
        delay(300);
      } else {
        // Lê a distância do sensor ultrassônico
        distancia = lerDistancia();

        // Avança se detectar o oponente
        if (distancia < 40) {
          frente();
          Serial.println("Frente");
        } else {
          // Procura o oponente
          parar();
          delay(5); 
          virarEsquerda();
          delay(30);
          Serial.println("Gira");
        
        }
        
      }
  } else {
    parar(); // Para os motores se não estiver ligado
  }
}
