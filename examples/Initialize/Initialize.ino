// Define Ovoo Libraries
#include "Telit_xE910.h"

bool Interrupt = false;
bool Interrupt_Control = false;

void setup() {

    // Start Terminal
    Serial.begin(115200);

	// Start Console
	#ifdef Debug
	    Terminal.Begin(Serial);
		Terminal.Telit_xE910();
	#endif

	// Start GSM
	if (GSM.Begin()) {

		// GSM Details
		#ifdef Debug
			Terminal.Text(5, 116, CYAN, String(GSM.Variables.Manufacturer));
			Terminal.Text(6, 116, CYAN, String(GSM.Variables.Model));
			Terminal.Text(7, 108, CYAN, String(GSM.Variables.Firmware));
			Terminal.Text(8, 102, CYAN, String(GSM.Variables.IMEI));
			Terminal.Text(9, 107, CYAN, String(GSM.Variables.Serial_Number));
			Terminal.Text(10, 98, CYAN, String(GSM.Variables.ICCID));
		#endif

		// Print Initialize Status
		#ifdef Debug
			if (GSM.Modem.Initialize_Status) Terminal.Text(23, 105, CYAN, String(F("Initialized    ")));
		#endif

		// Connect
		if (GSM.Connect()) {

			// GSM Details
			#ifdef Debug
				Terminal.Text(13, 113, CYAN, String(GSM.Variables.Connection_Time));
				Terminal.Text(14, 115, CYAN, String(GSM.Variables.RSSI));
				Terminal.Text(15, 112, CYAN, String(GSM.Variables.Operator));
				Terminal.Text(16, 102, CYAN, String(GSM.Variables.IP_Address));
			#endif

			// Print Initialize Status
			#ifdef Debug
				if (GSM.Modem.Connection_Status) Terminal.Text(23, 105, CYAN, String(F("Connected      ")));
			#endif

			// Update Time
			if (GSM.Time_Update()) {

				// Handle TimeStamp
				#ifdef Debug
					char _Time_Stamp[25];
					sprintf(_Time_Stamp, "%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", GSM.Time.Year, GSM.Time.Month, GSM.Time.Day, GSM.Time.Hour, GSM.Time.Minute, GSM.Time.Second);
					Terminal.Text(2, 3, CYAN, String(_Time_Stamp));
				#endif

				// Print Initialize Status
				#ifdef Debug
					if (GSM.Modem.Time_Status) Terminal.Text(23, 105, CYAN, String(F("Time Updated   ")));
				#endif

				// Socket Open
				if (GSM.Socket_Open()) {

					// Print Initialize Status
					#ifdef Debug
						Terminal.Text(23, 105, CYAN, String(F("Socket Opened  ")));
					#endif

				} else {
				
					#ifdef Debug
						Terminal.Text(23, 105, RED, String(F("Socket Error   ")));
					#endif
			
				}

			} else {
			
				#ifdef Debug
					Terminal.Text(23, 105, RED, String(F("Not Updated    ")));
				#endif
			
			}

		} else {
		
			#ifdef Debug
				Terminal.Text(23, 105, RED, String(F("Not Connected  ")));
			#endif
		
		}

	} else {
	
		#ifdef Debug
			Terminal.Text(23, 105, RED, String(F("Not Initialized")));
		#endif
	
	}

	// Interupt Definations
	cli();
	PCICR	|= 0b00000111;	// Set All Interrupt
	PCMSK1	|= 0b00001000;	// GSM Ring Interrupt Defination
	sei();

	Interrupt_Control = true;

}

void loop() {

	if (Interrupt) {

		// Set Interrupt Variable
		Interrupt_Control = false;

		// Get Command
		uint16_t Command = GSM.Socket_Answer();

		// Send Response
		GSM.Socket_Send("{\"Response\":{\"Event\":500,\"TimeStamp\":\"2022-03-17  09:06:32\"}}");

		// Set Interrupt Variable
		Interrupt = false;

		// Set Interrupt Variable
		Interrupt_Control = true;

	}

}

// GSM Ring Interrupt
ISR(PCINT1_vect) {

	// Set Interrupt Variable
	if (Interrupt_Control) Interrupt = true;

}
