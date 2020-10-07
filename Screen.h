#include <LiquidCrystal_I2C.h>
#define BUFFERSIZE 16

LiquidCrystal_I2C lcd(0x27,16,2);

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
