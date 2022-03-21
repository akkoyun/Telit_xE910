// Define Ovoo Libraries
#include "Telit_xE910.h"

char * _Data = "{\"Device\":{\"Type\":\"402-P01\",\"ID\":\"260000011D1DA170\",\"Hardware\":\"02.01.00\",\"Firmware\":\"02.04.00\"},\"Power\":{\"Battery\":{\"Firmware\":\"02.01.02\",\"IV\":4.27,\"T\":25.42,\"AC\":-30,\"SOC\":98.9,\"FB\":2000,\"IB\":1400,\"CYC\":10}},\"IoT\":{\"GSM\":{\"Module\":{\"Manufacturer\":1,\"Model\":1,\"Firmware\":\"13.00.007\",\"Serial\":\"0001767746\",\"IMEI\":\"353613080341905\"},\"SIM\":{\"SIM_Type\":1,\"Iccid\":\"8990011916180288191\"},\"Operator\":{\"Code\":28601,\"RSSI\":12,\"Conn_Time\":10}}},\"Data\":{\"DeviceStatus\":241,\"FaultStatus\":500,\"TimeStamp\": \"2022-03-17  09:06:32\",\"Temperature\":32.53535,\"Humidity\":33.45655}}";
//String _Data;

bool Interrupt = false;
bool Interrupt_Control = false;

void setup() {

    // Start Terminal
    Serial.begin(115200);

	// Start Console
    Terminal.Begin(Serial);
	Terminal.Telit_xE910();

    // Start GSM Serial
    Serial3.begin(115200);
	GSM.Begin(Serial3, true);

	// Initialize Modem
	GSM.Set_Modem();

	// Connect
	uint8_t _Connection_Time;
	GSM.Connect(_Connection_Time);







	if (GSM.Get_Connection_Status()) {Terminal.Text(23, 105, CYAN, String("Connected      "));} else {Terminal.Text(23, 105, RED, String("Not Connected  "));}

	// Declare Timestamp Variable
	char _Time_Stamp[25];	// dd-mm-yyyy hh.mm.ss

	// Handle TimeStamp
	sprintf(_Time_Stamp, "%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", GSM.Get_Year(), GSM.Get_Month(), GSM.Get_Day(), GSM.Get_Hour(), GSM.Get_Minute(), GSM.Get_Second());
	Terminal.Text(2, 3, CYAN, String(_Time_Stamp));

	if (GSM.Get_Connection_Status()) {Terminal.Text(23, 105, CYAN, String("Time Updated   "));} else {Terminal.Text(23, 105, RED, String("Time Not Upd.  "));}

	// Open Socket
	if (GSM.Socket_Open()) {Terminal.Text(23, 105, CYAN, String("Socket Opened  "));} else {Terminal.Text(23, 105, RED, String("Error          "));}

	// Interupt Definations
	cli();
	PCICR	|= 0b00000111;	// Set All Interrupt
	PCMSK1	|= 0b00001000;	// GSM Ring Interrupt Defination
	sei();

	Interrupt_Control = true;

}

void loop() {

	// Terminal Command Routine
	if (Serial.available() > 0) {
	
		uint16_t Key = Serial.read();

		// Terminal Beep
		Terminal.Beep();

		switch (Key) {

		case '4':

            if (GSM.Send_Data_Pack(1, _Data)) {Terminal.Text(23, 105, CYAN, String("Succesful      "));} else {Terminal.Text(23, 105, RED, String("Error          "));}

            break;		

		default:
			break;
		}
	}






	if (Interrupt) {

		// Set Interrupt Variable
		Interrupt_Control = false;

		// Get Command
		uint16_t Command = GSM.Socket_Answer();

		// Declare Message Variable
		char _Response[65];

		// Handle Message
		sprintf(_Response, "{\"Response\":{\"Event\":%02hhu,\"TimeStamp\":\"20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu\"}}", 100, GSM.Get_Year(), GSM.Get_Month(), GSM.Get_Day(), GSM.Get_Hour(), GSM.Get_Minute(), GSM.Get_Second());

		// Send Message
		GSM.Socket_Send(_Response);

		// Set Interrupt Variable
		Interrupt = false;
		Interrupt_Control = true;

	}
	

}

// GSM Ring Interrupt
ISR(PCINT1_vect) {

	// Set Interrupt Variable
	if (Interrupt_Control) Interrupt = true;

}
