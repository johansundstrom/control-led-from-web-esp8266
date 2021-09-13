#include <ESP8266WebServer.h>
#include "index.h"
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>  //https://arduinojson.org/
#include "includes.h"
//https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
ESP8266WebServer www_server(80); //create an instance

extern const char *ssid;
extern const char *password;
const int LED = LED_BUILTIN;
String server_state = "";



void connectToWiFi() {
  Serial.println("Connecting to the WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupServer() {
  www_server.begin();             //Start
  Serial.println("HTTP server started");
  www_server.on("/", htmlIndex);  //Client request handlerFunction
  www_server.on("/led", led);     //Client request handlerFunction
}

void led() {
  const char* state;

  //Hämta request JSON
  if (www_server.hasArg("plain")== true) {
    String jsondata = www_server.arg("plain");
    Serial.println(jsondata);   //klientdata
    
    //parse JSON till object
    DynamicJsonDocument req_json(64);     //malloc
    deserializeJson(req_json, jsondata);
    state = req_json["state"];
    long long clientEpoch = req_json["clientEpoch"];

    //parse object till JSON
    DynamicJsonDocument res_json(64);     //malloc
    res_json["state"] = state;
    char buffer[100];
    //parse från object till JSON
    serializeJson(res_json, buffer);
    //Skicka response
    www_server.send(200, "application/json", buffer);
  }
  if (String(state) == "ON") {
    digitalWrite(LED, !HIGH);
  } else {
    digitalWrite(LED, !LOW);
  }
}

void htmlIndex() {
  String res_page = FPSTR(MAIN_page);
  www_server.send(200, "text/html", res_page);
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  connectToWiFi();
  setupServer();
  delay(4000);
}

void loop() {
  www_server.handleClient();      //Listen for HTTP requests from clients
}
