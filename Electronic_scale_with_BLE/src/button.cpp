#include <Arduino.h>
#include "HX711_Loadcell.h"

// nút nhấn
#define BUTTON_CALIB_PIN 12
#define BUTTON_HOLD_PIN 14
#define BUTTON_UNHOLD_PIN 27

bool calib_flag = 0;
bool hold_flag = 0;
bool print_flag = 1;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void Setup_Btn(){
  pinMode(BUTTON_CALIB_PIN, INPUT_PULLUP);
  pinMode(BUTTON_HOLD_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UNHOLD_PIN, INPUT_PULLUP);
}

void IRAM_ATTR onTimer(){
    portENTER_CRITICAL_ISR(&timerMux); //vào chế độ tránh xung đột

    // Đọc trạng thái của các nút nhấn
    int currentButtonCalibState = digitalRead(BUTTON_CALIB_PIN);
    int currentButtonHoldState = digitalRead(BUTTON_HOLD_PIN);
    int currentButtonUnholdState = digitalRead(BUTTON_UNHOLD_PIN);
    if(currentButtonCalibState == LOW) calib_flag = 1;
    else if(currentButtonHoldState == LOW) hold_flag = 1;
    else if(currentButtonUnholdState == LOW) hold_flag = 0;

    portEXIT_CRITICAL_ISR(&timerMux); // thoát 
}

void Set_Timer_Interrupt(){
  // khởi tạo timer với chu kì 1us vì thạch anh của ESP chạy 80MHz
  timer = timerBegin(0,80,true);
  //khởi tạo hàm xử lý ngắt ngắt cho Timer
  timerAttachInterrupt(timer, &onTimer, true);
  //khởi tạo thời gian ngắt cho timer là 1ms (1000 us)
  timerAlarmWrite(timer, 1000, true);
  //bắt đầu chạy timer
  timerAlarmEnable(timer);
}

void Calib_Action(){
  if(calib_flag){
    calib_flag = 0;
    Calibrating();
  }
}

void Hold_Action(){
  if(hold_flag) print_flag = 0;
  else print_flag = 1;
}

bool is_print()
{
  return print_flag;
}

void Change_Calib_Flag()
{
  calib_flag = 1;
}

void Change_Hold_Flag()
{
  if(hold_flag == 0) hold_flag = 1;
  else hold_flag = 0;
}