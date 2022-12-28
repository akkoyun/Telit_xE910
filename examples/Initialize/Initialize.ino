// Serial Communications Definations
#define Serial_Terminal 			Serial
#define Serial_GSM 					Serial3

// Define Libraries
#include "Terminal_Variables.h"
#include <Telit_xE910.h>
#include <ArduinoJson.h>
#include <Console.h>

// Define Console
Console Terminal(Serial_Terminal);

// Set PostOffice Cloud API
PostOffice Postoffice(Serial_GSM);

// Define Object
xE910_RTC GSM_RTC;


// Declare Global Variable
uint32_t Timer_Counter = 0;
bool Timer_Display = false;
bool Timer_Send = false;







void setup() {

	// Pin Definitions
	DDRJ &= 0b11110011; PORTJ |= 0b00000100;
	DDRJ |= 0b11110000; PORTJ &= 0b00000111;

	// Start GSM Serial
	Serial3.begin(115200);

	// Start Terminal
	Serial.begin(115200);

	// Start Console
	Terminal.Begin();
	Terminal.Telit_xE910();





	// Power ON GSM Modem
	Postoffice.Power(true);

	// Initialize Modem
	Postoffice.Initialize();
	Postoffice.Online();

	// Time Update
	GSM_RTC.Sync();

	// Set PostOffice
	Postoffice.Connect("70A11D1D01000026");
	Postoffice.Environment(22.22, 33.33);
	Postoffice.Battery(4.12, 0.34, 99.98, 3, 30.30, 2000, 1200);
	Postoffice.TimeStamp("2022-03-23 14:18:28");
	Postoffice.Status(240, 500);

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
		Terminal.Text(2, 95, BLUE, String(GSM_RTC.Time.Hour));
		Terminal.Text(2, 97, BLUE, String(":"));
		Terminal.Text(2, 98, BLUE, String(GSM_RTC.Time.Minute));
		Terminal.Text(2, 100, BLUE, String(":"));
		Terminal.Text(2, 101, BLUE, String(GSM_RTC.Time.Second));

	}

	// Send Timer
	if (Timer_Send) {

		if (Postoffice.Send(Pack_Online)) {
			Terminal.Text(21, 108, GREEN, "Sended   ");
			delay(1000);
		} else {
			Terminal.Text(21, 108, RED, "Failed    ");
			delay(1000);
		}

		Terminal.Text(21, 108, GREEN, "         ");

		// Release Interrupt
		Timer_Send = false;

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
