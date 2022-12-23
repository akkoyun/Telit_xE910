// Serial Communications Definations
#define Serial_Terminal 			Serial
#define Serial_GSM 					Serial3






// Terminal Coordinates
#define GSM_Console_Initialize_X	(uint8_t) 5
#define GSM_Console_Initialize_Y 	(uint8_t) 4
#define GSM_Console_Detail_X		(uint8_t) 5
#define GSM_Console_Detail_Y 		(uint8_t) 82
#define GSM_Console_Connect_X 		(uint8_t) 5
#define GSM_Console_Connect_Y 		(uint8_t) 42
#define GSM_Console_Boot_X 			(uint8_t) 24
#define GSM_Console_Boot_Y 			(uint8_t) 10

// Define Libraries
#include <Telit_xE910.h>
#include <ArduinoJson.h>

// Define Object
xE910 GSM(Serial3);
xE910_Outgoing GSM_Out(Serial3, 3, "54.216.226.171", "/api/v1.1/p402");
xE910_Incoming GSM_IN(Serial3, 2);
Console Terminal(Serial);


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

	// GSM Begin
	GSM.Power(true);

	// Initialize Modem
	GSM.Initialize();
	
	// Connect Modem
	GSM.Connect();

	// Time Update
	GSM.RTC_Update();





	// Socket Config
	GSM_Out.Configure();

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
