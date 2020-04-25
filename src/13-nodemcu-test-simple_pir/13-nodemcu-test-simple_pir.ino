 const int PIRSensorOutPin = D1;    //PIR Sensor OUT Pin
 void setup() {
    Serial.begin(115200);
    pinMode(PIRSensorOutPin, INPUT);
 }
 void loop()
 {
    if (digitalRead(PIRSensorOutPin) == LOW) {
       Serial.println("Movement not detected.");
    }
    else {
      Serial.println("Movement detected.");
    }
}
