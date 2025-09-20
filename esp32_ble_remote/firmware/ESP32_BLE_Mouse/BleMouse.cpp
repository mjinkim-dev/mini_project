#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
#include <driver/adc.h>
#include "sdkconfig.h"

#include "BleConnectionStatus.h"
#include "BleMouse.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define LOG_TAG ""
#else
#include "esp_log.h"
static const char* LOG_TAG = "BLEDevice";
#endif

static const uint8_t _hidReportDescriptor[] = {
    // HID Report Descriptor 내용 생략
};

BleMouse::BleMouse(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) :
    _buttons(0),
    hid(0)
{
    this->deviceName = deviceName;
    this->deviceManufacturer = deviceManufacturer;
    this->batteryLevel = batteryLevel;
    this->connectionStatus = new BleConnectionStatus();
}

void BleMouse::begin(void)
{
    xTaskCreate(this->taskServer, "server", 20000, (void*)this, 5, NULL);
}

void BleMouse::end(void)
{
}

void BleMouse::click(uint8_t b)
{
    _buttons = b;
    move(0, 0, 0, 0);
    _buttons = 0;
    move(0, 0, 0, 0);
}

void BleMouse::move(signed char x, signed char y, signed char wheel, signed char hWheel)
{
    if (this->isConnected())
    {
        uint8_t m[5];
        m[0] = _buttons;
        m[1] = x;
        m[2] = y;
        m[3] = wheel;
        m[4] = hWheel;
        this->inputMouse->setValue(m, 5);
        this->inputMouse->notify();
    }
}

void BleMouse::buttons(uint8_t b)
{
    if (b != _buttons)
    {
        _buttons = b;
        move(0, 0, 0, 0);
    }
}

void BleMouse::press(uint8_t b)
{
    buttons(_buttons | b);
}

void BleMouse::release(uint8_t b)
{
    buttons(_buttons & ~b);
}

bool BleMouse::isPressed(uint8_t b)
{
    return (b & _buttons) > 0;
}

bool BleMouse::isConnected(void)
{
    return this->connectionStatus->connected;
}

void BleMouse::setBatteryLevel(uint8_t level)
{
    this->batteryLevel = level;
    if (hid != 0)
    {
        this->hid->setBatteryLevel(this->batteryLevel);
    }
}

void BleMouse::scroll(int8_t scroll)
{
    uint8_t report[3] = { 0x00, 0x00, scroll }; // No X/Y movement, only scroll

    // Assuming this->inputMouse is the HID device instance and characteristic for mouse input
    this->inputMouse->setValue(report, sizeof(report));
    this->inputMouse->notify();
}

void BleMouse::taskServer(void* pvParameter)
{
    BleMouse* bleMouseInstance = (BleMouse*)pvParameter;

    // std::string을 String으로 변환
    String deviceName = String(bleMouseInstance->deviceName.c_str());
    String deviceManufacturer = String(bleMouseInstance->deviceManufacturer.c_str());

    BLEDevice::init(deviceName);
    BLEServer* pServer = BLEDevice::createServer();
    pServer->setCallbacks(bleMouseInstance->connectionStatus);

    bleMouseInstance->hid = new BLEHIDDevice(pServer);
    bleMouseInstance->inputMouse = bleMouseInstance->hid->inputReport(0); // <-- input REPORTID from report map
    bleMouseInstance->connectionStatus->inputMouse = bleMouseInstance->inputMouse;

    bleMouseInstance->hid->manufacturer()->setValue(deviceManufacturer);

    bleMouseInstance->hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    bleMouseInstance->hid->hidInfo(0x00, 0x02);

    BLESecurity* pSecurity = new BLESecurity();

    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    bleMouseInstance->hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
    bleMouseInstance->hid->startServices();

    bleMouseInstance->onStarted(pServer);

    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->addServiceUUID(bleMouseInstance->hid->hidService()->getUUID());
    pAdvertising->start();
    bleMouseInstance->hid->setBatteryLevel(bleMouseInstance->batteryLevel);

    ESP_LOGD(LOG_TAG, "Advertising started!");
    vTaskDelay(portMAX_DELAY); //delay(portMAX_DELAY);
}
