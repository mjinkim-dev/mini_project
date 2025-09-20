#include <BleMouse.h>
// 24.06.02 am 12:24 수정본
// x축의 값이 0일때 클릭 이벤트가 일어나도록 하였다
// 전체적으로 디바운싱을 위한 딜레이 시간을 늘렸다
// 버튼 클릭 예민도를 높였다
// ESP32 업데이트로 인해 돌아온 BLEMouse.cpp 파일을 재수정하였다
// y축으로 스크롤링 되는 동안 좌클릭이 발생하지 않도록하였다
BleMouse bleMouse;
const int joystickX = 34; // Joystick X-axis pin
const int joystickY = 35; // Joystick Y-axis pin
const int swButtonPin = 32; // SW button pin

void setup() {
  Serial.begin(115200);
  pinMode(swButtonPin, INPUT_PULLUP); // Set SW button as input with pull-up resistor
  bleMouse.begin(); // Start BLE mouse
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
}

void loop() {
  if (bleMouse.isConnected()) {
    int xValue = analogRead(joystickX);
    int yValue = analogRead(joystickY);

    if (digitalRead(swButtonPin) == LOW) { // If SW button is pressed
      int deltaX = map(xValue, 0, 4095, -10, 10);
      int deltaY = map(yValue, 0, 4095, -10, 10);

      bleMouse.move(deltaX, deltaY, 0);
    } else {
      // 스크롤을 하기 위한 y축 조건 확인
      if (yValue <= 1800 || yValue >= 1950) {
        float normalizedY = ((float)yValue - 2048.0) / 2048.0;
        if (abs(normalizedY) > 0.05) {
          int scrollAmount = (int)(normalizedY * 10);
          bleMouse.move(0, 0, scrollAmount);
          delay(200);
        }
      } else {
        // y축 값이 스크롤 범위 내에 없을 때만 x축 값을 확인하여 클릭 이벤트 발생
        if (xValue <= 1500) {
          bleMouse.click(MOUSE_LEFT);
          delay(500); // Debounce delay
        }
      }
    }
  }

  delay(20); // General delay for loop responsiveness
}

