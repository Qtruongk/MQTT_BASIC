#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// Khởi tạo client WiFi và MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

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

// Hàm callback nhận dữ liệu khi có tin nhắn từ broker
void mqttCallback(char* topic, byte* message, unsigned int length) {
    Serial.print("\n[MQTT Callback] Nhận tin nhắn từ topic: ");
    Serial.println(topic);
    
    String messageContent = "";
    for (unsigned int i = 0; i < length; i++) {
        messageContent += (char)message[i];
    }
    
    Serial.print("[MQTT Callback] Nội dung: ");
    Serial.println(messageContent);
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("[MQTT] Đang kết nối tới Broker...");
        String clientId = "ESP32-Subscriber-" + String(random(0xffff), HEX);
        
        bool isConnected = false;
        if (strlen(MQTT_USER) > 0) {
            isConnected = mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD);
        } else {
            isConnected = mqttClient.connect(clientId.c_str());
        }

        if (isConnected) {
            Serial.println(" Thành công!");
            // Đăng ký nhận tin từ các topic
            mqttClient.subscribe(MQTT_TOPIC_DATA);
            mqttClient.subscribe(MQTT_TOPIC_COMMAND);
            Serial.print("[MQTT] Đã đăng ký topic: ");
            Serial.print(MQTT_TOPIC_DATA);
            Serial.print(", ");
            Serial.println(MQTT_TOPIC_COMMAND);
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
    Serial.println("\n--- [ESP32 SUBSCRIBER NODE] Khởi động ---");

    setupWiFi();
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
}

void loop() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();
}

