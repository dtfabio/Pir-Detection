int inputPinSensor1 = 2; // input pin for sensor 1
int sensor_1_State = LOW; // we start assuming no motion detected for sensor 1
int val = 0; // variable for reading the pin status
     
void setup() {
  pinMode(inputPinSensor1, INPUT); // declare sensor 1 pin as input

  Serial.begin(9600);
}

void loop(){
  detectChange(&inputPinSensor1, &sensor_1_State);
}

void detectChange(int* inputPin, int* pirState) {
  // read input value
  val = digitalRead(*inputPin);
  // check if the input is HIGH
  if (val == HIGH) {
    if (*pirState == LOW) {
      // we have just turned on
      Serial.println("Current detected");
      *pirState = HIGH;
    }
  } else {
    if (*pirState == HIGH){
      // we have just turned off
      Serial.println("Current ended");
      *pirState = LOW;
    }
  }
}
