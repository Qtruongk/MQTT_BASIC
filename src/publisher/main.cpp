#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// Khởi tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastMsgTime = 0;
int counter = 0;

void setupWiFi() {
    delay(10);
    Serial.println();
    Serial.print("[WiFi] Đang kết nối tới: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("[WiFi] Đã kết nối thành công!");
    Serial.print("[WiFi] Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("[MQTT] Đang kết nối tới Broker...");
        String clientId = "ESP32-Publisher-" + String(random(0xffff), HEX);
        
        bool isConnected = false;
        if (strlen(MQTT_USER) > 0) {
            isConnected = mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD);
        } else {
            isConnected = mqttClient.connect(clientId.c_str());
        }

        if (isConnected) {
            Serial.println(" Thành công!");
        } else {
            Serial.print(" Thất bại, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" -> Thử lại sau 5 giây...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n--- [ESP32 PUBLISHER NODE] Khởi động ---");

    setupWiFi();
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_PORT);
}

void loop() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();

    // Gửi dữ liệu định kỳ mỗi 5 giây
    unsigned long now = millis();
    if (now - lastMsgTime > 5000) {
        lastMsgTime = now;
        counter++;

        String payload = "{\"device\":\"esp32_publisher\",\"counter\":" + String(counter) + "}";
        Serial.print("[MQTT] Đang publish tin nhắn: ");
        Serial.println(payload);

        mqttClient.publish(MQTT_TOPIC_DATA, payload.c_str());
    }
}

