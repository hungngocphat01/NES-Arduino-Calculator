#include "Input.h"
#include "Screen.h"

#define COMPMODE 1
#define EQNMODE 2
#define INTGRLMODE 3
#define MENUMODE 0

// Function prototypes
bool modeKeyCheck (String key); 
void eqnMode();
void compMode ();
void menuMode();

void printExpression(String* tokens, int n, int scroll = 0) {    
    int totalLength = 0;
    char buffer[BUFFERSIZE + 1];
    int k = BUFFERSIZE;

    for (int i = n - 1; i >= 0; i--) {
        for (int j = tokens[i].length() - 1; j >= 0; j--) {
            k--;
            buffer[k] = tokens[i][j];
            if (k <= 0) break;
        }
        if (k <= 0) break;
    }
    int len = BUFFERSIZE - k;

    // Remove padding at the start
    if (k > 0) {
        for (int i = 0; i < len; i++) {
            buffer[i] = buffer[i + k];
        }   
    }
    buffer[len] = '\0';
        
    lcd.clear();
    lcd.blink();
    lcd.print(buffer);
}

byte mode = MENUMODE;
int currentDisplayingChars = 0;
int scrollIndex = 0;

bool modeKeyCheck (String key) {
    if (key == "MODE") {
        mode = MENUMODE;
        return true;
    }
    return false;
}

void menuMode() {
    bool printMenu = true; // to fix flickering
    
    String key;
    while(true) {
        key = getKeyStr();
        if (printMenu) {
            lcd.clear(); lcd.noBlink();
            lcd.print("1.CMP      2.EQN");
            lcd.setCursor(0, 1);
            lcd.print("3.INTEGRAL");
            printMenu = false;
        }
        
        if (key == "1") {
            mode = COMPMODE;
            compMode();
            Serial.println("Get out of func compMode");
            printMenu = true;
        }
        else if (key == "2") {
            mode = EQNMODE;
            eqnMode();  
            printMenu = true;
        }
    }
}

void eqnMode() {
    lcd.noBlink();

    int page = 0;
    String key;

    while (true) {
        if (modeKeyCheck(key)) return;
        if (page == 0) {
            lcd.clear();
            lcd.print("1.ax^2+bx+c=0");
            lcd.setCursor(0, 1);
            lcd.print("2.ax^3+bx^2...=0");
    
            do {
                key = getKeyStr();
                if (key == ">") page++;
                else if (key == "1");
                else if (key == "2");            
            } while (key != ">" && key != "1" && key != "2" && key != "MODE");
        }
        if (page == 1) {
            lcd.clear();
            lcd.print("1.ax+by=0");
            lcd.setCursor(2,1);
            lcd.print("cx+dy=0");
    
            do {
                key = getKeyStr();
                if (key == "<") {
                    page--;
                }
                else if (key == "1");        
            } while (key != "<" && key != "1" && key != "MODE");
        }
    }
}

void compMode () {
    lcd.clear();
    lcd.blink();

    Serial.println("Get in function compMode:");
    
    String tokens[max_size];
    int ti = 0;

    while (true) {
        String key = getKeyStr();
        
        if (key != None) {
            byte keyTokenType = getTokenType(key);
            byte currTokenType = getTokenType(tokens[ti]);
            Serial.print(key);
            
            if (modeKeyCheck(key)) return;
            Serial.print("ti: ");
            Serial.println(ti);
            
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
                    lcd.print("Result:");
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
