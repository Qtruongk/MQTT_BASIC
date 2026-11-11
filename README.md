# MQTT Basic with ESP32 & PlatformIO

Dự án giao tiếp MQTT giữa 2 bo mạch ESP32 thông qua MQTT Broker chạy trên mạng nội bộ (LAN).

## Cấu trúc thư mục

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

