//CODE FOR ESP8266

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define WIFI_SSID     "wellsJuh"
#define WIFI_PASS     "Greenearth196"
#define BROKER_IP     "10.0.0.220"
#define BROKER_PORT   1883

SoftwareSerial arduinoSerial(D2, D3);  // RX TX respectively
WiFiClient espClient;
PubSubClient mqtt(espClient);

void connectWifi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("WiFi connected! IP: ");
    Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
    while (!mqtt.connected()) {
        Serial.print("Connecting to MQTT broker...");
        if (mqtt.connect("ESP8266Client")) {
            Serial.println("Connected!");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(mqtt.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);       // USB debug
    arduinoSerial.begin(9600);  // Arduino communication
    Serial.println("\nESP8266 Starting...");
    connectWifi();
    mqtt.setServer(BROKER_IP, BROKER_PORT);
    Serial.println("Setup complete!");
}

void loop() {
    if (!mqtt.connected()) reconnectMQTT();
    mqtt.loop();

    if (arduinoSerial.available()) {
        String data = arduinoSerial.readStringUntil('\n');
        data.trim();

        Serial.print("Received from Arduino: ");
        Serial.println(data);

        if (mqtt.publish("sensors/data", data.c_str())) {
            Serial.println("Published successfully!");
        } else {
            Serial.println("Publish failed!");
        }
    }
}
