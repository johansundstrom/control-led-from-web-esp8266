#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "index.h"
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>  //https://arduinojson.org/
#include "includes.h"
//https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
ESP8266WebServer www_server(80); //create an instance

#define NTP_offset   60*60  // In seconds (3600S)
#define NTP_interval 60*1  // In miliseconds
#define NTP_adress  "europe.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_adress, NTP_offset, NTP_interval);

extern const char *ssid;
extern const char *password;

const int LED1 = LED_BUILTIN;
const int LED2 = D6;
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
  www_server.on("/api", led);     //Client request handlerFunction
}

//hantera användarklick (från setupServer)
void led() {
  const char* state;
  long long serverEpoch;
  long roundTrip;
  //server_state = www_server.arg("browser_state"); //Getting information about request arguments
  
  //Hantera inkommande request JSON
  if (www_server.hasArg("plain")== true) {
    String jsondata = www_server.arg("plain");
    StaticJsonDocument<64> req_json;
    DeserializationError error = deserializeJson(req_json, jsondata);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    //Plocka ut data ur JSON
    state = req_json["state"];
    long long clientEpoch = req_json["clientEpochInMilliSec"];
    Serial.print("clientEpoch: ");
    Serial.println(clientEpoch);

    
    //Skicka response
    timeClient.update();

    /*
    Serial.print(timeClient.getFormattedTime());
    Serial.print("."); 
    int ms = timeClient.get_millis(); 
    if(ms<10){ 
      Serial.print("00");
      Serial.println(ms);
    } 
    else if(ms>=10 && ms<100){ 
      Serial.print("0");
      Serial.println(ms); 
    } else { 
      Serial.println(ms); 
    } 
    */
    
    long long serverEpoch = timeClient.getEpochTime();  //ger tid utan millisec
    //Serial.println(timeClient.getFormattedTime());
    long long roundTrip = clientEpoch - serverEpoch;
    Serial.print("clientEpoch (efter kalk): ");
    Serial.println(clientEpoch);
    StaticJsonDocument<100> res_json;
    res_json["state"] = state;
    res_json["roundTripInMilliSec"] = roundTrip;
    char buffer[100];
    Serial.println(serverEpoch);
    serializeJson(res_json, buffer);
    www_server.send(200, "application/json", buffer);
  }
  Serial.print("state: ");
  Serial.println(state);
  //if (state == "ON") {
  if (strlen(state) == 2) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    Serial.println("High");
  } else {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    Serial.println("Low");
  }
  //www_server.send(200, "text/html", "ok");
}

//sänd html-sida och HTTP200
void htmlIndex() {
  String res_page = FPSTR(MAIN_page);
  www_server.send(200, "text/html", res_page);
}


void setup() {
  pinMode(LED1, OUTPUT); //builtin
  pinMode(LED2, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  connectToWiFi();
  setupServer();
  delay(4000);
  timeClient.begin();
}

void loop() {
  www_server.handleClient();      //Listen for HTTP requests from clients
}
