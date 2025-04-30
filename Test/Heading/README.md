Thiết lập Chân IO ESP32 🚀
1️⃣ Giới thiệu
ESP32 là một vi điều khiển mạnh mẽ với nhiều chân IO có thể sử dụng để điều khiển động cơ, giao tiếp cảm biến, và hiển thị dữ liệu. Dưới đây là hướng dẫn chi tiết về cách thiết lập các chân GPIO theo đúng dự án của bạn.

2️⃣ Sơ đồ kết nối
Chức năng	Chân ESP32
Động cơ trái tiến	GPIO 16
Động cơ trái lùi	GPIO 17
Động cơ phải tiến	GPIO 25
Động cơ phải lùi	GPIO 26
OLED SDA	GPIO 21
OLED SCL	GPIO 22
La bàn SDA	GPIO 21
La bàn SCL	GPIO 22
📝 Lưu ý:

Các chân GPIO của ESP32 có thể thay đổi tùy theo mô-đun phần cứng bạn sử dụng.

Chân SDA/SCL có thể khác nếu bạn dùng một module I2C khác.

Nếu có xung đột chân giữa OLED và cảm biến la bàn, hãy kiểm tra cách kết nối I2C.

3️⃣ Cách thiết lập trong mã nguồn
Bạn cần thiết lập các chân IO trong setup() của main.cpp như sau:

cpp
void setup() {
    Serial.begin(115200);

    // Khởi tạo các chân điều khiển động cơ
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(26, OUTPUT);

    // Khởi tạo giao tiếp I2C cho cảm biến la bàn và OLED
    Wire.begin(21, 22);

    Serial.println("🚀 ESP32 đã được thiết lập!");
}
4️⃣ Cách nhập tọa độ mới
Bạn có thể nhập tọa độ mục tiêu trực tiếp từ trình duyệt bằng cách truy cập vào địa chỉ:

🖥️ http://192.168.4.1/target

Tại đây, bạn có thể nhập tọa độ X và Y cho robot, sau đó nhấn "Cập nhật" để gửi dữ liệu đến ESP32.

5️⃣ Mở rộng - Kiểm tra trạng thái GPIO
Bạn có thể kiểm tra trạng thái các chân GPIO bằng cách đọc dữ liệu từ cảm biến hoặc sử dụng lệnh digitalRead() nếu cần:

cpp
void loop() {
    Serial.print("Trạng thái động cơ trái tiến: ");
    Serial.println(digitalRead(16));
    delay(500);
}
📌 Mẹo:

Nếu động cơ không hoạt động đúng, hãy kiểm tra các chân nối đất (GND) và nguồn cấp (VCC).

Sử dụng Serial.print() để kiểm tra tín hiệu GPIO trên Serial Monitor.

6️⃣ Mô tả chức năng các tệp h và cpp
🔹 OledDisplay.h / OledDisplay.cpp
📌 Chức năng: Hiển thị thông tin lên màn hình OLED về góc la bàn, khoảng cách đến mục tiêu và hướng mong muốn.

🔹 Compass.h / Compass.cpp
📌 Chức năng: Đọc dữ liệu từ cảm biến la bàn để xác định góc di chuyển của robot.

🔹 MotorControl.h / MotorControl.cpp
📌 Chức năng: Điều khiển động cơ để di chuyển robot theo hướng mong muốn.

🔹 Navigation.h / Navigation.cpp
📌 Chức năng: Tính toán hướng di chuyển đến mục tiêu, xác định sai số góc.

🔹 StepTracker.h / StepTracker.cpp
📌 Chức năng: Đếm số bước chân bằng cảm biến gia tốc để xác định khoảng cách di chuyển.

🔹 WiFiConfig.h / WiFiConfig.cpp
📌 Chức năng: Thiết lập ESP32 như một web server để nhập tọa độ mục tiêu từ trình duyệt.

🔹 main.cpp
📌 Chức năng: Tích hợp tất cả thành phần trên để điều khiển robot.

🚀 Quy trình hoạt động 1️⃣ Khởi động hệ thống 2️⃣ Kiểm tra tọa độ nhập từ web 3️⃣ Tính toán góc di chuyển và sai số 4️⃣ Điều khiển động cơ theo hướng mong muốn 5️⃣ Kiểm tra trạng thái đạt mục tiêu 6️⃣ Nếu đạt mục tiêu, reset tọa độ và gửi (0,0) lên web server

7️⃣ Tổng kết
✅ Thiết lập đúng chân GPIO theo sơ đồ phần cứng. ✅ Cấu hình giao tiếp I2C cho OLED và la bàn trên cùng đường dữ liệu. ✅ Truy cập http://192.168.4.1/target để nhập tọa độ mới. ✅ Kiểm tra trạng thái GPIO để đảm bảo hoạt động đúng. ✅ Tự động reset tọa độ sau khi hoàn thành nhiệm vụ.

🚀