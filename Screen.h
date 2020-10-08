#include <LiquidCrystal_I2C.h>
#define BUFFERSIZE 16

LiquidCrystal_I2C lcd(0x27,16,2);

void lcdClrLine(byte line) {
    lcd.setCursor(0, line);
    for (int i = 0; i < BUFFERSIZE; i++) {
        lcd.print(" ");
    }
    lcd.setCursor(0, line);
}

void printExpression(String* tokens, int n, byte line = 0) {    
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
        
    lcdClrLine(line);
    lcd.setCursor(0, line);
    lcd.blink();
    lcd.print(buffer);
}

template <typename T>
void sprintVariable(String varname, T value) {
    Serial.print(varname);
    Serial.print(": ");
    Serial.print(value);
    Serial.println();
}
