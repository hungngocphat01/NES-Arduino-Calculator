#include "Input.h"

// Function prototypes
bool modeKeyCheck (String key); 
void eqnMode();
void compMode ();
void menuMode();
void aboutMode();

int currentDisplayingChars = 0;
int scrollIndex = 0;

void menuMode() {
    bool printMenu = true; // to fix flickering
    
    String key;
    while(true) {
        key = getKeyStr();
        if (printMenu) {
            lcd.clear(); lcd.noBlink();
            lcd.print("1.COMP     2.EQN");
            lcd.setCursor(0, 1);
            lcd.print("3.INTGRL 4.ABOUT");
            printMenu = false;
        }
        
        if (key == "1") {
            compMode();
            printMenu = true;
        }
        else if (key == "2") {
            eqnMode();  
            printMenu = true;
        }
        else if (key == "4") {
            aboutMode();
            printMenu = true;
        }
        
    }
}

void aboutMode() {
    while (true) {
        lcd.clear();
        lcd.print(F("NES  HCMUS  2020"));
        lcd.setCursor(0, 1);
        lcd.print(F("Ngoc Phat  K19IT"));
        delay(4000);
        lcd.clear();
        lcd.print(F("Source code"));
        lcd.setCursor(0, 1);
        lcd.print(F("link:"));
        delay(2000);
        lcd.clear();
        lcd.print(F("github/phathung2001/arduino-calculator"));
        delay(1000);
        for (int i = 0; i < 26; i++) {
            lcd.scrollDisplayLeft();
            delay(500);
        }
        lcd.clear();
        lcd.print(F("RESET to quit"));
        delay(2000);
    }
}

void printSolution(String msg, float value) {
    lcd.clear();
    lcd.print(msg);
    lcd.setCursor(0, 1);
    lcd.print(value, 6);

    pressAnyKey();
    lcd.clear();
}

void eqnMode() {
    Serial.println(F("EQN mode choosen"));
    lcd.noBlink();
    String key;
        
    lcd.clear();
    lcd.print(F("1.ax^2+bx+c=0"));
    lcd.setCursor(0, 1);
    lcd.print(F("2.ax+by=c"));

    do {
        key = getKeyStr();
        delay(50);        
    } while (key != "1" && key != "2" && key != "MODE");
    
    if (modeKeyCheck(key)) return;
    
    // ax^2 + bx + c = 0
    if (key == "1") {
        Serial.println(F("Quadratic eq chosen"));
        lcd.clear();
        lcd.blink();
        float a = scanCoefficient("a=");
        float b = scanCoefficient("b=");
        float c = scanCoefficient("c=");
        lcd.noBlink();

        float delta = b*b-4*a*c;

        if (delta == 0) {
            float x = -b/(2*a);
            printSolution("x=", x);
        }
        else if (delta > 0) {
            float x1 = (-b-sqrt(delta))/(2*a);
            float x2 = (-b+sqrt(delta))/(2*a); 
            printSolution("x1=", x1);
            printSolution("x2=", x2);
        }
        else if (delta < 0) {
            lcd.clear();
            lcd.print("No solution");
            pressAnyKey();
        }
    }
    else if (key == "2") {
        Serial.println(F("Sys of eqs 2 vars chosen"));
        lcd.clear();
        lcd.blink();

        float a1 = scanCoefficient("[a1x+b1y=c1] a1=");
        float b1 = scanCoefficient("[a1x+b1y=c1] b1=");
        float c1 = scanCoefficient("[a1x+b1y=c1] c1=");
        float a2 = scanCoefficient("[a2x+b2y=c2] a2=");
        float b2 = scanCoefficient("[a2x+b2y=c2] b2=");
        float c2 = scanCoefficient("[a2x+b2y=c2] c2=");

        float d = a1*b2 - a2*b1;
        float dx = c1*b2 - c2*b1;
        float dy = a1*c2 - a2*c1;

        sprintVariable<float>("d", d);
        sprintVariable<float>("dx", dx);
        sprintVariable<float>("dy", dy);

        if (d == 0 && (dx != 0 || dy != 0)) {
            lcd.clear();
            lcd.print("No solution");
            pressAnyKey();
        }
        else if (d == 0 && dx == 0 && dy == 0) {
            lcd.clear();
            lcd.print("Infnt solutions");
            pressAnyKey();
        }
        else {
            float x = dx/d;
            float y = dy/d;
            printSolution("x=", x);
            printSolution("y=", y);            
        }
    }
}

void compMode () {
    lcd.clear();
    bool menuflag = false;
    float result = 0;

    Serial.println(F("Get in function compMode:"));
    
    do {
        lcd.blink();
        result = scanExpression(menuflag);
        if (!menuflag) {
            lcd.noBlink();
            printSolution("Result:", result);
        }
    } while (!menuflag);
}
