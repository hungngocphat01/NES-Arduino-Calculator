#include <Keypad.h>
#include "ExprCalc.h"

#define None "None"
#define NO_NEWLINE false

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

String getKeyStr() {
  char key = readKey();
  if (key) {
    switch (key) {
    case SIN:
      return "sin";
      break;
     case COS:
      return "cos";
      break;
     case TAN:
      return "tan";
      break;
     case SQRT:
      return "sqrt";
     case PIKEY:
      return "pi";
     case EKEY:
      return "e";
     case ANSKEY:
      return "Ans";
     }
     return String(key);
  }
  return None;
}

String tokens[max_size];
int ti = 0;
int scrollIndex = 0;

// main func
void loop(){
  String key = getKeyStr();
  if (key != None) {
    byte keyTokenType = getTokenType(key);
    byte currTokenType = getTokenType(tokens[ti]);
    Serial.print(key);
    
    
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
       return;
    }
    else if (key == "<") {
        return;
    }
    else if (key == ">") {
        return;
    }
    else if (key == "!") {
        if (currTokenType == NUM && tokens[ti] != "Ans") {
            tokens[ti].remove(tokens[ti].length() - 1);
        }
        else {
            tokens[ti] = "";
            ti--;
        }
                
        Serial.print(F("\nAfter remove: "));
        sprintArr(tokens, ti + 1, NO_NEWLINE);

        printExpression(tokens, ti + 1);
        return;
    }

    // Bound checking
    if (ti >= max_size) return;
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
    printExpression(tokens, ti + 1);
  }
  delay(100);
}
