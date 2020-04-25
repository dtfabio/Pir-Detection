#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "PIR T-COM";
const char* password = "1234martinrouterking";
ESP8266WebServer server(80);
int LEDpin = D0;
bool LEDstatus = LOW;

String SendHTML(char led){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title> Controllo OUTPUT</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>CONTROLLO OUTPUT</h1>\n";
  ptr +="<p>Click to switch OUTPUT on and off</p>\n";
  ptr +="<form method=\"get\">\n";
  if (led) {
    ptr +="<input type=\"button\" value=\"OUTPUT OFF\" onclick=\"window.location.href= '/outputoff'\">\n";
  }
  else {
    ptr +="<input type=\"button\" value=\"OUTPUT ON\" onclick=\"window.location.href= '/outputon'\">\n";
  }
  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_outputon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true));
}

void handle_outputoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LEDpin, OUTPUT); 
  Serial.println("Connessione a...."); 
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connessa!"); 
  Serial.print("IP:"); 
  Serial.println(WiFi.localIP()); 
  server.on("/", handle_OnConnect); 
  server.on("/outputon", handle_outputon); 
  server.on("/outputoff", handle_outputoff); 
  server.onNotFound(handle_NotFound); 
  server.begin();
  Serial.println("Server HTTP pronto!"); 
}

void loop() { 
  server.handleClient(); 
  if (LEDstatus) {
    digitalWrite(LEDpin, HIGH);
  }
  else {
    digitalWrite(LEDpin, LOW);
  }
}
