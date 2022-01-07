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
            Serial.print(F("IP Address        : ")); Serial.println(GSM.IP());
            Serial.println(F("--------------------------------------------------"));

        }

        GSM.RSSI_Refresh();

        if (GSM.Debug_Mode) {

            Serial.println(F("--------------------------------------------------"));
            Serial.print(F("GSM RSS Signal    : ")); Serial.println(GSM.Signal_Strength());
            Serial.println(F("--------------------------------------------------"));

        }

        GSM_RTC.Time_Update();

        if (GSM.Debug_Mode) {

            Serial.println(F("--------------------------------------------------"));
            char buffer[40];
            sprintf(buffer, "Time              : %d/%d/%d  %d:%d:%d", GSM_AT.RTC_Day, GSM_AT.RTC_Month, GSM_AT.RTC_Year, GSM_AT.RTC_Hour, GSM_AT.RTC_Minute, GSM_AT.RTC_Second);
            Serial.println(buffer);
            Serial.println(F("--------------------------------------------------"));

        }


    }
    
}

void loop() {


    delay(10000);

}
