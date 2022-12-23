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

// Define Object
xE910 GSM(Serial_GSM);
xE910_RTC GSM_RTC;



xE910_Cloud Cloud("54.216.226.171", "/api/v1.1/p402");




xE910_Incoming GSM_IN(2);









// Declare Global Variable
bool Interrupt = false;
uint32_t Timer_Counter = 0;
bool Command_Clear = false;
bool Listen_Control = false;
bool Timer_Display = false;








void setup() {

	// Pin Definitions
	DDRJ &= 0b11110011; PORTJ |= 0b00000100;
	DDRJ |= 0b11110000; PORTJ &= 0b00000111;

	// Set Pin Change Interrupt Mask 1
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT12);

	// Start GSM Serial
	Serial3.begin(115200);

	// Start Terminal
	Serial.begin(115200);

	// Start Console
	Terminal.Begin();
	Terminal.Telit_xE910();





	// Power ON GSM Modem
	GSM.Power(true);

	// Initialize Modem
	GSM.Initialize();
	
	// Connect Modem
	GSM.Connect();

	// Time Update
	GSM_RTC.RTC_Update();

	// Socket Config
	//GSM_Out.Configure();

	// Listen Socket
	GSM_IN.Configure();






	// Set 1sec Timer
	AVR_Timer_1sn();

	// Start Interrupts
	sei();

}

void loop() {

	Terminal.Text(2, 13, WHITE, String(Timer_Counter));

}

// AVR 1sec Timer
void AVR_Timer_1sn(void) {

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

	if (Timer_Counter % 1) Timer_Display = true;

}
