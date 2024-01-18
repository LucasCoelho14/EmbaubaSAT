#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char* ssid = "Supernova Rocketry"; //Define o nome do ponto de acesso
const char* pass = "foguetaos2"; //Define a senha

WiFiUDP udp;

IPAddress raspberryPiIP(255, 255, 255, 255);  // Replace with the Raspberry Pi's IP address (192, 168, 4, 255)
const int udpPort = 1234;

void setup() {
  Serial.begin(9600); //Inicia o monitor serial
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
  udp.begin(udpPort); // Choose a port number
}

void loop() {
  // Create a JSON object
  DynamicJsonDocument jsonDoc(256);

  jsonDoc["sensor"] = "ESP32";
  jsonDoc["data"] = "Hello, Raspberry Pi!";
  
  // Serialize the JSON object to a string
  String jsonStr;
  serializeJson(jsonDoc, jsonStr);
  //Serial.print(jsonStr);
  // Send the JSON packet to the Raspberry Pi's IP address
  udp.beginPacket(raspberryPiIP, udpPort);
  udp.print(jsonStr);
  udp.endPacket();

  delay(1000);
}

// Broadcast the JSON packet to all connected clients
//  IPAddress multicastIP(239, 0, 0, 1);
//  udp.beginPacket(multicastIP, 1234);
//  udp.print(jsonStr);
//  udp.endPacket();