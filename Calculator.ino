#include <Keypad.h>
#include "ExprCalc.h"
#include "Modes.h"

LiquidCrystal_I2C lcd(0x27,16,2);    

void setup(){
    Serial.begin(9600);
    pinMode(shiftPin, INPUT_PULLUP);
    
    lcd.init();
    lcd.backlight();
    lcd.print(F("Arduino"));
    lcd.setCursor(0, 1);
    lcd.print(F("Simple Calc"));
    delay(1500);
    lcd.clear();
    lcd.print(F("NES  HCMUS  2020"));
    lcd.setCursor(0, 1);
    lcd.print(F("Ngoc Phat  K19IT"));
    delay(500);
}

// main func
void loop(){
    // Default: menu mode
    menuMode();

    // The program shouldn't exit to here. If so, probably there was a bug
    lcd.clear();
    lcd.print(F("An error occured"));
    lcd.setCursor(0, 1);
    lcd.print(F("Please RESET"));
}
