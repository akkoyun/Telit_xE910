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
            Serial.print(F("GSM RSS Level     : ")); Serial.println(GSM_AT.Signal_RSSI);
            Serial.print(F("Signal Strength   : ")); Serial.println(GSM.Signal_Strength());
            Serial.println(F("--------------------------------------------------"));

        }

        GSM_RTC.Time_Update();

        if (GSM.Debug_Mode) {

            char buffer[40];
            sprintf(buffer, "Time              : %d/%d/%d  %d:%d:%d", GSM_AT.RTC_Day, GSM_AT.RTC_Month, GSM_AT.RTC_Year, GSM_AT.RTC_Hour, GSM_AT.RTC_Minute, GSM_AT.RTC_Second);

            Serial.println(F("--------------------------------------------------"));
            Serial.println(buffer);
            Serial.println(F("--------------------------------------------------"));

        }

char *DD = "{\"Device\":{\"Type\":\"402-P02\",\"ID\":\"70A11D1D01000026\"},\"Power\":{\"Battery\": {\"IV\":4.115156,\"T\":25.5,\"AC\":-49.6875,\"SOC\":98.99609,\"FB\":1519,\"IB\":1503,\"CYC\":38}},\"IoT\":{\"GSM\":{\"Operator\":{\"RSSI\":12}}},\"Data\":{\"DeviceStatus\":240,\"FaultStatus\": 500,\"TimeStamp\":\"2000-01-01  23:46:51\"}}";

GSM.Send_Data_Pack(DD);

//Serial.println(GSM_AT.SS(1));

    }
    
}

void loop() {


    delay(10000);

}
