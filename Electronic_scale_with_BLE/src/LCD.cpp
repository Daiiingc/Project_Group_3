#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // thư viện cho module I2C và LCD  

/*
Kết nối LCD I2C với ESP32
VCC       VIN	
GND	      GND
SCL	      D22
SDA	      D21
*/  

// Thiết lập địa chỉ của I2C cho LCD là 0x27 với 16 cột và 2 hàng
LiquidCrystal_I2C lcd(0x27,16,2);  

void Setup_LCD(){
  lcd.clear();
  lcd.init();
  lcd.noBacklight();
  delay(1000);
  lcd.backlight();      // Make sure backlight is on
  lcd.home();

}

void clearRow(int rowToClear)
{
  lcd.setCursor(0, rowToClear);
  lcd.print("                ");  
}

void LCD_Print_Weight(int weight)
{
  clearRow(1);
  lcd.setCursor(1, 0);
  lcd.print("Weight Scale:"); 

  if((weight >= 1000 && weight <= 5000) or (weight >= -5000 && weight <= -1000)){
    lcd.setCursor(4,1);
    lcd.print(weight / 1000);
    lcd.setCursor(5,1);
    lcd.print(".");
    lcd.setCursor(6,1);

    if(weight % 1000 < 10){
      lcd.print("00");
      lcd.setCursor(8,1);
    }else if(weight % 1000 < 100){
      lcd.print("0");
      lcd.setCursor(7,1);
    }
    lcd.print(weight % 1000);
    lcd.setCursor(11,1);
    lcd.print("kg");
  }
  else if((weight >= 0 && weight <= 9) or (weight >= -9 && weight <= 0)){
    lcd.setCursor(7,1);
    lcd.print(weight);
  }else if((weight >= 10 && weight <= 99) or (weight > -99 && weight <= -10)){
    lcd.setCursor(6,1);
    lcd.print(weight);
  }else{
    lcd.setCursor(5,1);
    lcd.print(weight);
  }
  if((weight >= 0 && weight <= 999) or (weight >= -999 && weight <= 0)){
    lcd.setCursor(10,1);
    lcd.print("g");
  } 

}



void LCD_Print_Setup()
{
  delay(1000);
  lcd.setCursor(4,0);
  lcd.print("Setup...");
  
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Do not place any");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("object or weight");
  delay(1000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("on the scale");
  delay(2000);

  lcd.setCursor(1,0);
  lcd.print("Initializing...");
  delay(3000);
  lcd.setCursor(0,0);
  lcd.print("Waiting a client");
  delay(1000);
  lcd.setCursor(1,1);
  lcd.print("connection to");
  delay(1000);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("notify");
  delay(2000);

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Calibrating...");
}