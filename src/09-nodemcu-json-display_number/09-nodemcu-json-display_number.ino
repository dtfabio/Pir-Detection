#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Console.h>

char input_byte = 0;
bool input_valid = false;
byte real_value = 0;
String output_string = "";
String local_ip = "";

const char* ssid = "PIR T-COM";
const char* password = "1234martinrouterking";
ESP8266WebServer server(80);
int inputPinSensor1 = D1; // input pin for sensor 1
int sensor_1_State = LOW; // we start assuming no motion detected for sensor 1
int val = 0; // variable for reading the pin status

String SendHTML(){
  String ptr = "<!DOCTYPE html>\n";
ptr += "<html>\n";
ptr += "<head>\n";
ptr += "<title>test</title>\n";
ptr += "</head>\n";
ptr += "<body>\n";
ptr += "<p id=\"output\"></p>\n";
ptr += "<script>\n";
ptr += "var xmlhttp = new XMLHttpRequest();\n";
ptr += "xmlhttp.onreadystatechange = function() {\n";
ptr += "  if (this.readyState == 4 && this.status == 200) {\n";
ptr += "    var myObj = JSON.parse(this.responseText);\n";
ptr += "    document.getElementById(\"output\").innerHTML = myObj.status;\n";
ptr += "  }\n";
ptr += "};\n";
ptr += "xmlhttp.open(\"GET\", \"http://";
ptr += local_ip;
ptr += "/status.json\", true);\n";
ptr += "xmlhttp.send(); \n";
ptr += "</script>\n";
ptr += "<script type=\"text/javascript\">\n";
ptr += "window.onload = function () {\n";
ptr += "    setInterval(function() {\n";
ptr += "     var xmlhttp = new XMLHttpRequest();\n";
ptr += "    xmlhttp.onreadystatechange = function() {\n";
ptr += "        if (this.readyState == 4 && this.status == 200) {\n";
ptr += "          var myObj = JSON.parse(this.responseText);\n";
ptr += "          document.getElementById(\"output\").innerHTML = myObj.status;\n";
ptr += "        }\n";
ptr += "    };\n";
ptr += "    xmlhttp.open(\"GET\", \"http://";
ptr += local_ip;
ptr += "/status.json\", true);\n";
ptr += "    xmlhttp.send(); \n";
ptr += "  }, 1000);\n";
ptr += "};\n";
ptr += "</script>\n";
ptr += "</body>\n";
ptr += "</html>";
  return ptr;
}

String sendStatusJson(byte real_value) {
  String ptr = "{\n";
  ptr += "  \"status\": \"";
  ptr += (String) real_value;
  ptr += "\"\n";
  ptr += "}";
  return ptr;
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_update_status() {
  server.send(200, "text/html", sendStatusJson(real_value)); }

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void setup() {
  pinMode(inputPinSensor1, INPUT); // declare sensor 1 pin as input
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
  local_ip = WiFi.localIP().toString();
  server.on("/", handle_OnConnect); 
  server.on("/status.json", handle_update_status); 
  server.onNotFound(handle_NotFound); 
  server.begin();
  Serial.println("Server HTTP pronto!");
}

void loop() { 
  server.handleClient(); 
  detectChange(&inputPinSensor1, &sensor_1_State, "1");

  if (Serial.available() > 0) {    // is a character available?
    input_byte = Serial.read();       // get the character
  
    // check if a number was received
    if ((input_byte >= '0') && (input_byte <= '9')) {
      input_valid = true;
      real_value = input_byte - 48;
      //output_string = "Real value: " + (String) real_value;
      //Serial.println(output_string);
    }
    else if ((input_byte >= 'a') && (input_byte <= 'f')) {
      input_valid = true;
      real_value = input_byte - 87;
      //output_string = "Real value: " + (String) real_value;
      //Serial.println(output_string);
    }
    else {
      input_valid = false;
      Serial.println("Input not valid.");
    }
  }
}

void detectChange(int* inputPin, int* pirState, String sensorNumber) {
  // read input value
  val = digitalRead(*inputPin);
  // check if the input is HIGH 
  if (val == HIGH) {
    if (*pirState == LOW) {
      // we have just turned on
      Serial.println("Sensor " + sensorNumber + ": Motion detected");
      *pirState = HIGH;
      //server.send(200, "text/html", SendHTML());
    }
  } else {
    if (*pirState == HIGH){
      // we have just turned off
      Serial.println("Sensor " + sensorNumber + ": Motion ended");
      *pirState = LOW;
      //server.send(200, "text/html", SendHTML());
    }
  }
}
