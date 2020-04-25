int inputPin = 2; // choose the input pin (for PIR sensor)
int pirState = LOW; // we start, assuming no motion detected
int val = 0; // variable for reading the pin status
     
void setup() {
  pinMode(inputPin, INPUT); // declare sensor as input

  Serial.begin(9600);
}

void loop(){
  // read input value
  val = digitalRead(inputPin);
  // check if the input is HIGH
  if (val == HIGH) {
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected");
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned off
      Serial.println("Motion ended");
      pirState = LOW;
    }
  }
}
