#include <Keypad.h>
#include "ExprCalc.h"

#define None "None"
#define NO_NEWLINE false

// Key codes
#define SINKEY 1
#define COSKEY 2
#define TANKEY 3
#define SQRTKEY 4
#define PIKEY 5
#define EKEY 6
#define ANSKEY 7
#define MODEKEY 8
#define XKEY 9

bool menuflag = false;
bool allowxkey = false;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'1','4','7','.'},
    {'2','5','8','0'},
    {'3','6','9','='},
    {'+','-','*','/'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11, 10, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Shift key
const byte shiftPin = A0;
const byte shiftKey = LOW;

char readKey() {
    char key = customKeypad.getKey();
    if (digitalRead(shiftPin) == LOW) {
        switch (key) {
        case '+': return SINKEY;
        case '-': return COSKEY;
        case '*': return TANKEY;
        case '1': return '^';
        case '2': return SQRTKEY;
        case '3': return PIKEY;
        case '6': return EKEY;
        case '9': return ANSKEY;
        case '/': return MODEKEY;
        case '.': if (allowxkey) return XKEY; else break;
        case '4': return '(';
        case '5': return ')';
        case '0': return '!'; // Delete key
        }
    }
    return key;
}

bool isSpecialKey(String key) {
    return (key == "<" || key == ">" || key == "!");
}

bool isDigitKey(String key) {
    return (key == "0" || key == "1" || key == "2" || key == "3" || key == "4" || key == "5" || key == "6" ||
            key == "7" || key == "8" || key == "9" || key == "." || key == "-");
}

String getKeyStr() {
    char key = readKey();
    if (key) {
        switch (key) {
        case SINKEY:
            return "sin";
        case COSKEY:
            return "cos";
        case TANKEY:
            return "tan";
        case SQRTKEY:
            return "sqrt";
        case PIKEY:
            return "pi";
        case EKEY:
            return "e";
        case ANSKEY:
            return "Ans";
        case MODEKEY:
            return "MODE";
        case XKEY:
            return "X";
        }
        return String(key);
    }
    return None;
}

bool modeKeyCheck (String key) {
    if (key == "MODE") {
        return true;
    }
    return false;
}

void pressAnyKey() {
    while (getKeyStr() == None) {
        delay(50);
    }
}

void scanTokens (String* tokens, int& ti) {
    Serial.println(F("[STAT] Scanning tokens"));
    ti = 0;
    
    Serial.print("[INP] ");

    while (true) {
        String key = getKeyStr();

        if (key != None) {
            // Bound checking
            if (ti >= max_size) continue;
            if (ti < 0) ti = 0;
            
            byte keyTokenType = getTokenType(key);
            byte currTokenType = getTokenType(tokens[ti]);
            Serial.print(key);

            if (modeKeyCheck(key)) {
                menuflag = true;
                return;
            }
            
            if (key == "=") {
                return;
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
                
                Serial.print(F("\n[INP] Expr after delete: "));
                sprintArr(tokens, ti + 1, NO_NEWLINE);
                
                printExpression(tokens, ti + 1);
                continue;
            }
            
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
            printExpression(tokens, ti + 1);
        }
    }
}

float evalExpression (String* tokens, int ti) {
    Serial.println(F("[STAT] Evaluating expressions"));
    sstack postfix;

    Serial.println();
    
    ConvertToPostfix(tokens, ti + 1, postfix);

    // Syntax error check
    if (!errflag) {
        Serial.print(F("\n[OUTP] Postfix: "));
        sprintArr(postfix.data, postfix.index + 1);
        
        float result = PostfixEvaluate(postfix);
        // Math error check
        if (!errflag) {
            return result;
        }
    }
    return 0;
}

float scanAndEvalExpression() {
    String tokens[max_size];
    int ti = 0;

    while (true) {
        ti = 0;
        tokens[ti] = "";
        
        Serial.println(F("[STAT] Sc&ev - next loop"));
        scanTokens(tokens, ti);
        if (menuflag) return;
        
        float result = evalExpression(tokens, ti);
    
        if (errflag) {
            // After handling
            errflag = false;
            
            // Reset
            ti = 0;
            tokens[ti] = "";
            pressAnyKey();
            lcdClrLine(prntline);
            lcd.blink();
             
            continue;
        }
        return result;
    }
}


float scanCoefficient(String msg) {
    lcd.clear();
    lcd.print(msg);
    bool menuflag = false;
    
    lcd.setCursor(0, 1);
    
    float scan = scanAndEvalExpression();
    sprintVariable("[INP] " + msg, scan);

    return scan;    
}
