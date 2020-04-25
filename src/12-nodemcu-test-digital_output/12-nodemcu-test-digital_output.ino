#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "PIR T-COM";
const char* password = "1234martinrouterking";
ESP8266WebServer server(80);
int outputPin = D1; // input pin for sensor 1

String SendHTML(){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title> Controllo OUTPUT</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>CONTROLLO OUTPUT</h1>\n";
  ptr +="<p>Click to switch OUTPUT on and off</p>\n";
  ptr +="<form method=\"get\">\n";
  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

String SendJsonSensor() {
  String ptr = "{";
  ptr += "  \"status\": \"1\"";
  ptr += "}";
  return ptr;
}

String SendJsonDriver() {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html>";
ptr += "<head>";
ptr += "<title>test</title>";
ptr += "</head>";
ptr += "<body>";
ptr += "<p id=\"demo\"></p>";
ptr += "<script>";
ptr += "var xmlhttp = new XMLHttpRequest();";
ptr += "xmlhttp.onreadystatechange = function() {";
ptr +=   "if (this.readyState == 4 && this.status == 200) {";
ptr +=     "var myObj = JSON.parse(this.responseText);";
ptr +=     "document.getElementById(\"demo\").innerHTML = myObj.status;";
ptr +=   "}";
ptr += "};";
ptr += "xmlhttp.open(\"GET\", \"http://192.168.43.77/jsonsensor.json\", true);";
ptr += "xmlhttp.send(); ";
ptr += "</script>";
ptr += "</body>";
ptr += "</html>";
return ptr;
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_jsonsensor() {
  server.send(200, "text/json", SendJsonSensor());
}

void handle_jsondriver() {
  server.send(200, "text/html", SendJsonDriver());
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void setup() {
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, HIGH);
  Serial.begin(115200);
  delay(100);
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
  server.on("/jsonsensor.json", handle_jsonsensor); 
  server.on("/jsondriver", handle_jsondriver); 
  server.onNotFound(handle_NotFound); 
  server.begin();
  Serial.println("Server HTTP pronto!");
}

void loop() { 
  server.handleClient(); 
}
