# ESP32 MQTT Communication System

Dự án triển khai mô hình truyền thông phân tán dựa trên giao thức MQTT (Message Queuing Telemetry Transport) giữa hai vi điều khiển ESP32 thông qua một MQTT Broker nội bộ (Local Network).

## 1. Tổng quan hệ thống

Hệ thống hoạt động theo mô hình Client - Server (Publish/Subscribe) gồm 3 thành phần chính:

```text
+-----------------------+                    +-------------------------+
|   ESP32 - Publisher   |                    |    ESP32 - Subscriber   |
|       (Port: COM15)   |                    |       (Port: COM16)     |
+-----------+-----------+                    +------------+------------+
            |                                             ^
       MQTT | Publish                                MQTT | Subscribe
      Topic | "esp32/message"                       Topic | "esp32/message"
            v                                             |
    +-------+---------------------------------------------+-------+
    |                     Local MQTT Broker                       |
    |                 (Mosquitto - 192.168.1.21:1883)             |
    +-------------------------------------------------------------+
```

- **Publisher Node (COM15)**: Thu nhận chuỗi ký tự do người dùng nhập từ Serial Monitor, sau đó đóng gói và xuất bản (Publish) tin nhắn lên topic `esp32/message`.
- **MQTT Broker (Host Machine)**: Máy chủ trung gian (Mosquitto Broker) điều phối và định tuyến các gói tin trong mạng nội bộ qua cổng mặc định `1883`.
- **Subscriber Node (COM16)**: Đăng ký (Subscribe) topic `esp32/message`, tiếp nhận gói tin thông qua hàm callback ngắt sự kiện và in nội dung trực tiếp ra Serial Monitor.

## 2. Đặc điểm kỹ thuật

- **Kiến trúc mã nguồn đơn (Monorepo with Multi-Environment)**: Cả hai node được phát triển trong cùng một dự án PlatformIO. Tệp `platformio.ini` sử dụng tính năng `build_src_filter` để tách biệt mã nguồn biên dịch cho từng node mà không gây xung đột hàm `main()` hay tài nguyên.
- **Tập trung hóa cấu hình**: Thông số mạng WiFi, địa chỉ IP Broker và các Topic được quy định tại duy nhất một tệp tiêu đề `include/config.h`.
- **Cơ chế truyền nhận tương tác**: Node Publisher không phát tin tự động theo chu kỳ đếm, mà trực tiếp đọc luồng dữ liệu vào (Serial stream) từ người dùng. Node Subscriber tiếp nhận dữ liệu bằng cơ chế bất đồng bộ qua callback của thư viện `PubSubClient`.

## 3. Cấu trúc thư mục

```text
MQTT_Basic/
├── include/
│   └── config.h         # Cấu hình chung: WiFi SSID/Pass, Broker IP, Port, Topic
├── lib/                 # Thư viện cục bộ (nếu có bổ sung)
├── src/
│   ├── publisher/
│   │   └── main.cpp     # Mã nguồn dành riêng cho Publisher (COM15)
│   └── subscriber/
│       └── main.cpp     # Mã nguồn dành riêng cho Subscriber (COM16)
├── test/                # Thư mục kiểm thử đơn vị (Unit Tests)
├── platformio.ini       # Khai báo môi trường biên dịch, cổng COM và thư viện
└── .gitignore           # Danh sách loại trừ các file build nhị phân (.pio, .o, .bin)
```

## 4. Hướng dẫn cấu hình và triển khai

### Yêu cầu phần mềm
- Visual Studio Code kèm tiện ích mở rộng PlatformIO IDE.
- Mosquitto MQTT Broker (hoặc bất kỳ MQTT Broker nào chạy trên máy chủ/máy tính cá nhân).

### Bước 1: Thiết lập cấu hình mạng
Mở tệp `include/config.h` và cập nhật thông số mạng nội bộ của bạn:

```c
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"
#define MQTT_BROKER_IP  "192.168.1.21"    // IP máy tính chạy Mosquitto Broker
#define MQTT_PORT       1883
#define MQTT_TOPIC      "esp32/message"
```

### Bước 2: Nạp chương trình vào bo mạch

Dự án đã định nghĩa sẵn 2 môi trường trong `platformio.ini`:
- `publisher_node` tương ứng cổng **COM15**
- `subscriber_node` tương ứng cổng **COM16**

Nạp chương trình bằng giao diện dòng lệnh (CLI):

```bash
# Nạp code cho Publisher Node (COM15)
pio run -e publisher_node -t upload

# Nạp code cho Subscriber Node (COM16)
pio run -e subscriber_node -t upload
```

*(Hoặc sử dụng bảng Project Tasks của PlatformIO trên giao diện VS Code).*

### Bước 3: Kiểm thử và vận hành

Mở đồng thời hai cửa sổ giám sát Serial Monitor với tốc độ baud `115200`:

```bash
# Terminal 1 - Theo dõi Publisher
pio device monitor -p COM15 -b 115200

# Terminal 2 - Theo dõi Subscriber
pio device monitor -p COM16 -b 115200
```

1. Tại cửa sổ **COM15 (Publisher)**: Nhập một chuỗi ký tự bất kỳ và nhấn `Enter`. Terminal hiển thị:
   ```text
   [Sent] Hello ESP32
   ```
2. Tại cửa sổ **COM16 (Subscriber)**: Nhận dữ liệu tức thời từ Broker và hiển thị:
   ```text
   [Received] Hello ESP32
   ```
