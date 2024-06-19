#include <Arduino.h>
// Comunicação LoRa com Arduino 
#include <SPI.h>             
#include <LoRa.h>
#include <ArduinoJson.h>

StaticJsonDocument<512> deserializedDoc;

// Definicacao de constantes
const int csPin = 5;         //5 Chip Select (Slave Select do protocolo SPI) do modulo Lora 0
const int resetPin = 14;       //14 Reset do modulo LoRa 10
const int irqPin = 2;         //2 Pino DI0 14

String outgoing;              // outgoing message

byte localAddress = 0xFF;     // Endereco deste dispositivo LoRa
byte msgCount = 0;            // Contador de mensagens enviadas
byte destination = 0xFF;      // Endereco do dispositivo para enviar a mensagem (0xFF envia para todos devices)
long lastSendTime = 0;        // TimeStamp da ultima mensagem enviada
int interval = 5000;          // Intervalo em ms no envio das mensagens (inicial 5s)

// Funcao que envia uma mensagem LoRa
void sendMessage(String outgoing) 
{
  LoRa.beginPacket();                   // Inicia o pacote da mensagem
  LoRa.write(destination);              // Adiciona o endereco de destino
  LoRa.write(localAddress);             // Adiciona o endereco do remetente
  LoRa.write(msgCount);                 // Contador da mensagem
  LoRa.write(outgoing.length());        // Tamanho da mensagem em bytes
  LoRa.print(outgoing);                 // Vetor da mensagem 
  LoRa.endPacket();                     // Finaliza o pacote e envia
  msgCount++;                           // Contador do numero de mensagnes enviadas
}
 
// Funcao para receber mensagem 
void onReceive(int packetSize) 
{
  if (packetSize == 0) return;       
  DeserializationError err = deserializeJson(deserializedDoc, LoRa);
  deserializedDoc["RSSI"] = LoRa.packetRssi();
  serializeJson(deserializedDoc, Serial);
  Serial.println();
}

// Setup do Microcontrolador
void setup() 
{
  // inicializacao da serial 
  Serial.begin(115200);                   
  while (!Serial);
 
  Serial.println(" Comunicacao LoRa Duplex - Ping&Pong ");
 
  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin
 
  // Inicializa o radio LoRa em 915MHz e checa se esta ok!
  if (!LoRa.begin(915E6)) 
  {             
    Serial.println(" Erro ao iniciar modulo LoRa. Verifique a coenxao dos seus pinos!! ");
    while (true);                      
  }
 
  Serial.println("LoRa iniciado! ");
}
 
// Loop do microcontrolador - Operacoes de comunicacao LoRa
void loop() 
{
  onReceive(LoRa.parsePacket());
}
 
