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

extern byte mode;

bool modeKeyCheck (String key) {
    if (key == "MODE") {
        mode = 0;
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

float scanCoefficient(String msg) {
    lcd.clear();
    lcd.print(msg);
    
    lcd.setCursor(0, 1);
    String scanstr = scanNumber();
    
    return convToFloat(scanstr);
}

void pressAnyKey() {
    while (getKeyStr() == None) {
        delay(50);
    }
}
