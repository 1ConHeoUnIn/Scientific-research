#include "WiFiConfig.h"

extern float target_x;
extern float target_y;

WiFiConfig::WiFiConfig() : server(80) {}

void WiFiConfig::begin() {
    WiFi.softAP("ESP32_Config");  // ESP32 tự phát WiFi với SSID `ESP32_Config`
    Serial.println("\n⚙️ Chế độ AP đang chạy!");
    Serial.print("📡 ESP32 đang phát WiFi tại IP: ");
    Serial.println(WiFi.softAPIP());

    setupRoutes();
    server.begin();
}

void WiFiConfig::setupRoutes() {
    // Trang nhập tọa độ mục tiêu
    server.on("/target", [this]() {
        String html = "<!DOCTYPE html><html lang='vi'>";
        html += "<head><meta charset='UTF-8'>";
        html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        html += "<title>Nhập Tọa độ Mục tiêu</title>";
        html += "<style>body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }</style></head>";
        html += "<body><h1>Nhập Tọa độ Mục tiêu</h1>";
        html += "<form id='targetForm'>";
        html += "<label>Tọa độ X:</label><br><input type='text' id='target_x'><br>";
        html += "<label>Tọa độ Y:</label><br><input type='text' id='target_y'><br>";
        html += "<button type='button' onclick='sendTarget()'>Cập nhật</button></form>";
        html += "<p id='status'></p>";
        html += "<script>";
        html += "function sendTarget() {";
        html += "let data = new FormData();";
        html += "data.append('target_x', document.getElementById('target_x').value);";
        html += "data.append('target_y', document.getElementById('target_y').value);";
        html += "fetch('/updateTarget', { method: 'POST', body: data })";
        html += ".then(res => res.text()).then(msg => document.getElementById('status').innerText = msg);";
        html += "}";
        html += "</script>";
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    // Xử lý cập nhật tọa độ (giữ nguyên trang)
    server.on("/updateTarget", HTTP_POST, [this]() {
        if (server.hasArg("target_x") && server.hasArg("target_y")) {
            target_x = server.arg("target_x").toFloat();
            target_y = server.arg("target_y").toFloat();

            // In ra Serial để kiểm tra
            Serial.print("📡 Dữ liệu nhận được - X: ");
            Serial.print(target_x);
            Serial.print(", Y: ");
            Serial.println(target_y);

            server.send(200, "text/plain", "✅ Đã lưu tọa độ mới: X = " + String(target_x) + ", Y = " + String(target_y));
        } else {
            server.send(400, "text/plain", "❌ Lỗi! Vui lòng nhập tọa độ hợp lệ.");
        }
    });

    // Trả về trạng thái tọa độ hiện tại
    server.on("/status", [this]() {
        server.send(200, "text/plain", "🎯 Tọa độ hiện tại: X = " + String(target_x) + ", Y = " + String(target_y));
    });

    // Xử lý lỗi nếu người dùng nhập sai đường dẫn
    server.onNotFound([this]() {
        server.send(404, "text/plain", "❌ Đường dẫn không tồn tại! Hãy truy cập /target.");
    });
}



void WiFiConfig::handleClient() {
    server.handleClient();
}

bool WiFiConfig::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiConfig::getLocalIP() {
    return WiFi.softAPIP().toString();  // Trả về địa chỉ IP của ESP32 trong chế độ AP
}
