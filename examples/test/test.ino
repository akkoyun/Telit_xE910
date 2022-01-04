// Define Ovoo Libraries
#include "Telit_xE910.h"

void setup() {

    Serial.begin(115200);

    if (GSM.Begin(true)) {
        
        if (GSM.Debug_Mode) {

            Serial.println(F("--------------------------------------------------"));
            Serial.print(F("Manufacturer      : ")); Serial.println(GSM.Manufacturer());
            Serial.print(F("Model             : ")); Serial.println(GSM.Model());
            Serial.print(F("Firmware          : ")); Serial.println(GSM.Firmware());
            Serial.print(F("IMEI Number       : ")); Serial.println(GSM.IMEI());
            Serial.print(F("Serial Number     : ")); Serial.println(GSM.Serial_ID());
            Serial.print(F("ICCID             : ")); Serial.println(GSM.ICCID());
            Serial.println(F("--------------------------------------------------"));

        }
        
        GSM.Connect();

        if (GSM.Debug_Mode) {

            Serial.println(F("--------------------------------------------------"));
            Serial.print(F("Operator          : ")); Serial.println(GSM.Operator());
            Serial.println(F("--------------------------------------------------"));

        }

    }
    
}

void loop() {

    delay(1000);

}
