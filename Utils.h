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
