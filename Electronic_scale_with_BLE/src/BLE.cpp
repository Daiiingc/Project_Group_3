#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "button.h"

#define SERVICE_UUID                      "19b10000-e8f2-537e-4f6c-d104768a1214"
#define WEIGHT_SCALE_CHARACTERISTIC_UUID  "19b10001-e8f2-537e-4f6c-d104768a1214"
#define BUTTON_CHARACTERISTIC_UUID        "19b10002-e8f2-537e-4f6c-d104768a1214"

#define CALIB_COMMAND   0
#define HOLD_COMMAND    1
#define UNHOLD_COMMAND  2

// Thiết lập thành phần cho Bluetooth low energy
BLEServer* pServer = NULL;
BLECharacteristic* pWeight_Scale_Characteristic = NULL;
BLECharacteristic* pButton_Characteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pButton_Characteristic) {
    std::string value = pButton_Characteristic->getValue();
    if(value.length() > 0) {
      Serial.print("Characteristic event, written: ");
      int receivedValue = static_cast<int>(value[0]);
      Serial.println(receivedValue); 
      if(receivedValue == CALIB_COMMAND) {
        Serial.println("HOLDING");
        Change_Calib_Flag();
      }else if(receivedValue == HOLD_COMMAND) {
        Change_Hold_Flag();
        Serial.println("HOLDING");
      }else if(receivedValue == UNHOLD_COMMAND){
        Change_Hold_Flag();
        Serial.println("UNHOLD");
      }
    }
  }
};

void Setup_BLE_Connect(){
   // Đặt tên cho kết nối BLE
  BLEDevice::init("ESP32");

  // Thiết lập máy chủ cho kết nối BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Thiết lập BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Thiết lập Weight Scale Characteristic
  pWeight_Scale_Characteristic = pService->createCharacteristic(
                      WEIGHT_SCALE_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // Thiết lập button Characteristic
  pButton_Characteristic = pService->createCharacteristic(
                      BUTTON_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  // Thiết lập tính năng gửi lại của button characteristic
  pButton_Characteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Thiết lập mô tả cho BLE 
  pWeight_Scale_Characteristic->addDescriptor(new BLE2902());
  pButton_Characteristic->addDescriptor(new BLE2902());

  // Bắt đầu dịch vụ
  pService->start();

  // Bắt đầu quảng bá
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  
}

void Connect_To_Web()
{
  // Kết nối
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
    Serial.println("Device Connected");
  }
}

void Disconnect_To_Web(){
   // Ngắt kết nối
  if (!deviceConnected && oldDeviceConnected) {
    Serial.println("Device disconnected.");
    delay(500);
    pServer->startAdvertising(); // Quảng bá lại
    Serial.println("Start advertising");
    oldDeviceConnected = deviceConnected;
  }
}

void WEB_Print_Weight(int weight){
  // Thông báo có sự thay đổi dữ liệu
  if (deviceConnected) {
    pWeight_Scale_Characteristic->setValue(String(weight).c_str());
    pWeight_Scale_Characteristic->notify();
    Serial.print("New value notified: ");
    Serial.println(weight);
  }
}