# ESP32 BLE Remote Controller (Joystick Mouse)

This project implements a wireless BLE remote using an ESP32 and a custom 3D-printed frame.  

## Project Overview
- Goal: Create a one-hand remote to reduce wrist strain from smartphone use.
- Functionality:
  - Joystick input to control smartphone cursor
  - X-axis for click
  - Y-axis for scroll
  - BLE communication with ESP32

## Hardware and Design


- MCU: ESP32 WROOM 32 (BLE enabled)
- Joystick: Arduino PS2 Joystick Module
- Frame: Custom cylindrical 3D-printed case (using Fusion)
- Power: USB-C input

Design Iterations:
- Initial design had issues with jumper wire space and USB port fit
- Revised design added cover protection and improved grip


## Demo
[![Demo Video](https://img.youtube.com/vi/v4lBZTQJYww/0.jpg)](https://youtu.be/v4lBZTQJYww)

## Next Steps
- Add power management with battery support
- Improve BLE stability
- Update firmware for compatibility with the latest Android versions

---

## 한국어 설명

이 프로젝트는 ESP32와 3D 프린터로 제작한 BLE 리모컨(조이스틱 마우스)입니다.  

### 프로젝트 개요
- 목표: 장시간 스마트폰 사용으로 인한 손목 피로를 줄이기 위한 조작용 리모컨 제작
- 기능:
  - 조이스틱 입력으로 스마트폰 커서 제어
  - X축 → 클릭
  - Y축 → 스크롤
  - ESP32 BLE 통신을 통한 무선 연결

### 하드웨어 및 디자인
- MCU: ESP32 WROOM 32  
- 조이스틱: 아두이노 PS2 조이스틱 모듈
- 프레임: Fusion 기반 원통형 3D 출력  
- 전원: USB-C 입력  

설계 과정:  
- 초기 설계는 점퍼선 및 USB 포트 공간 부족 문제가 있었음  
- 수정 설계에서는 뚜껑 보호 및 그립감 개선 추가  


### 시연
[![시연 영상](https://img.youtube.com/vi/v4lBZTQJYww/0.jpg)](https://youtu.be/v4lBZTQJYww)


### 차후 개선
- 배터리 전원 모듈 추가  
- BLE 통신 안정성 개선  
- 최신 안드로이드 버전과 호환되도록 펌웨어 업데이트   
