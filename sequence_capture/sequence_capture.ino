/*
Created by: Junior ALZATE 10/08/2025
*/

volatile int state = 1;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(13, OUTPUT);
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  //pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), Lectura, FALLING);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(3, HIGH);
  delay(1000);
  //digitalWrite(7, HIGH);

  //state = 'LOW';
  
  
  //else{
    //digitalWrite(7, LOW);
  //}  


                        // wait for a second
}

void Lectura(){
  //state = !state;
  state = digitalRead(2);
  Serial.println(state);
  digitalWrite(3, state);

}
