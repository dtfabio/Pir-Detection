#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String local_ip = "";
String color_div_sensor_1 = "green";
String color_div_sensor_2 = "green";
String color_div_sensor_3 = "green";
String color_div_sensor_4 = "green";

const char* ssid = "PIR T-COM";
const char* password = "1234martinrouterking";
ESP8266WebServer server(80);
int inputPinSensor1 = D1;
int inputPinSensor2 = D2;
int inputPinSensor3 = D3;
int inputPinSensor4 = D4;
int sensor_1_State;
int sensor_2_State;
int sensor_3_State;
int sensor_4_State;
int val = 0; // variable for reading the pin status

String SendHTML(){
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<title>test</title>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1 style=\"font-size: 45px; font-family: sans-serif;\">PIR Detection</h1>\n";
  ptr += "<div id=\"wrapper_1_2\" style=\"margin: 0px; border-style: none; width: 710px; overflow: auto; padding: 8px 8px 5px 8px; background-color: gray;\"> \n";
  ptr += "  <div id=\"div_sensor_1\" style=\"float: left; width: 350px; height: 220px; border-style: none; background-color: gray; margin-right: 5px;\">\n";
  ptr += "    <h1 style=\"font-size: 50px; font-family: sans-serif; margin: 0px; padding: 20px;\">#1</h1>\n";
  ptr += "  </div>\n";
  ptr += "  <div id=\"div_sensor_2\" style=\"float: left; width:350px; height: 220px; border-style: none; background-color: gray; margin-left: 5px;\"> \n";
  ptr += "    <h1 style=\"font-size: 50px; font-family: sans-serif; margin: 0px; padding: 20px;\">#2</h1>\n";
  ptr += "  </div>\n";
  ptr += "</div>\n";
  ptr += "<div id=\"wrapper_3_4\" style=\"margin: 0px; border-style: none; width: 710px; overflow: auto; padding: 5px 8px 8px 8px; background-color: gray;\"> \n";
  ptr += "  <div id=\"div_sensor_4\" style=\"float: left; width: 350px; height: 220px; border-style: none; background-color: gray; margin-right: 5px;\">\n";
  ptr += "    <h1 style=\"font-size: 50px; font-family: sans-serif; margin: 0px; padding: 20px;\">#4</h1>\n";
  ptr += "  </div>\n";
  ptr += "  <div id=\"div_sensor_3\" style=\"float: left; width:350px; height: 220px; border-style: none; background-color: gray; margin-left: 5px;\"> \n";
  ptr += "    <h1 style=\"font-size: 50px; font-family: sans-serif; margin: 0px; padding: 20px;\">#3</h1>\n";
  ptr += "  </div>\n";
  ptr += "</div>\n";
  ptr += "<p id=\"output\"></p>\n";
  ptr += "<script>\n";
  ptr += "var xmlhttp = new XMLHttpRequest();\n";
  ptr += "xmlhttp.onreadystatechange = function() {\n";
  ptr += "  if (this.readyState == 4 && this.status == 200) {\n";
  ptr += "    var myObj = JSON.parse(this.responseText);\n";
  ptr += "    document.getElementById(\"div_sensor_1\").style.backgroundColor = myObj.color_sensor_1;\n";
  ptr += "    document.getElementById(\"div_sensor_2\").style.backgroundColor = myObj.color_sensor_2;\n";
  ptr += "    document.getElementById(\"div_sensor_3\").style.backgroundColor = myObj.color_sensor_3;\n";
  ptr += "    document.getElementById(\"div_sensor_4\").style.backgroundColor = myObj.color_sensor_4;\n";
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
  ptr += "      var xmlhttp = new XMLHttpRequest();\n";
  ptr += "    xmlhttp.onreadystatechange = function() {\n";
  ptr += "        if (this.readyState == 4 && this.status == 200) {\n";
  ptr += "          var myObj = JSON.parse(this.responseText);\n";
  ptr += "          document.getElementById(\"div_sensor_1\").style.backgroundColor = myObj.color_sensor_1;\n";
  ptr += "          document.getElementById(\"div_sensor_2\").style.backgroundColor = myObj.color_sensor_2;\n";
  ptr += "          document.getElementById(\"div_sensor_3\").style.backgroundColor = myObj.color_sensor_3;\n";
  ptr += "          document.getElementById(\"div_sensor_4\").style.backgroundColor = myObj.color_sensor_4;\n";
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

String sendStatusJson(String color_div_sensor_1, String color_div_sensor_2,
        String color_div_sensor_3, String color_div_sensor_4) {
  String ptr = "{\n";
  ptr += "  \"color_sensor_1\": \"";
  ptr += color_div_sensor_1;
  ptr += "\",\n";
  ptr += "  \"color_sensor_2\": \"";
  ptr += color_div_sensor_2;
  ptr += "\",\n";
  ptr += "  \"color_sensor_3\": \"";
  ptr += color_div_sensor_3;
  ptr += "\",\n";
  ptr += "  \"color_sensor_4\": \"";
  ptr += color_div_sensor_4;
  ptr += "\"\n";
  ptr += "}";
  return ptr;
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_update_status() {
  server.send(200, "text/html", sendStatusJson(color_div_sensor_1, color_div_sensor_2, color_div_sensor_3, color_div_sensor_4));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  pinMode(inputPinSensor1, INPUT);
  pinMode(inputPinSensor2, INPUT);
  pinMode(inputPinSensor3, INPUT);
  pinMode(inputPinSensor4, INPUT);
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
  sensor_1_State = LOW;
  sensor_2_State = LOW;
  sensor_3_State = LOW;
  sensor_4_State = LOW;
  server.begin();
  Serial.println("Server HTTP pronto!");
}

void loop() { 
  server.handleClient(); 
  detectChange(inputPinSensor1, &sensor_1_State, &color_div_sensor_1, "1");
  detectChange(inputPinSensor2, &sensor_2_State, &color_div_sensor_2, "2");
  detectChange(inputPinSensor3, &sensor_3_State, &color_div_sensor_3, "3");
  detectChange(inputPinSensor4, &sensor_4_State, &color_div_sensor_4, "4");
}

void detectChange(int inputPin, int* pirState, String* colorDivSensor, String sensorNumber) {
  // read input value
  val = digitalRead(inputPin);
  // check if the input is HIGH 
  if (val == HIGH) {
    if (*pirState == LOW) {
      // we have just turned on
      Serial.println("Sensor " + sensorNumber + ": Motion detected");
      *pirState = HIGH;
      *colorDivSensor = "red";
    }
  } else {
    if (*pirState == HIGH){
      // we have just turned off
      Serial.println("Sensor " + sensorNumber + ": Motion ended");
      *pirState = LOW;
      *colorDivSensor = "green";
    }
  }
}
