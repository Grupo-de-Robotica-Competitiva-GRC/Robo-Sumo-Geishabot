#define pinoTrigger 12        // Pino Trigger do sensor ultrassônico
#define pinoEcho 11           // Pino Echo do sensor ultrassônico
#define pinoSensorEsquerdo 4  // Pino do sensor de linha esquerdo
#define pinoSensorDireito 3   // Pino do sensor de linha direito

//variaveis que recebem o estado dos sensores
int EstadoSensorEsquerdo, EstadoSensorDireito;

double Distancia;

void setup() {
  // Inicializa comunicação serial 
  Serial.begin(9600);

  // Define os pinos como entrada ou saída
  pinMode(pinoTrigger, OUTPUT);
  pinMode(pinoEcho, INPUT);
  pinMode(pinoSensorEsquerdo, INPUT);
  pinMode(pinoSensorDireito, INPUT);
}

//---funcao que calcula a distancia pelo sensor ultra----

int lerDistancia() {
  double duracao;
  digitalWrite(pinoTrigger, LOW);
  delayMicroseconds(5);
  digitalWrite(pinoTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrigger, LOW);

  duracao = pulseIn(pinoEcho, HIGH);
  return duracao * 0.034 / 2;
}

void loop() {

  //-----------TESTE SENSORES INFRAVERMELHOS--------
  EstadoSensorEsquerdo = digitalRead(pinoSensorEsquerdo);
  EstadoSensorDireito = digitalRead(pinoSensorDireito);

  //------TESTE SENSOR ULTRASSONICO-----------------
  Distancia = lerDistancia();


  //---------Serial print ------------------------
  Serial.print("Sensor esquerdo:");
  Serial.println( EstadoSensorEsquerdo);

  Serial.print("Sensor direito:");
  Serial.println( EstadoSensorDireito);

  Serial.print("Distância em cm:");
  Serial.println(Distancia);
}
