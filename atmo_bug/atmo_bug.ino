#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_HDC1000.h>
#include "secrets.h"

// 3 Digit ID; Unique per device
const char* atmo_bug_id = "000";

char* mqtt_client_name = "atmo_bug_XXX";
char* mqtt_temp_topic = "atmo_bug/temperature/XXX";
char* mqtt_humidity_topic = "atmo_bug/humidity/XXX";

void reconnect();
void callback(char*, byte*, unsigned int);
void set_id(char*, const char*);

Adafruit_HDC1000 hdc = Adafruit_HDC1000();

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(0, OUTPUT);

  set_id(mqtt_client_name, atmo_bug_id);
  set_id(mqtt_temp_topic, atmo_bug_id);
  set_id(mqtt_humidity_topic, atmo_bug_id);

  Serial.println(mqtt_client_name);
  Serial.println(mqtt_temp_topic);
  Serial.println(mqtt_humidity_topic);

  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}

// Replaces the last 3 characters of value with id
void set_id(char* value, const char* id) {
  while (*value != '\0')
    value++;

  *(value - 3) = id[0];
  *(value - 2) = id[1];
  *(value - 1) = id[2];
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(0, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(0, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_name, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    char num_str[8];
    dtostrf(hdc.readTemperature(), 5, 2, num_str);
    snprintf (msg, 75, "%s", num_str);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_temp_topic, msg);

    dtostrf(hdc.readHumidity(), 5, 2, num_str);
    snprintf (msg, 75, "%s", num_str);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqtt_humidity_topic, msg);
  }

}
