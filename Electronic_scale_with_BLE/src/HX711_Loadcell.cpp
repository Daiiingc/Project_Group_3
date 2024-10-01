#include <Arduino.h>
#include <HX711.h>
#include "LCD.h"

// Kết nối HX711 với ESP32
#define LOADCELL_DOUT_PIN 16                     // Chân Data kết nối GPIO16
#define LOADCELL_SCK_PIN 4                       // Chân CLK kết nối GPIO4  

HX711 scale; // Đặt tên cho lớp HX711 là scale 

// Thông số hiệu chuẩn cân
float CALIBRATION_FACTOR = 389.9925;

void Setup_HX711(){
  LCD_Print_Setup();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);   	// Khởi tạo cảm biến lực hoạt động 
  scale.set_scale(CALIBRATION_FACTOR); 					//thêm thông số hiệu chuẩn cân
  scale.tare(); 										//Hiệu chuẩn cân về 0
  delay(2000);
  clearRow(0);
  clearRow(1);
}

int HX711_Get_Value(){
  if (scale.wait_ready_timeout(100)){
    return scale.get_units();
  }
  else {
    Serial.println("HX711 not found.");
    return 0;
  }
}

void Calibrating(){
  scale.tare();
}