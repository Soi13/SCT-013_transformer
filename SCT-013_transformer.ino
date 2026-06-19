
//First step — check raw signal

//Upload this code:
/*
void setup() {
  Serial.begin(115200);
}

void loop() {
  int v = analogRead(A0);
  Serial.println(v);
  delay(100);
}
*/

//What you should see

//With NO load:

//Values fluctuate around ~512 (middle point)

//With load ON (kettle, heater, etc.):

//Wave-like fluctuation around 512, e.g.:
//480 → 550 → 500 → 530 → ...

//If this works → hardware is correct.

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Soi13"
#define wifi_password ""

#define mqtt_server "192.168.1.64"
#define mqtt_user "mqtt_user"
#define mqtt_password ""

#define dryer_current "homeassistant/sensor/dryer_current"
#define dryer_on_off "homeassistant/sensor/dryer_on_off"

#define SAMPLES 1000

WiFiClient espClient;
PubSubClient client(espClient);

const float ADC_REF = 3.3;        // NodeMCU ADC reference
const float ADC_MAX = 1024.0;     // 10-bit ADC
const float BURDEN = 30.0;        // 30Ω (3x10Ω)
const float CT_RATIO = 2000.0;    // SCT-013-000 = 100A : 50mA => 2000:1

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    String clientId = "ESP8266-";
    clientId += String(ESP.getChipId());
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 5 sec");
      delay(5000);
    }
  }
}

float calcIrms() {
  float sumSq = 0;

  for (int i = 0; i < SAMPLES; i++) {
    int raw = analogRead(A0);

    // remove DC bias (~512)
    float voltage = (raw - 484) * (ADC_REF / ADC_MAX);

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
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  connectMQTT();
}

void loop() {
  // Check WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost. Reconnecting...");
    setup_wifi();
  }

  // Check MQTT
  if (!client.connected()) {
    Serial.println("MQTT disconnected. Reconnecting...");
    connectMQTT();
  }

  client.loop();

  float Irms = calcIrms();

  //If current more than 5A we count that dryer is working and show status as On and display current, otherwise show status is Off and current = 0
  if (Irms > 5.0) {
    client.publish(dryer_on_off, "Dryer is On");
    client.publish(dryer_current, String(Irms).c_str());
  } else {
    client.publish(dryer_on_off, "Dryer is Off");
    client.publish(dryer_current, "0");
  }

  //Serial.print("Current: ");
  //Serial.print(Irms);
  //Serial.println(" A");

  delay(1000);
}
