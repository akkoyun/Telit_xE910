// Define Ovoo Libraries
#include "Telit_xE910.h"

void setup() {

    // Start Terminal
    Serial.begin(115200);

	// Start Console
    Terminal.Begin(Serial);
	Terminal.Telit_xE910();

    // Start GSM Serial
    Serial3.begin(115200);

   
}

void loop() {

	// Terminal Command Routine
	if (Serial.available() > 0) {
	
		uint16_t Key = Serial.read();

		String _Data = "{\"Device\":{\"Type\":\"402-P01\",\"ID\":\"260000011D1DA170\",\"Hardware\":\"02.01.00\",\"Firmware\":\"02.04.00\"},\"Power\":{\"Battery\":{\"Firmware\":\"02.01.02\",\"IV\":4.27,\"T\":25.42,\"AC\":-30,\"SOC\":98.9,\"FB\":2000,\"IB\":1400,\"CYC\":10}},\"IoT\":{\"GSM\":{\"Module\":{\"Manufacturer\":1,\"Model\":1,\"Firmware\":\"13.00.007\",\"Serial\":\"0001767746\",\"IMEI\":\"353613080341905\"},\"SIM\":{\"SIM_Type\":1,\"Iccid\":\"8990011916180288191\"},\"Operator\":{\"Code\":28601,\"RSSI\":12,\"Conn_Time\":10}}},\"Data\":{\"DeviceStatus\":241,\"FaultStatus\":500,\"TimeStamp\": \"08-03-2022  12:36:53\",\"Temperature\":32.53535,\"Humidity\":33.45655}}";

		// Terminal Beep
		Terminal.Beep();

		switch (Key) {
		case '1':

            GSM.Begin(Serial3);

            // GSM Details
            Terminal.Text(5, 116, CYAN, String(GSM.Variables.Manufacturer));
            Terminal.Text(6, 116, CYAN, String(GSM.Variables.Model));
            Terminal.Text(7, 108, CYAN, String(GSM.Variables.Firmware));
            Terminal.Text(8, 102, CYAN, String(GSM.Variables.IMEI));
            Terminal.Text(9, 107, CYAN, String(GSM.Variables.Serial_Number));
            Terminal.Text(10, 98, CYAN, String(GSM.Variables.ICCID));

			break;		
		case '2':
            
            // Connect
            GSM.Connect(true, 5, 73);

            // GSM Details
            Terminal.Text(13, 113, CYAN, String(GSM.Variables.Connection_Time));
            Terminal.Text(14, 115, CYAN, String(GSM.Variables.RSSI));
            Terminal.Text(15, 112, CYAN, String(GSM.Variables.Operator));
            Terminal.Text(16, 102, CYAN, String(GSM.Variables.IP_Address));

            if (GSM.Modem.Connection_Status) {Terminal.Text(23, 106, CYAN, String("Connected"));} else {Terminal.Text(23, 106, RED, String("Not Connected"));}

            break;		
		case '3':
            
            // Connect
            GSM.Time_Update();

            // GSM Details
            Terminal.Text(2, 3, CYAN, String(GSM.Time.Hour));
            Terminal.Text(2, 6, CYAN, String(GSM.Time.Minute));
            Terminal.Text(2, 9, CYAN, String(GSM.Time.Second));
            Terminal.Text(2, 12, CYAN, String(GSM.Time.Day));
            Terminal.Text(2, 15, CYAN, String(GSM.Time.Month));
            Terminal.Text(2, 18, CYAN, String(GSM.Time.Year));

            if (GSM.Modem.Connection_Status) {Terminal.Text(23, 106, CYAN, String("Time Updated"));} else {Terminal.Text(23, 106, RED, String("Time Not Upd."));}

            break;		

		case '4':

            if (GSM.Send_Data_Pack(1, _Data)) {Terminal.Text(30, 0, CYAN, String("Succesful"));} else {Terminal.Text(30, 0, RED, String("Error"));}

            break;		

		default:
			break;
		}
	}
    

}
