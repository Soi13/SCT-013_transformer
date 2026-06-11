/*
First step — check raw signal

Upload this code:

void setup() {
  Serial.begin(115200);
}

void loop() {
  int v = analogRead(A0);
  Serial.println(v);
  delay(10);
}

What you should see

With NO load:

Values fluctuate around ~512 (middle point)

With load ON (kettle, heater, etc.):

Wave-like fluctuation around 512, e.g.:
480 → 550 → 500 → 530 → ...

If this works → hardware is correct.
*/



void setup() {

}

void loop() {

}
