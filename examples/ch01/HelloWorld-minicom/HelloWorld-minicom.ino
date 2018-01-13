int LED = 7;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("HIGH");
  delay(1000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("LOW");
  delay(1000);               // wait for a second
  
  
}
