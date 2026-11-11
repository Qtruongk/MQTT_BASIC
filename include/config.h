#ifndef CONFIG_H
#define CONFIG_H

// ==================== CẤU HÌNH WIFI ====================
#define WIFI_SSID       "THUY MAI"
#define WIFI_PASSWORD   "212111605"

// ==================== CẤU HÌNH MQTT BROKER ====================
#define MQTT_BROKER_IP  "192.168.1.21" // IP Wi-Fi của máy Windows chạy Broker
const char* const mqtt_server = "192.168.1.21";
#define MQTT_PORT       1883
#define MQTT_USER       ""              // Để trống nếu broker không yêu cầu xác thực
#define MQTT_PASSWORD   ""

// ==================== TOPIC MQTT ====================
#define MQTT_TOPIC_DATA     "esp32/sensor_data"
#define MQTT_TOPIC_COMMAND  "esp32/control"

#endif // CONFIG_H

