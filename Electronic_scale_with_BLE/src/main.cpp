#include <Arduino.h>
#include "BLE.h"
#include "HX711_Loadcell.h"
#include "LCD.h"
#include "button.h"

// Biến lưu giá trị cũ của cân.
int old_weight = 1;
void Show_Weight();

void setup() {
  Serial.begin(9600);                               
  Setup_Btn();
  Setup_LCD();
  Setup_BLE_Connect();
  Setup_HX711();
  Set_Timer_Interrupt();
}

void loop() {
  Disconnect_To_Web();
  Connect_To_Web();
  Show_Weight();
}

void Show_Weight(){
  Hold_Action();
  Calib_Action();
  int weight = HX711_Get_Value();
  if(weight != old_weight and is_print()){
    old_weight = weight;
    LCD_Print_Weight(weight);
    WEB_Print_Weight(weight);
  }
}

// Thêm các thư viện vào chương trình

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>

// #include "HX711.h" // thư viện cho module HX711
// #include <LiquidCrystal_I2C.h> // thư viện cho module I2C và LCD  

// // Thiết lập thành phần cho Bluetooth low energy
// BLEServer* pServer = NULL;
// BLECharacteristic* pWeight_Scale_Characteristic = NULL;
// BLECharacteristic* pButton_Characteristic = NULL;
// bool deviceConnected = false;
// bool oldDeviceConnected = false;

// // See the following for generating UUIDs:
// // https://www.uuidgenerator.net/
// #define SERVICE_UUID                      "19b10000-e8f2-537e-4f6c-d104768a1214"
// #define WEIGHT_SCALE_CHARACTERISTIC_UUID  "19b10001-e8f2-537e-4f6c-d104768a1214"
// #define BUTTON_CHARACTERISTIC_UUID        "19b10002-e8f2-537e-4f6c-d104768a1214"

// #define CALIB_COMMAND   0
// #define HOLD_COMMAND    1
// #define UNHOLD_COMMAND  2

// // Kết nối LCD I2C với ESP32
// // VCC        VIN	
// // GND	      GND
// // SCL	      D22
// // SDA	      D21

// hw_timer_t * timer = NULL;
// portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// // Thiết lập địa chỉ của I2C cho LCD là 0x27 với 16 cột và 2 hàng
// LiquidCrystal_I2C lcd(0x27,16,2);  

// // Biến lưu giá trị của cân.
// int old_weight = 1;

// // Thông số hiệu chuẩn cân
// float CALIBRATION_FACTOR = 389.9925;

// // Kết nối HX711 với ESP32
// #define LOADCELL_DOUT_PIN 16                     // Chân Data kết nối GPIO16
// #define LOADCELL_SCK_PIN 4                       // Chân CLK kết nối GPIO4   

// // nút nhấn
// #define BUTTON_CALIB_PIN 12
// #define BUTTON_HOLD_PIN 14
// #define BUTTON_UNHOLD_PIN 27

// bool calib_flag = 0;
// int hold_flag = 0;
// bool print_flag = 1;

// HX711 scale; // Đặt tên cho lớp HX711 là scale

// void Setup_HX711();
// void Setup_LCD();

// void Setup_BLE_Connect();

// void Setup_Btn();
// void Set_Timer_Interrupt();

// void Connect();
// void Disconnect();

// int HX711_Get_Value();
// void LCD_Print_Weight(int weight);
// void WEB_Print_Weight(int weight);
// void Show_Weight(); 

// void Calib_Action();
// void Hold_Action();
// void Unhold_Action();

// class MyServerCallbacks: public BLEServerCallbacks {
//   void onConnect(BLEServer* pServer) {
//     deviceConnected = true;
//   };

//   void onDisconnect(BLEServer* pServer) {
//     deviceConnected = false;
//   }
// };

// class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
//   void onWrite(BLECharacteristic* pButton_Characteristic) {
//     std::string value = pButton_Characteristic->getValue();
//     if(value.length() > 0) {
//       Serial.print("Characteristic event, written: ");
//       Serial.println(static_cast<int>(value[0])); // Print the integer value

//       int receivedValue = static_cast<int>(value[0]);
//       if(receivedValue == CALIB_COMMAND) {
//         Serial.println("HOLDING");
//         calib_flag = 1;
//       }else if(receivedValue == HOLD_COMMAND) {
//         hold_flag = 1;
//         Serial.println("HOLDING");
//       }else if(receivedValue == UNHOLD_COMMAND){
//         hold_flag = 0;
//         Serial.println("UNHOLD");
//       }
//     }
//   }
// };

// void IRAM_ATTR onTimer(){
//     // Đọc trạng thái của các nút nhấn với debounce
//   int currentButtonCalibState = digitalRead(BUTTON_CALIB_PIN);
//   int currentButtonHoldState = digitalRead(BUTTON_HOLD_PIN);
//   int currentButtonUnholdState = digitalRead(BUTTON_UNHOLD_PIN);
//   if(currentButtonCalibState == LOW) calib_flag = 1;
//   else if(currentButtonHoldState == LOW) hold_flag = 1;
//   else if(currentButtonUnholdState == LOW) hold_flag = 0;
// }

// void setup() {
//   Serial.begin(115200);                               // Thiết lập cho Serial Monitor
//   Setup_Btn();
//   Setup_LCD();
//   Setup_HX711();
//   Setup_BLE_Connect();
//   Set_Timer_Interrupt();
// }

// void loop() {
//   Disconnect();
//   Connect();
//   Show_Weight();
// }

// void Setup_Btn(){
//   pinMode(BUTTON_CALIB_PIN, INPUT_PULLUP);
//   pinMode(BUTTON_HOLD_PIN, INPUT_PULLUP);
//   pinMode(BUTTON_UNHOLD_PIN, INPUT_PULLUP);
// }

// void Set_Timer_Interrupt(){
//   // khởi tạo timer với chu kì 1us vì thạch anh của ESP chạy 80MHz
//   timer = timerBegin(0,80,true);
//   //khởi tạo hàm xử lý ngắt ngắt cho Timer
//   timerAttachInterrupt(timer, &onTimer, true);
//   //khởi tạo thời gian ngắt cho timer là 1s (1000000 us)
//   timerAlarmWrite(timer, 1000, true);
//   //bắt đầu chạy timer
//   timerAlarmEnable(timer);
// }

// void Setup_LCD(){
//   lcd.clear();
//   lcd.init();
//   lcd.noBacklight();
//   delay(1000);
//   lcd.backlight();      // Make sure backlight is on
//   lcd.home();

// }

// void Setup_HX711(){
//   delay(1000);
//   lcd.setCursor(4,0);
//   lcd.print("Setup...");
//   delay(2000);
  
//   Serial.println();
//   Serial.println("Do not place any object or weight on the scale.");
//   lcd.setCursor(0,0);
//   lcd.print("Do not place any");
//   delay(1000);
//   lcd.setCursor(0,1);
//   lcd.print("object or weight");
//   delay(1000);
//   lcd.clear();
//   lcd.setCursor(2,0);
//   lcd.print("on the scale");
//   delay(2000);

//   // lcd.clear();
//   lcd.setCursor(1,0);
//   lcd.print("Initializing...");
//   scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);   // Khởi tạo cảm biến lực hoạt động 
//   delay(3000);  

//   lcd.clear();
//   lcd.setCursor(2,0);
//   lcd.print("Calibrating...");

//   scale.set_scale(CALIBRATION_FACTOR); //thêm thông số hiệu chuẩn cân
//   scale.tare(); //Reset the scale to 0
//   delay(2000);
//   lcd.clear();
// }

// void Setup_BLE_Connect(){
//    // Create the BLE Device
//   BLEDevice::init("ESP32");

//   // Create the BLE Server
//   pServer = BLEDevice::createServer();
//   pServer->setCallbacks(new MyServerCallbacks());

//   // Create the BLE Service
//   BLEService *pService = pServer->createService(SERVICE_UUID);

//   // Create a BLE Characteristic
//   pWeight_Scale_Characteristic = pService->createCharacteristic(
//                       WEIGHT_SCALE_CHARACTERISTIC_UUID,
//                       BLECharacteristic::PROPERTY_READ   |
//                       BLECharacteristic::PROPERTY_WRITE  |
//                       BLECharacteristic::PROPERTY_NOTIFY |
//                       BLECharacteristic::PROPERTY_INDICATE
//                     );

//   // Create the button Characteristic
//   pButton_Characteristic = pService->createCharacteristic(
//                       BUTTON_CHARACTERISTIC_UUID,
//                       BLECharacteristic::PROPERTY_WRITE
//                     );

//   // Register the callback for the ON button characteristic
//   pButton_Characteristic->setCallbacks(new MyCharacteristicCallbacks());

//   // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
//   // Create a BLE Descriptor
//   pWeight_Scale_Characteristic->addDescriptor(new BLE2902());
//   pButton_Characteristic->addDescriptor(new BLE2902());

//   // Start the service
//   pService->start();

//   // Start advertising
//   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//   pAdvertising->addServiceUUID(SERVICE_UUID);
//   pAdvertising->setScanResponse(false);
//   pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
//   BLEDevice::startAdvertising();
//   Serial.println("Waiting a client connection to notify...");
// }

// int HX711_Get_Value(){
//   if (scale.wait_ready_timeout(100)){
//     // Serial.println(scale.get_units());
//     return round(scale.get_units());
//   }
//   else {
//     Serial.println("HX711 not found.");
//   }
// }

// void clearRow(byte rowToClear)
// {
//   lcd.setCursor(0, rowToClear);
//   lcd.print("                ");  
// }

// void Show_Weight(){
//   Hold_Action();
//   Calib_Action();
//   int weight = HX711_Get_Value();
//   if(weight != old_weight and print_flag == 1){
//     old_weight = weight;
//     LCD_Print_Weight(weight);
//     WEB_Print_Weight(weight);
//   }
// }

// void LCD_Print_Weight(int weight)
// {
//   clearRow(1);
//   lcd.setCursor(1, 0);
//   lcd.print("Weight Scale:"); 

//   if((weight >= 1000 && weight <= 5000) or (weight >= -5000 && weight <= -1000)){
//     lcd.setCursor(4,1);
//     lcd.print(weight / 1000);
//     lcd.setCursor(5,1);
//     lcd.print(".");
//     lcd.setCursor(6,1);

//     if(weight % 1000 < 10){
//       lcd.print("00");
//       lcd.setCursor(8,1);
//     }else if(weight % 1000 < 100){
//       lcd.print("0");
//       lcd.setCursor(7,1);
//     }
//     lcd.print(weight % 1000);
//     lcd.setCursor(11,1);
//     lcd.print("kg");
//   }
//   else if((weight >= 0 && weight <= 9) or (weight >= -9 && weight <= 0)){
//     lcd.setCursor(7,1);
//     lcd.print(weight);
//   }else if((weight >= 10 && weight <= 99) or (weight > -99 && weight <= -10)){
//     lcd.setCursor(6,1);
//     lcd.print(weight);
//   }else{
//     lcd.setCursor(5,1);
//     lcd.print(weight);
//   }
//   if((weight >= 0 && weight <= 999) or (weight >= -999 && weight <= 0)){
//     lcd.setCursor(10,1);
//     lcd.print("g");
//   } 
//   Serial.print("Weight: ");
//   Serial.print(weight);
//   Serial.println(" gram");
// }


// void Connect()
// {
//   // connecting
//   if (deviceConnected && !oldDeviceConnected) {
//     // do stuff here on connecting
//     oldDeviceConnected = deviceConnected;
//     Serial.println("Device Connected");
//   }
// }

// void Disconnect(){
//    // disconnecting
//   if (!deviceConnected && oldDeviceConnected) {
//     Serial.println("Device disconnected.");
//     delay(500); // give the bluetooth stack the chance to get things ready
//     pServer->startAdvertising(); // restart advertising
//     Serial.println("Start advertising");
//     oldDeviceConnected = deviceConnected;
//   }
// }

// void WEB_Print_Weight(int weight){
//   // notify changed value
//   if (deviceConnected) {
//     pWeight_Scale_Characteristic->setValue(String(weight).c_str());
//     pWeight_Scale_Characteristic->notify();
//     Serial.print("New value notified: ");
//     Serial.println(weight);
//     // delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
//   }
// }

// void Calib_Action(){
//   if(calib_flag){
//     calib_flag = 0;
//     scale.tare();
//   }
// }

// void Hold_Action(){
//   if(hold_flag)
//     print_flag = 0;
//   else print_flag = 1;
// }
