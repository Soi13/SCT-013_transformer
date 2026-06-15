
//First step — check raw signal

//Upload this code:

void setup() {
  Serial.begin(115200);
}

void loop() {
  int v = analogRead(A0);
  Serial.println(v);
  delay(1000);
}

//What you should see

//With NO load:

//Values fluctuate around ~512 (middle point)

//With load ON (kettle, heater, etc.):

//Wave-like fluctuation around 512, e.g.:
//480 → 550 → 500 → 530 → ...

//If this works → hardware is correct.


/*
#define SAMPLES 1000

const float ADC_REF = 3.3;        // NodeMCU ADC reference
const float ADC_MAX = 1024.0;     // 10-bit ADC
const float BURDEN = 30.0;        // 30Ω (3x10Ω)
const float CT_RATIO = 2000.0;    // SCT-013-000 = 100A : 50mA => 2000:1

float calcIrms() {
  float sumSq = 0;

  for (int i = 0; i < SAMPLES; i++) {
    int raw = analogRead(A0);

    // remove DC bias (~512)
    float voltage = (raw - 512) * (ADC_REF / ADC_MAX);

    // convert voltage → current in CT secondary
    float current = voltage / BURDEN;

    // convert to primary current
    current = current * CT_RATIO;

    sumSq += current * current;
    delayMicroseconds(200);
  }

  float Irms = sqrt(sumSq / SAMPLES);
  return Irms;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  float Irms = calcIrms();

  Serial.print("Current: ");
  Serial.print(Irms);
  Serial.println(" A");

  delay(1000);
}
*/