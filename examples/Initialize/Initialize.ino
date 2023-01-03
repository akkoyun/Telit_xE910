// Define Libraries
#include "Terminal_Variables.h"
#include <Telit_xE910.h>
#include <ArduinoJson.h>
#include <Console.h>

// Define Functions
void Device_Data_CallBack();
void Send_CallBack(uint16_t);
void Command_CallBack(uint16_t);

// Define Console
Console Terminal(Serial_Terminal);

// Set PostOffice Cloud API
PostOffice Postoffice(Serial3);







// Declare Global Variable
uint32_t Timer_Counter = 0;
bool Timer_Display = false;
bool Timer_Send = false;
bool GSM_Ring = false;




void setup() {

	// Pin Definitions
	DDRJ &= 0b11110011; PORTJ |= 0b00000100;
	DDRJ |= 0b11110000; PORTJ &= 0b00000111;

	// Start Terminal
	Serial.begin(115200);

	// Start Console
	Terminal.Begin();
	Terminal.Telit_xE910();




	// Power ON GSM Modem
	Postoffice.Power(false);


	// Set CallBacks
	Postoffice.Device_Data_CallBack(Device_Data_CallBack);
	Postoffice.Payload_Data_CallBack(Payload_Data_CallBack);
	Postoffice.PackSend_CallBack(Send_CallBack);
	Postoffice.Request_CallBack(Command_CallBack);







	// Power ON GSM Modem
	Postoffice.Power(true);

	// Initialize Modem
	Postoffice.Initialize();
	Postoffice.Connect();





	// Set PostOffice
	Postoffice.Online("70A11D1D01000026");


	
	// Environment Structure Define
	struct Environment_Struct {
		float Temperature;
		float Humidity;
	} Environment;




	// Set Pin Change Interrupt Mask 1
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT12);

	// Set 1sec Timer
	AVR_Timer_1sn();

	// Start Interrupts
	sei();

}

void loop() {

	// Update Timer
	if (Timer_Display) {

		// Update Timer
		Terminal.Text(2, 13, BLUE, String(Timer_Counter));

		// Release Interrupt
		Timer_Display = false;

		// Update Timer
		Terminal.Text(2, 95, BLUE, String(Postoffice.Time.Hour));
		Terminal.Text(2, 97, BLUE, String(":"));
		Terminal.Text(2, 98, BLUE, String(Postoffice.Time.Minute));
		Terminal.Text(2, 100, BLUE, String(":"));
		Terminal.Text(2, 101, BLUE, String(Postoffice.Time.Second));

	}

	// Send Timer
	if (Timer_Send) {

		Postoffice.Send(Pack_Online);

		// Release Interrupt
		Timer_Send = false;

	}


	if (GSM_Ring) {
		
		Postoffice.Get();

		GSM_Ring = false;
	}

}


// Timer Interrupt
ISR(TIMER5_COMPA_vect) {

	// Set Timer Counter
	Timer_Count();

	// Activate Timer Interrupt
	if (Timer_Control(1)) Timer_Display = true;

	// Data Send Timer Interrupt
	if (Timer_Control(60)) Timer_Send = true;

}

// GSM Ring Interrupt
ISR(PCINT1_vect) {

	// Control Ring Interrupt [PJ2]
	if ((PINJ & (1 << PINJ2)) == (1 << PINJ2)) {
		
		// Set Interrupt Variable
		GSM_Ring = true;

		// Interrupt Delay
		delay(75);

	} else {
		
		// Set Interrupt Variable
		GSM_Ring = false;

	}

}

// Timer Functions
void Timer_Count(void) {

	// Set Timer Counter
	Timer_Counter += 1;

	// Handle Max
	if (Timer_Counter == 65534) Timer_Counter = 0;

}
bool Timer_Control(const uint16_t _Interval) {

	// Timer Counter
	if (Timer_Counter % _Interval == 0) {

		// End Function
		return(true);

	} else {

		// End Function
		return(false);

	}

}

// AVR 1sec Timer
void AVR_Timer_1sn(void) {

	// Clear Registers
	TCCR5A = 0x00;
	TCCR5B = 0x00;

	// Clear Counter
	TCNT5 = 0;

	// Set Counter Value
	OCR5A = (F_CPU / (1024)) - 1;

	// Set CTC Mod
	TCCR5B |= (1 << WGM52);

	// Set Rescale (1024)
	TCCR5B |= (1 << CS52) | (1 << CS50);

	// Start Timer
	TIMSK5 |= (1 << OCIE5A);

}

// PostOffice Call Back Functions
void Send_CallBack(uint16_t _Response) {

	// Terminal Beep
	Terminal.Beep();

	// Control for Command
	if (_Response == 200) {
		Terminal.Text(21, 108, GREEN, "Sended   ");
		delay(1000);
	} else {
		Terminal.Text(21, 108, RED, "Failed    ");
		delay(1000);
	}

	Terminal.Text(21, 108, GREEN, "         ");

}
void Command_CallBack(uint16_t _Command, char * _Pack) {

	// Terminal Beep
	Terminal.Beep();

	Terminal.Text(21, 108, GREEN, "         ");
	Terminal.Text(21, 110, YELLOW, String(_Command));

	Postoffice.Response(200, "Deneme");

}
void Device_Data_CallBack(void) { 

	// Print Text
	Terminal.Text(21, 108, YELLOW, "Data Upd.");

	// Set Data Pack
	Postoffice.Environment(22.22, 33.33);

	Postoffice.Battery(1, 2, 3, 3, 11, 1200, 1000);

	// Print Text
	Terminal.Text(21, 108, YELLOW, "         ");

}
void Payload_Data_CallBack(void) {

	// Set Payload Data
	Postoffice.TimeStamp("2022-03-23 14:18:28");
	Postoffice.SetStatus(240, 500);

}
