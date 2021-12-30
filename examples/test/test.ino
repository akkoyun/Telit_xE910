// Define Ovoo Libraries
#include "Telit_xE910.h"

void setup() {

    Serial.begin(115200);

    GSM.Power_ON(false, false, true);
    GSM.Initialize(true);

}

void loop() {

    //Serial.println(GSM_AT.IMEI);
    Serial.println(GSM_AT.Serial_Number);
    //Serial.println(GSM_AT.ICCID);
    Serial.println(GSM_AT.Manufacturer);
    Serial.println(GSM_AT.Model);
    Serial.println(GSM_AT.Modem_Firmware);

    delay(1000);

}
