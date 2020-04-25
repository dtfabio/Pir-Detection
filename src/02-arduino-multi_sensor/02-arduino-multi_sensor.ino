int inputPinSensor1 = 2; // input pin for sensor 1
int inputPinSensor2 = 3; // input pin for sensor 2
int inputPinSensor3 = 4; // input pin for sensor 3
int inputPinSensor4 = 5; // input pin for sensor 4
int sensor_1_State = LOW; // we start assuming no motion detected for sensor 1
int sensor_2_State = LOW; // we start assuming no motion detected for sensor 2
int sensor_3_State = LOW; // we start assuming no motion detected for sensor 3
int sensor_4_State = LOW; // we start assuming no motion detected for sensor 4
int val = 0; // variable for reading the pin status
     
void setup() {
  pinMode(inputPinSensor1, INPUT); // declare sensor 1 pin as input
  pinMode(inputPinSensor2, INPUT); // declare sensor 1 pin as input
  pinMode(inputPinSensor3, INPUT); // declare sensor 1 pin as input
  pinMode(inputPinSensor4, INPUT); // declare sensor 1 pin as input

  Serial.begin(9600);
}

void loop(){
  detectChange(&inputPinSensor1, &sensor_1_State, "1");
  detectChange(&inputPinSensor2, &sensor_2_State, "2");
  detectChange(&inputPinSensor3, &sensor_3_State, "3");
  detectChange(&inputPinSensor4, &sensor_4_State, "4");
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
    }
  } else {
    if (*pirState == HIGH){
      // we have just turned off
      Serial.println("Sensor " + sensorNumber + ": Motion ended");
      *pirState = LOW;
    }
  }
}
