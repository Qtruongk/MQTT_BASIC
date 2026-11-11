# MQTT Basic with ESP32 & PlatformIO

Dự án giao tiếp MQTT giữa 2 bo mạch ESP32 thông qua MQTT Broker chạy trên mạng nội bộ (LAN).

## 📁 Cấu trúc dự án

```plaintext
MQTT_Basic/
├── include/
│   └── config.h         # Cấu hình kết nối WiFi và MQTT Broker chung
├── src/
│   ├── publisher/
│   │   └── main.cpp     # Mã nguồn Node Gửi (COM15) - định kỳ gửi dữ liệu JSON
│   └── subscriber/
│       └── main.cpp     # Mã nguồn Node Nhận (COM16) - nhận và xử lý callback tin nhắn
└── platformio.ini       # File cấu hình môi trường PlatformIO
```

## ⚙️ Cấu hình môi trường (`platformio.ini`)

- **Board**: ESP32 Dev Module (`esp32dev`)
- **Framework**: Arduino
- **Thư viện chung**: `knolleary/PubSubClient @ ^2.8`
- **Môi trường**:
  - `[env:publisher_node]`: Nạp vào bo mạch Publisher qua cổng **COM15**
  - `[env:subscriber_node]`: Nạp vào bo mạch Subscriber qua cổng **COM16**

## 🚀 Hướng dẫn nạp code

### 1. Nạp cho Publisher (COM15):
```bash
pio run -e publisher_node -t upload
```

### 2. Nạp cho Subscriber (COM16):
```bash
pio run -e subscriber_node -t upload
```

### 3. Theo dõi Serial Monitor:
- Publisher:
  ```bash
  pio device monitor -p COM15 -b 115200
  ```
- Subscriber:
  ```bash
  pio device monitor -p COM16 -b 115200
  ```
