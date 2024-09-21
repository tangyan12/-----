

#include <SPIFFS.h>
#include <esp32-hal.h>
#include <esp32-hal-timer.h>

#include "common.h"

void setup()
{
    Serial.begin(115200);
    Serial.printf("HELLO");
    screen.init(4,80);
}

void loop()
{
    for (; ;)
    {
        /* code */
    }
    
}