#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SimpleKalmanFilter.h>

Adafruit_MPU6050 mpu;
SimpleKalmanFilter kalmanAccelX(2, 2, 0.01);
SimpleKalmanFilter kalmanVelX(2, 2, 0.01);

// Biến hiệu chỉnh gia tốc ban đầu
float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
const int numSamples = 100; // Số lần lấy mẫu để hiệu chỉnh

// Biến đo vận tốc và khoảng cách
float velocityX = 0, distanceX = 0;
unsigned long lastTime, motionStartTime = 0, stopTime = 0;
bool isMoving = false, hasStopped = false;

void calibrateMPU6050() {
    float sumX = 0, sumY = 0, sumZ = 0;

    Serial.println("🔄 Hiệu chỉnh MPU6050... Hãy giữ cảm biến đứng yên!");
    for (int i = 0; i < numSamples; i++) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        sumX += a.acceleration.x;
        sumY += a.acceleration.y;
        sumZ += a.acceleration.z;
        delay(50);
    }

    accelOffsetX = sumX / numSamples;
    accelOffsetY = sumY / numSamples;
    accelOffsetZ = sumZ / numSamples;

    Serial.println("✅ Hiệu chỉnh hoàn tất!");
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!mpu.begin()) {
        Serial.println("❌ Không tìm thấy MPU6050! Kiểm tra kết nối.");
        while (1);
    }

    Serial.println("✅ MPU6050 đã sẵn sàng!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    calibrateMPU6050();

    lastTime = millis();
}

void loop() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float adjX = abs(a.acceleration.x - accelOffsetX);
    float adjY = abs(a.acceleration.y - accelOffsetY);
    float adjZ = abs(a.acceleration.z - accelOffsetZ);

    unsigned long currentTime = millis();

    if (adjX > 0.3 || adjY > 0.3 || adjZ > 0.3) {
        if (!isMoving) {
            motionStartTime = currentTime;
            isMoving = true;
            hasStopped = false;
        }

        if (currentTime - motionStartTime > 1000) {
            float accelX = a.acceleration.x * 9.81;
            accelX = kalmanAccelX.updateEstimate(accelX);

            float dt = (currentTime - lastTime) / 1000.0;
            velocityX += accelX * dt;
            velocityX = kalmanVelX.updateEstimate(velocityX);
            distanceX += velocityX * dt;

            Serial.print("⏩ Vận tốc X (m/s): "); Serial.print(velocityX);
            Serial.print(" | Khoảng cách X (m): "); Serial.println(distanceX);
        }
    } else if (isMoving && (currentTime - stopTime > 500)) {
        Serial.print("📏 Tổng quãng đường đã đi: "); Serial.println(distanceX);
        velocityX = 0;
        hasStopped = true;
    }

    lastTime = currentTime;
    delay(100);
}
