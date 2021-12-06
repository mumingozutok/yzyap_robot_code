/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

//Import headers
#include "html.h"
#include "util.h"
#include "jsonparsers.h"

// Replace with your network credentials
const char* ssid = "Gesislab-wifi";
const char* password = "5319686250zafer";

//const char* ssid = "TP-LINK_C05C";
//const char* password = "kalakcha1$";

bool ledState = 0;
const int ledPin = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//Dynamic JSON Buffer going to use in IDE communication
DynamicJsonDocument doc(1024);

void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

    comPrintBuffer(data, len); //prints incoming data to the com port
      
    /*if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }*/

    auto error = deserializeJson(doc, (char*)data);
    if (error) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
    }

    if( parseJSONData(doc) == false ){
      Serial.println("Data parsing failed");
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE1"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);

  if(Serial2.available() > 0){
    byte c = Serial2.read();
    if(c == 1)  {
      ws.textAll("1");
      Serial.println("uart data rsved and match");
    }
    else {
      ws.textAll("0");
      Serial.println("uart data rsved but not match");
    }
  }
}
