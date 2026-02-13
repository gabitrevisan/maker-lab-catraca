/**
 * @file controle_catraca.cpp
 * @author gabriela, breno / Projeto Catraca
 * @brief Controle de densidade de luz por proximidade para auxílio em leitores.
 * @version 1.0
 * @date 2026-02-13
 */

#include <Arduino.h>

// Definição dos Pinos
const int PIN_PING = 7;  // Pino de sinal do sensor PING)))
const int PIN_LED = 9;   // Pino PWM para controle de intensidade do LED

// Protótipos das funções
long lerDistanciaCM(int pino);
void ajustarLuz(long distancia);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  Serial.println("--- Sistema de Catraca Iniciado ---");
}

void loop() {
  // 1. Obtém a distância do usuário
  long distancia = lerDistanciaCM(PIN_PING);

  // 2. Ajusta a intensidade do LED com base na distância
  ajustarLuz(distancia);

  // 3. Debug via Serial Monitor (Opcional)
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(100); // Estabiliza a leitura
}

/**
 * Função para disparar o sensor PING))) e converter o tempo em cm.
 */
long lerDistanciaCM(int pino) {
  long duracao;

  // Pulso de disparo (Trigger)
  pinMode(pino, OUTPUT);
  digitalWrite(pino, LOW);
  delayMicroseconds(2);
  digitalWrite(pino, HIGH);
  delayMicroseconds(5);
  digitalWrite(pino, LOW);

  // Recebimento do eco (Echo)
  pinMode(pino, INPUT);
  duracao = pulseIn(pino, HIGH);

  // Conversão: velocidade do som (340m/s) -> 29 microssegundos por cm
  // Dividido por 2 (ida e volta do som)
  return duracao / 29 / 2;
}

/**
 * Função que controla o brilho do LED via PWM.
 * Regra: Quanto menor a distância, maior o brilho.
 */
void ajustarLuz(long distancia) {
  int brilho = 0;

  // Limites: 100cm (apagado) até 10cm (brilho máximo)
  if (distancia <= 100 && distancia >= 10) {
    // map(valor, deBaixo, deCima, paraBaixo, paraCima)
    brilho = map(distancia, 100, 10, 0, 255);
    analogWrite(PIN_LED, brilho);
  } 
  else if (distancia < 10) {
    analogWrite(PIN_LED, 255); // Usuário posicionado: luz máxima
  } 
  else {
    analogWrite(PIN_LED, 0);   // Ninguém detectado: luz apagada
  }
}