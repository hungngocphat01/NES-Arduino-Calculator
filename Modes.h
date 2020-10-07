#include "Input.h"

// Modes
#define COMPMODE 1
#define EQNMODE 2
#define INTGRLMODE 3
#define ABOUTMODE 4
#define MENUMODE 0

// Function prototypes
bool modeKeyCheck (String key); 
void eqnMode();
void compMode ();
void menuMode();
void aboutMode();

byte mode = MENUMODE;
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
            mode = COMPMODE;
            compMode();
            printMenu = true;
        }
        else if (key == "2") {
            mode = EQNMODE;
            eqnMode();  
            printMenu = true;
        }
        else if (key == "4") {
            mode = ABOUTMODE;
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
    lcd.print(F("2.ax+by=0"));

    do {
        key = getKeyStr();
        delay(50);        
    } while (key != "1" && key != "2" && key != "MODE");
    
    if (modeKeyCheck(key)) return;
    
    // ax^2 + bx + c = 0
    if (key == "1") {
        Serial.println(F("Quadratic eq chosen"));
        lcd.clear();
        float a = scanCoefficient("a=");
        float b = scanCoefficient("b=");
        float c = scanCoefficient("c=");

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
        }
    }
    else if (key == "2") {
        
    }
    pressAnyKey();
}

void compMode () {
    lcd.clear();
    lcd.blink();

    Serial.println(F("Get in function compMode:"));
    
    String tokens[max_size];
    int ti = 0;

    while (true) {
        String key = getKeyStr();
        
        if (key != None) {
            byte keyTokenType = getTokenType(key);
            byte currTokenType = getTokenType(tokens[ti]);
            Serial.print(key);
            
            if (modeKeyCheck(key)) return;
            
            if (key == "=") {
                Serial.println();
                sprintMemoryUsage(F("Free mem when pressed =: "));
                
                sstack postfix;
                ConvertToPostfix(tokens, ti + 1, postfix);
                
                if (!errflag) {
                    sprintArr(postfix.data, postfix.index + 1);
                       
                    float result = PostfixEvaluate(postfix);
                    int precision = 6;
                    Serial.println();
                    Serial.println(result, precision);
                     
                    lcd.clear();
                    lcd.home();
                    lcd.noBlink();
                    lcd.print(F("Result:"));
                    lcd.setCursor(0, 1);
                    lcd.print(result, precision);
                }
                else {
                    // After handling
                    errflag = false;
                }
                // Reset
                ti = 0;
                tokens[ti] = "";
                continue;
            }
            else if (key == "<") {
                if (currentDisplayingChars == 16) scrollIndex--;
                printExpression(tokens, scrollIndex + 1);
                continue;
            }
            else if (key == ">") {
                if (scrollIndex < ti) scrollIndex++;
                printExpression(tokens, scrollIndex + 1);
                continue;
            }
            else if (key == "!") {
                if (currTokenType == NUM && tokens[ti] != "Ans" && tokens[ti] != "pi") {
                    tokens[ti].remove(tokens[ti].length() - 1);
                    printExpression(tokens, ti + 1);
                }
                else {
                    tokens[ti] = "";
                    ti--;
                }
                
                Serial.print(F("\nAfter remove: "));
                sprintArr(tokens, ti + 1, NO_NEWLINE);
                
                printExpression(tokens, ti + 1);
                continue;
            }
            
            // Bound checking
            if (ti >= max_size) continue;
            if (ti < 0) ti = 0;
            
            if (currTokenType == UNK) {
                tokens[ti] = key;
            }
            else if (keyTokenType == currTokenType && keyTokenType == NUM) {
                tokens[ti] += key;
            }
            else {
                ti++;
                tokens[ti] = key;
            }
            scrollIndex = ti;
            printExpression(tokens, ti + 1);
        }
    }
}
