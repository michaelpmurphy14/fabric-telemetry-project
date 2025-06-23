#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "secrets.h"         // WiFi + endpoint + credentials
#ifndef WIFI_SSID
  #error "Missing WIFI_SSID: Did you forget to create secrets.h from secrets_example.h?"
#endif

#include "certificates.h"    // AWS and HiveMQ certs (R"EOF" blocks)
#ifndef AWS_CERT_CA
  #error "Missing AWS_CERT_CA: Did you forget to create certificates.h from certificates_example.h?"
#endif


#include "node_config.h"    // <-- the generated header
#ifndef NODE_CONFIG
  #error "NODE_CONFIG not defined. Build with -DNODE_CONFIG=\"configs/nodeA.json\""
#endif

// Global MQTT clients
WiFiClientSecure awsNet;
PubSubClient awsClient(awsNet);

WiFiClientSecure hiveNet;
PubSubClient hiveClient(hiveNet);

// Telemetry state
float temp = 22.0;

// ---------- Reconnection Helpers ----------
void reconnectAWS() {
  while (!awsClient.connected()) {
    Serial.print("Connecting to AWS...");
    String clientId = "esp32cam-" + String(random(0xffff), HEX);
    if (awsClient.connect(clientId.c_str())) {
      Serial.println("connected to AWS");
    } else {
      Serial.print("failed, rc=");
      Serial.print(awsClient.state());
      delay(3000);
    }
  }
}

void reconnectHive() {
  while (!hiveClient.connected()) {
    Serial.print("Connecting to HiveMQ...");
    String clientId = "esp32cam-" + String(random(0xffff), HEX);
    if (hiveClient.connect(clientId.c_str(), HIVEMQ_USER, HIVEMQ_PASS)) {
      Serial.println("connected to HiveMQ");
    } else {
      Serial.print("failed, rc=");
      Serial.print(hiveClient.state());
      delay(3000);
    }
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // AWS TLS Setup
  awsNet.setCACert(AWS_CERT_CA);
  awsNet.setCertificate(AWS_CERT_CRT);
  awsNet.setPrivateKey(AWS_CERT_PRIVATE);
  awsClient.setServer(AWS_IOT_ENDPOINT, AWS_IOT_PORT);

  // HiveMQ TLS Setup
  hiveNet.setCACert(HIVEMQ_CA_CERT);
  hiveClient.setServer(HIVEMQ_ENDPOINT, 8883);
}

// ---------- Loop ----------
void loop() {
  if (!awsClient.connected()) reconnectAWS();
  if (!hiveClient.connected()) reconnectHive();

  awsClient.loop();
  hiveClient.loop();

  // Simulate sensor drift
  float drift = random(-5, 6) / 10.0;
  temp += drift;
  temp = constrain(temp, 21.0, 30.0);

  int rssi = WiFi.RSSI();
  int heap_free = ESP.getFreeHeap();

  // JSON payload
  StaticJsonDocument<256> doc;
  doc["device_id"] = DEVICE_ID;
  doc["status"] = "online";
  doc["temp_c"] = temp;
  doc["uptime_sec"] = millis() / 1000;
  doc["rssi"] = rssi;
  doc["heap_free"] = heap_free;

  String alert = "";
  if (rssi < ALERT_RSSI || heap_free < 15000) {
    alert = "weak signal or low memory";
  } else if (temp > 28.0) {
    alert = "overtemperature";
  }
  if (alert != "") {
    doc["alert"] = alert;
  }

  char payload[256];
  serializeJson(doc, payload);

  // Dual publish
  String topic = String(MQTT_BASE_TOPIC) + "/status";
  hiveClient.publish(topic.c_str(), payload);
  awsClient.publish(topic.c_str(), payload);
  
  Serial.println("Published telemetry to HiveMQ and AWS.");
  delay(5000);
}
