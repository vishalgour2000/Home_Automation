#include <ESP8266WiFi.h> 
#include "DHT.h"
#include <ESP8266WebServer.h> 
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHT.h"
#define TYPE DHT11
#define PIN D0
String str;
const char* ssid = "Honor 9 Lite_14EF";
const char* password = "son29082000";
int out=D5; // for relay 2
int out2 =D4; //for relay 1
int dhtled=D1; // for dht LED(relay 3)
int echo=D6; // 
int trigger=D7;
int ultraled=D8; // for ultrasonic sensor (relay 4) 
int buzzer=D2;
DHT obj(PIN,TYPE);
ESP8266WebServer server(80);

void handleRoot() 
{
  char temp[400];

  snprintf(temp, 400,
    "<html>\\
  <head>\
  </head>\
    <body>\
    <h1><b>HOME AUTOMATION</b></h1>\
    <h4>led1</h4>\
    <a href =\"/1\"> <button> Turn ON</button> </a>\
    <a href =\"/0\"> <button> Turn OFF</button> </a>\
    <h4>led2</h4>\
    <a href =\"/2\"> <button> Turn ON</button> </a>\
    <a href =\"/3\"> <button> Turn OFF</button> </a>\
  </body>\
</html>");
  server.send(200, "text/html", temp);

}


void ledOn() {
  server.send(200,"text/plain", "led 1 turned on");
  pinMode(out, OUTPUT);
  digitalWrite(out, LOW);
}
void led2On() {
  server.send(200,"text/plain", "led 2 turned on");
  pinMode(out2, OUTPUT);
  digitalWrite(out2, LOW);
}
void ledOff() {
  server.send(200,"text/plain", "led 1 turned off");
  pinMode(out, OUTPUT);
  digitalWrite(out, HIGH);
}

void led2Off() {
  server.send(200,"text/plain", "led 1 turned off");
  pinMode(out2, OUTPUT);
  digitalWrite(out2, HIGH);
}


void setup() {
  obj.begin();
  Serial.begin(9600);
  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  pinMode(ultraled,OUTPUT);
  pinMode(dhtled,OUTPUT);
  pinMode(PIN,INPUT);
  Serial.print("Connecting to network");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);   // Connect to WiFi network

  while (WiFi.status() != WL_CONNECTED) {    // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/0", ledOff);
  server.on("/1", ledOn);
  server.on("/2", led2On);
  server.on("/3", led2Off);
  

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
   server.handleClient();
   WiFiClient client;
   HTTPClient http;
   //code for temperature sensor
  float h=obj.readTemperature();
  Serial.println(h);
  Serial.print("C");
  if(h>=30)
  digitalWrite(dhtled,LOW);
  else if(h<=16)
  digitalWrite(dhtled,HIGH);
  else if(h>=50)
  {
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
  delay(500);
  }
  //code for ultrasonic sensor
  long distance,duration;
  digitalWrite(trigger,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigger,LOW);
  duration=pulseIn(echo,HIGH);
  distance=((duration/2)/29.1);
  Serial.print(distance);
  Serial.println("cm");
  if(distance<=10)
  {
  digitalWrite(ultraled,LOW);
  delay(1000);
  }
  else if(distance>=10)
  {
  digitalWrite(ultraled,HIGH);
  }
  delay(1000);
    
}
