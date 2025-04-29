# Thiết lập Chân IO ESP32 🚀

## 1️⃣ Giới thiệu
ESP32 là một vi điều khiển mạnh mẽ với nhiều chân IO có thể sử dụng cho điều khiển động cơ, giao tiếp cảm biến, và hiển thị dữ liệu. Dưới đây là hướng dẫn thiết lập các chân IO theo dự án của bạn.

---

## 2️⃣ Sơ đồ kết nối
| Chức năng          | Chân ESP32 |
|--------------------|-----------|
| **Động cơ trái tiến**  | GPIO 16 |
| **Động cơ trái lùi**   | GPIO 17 |
| **Động cơ phải tiến**  | GPIO 25 |
| **Động cơ phải lùi**   | GPIO 26 |
| **OLED SDA**      | GPIO 21 |
| **OLED SCL**      | GPIO 22 |
| **La bàn SDA**    | GPIO 21 |
| **La bàn SCL**    | GPIO 22 |

📝 **Lưu ý:**  
- **Các chân GPIO của ESP32 có thể thay đổi** tùy theo mô-đun phần cứng bạn sử dụng.  
- **Chân SDA/SCL** có thể khác nếu bạn dùng một module I2C khác.

---

## 3️⃣ Cách thiết lập trong mã nguồn
Bạn cần thiết lập các chân IO trong `setup()` của `main.cpp` như sau:

```cpp
void setup() {
    Serial.begin(115200);

    // Khởi tạo các chân điều khiển động cơ
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);

    // Khởi tạo giao tiếp I2C cho cảm biến la bàn và OLED
    Wire.begin(21, 22);

    Serial.println("ESP32 đã được thiết lập!");
}
