#include <Keypad.h>
#include "ExprCalc.h"

#define None "None"

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
    case '4': return '(';
    case '5': return ')';
    }
  }
  return key;
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
     }
     return String(key);
  }
  return None;
}

String tokens[max_size];
byte ti = 0;

void loop(){
  String key = getKeyStr();
  if (key != None) {
    byte keyTokenType = getTokenType(key);
    byte currTokenType = getTokenType(tokens[ti]);
    Serial.print(key);
    
    if (key == "=") {
       Serial.println();
       //foo(tokens, ti + 1);
       //printArr(tokens, ti + 1);
       sstack postfix = ConvertToPostfix(tokens, ti + 1);
       printArr(postfix.data, postfix.index + 1);
       float result = PostfixEvaluate(postfix);
       Serial.println();
       Serial.println(result);
       halt;
    }

    if (ti >= max_size) return;
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
  }
  delay(100);
}
