#include <WiFi.h>
#include <WiFiUDP.h>
#include <ArduinoJson.h>

const int udpPort = 1234;

WiFiUDP udp;

// -------------- Configuracoes WiFi -----------------------
//Definindo as informações da rede Wi-Fi
const char* ssid = "ssid"; //Define o nome do ponto de acesso
const char* password = "password"; //Define a senha

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando WiFi...");
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  udp.begin(udpPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    
    Serial.print("Pacote recebido: ");
    Serial.println(packetSize);
    char packetBuffer[packetSize];
    udp.read(packetBuffer, packetSize);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, packetBuffer);

    if (error) {
      Serial.print("Erro ao analisar JSON: ");
      Serial.println(error.c_str());
    } else {
      String jsonString;
      serializeJson(doc, jsonString);
      Serial.println(jsonString);

    }
  }
}
