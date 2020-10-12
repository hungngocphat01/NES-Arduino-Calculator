#include <LiquidCrystal_I2C.h>
#define BUFFERSIZE 16

byte prntline = 0;

LiquidCrystal_I2C lcd(0x27,16,2);

void lcdClrLine(byte line) {
    lcd.setCursor(0, line);
    for (int i = 0; i < BUFFERSIZE; i++) {
        lcd.print(" ");
    }
    lcd.setCursor(0, line);
}

void printExpression(String* tokens, int n) {    
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
        
    lcdClrLine(prntline);
    lcd.setCursor(0, prntline);
    lcd.blink();
    lcd.print(buffer);
}

template <typename T>
void sprintVariable(String varname, T value) {
    Serial.print(varname);
    Serial.print(value);
    Serial.println();
}

static void sprintArr(String* arr, int n, bool newline = true) {
  for (int i = 0; i < n; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  if (newline) Serial.println();
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
static int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

static void sprintMemoryUsage(const String& msg) {
    Serial.print(msg);
    Serial.println(freeMemory());
}
