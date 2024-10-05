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