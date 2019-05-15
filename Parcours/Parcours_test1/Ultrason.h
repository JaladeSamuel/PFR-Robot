/*Ultrason.h*/
#include "Arduino.h"
//PIN trigger
extern const byte TRIGGER_PIN_ULTRASON = 5;
//PIN Echo
extern const byte ECHO_PIN_ULTRASON = 3;
float distance_mm_ultrason();
void initSerialSon();
