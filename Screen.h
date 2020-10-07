#include <LiquidCrystal_I2C.h>
#define BUFFERSIZE 16

extern LiquidCrystal_I2C lcd;
extern int currentDisplayingChars;

extern void printExpression(String* tokens, int n, int scroll = 0);
