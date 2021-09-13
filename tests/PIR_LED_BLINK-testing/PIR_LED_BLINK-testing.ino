const int LED_PIN = 2; // D4
const int SENSOR_PIN = 15; // D8

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  if(digitalRead(SENSOR_PIN) == HIGH){ // motion detected
    digitalWrite(LED_PIN, HIGH); // turn LED on
    delay(1000);
  }
  else{ //motion not detected
    digitalWrite(LED_PIN, LOW); // turn LED off
  }
}
