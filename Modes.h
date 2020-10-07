#include "Input.h"
#include "Screen.h"

#define COMPMODE 1
#define EQNMODE 2
#define INTGRLMODE 3
#define MENUMODE 0

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

byte mode = COMPMODE;
int currentDisplayingChars = 0;
int scrollIndex = 0;

void menuMode() {
    lcd.clear();
    lcd.noBlink();
}

void modeKeyCheck (String key) {
    if (key == "MODE") {
        mode = MENUMODE;
        menuMode();
    }
}

void compMode () {
    String tokens[max_size];
    int ti = 0;

    while (true) {
        String key = getKeyStr();
        
        if (key != None) {
            byte keyTokenType = getTokenType(key);
            byte currTokenType = getTokenType(tokens[ti]);
            Serial.print(key);
            Serial.print("Mode: ");
            Serial.println(mode);

            modeKeyCheck(key);
            if (mode != COMPMODE) return;
            
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
