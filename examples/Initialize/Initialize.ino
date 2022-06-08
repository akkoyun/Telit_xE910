// Define Ovoo Libraries
#include "Telit_xE910.h"
#include <ArduinoJson.h>

// Define Object
Telit_xE910 GSM;
GSM_Socket_Incomming Incomming_Socket(2);
GSM_Socket_Outgoing Outgoing_Socket(3, "54.216.226.171", "/api/v1.1/p402");

// Declare Global Variable
bool Interrupt = false;

void setup() {

	/*  PORT J
		PJ0 - RXD3
		PJ1 - TXD3
		PJ2 - Input / Pull Down [GSM Ring]		- PCINT11	- [-]
		PJ3 - Input / Pull Down [GSM PwrMon]	- PCINT12	- [-]
		PJ4 - Output / Pull Down [GSM CommEn]	- PCINT13	- [-]
		PJ5 - Output / Pull Down [GSM ShtDwn]	- PCINT14	- [-]
		PJ6 - Output / Pull Down [GSM On Off]	- PCINT15	- [-]
		PJ7 - Output / Pull Down [NC]			-  			- [-]
	*/
	DDRJ &= 0b11110011; PORTJ |= 0b00000100;
	DDRJ |= 0b11110000; PORTJ &= 0b00000111;

	// Start GSM Serial
	Serial3.begin(115200);

	// Start Terminal
	Serial.begin(115200);

	// Start Console
	Terminal.Begin(Serial);
	Terminal.Telit_xE910();

	// GSM Begin
	GSM.Power(true);

	// Initialize Modem
	GSM.Initialize();
	
	// Connect Modem
	GSM.Connect();

	// Socket Config
	Incomming_Socket.Configure();
	Outgoing_Socket.Configure();

	// Listen Socket
	Incomming_Socket.Listen(true);

	// Time Update
	GSM.Time_Update();

	// Set Pin Change Interrupt Mask 1
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT12);

}

void loop() {

	// Print Command State
	if (millis() % 2 == 0) Terminal.Text(23, 115, CYAN, "   ");

	if (Interrupt) {

		// Get Command
		uint16_t Command = Incomming_Socket.Get();

		// Command Handle
		if (Command == 157) Terminal.Beep();

		// Print Command State
		Terminal.Text(23, 115, CYAN, String(Command));

		// Command Delay
		delay(1000);

		// Data Send Variable Declaration
		char _Data[600] = "{\"Device\":{\"Type\":\"402-P01\",\"ID\":\"E10000011D00DB70\",\"Hardware\":\"03.00.00\",\"Firmware\":\"04.06.01\"},\"Power\":{\"Battery\":{\"IV\":4.13,\"T\":28.4,\"AC\":0.16,\"SOC\":80.41,\"FB\":2000,\"IB\":1200,\"Charge\":3}},\"IoT\":{\"GSM\":{\"Module\":{\"Manufacturer\":1,\"Model\":1,\"Firmware\":\"13.00.007\",\"Serial\":\"0001770243\",\"IMEI\":\"353613080366878\"},\"SIM\":{\"SIM_Type\":1,\"Iccid\":\"8990011916180288944\"},\"Operator\":{\"Code\":28601,\"RSSI\":8}}},\"Data\":{\"DeviceStatus\":240,\"FaultStatus\":500,\"TimeStamp\":\"2022-06-06 11:12:54\",\"Temperature\":28.78915,\"Humidity\":34.7837}}";
		char _Response[50];
		memset(_Response, '\0', 50);

		// Send Data Pack
		uint16_t Send_Response = Outgoing_Socket.Send(_Data, _Response);

		// Print Command State
		Terminal.Text(23, 115, CYAN, String(Send_Response));

		// Command Delay
		delay(1000);

		// Set Interrupt Variable
		Interrupt = false;

	}

}

// GSM Ring Interrupt
ISR(PCINT1_vect) {

		// Control Ring Interrupt [PJ2]
		if ((PINJ & (1 << PINJ2)) == (1 << PINJ2)) {
			
			// Set Interrupt Variable
			Interrupt = true;

			// Interrupt Delay
			delay(75);

		} else {
			
			// Set Interrupt Variable
			Interrupt = false;

		}

}
