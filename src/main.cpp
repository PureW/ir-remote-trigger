#include "Arduino.h"

#include "IRLibRecvLoop.h"
#include "IRLibDecodeBase.h"
#include "IRLib_HashRaw.h"
#include "IRLibCombo.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define PIN_DIGITAL_IR 8

long unsigned int valid_signals [] = {
    0xFB8F34F0, // Marantz NR1506 ON/OFF
    0x2F53B208 // Marantz NR1506 ON/OFF
};

IRrecvLoop receiver(PIN_DIGITAL_IR);
IRdecode decoder;

long unsigned int last_signal = 0;

/** Run this function to dump signal hash onto serial-bus.
 * Returns true when a request to toggle was detected */
bool detect_signal() {
    bool toggle = false;
    if (receiver.getResults()) {
        decoder.decode();
        Serial.print("Detected IR-signal, hash: 0x");
        Serial.println(decoder.value,HEX);
        bool match = false;
        for (int i=0;;++i) {
            if (valid_signals[i] == 0) {
                break;
            }
            if (decoder.value == valid_signals[i]) {
                match = true;
                break;
            }
        }
        if (match) {
            //Serial.println("  Signal matches!");
            if (last_signal != decoder.value) {
                last_signal = decoder.value;
                toggle = true;
                Serial.println("  Toggle power");
            }
        }
        receiver.enableIRIn();
    }
    return toggle;
}

void setup() {
    Serial.begin(9600);
    Serial.println("Enabling receiver");
    receiver.enableIRIn();
}
void loop() {
    bool toggle = detect_signal();
    if (toggle) {
        // Toggle transistor
    }
}
