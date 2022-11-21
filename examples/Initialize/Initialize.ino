// Define Console Parameters
#define CONSOLE_TEMPLATE_GE910
#define GSM_Debug
#define GSM_Boot_Bar_X (uint8_t) 24
#define GSM_Boot_Bar_Y (uint8_t) 10
#define GSM_Initialize_X (uint8_t) 5
#define GSM_Initialize_Y (uint8_t) 33
#define GSM_Detail_X (uint8_t) 4
#define GSM_Detail_Y (uint8_t) 80
#define GSM_Connection_X (uint8_t) 5
#define GSM_Connection_Y (uint8_t) 40
#define GSM_Connection_Detail_X (uint8_t) 12
#define GSM_Connection_Detail_Y (uint8_t) 80






// Define Libraries
#include <Telit_xE910.h>
#include <Console.h>
//#include <ArduinoJson.h>

// Define Object
Telit_xE910 GSM;
Console Terminal(Serial);
//GSM_Socket_Incoming Incoming_Socket(2);
//GSM_Socket_Outgoing Outgoing_Socket(3, "54.216.226.171", "/api/v1.1/p402");
//Console Terminal(Serial);

// Declare Global Variable
//bool Interrupt = false;
//uint32_t Timer_Counter = 0;
//bool Command_Clear = false;
//bool Listen_Control = false;
//bool Timer_Display = false;

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
	Terminal.Begin();
	Terminal.Telit_xE910();

	// GSM Begin
	GSM.Power(Enable);

	// Initialize Modem
	GSM.Initialize();
	
	// Connect Modem
	GSM.Connect();



	// Socket Config
//	Incoming_Socket.Configure();
//	Outgoing_Socket.Configure();

	// Listen Socket
//	Incoming_Socket.Listen(true);

	// Set Pin Change Interrupt Mask 1
//	PCICR |= (1 << PCIE1);
//	PCMSK1 |= (1 << PCINT11) | (1 << PCINT12);

//	AVR_Timer_1sn();

}

void loop() {

		Terminal.Text(2, 2, WHITE, String(millis()));

/*
	// Print Command State
	if (Command_Clear) {
		Terminal.Text(23, 115, CYAN, "   ");
		Command_Clear = false;
	}
	if (Listen_Control) {
		Terminal.Text(23, 115, CYAN, "1");
		Listen_Control = false;
	}

	if (Timer_Display) {
		Terminal.Text(2, 2, WHITE, String(millis()));
		Timer_Display = false;
	}
*/
/*
	if (Interrupt) {

		// Declare Variable
		char _Response1[50];

		// Get Command
		uint16_t Command = Incoming_Socket.Get(_Response1);

		// Command Handle
		if (Command == 157) Terminal.Beep();

		// Print Command State
		Terminal.Text(23, 115, CYAN, String(Command));

		// Command Delay
		delay(1000);

		// Data Send Variable Declaration
		char _Data[600] = "{\"Device\":{\"Type\":\"402-P01\",\"ID\":\"E10000011D00DB70\",\"Hardware\":\"03.00.00\",\"Firmware\":\"04.06.01\"},\"Power\":{\"Battery\":{\"IV\":4.13,\"T\":28.4,\"AC\":0.16,\"SOC\":80.41,\"FB\":2000,\"IB\":1200,\"Charge\":3}},\"IoT\":{\"GSM\":{\"Module\":{\"Manufacturer\":1,\"Model\":1,\"Firmware\":\"13.00.007\",\"Serial\":\"0001770243\",\"IMEI\":\"353613080366878\"},\"SIM\":{\"SIM_Type\":1,\"Iccid\":\"8990011916180288944\"},\"Operator\":{\"Code\":28601,\"RSSI\":8}}},\"Data\":{\"DeviceStatus\":240,\"FaultStatus\":500,\"TimeStamp\":\"2022-06-06 11:12:54\",\"Temperature\":28.78915,\"Humidity\":34.7837}}";
		char _Response2[50];
		memset(_Response2, '\0', 50);

		// Send Data Pack
		uint16_t Send_Response = Outgoing_Socket.Send(_Data, _Response2);

		// Print Command State
		Terminal.Text(23, 115, CYAN, String(Send_Response));

		// Command Delay
		delay(1000);

		// Set Interrupt Variable
		Interrupt = false;

	}
*/

}

/*
void AVR_Timer_1sn(void) {

	// Timer-0 : 8-bit		- Delay, Millis, Micros, AnalogWrite(5,6)
	// Timer-1 : 16-bit		- Servo functions, AnalogWrite(9,10)
	// Timer-2 : 8-bit		- Tone, AnalogWrite(3,11)
	// Timer-3 : 16-bit
	// Timer-4 : 16-bit
	// Timer-5 : 16-bit	 	- Module Timer

	// Set Timer Interval 1 Sn
	uint8_t _Interval = 1;

	// Clear Registers
	TCCR5A = 0x00;
	TCCR5B = 0x00;

	// Clear Counter
	TCNT5 = 0;

	// Set Counter Value
	OCR5A = (F_CPU / ((1 / _Interval) * 1024)) - 1;

	// Set CTC Mod
	TCCR5B |= (1 << WGM52);

	// Set Rescale (1024)
	TCCR5B |= (1 << CS52) | (1 << CS50);

	// Start Timer
	TIMSK5 |= (1 << OCIE5A);

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

// Timer Interrupt
ISR(TIMER5_COMPA_vect) {

	// Set Timer Counter
	Timer_Counter += 1;

	// Handle Max
	if (Timer_Counter == 65534) Timer_Counter = 0;

	if (Timer_Counter % 5) Command_Clear = true;

	if (Timer_Counter % 60) Listen_Control = true;

	if (Timer_Counter % 10) Timer_Display = true;

}
*/