#include <Keypad.h>
#include "ExprCalc.h"

#define None "None"
#define NO_NEWLINE false

// Key codes
#define PIKEY 5
#define EKEY 6
#define ANSKEY 7
#define MODEKEY 8

#define SIN 1
#define COS 2
#define TAN 3
#define SQRT 4

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
        case '+': return SIN;
        case '-': return COS;
        case '*': return TAN;
        case '1': return '^';
        case '2': return SQRT;
        case '3': return PIKEY;
        case '6': return EKEY;
        case '9': return ANSKEY;
        case '/': return MODEKEY;
        case '4': return '(';
        case '5': return ')';
        case '7': return '<'; // Left key
        case '8': return '>'; // Right key
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
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case TAN:
            return "tan";
        case SQRT:
            return "sqrt";
        case PIKEY:
            return "pi";
        case EKEY:
            return "e";
        case ANSKEY:
            return "Ans";
        case MODEKEY:
            return "MODE";
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

// Read a number until a '=' is pressed. Returns "MENU" if menu btn is pressed
String scanNumber() {
    String key, result;
    lcd.blink();
    
    while (key != "=") {
        key = getKeyStr();
        if (key == "!") {
            result.remove(result.length() - 1);
            lcd.setCursor(0, 1);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            lcd.print(result);
        }
        if (!isDigitKey(key)) continue;
        result += key;
    
        lcd.print(key);
        delay(10);
    }
    lcd.noBlink();
    
    return result;
}

void pressAnyKey() {
    while (getKeyStr() == None) {
        delay(50);
    }
}

float scanExpression (bool& menuflag, byte prntline = 0) {
    String tokens[max_size];
    int ti = 0;

    while (true) {
        String key = getKeyStr();
        
        if (key != None) {
            byte keyTokenType = getTokenType(key);
            byte currTokenType = getTokenType(tokens[ti]);
            Serial.print(key);
            
            if (modeKeyCheck(key)) {
                menuflag = true;
                return 0;
            }
            
            if (key == "=") {
                Serial.println();
                sprintMemoryUsage(F("Free mem when pressed =: "));
                
                sstack postfix;
                ConvertToPostfix(tokens, ti + 1, postfix, prntline);
                
                if (!errflag) {
                    sprintArr(postfix.data, postfix.index + 1);
                       
                    float result = PostfixEvaluate(postfix, prntline);
                    int precision = 6;
                    Serial.println();
                    Serial.println(result, precision);
                     
                    return result;
                }
                else {
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
            }
//            else if (key == "<") {
//                if (currentDisplayingChars == 16) scrollIndex--;
//                printExpression(tokens, scrollIndex + 1);
//                continue;
//            }
//            else if (key == ">") {
//                if (scrollIndex < ti) scrollIndex++;
//                printExpression(tokens, scrollIndex + 1);
//                continue;
//            }
            else if (key == "!") {
                if (currTokenType == NUM && tokens[ti] != "Ans" && tokens[ti] != "pi") {
                    tokens[ti].remove(tokens[ti].length() - 1);
                    printExpression(tokens, ti + 1, prntline);
                }
                else {
                    tokens[ti] = "";
                    ti--;
                }
                
                Serial.print(F("\nAfter remove: "));
                sprintArr(tokens, ti + 1, NO_NEWLINE);
                
                printExpression(tokens, ti + 1, prntline);
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
            printExpression(tokens, ti + 1, prntline);
        }
    }
}

float scanCoefficient(String msg) {
    lcd.clear();
    lcd.print(msg);
    bool menuflag = false;
    
    lcd.setCursor(0, 1);
    
    float scan = scanExpression(menuflag, 1);
    Serial.print(msg);
    Serial.print(scan);

    return scan;    
}
