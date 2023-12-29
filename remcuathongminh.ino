#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


const char* ssid = "Remcuathongminh";
const char* password = "12345678";
IPAddress esp8266_ip;


const int port = 80;


const int motorPin = 13;


int motorState = LOW;
WiFiServer server(port);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);


  Serial.begin(115200);
  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  esp8266_ip = WiFi.localIP();

  server.begin();
  Serial.println("Server started");


  Serial.println("Connecting to ESP8266...");
  WiFiClient client;
  client.connect(esp8266_ip, port);
  Serial.println("Connected to ESP8266");
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected");
    while (true);
  }


  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");


    String request = client.readStringUntil('\n');
    Serial.println(request);

    if (request.equals("open")) {
      motorState = HIGH;
    } else if (request.equals("close")) {
      motorState = LOW;
    }

    client.println("OK");
    client.flush();


    client.stop();


    digitalWrite(motorPin, motorState);
  }


  if (!client.connected()) {
    Serial.println("ESP8266 disconnected");
    client.connect(esp8266_ip, 80);
    Serial.println("Connected to ESP8266");
  }


  client.println(motorState);
  client.flush();
}