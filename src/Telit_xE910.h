#ifndef __Telit_xE910__
#define __Telit_xE910__

	// Define Arduino Library
	#ifndef __Arduino__
		#include <Arduino.h>
	#endif

	// Define Library Includes
	#ifndef __Telit_xE910_Includes__
		#include "Includes.h"
	#endif

	// Define Cloud Parameters
	#ifndef PostOffice_Server
		#define PostOffice_Server "165.227.154.147"
	#endif
	#ifndef PostOffice_EndPoint
		#define PostOffice_EndPoint "/"
	#endif

	// Define Pack Type
	#define Pack_Online 1
	#define Pack_Timed 2
	#define Pack_Interrupt 3
	#define Pack_Alarm 4
	#define Pack_Offline 5

	// Define JSON Handler
	#include <ArduinoJson.h>

	/**
	 * @brief GSM Serial Stream Definition
	 */
	Stream * GSM_Serial;

	// Define Modem Structure
	struct Struct_Modem {
		uint8_t 	PIN;
		uint8_t 	Connection_Time;
		char 		IMEI[17];
		char 		Serial_ID[11]; 
		char 		ICCID[21];
		uint8_t 	Manufacturer;
		uint8_t 	Model;
		char 		Firmware[10];
		uint8_t 	dBm;
		uint8_t		Signal;
		char 		IP_Address[16];
		uint16_t 	Operator;
		char 		LAC[5];
		char 		Cell_ID[5];
	} Modem;

	// Modem Hardware Class
	class xE910_Hardware {

		public:

			// Define Modem Status Structure
			struct Hardware_Status {
				bool Power = false;
			} Hardware_Status;

			/**
			 * @brief Get Power Monitor
			 * @return true Modem Powered
			 * @return false Modem is not Powered
			 */
			bool PowerMonitor(void) {

				// Control for PWMon (PJ3)
				if ((PINJ & (1 << PINJ3)) == (1 << PINJ3)) {

					// Response Delay
					delay(10);

					// Power Monitor 3V3 HIGH
					return (true);

				} else {

					// Response Delay
					delay(10);

					// Power Monitor 3V3 LOW
					return (false);

				}

			}

			/**
			 * @brief Enable Communication Buffer.
			 * @param _Function 
			 * _GET_ (0) : Get Buffer State.
			 * _SET_ (1) : Set Buffer State.
			 * @param _State 
			 * true : Enable Buffer.
			 * false : Disable Buffer.
			 * @return true Buffer is Enabled.
			 * @return false Buffer is Disabled.
			 */
			void Communication(const bool _State) {

				// Enable Communication 
				if (_State) PORTJ &= 0b11101111;

				// Disable Communication
				if (!_State) PORTJ |= 0b00010000;

			}

			/**
			 * @brief On or Off Modem.
			 * @param _Time On/Off Time
			 */
			void OnOff(const uint16_t _Time) {

				// Set On/Off Signal HIGH [PJ6]
				PORTJ |= 0b01000000;

				// Command Delay
				for (uint8_t i = 0; i < 100; i++) {

					// Calculate Delay (2000)
					uint8_t _Delay = _Time / 100;

					// Terminal Bar
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_Console_Boot_X, GSM_Console_Boot_Y + i, WHITE, F("▒"));
					#endif

					// Wait
					delay(_Delay); 

				}

				// Set On/Off Signal LOW [PJ6]
				PORTJ &= 0b10111111;

				// Clear Bar
				#ifdef GSM_Debug
					for (uint8_t i = 0; i < 100; i++) Terminal_GSM.Text(GSM_Console_Boot_X, GSM_Console_Boot_Y + i, WHITE, F(" "));
				#endif

			}

			/**
			 * @brief ShutDown Modem
			 * @param _Time ShutDown Time
			 */
			void ShutDown(const uint16_t _Time) {

				// Set Shut Down Signal HIGH [PJ5]
				PORTJ |= 0b00100000;

				// Command Delay
				delay(_Time);

				// Set Shut Down Signal LOW [PJ5]
				PORTJ &= 0b11011111;

			}

			/**
			 * @brief Power Switch
			 * @param _State Switch State
			 */
			void Power_Switch(const bool _State) {

				// Set GSM Power Enable
				if (_State) PORTH |= 0b00000100;

				// Set GSM Power Disable
				if (!_State) PORTH &= 0b11111011;
			
			}

			/**
			 * @brief LED Power
			 * @param _State LED State
			 */
			void LED(const bool _State) {

				// Set GSM LED Power Enable
				if (_State) PORTH &= 0b11101111;

				// Set GSM LED Power Disable
				if (!_State) PORTH |= 0b00010000;

			}

			/**
			 * @brief Power on Sequence of Modem
			 * @param _Power_Switch Power Switch State
			 * @param _LED_Switch  LED Stat
			 * @param _Communication_Switch Communication State
			 * @return true Modem is ON
			 * @return false Modem is OFF
			 */
			bool ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

				// Send Shut Down Signal
				this->ShutDown(500);

				// Enable GSM Modem Power Switch
				if (_Power_Switch) this->Power_Switch(true);  
				
				// Enable GSM Modem LED Feed
				if (_LED_Switch) this->LED(true);

				// Set Communication Signal LOW
				if (_Communication_Switch) this->Communication(true);
				
				// Turn On Modem
				if (this->PowerMonitor()) {

					// End Function
					return (true);

				} else {

					// Send On Off Signal
					this->OnOff(5000);

					// Control for PWMon (PH7)
					if (this->PowerMonitor()) {

						// End Function
						return(true);

					} else {

						// Send Shut Down Signal
						this->ShutDown(200);

					}

				}

				// End Function
				return (false);

			}

			/**
			 * @brief Power on Sequence of Modem
			 * @param _Power_Switch Power Switch State
			 * @param _LED_Switch  LED Stat
			 * @param _Communication_Switch Communication State
			 * @return true Modem is OFF
			 * @return false Modem is ON
			 */
			bool OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

				// Turn Off Modem
				if (this->PowerMonitor()) this->ShutDown(200);

				// Disable GSM LED Power
				if (_LED_Switch) this->LED(false);

				// Disable GSM Modem Voltage Translator
				if (_Communication_Switch) this->Communication(false);

				// Disable GSM Modem Main Power Switch
				if (_Power_Switch) this->Power_Switch(false);

				// Command Delay
				delay(1000);

				// End Function
				return (true);

			}

	};

	// Modem AT Command Set Class
	class AT_Command_Set {

		public:

			/**
			 * @brief GSM Serial Buffer Object Definition.
			 */
			struct Serial_Buffer {
				bool Response;
				uint8_t Read_Order;
				uint8_t Data_Order;
				const uint32_t Time_Out;
			};

			/**
			 * @brief Clear Serial Buffer Function
			 */
			void Clear_UART_Buffer(void) {

				// Clear UART Buffer
				while (GSM_Serial->available() > 0) {
					
					// Read GSM Buffer
					GSM_Serial->read();

					// Read Delay
					delay(1);

				}

			}

			/**
			 * @brief AT Command
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool AT(void) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables the command echo.
			 * @param _ECHO Parameter. false:Disable Command Echo, true:Enable Command Echo
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool ATE(const bool _ECHO) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("ATE"));
				GSM_Serial->print(_ECHO);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables the report of result code.
			 * @param _CMEE Parameter. 0:Disable, 1:Numeric Format, 2:Verbose Format
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CMEE(const uint8_t _CMEE) {

				// Control for Parameter
				if (_CMEE > 2) return(false);

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CMEE="));
				GSM_Serial->print(_CMEE);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command sets the wireless module in specified connection mode (data, fax, voice), hence all the calls done afterwards will be data or voice.
			 * @param _FCLASS Parameter. 0:Data, 1:Fax, 8:Voice
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool FCLASS(const uint8_t _FCLASS) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+FCLASS="));
				GSM_Serial->print(_FCLASS);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command controls the RS232 flow control behavior.
			 * @param _K Parameter. 
			 * 0 : No flow control
			 * 1 : Hardware mono-directional flow control
			 * 2 : Software mono-directional flow control
			 * 3 : Hardware bi-directional flow control
			 * 4 : Software bi-directional with filtering
			 * 5 : Pass through: software bi-directional without filtering
			 * 6 : Both hardware bi-directional flow control
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool K(const uint8_t _K) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT&K"));
				GSM_Serial->print(_K);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Read command reports the PIN/PUK/PUK2 request status of the device.
			 * @param _Status Return Code
			 * 0 : SIM Error 
			 * 1 : SIM Ready
			 * 2 : SIM PIN Required
			 * 3 : SIM PUK Required
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CPIN(uint8_t & _Status) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CPIN?"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Variable
				_Status = 0;

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Control for READY
					if (
						Buffer_Variable[i - 4] == 'R' and
						Buffer_Variable[i - 3] == 'E' and
						Buffer_Variable[i - 2] == 'A' and
						Buffer_Variable[i - 1] == 'D' and
						Buffer_Variable[i - 0] == 'Y'
					) _Status = 1;

					// Control for SIM PIN
					if (
						Buffer_Variable[i - 6] == 'S' and
						Buffer_Variable[i - 5] == 'I' and
						Buffer_Variable[i - 4] == 'M' and
						Buffer_Variable[i - 3] == ' ' and
						Buffer_Variable[i - 2] == 'P' and
						Buffer_Variable[i - 1] == 'I' and
						Buffer_Variable[i - 0] == 'N'
					) _Status = 2;

					// Control for SIM PUK
					if (
						Buffer_Variable[i - 6] == 'S' and
						Buffer_Variable[i - 5] == 'I' and
						Buffer_Variable[i - 4] == 'M' and
						Buffer_Variable[i - 3] == ' ' and
						Buffer_Variable[i - 2] == 'P' and
						Buffer_Variable[i - 1] == 'U' and
						Buffer_Variable[i - 0] == 'K'
					) _Status = 3;

				}

				// Handle Status
				if (_Status == 1) return(true);

				// End Function
				return(false);

			}

			/**
			 * @brief Query SIM Status unsolicited indication
			 * @param _Mode 
			 * @param _Status 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool QSS(uint8_t & _Mode, uint8_t & _Status) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#QSS?"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// #QSS: 0,1OK

				// Handle Response
				_Mode = Buffer_Variable[6];
				_Status = Buffer_Variable[8];

				// End Function
				return(false);

			}

			/**
			 * @brief Returns the product serial number, identified as the IMEI of the modem.
			 * @param _IMEI IMEI Number
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CGSN(char * _IMEI) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGSN"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

					// Response Wait Delay
					delay(2);

				}

				// Clear Variable
				memset(_IMEI, '\0', 17);

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Handle IMEI
					if (Buffer_Variable[i] > 47 and Buffer_Variable[i] < 58) {

						// Set IMEI Variable
						_IMEI[Buffer.Data_Order] = Buffer_Variable[i];

						// Set Data Order
						Buffer.Data_Order++;

					}

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Execution command returns the device board serial number.
			 * @param _Serial_ID Modem Serial Number
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool GSN(char * _Serial_ID) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+GSN"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Clear Variable
				memset(_Serial_ID, '\0', 11);

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Handle IMEI
					if (Buffer_Variable[i] > 47 and Buffer_Variable[i] < 58) {

						// Set IMEI Variable
						_Serial_ID[Buffer.Data_Order] = Buffer_Variable[i];

						// Set Data Order
						Buffer.Data_Order++;

					}

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Reads on SIM the ICCID.
			 * @param _ICCID SIM Serial Number
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CCID(char * _ICCID) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#CCID"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

					// Response Wait Delay
					delay(2);

				}

				// Clear Variable
				memset(_ICCID, '\0', 21);

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Handle IMEI
					if (Buffer_Variable[i] > 47 and Buffer_Variable[i] < 58) {

						// Set IMEI Variable
						_ICCID[Buffer.Data_Order] = Buffer_Variable[i];

						// Set Data Order
						Buffer.Data_Order++;

					}

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Returns the manufacturer identification.
			 * @param _Manufacturer Modem manufacturer
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool GMI(uint8_t & _Manufacturer) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+GMI"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Variable
				_Manufacturer = 0;

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Control for TELIT
					if (
						Buffer_Variable[i - 4] == 'T' and
						Buffer_Variable[i - 3] == 'e' and
						Buffer_Variable[i - 2] == 'l' and
						Buffer_Variable[i - 1] == 'i' and
						Buffer_Variable[i - 0] == 't'
					) _Manufacturer = 1;

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Returns the model identification.
			 * @param _Model Modem model.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool GMM(uint8_t & _Model) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+GMM"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Variable
				_Model = 0;

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Control for GE910-QUAD
					if (
						Buffer_Variable[i - 9] == 'G' and
						Buffer_Variable[i - 8] == 'E' and
						Buffer_Variable[i - 7] == '9' and
						Buffer_Variable[i - 6] == '1' and
						Buffer_Variable[i - 5] == '0' and
						Buffer_Variable[i - 4] == '-' and
						Buffer_Variable[i - 3] == 'Q' and
						Buffer_Variable[i - 2] == 'U' and
						Buffer_Variable[i - 1] == 'A' and
						Buffer_Variable[i - 0] == 'D'
					) _Model = 1;

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Returns the software revision identification.
			 * @param _Firmware Modem firmware.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool GMR(char * _Firmware) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+GMR"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Clear Variable
				memset(_Firmware, '\0', 10);

				// Handle for Response
				for (size_t i = 0; i < 255; i++) {

					// Handle IMEI
					if ((Buffer_Variable[i] > 47 and Buffer_Variable[i] < 58) or Buffer_Variable[i] == '.') {

						// Set IMEI Variable
						_Firmware[Buffer.Data_Order] = Buffer_Variable[i];

						// Set Data Order
						Buffer.Data_Order++;

					}

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Set command sets the behavior of the STAT_LED GPIO.
			 * @param _SLED Parameter
			 * 0 : GPIO tied Low
			 * 1 : GPIO tied High
			 * 2 : GPIO handled by Module Software
			 * 3 : GPIO is turned on and off alternatively
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SLED(const uint8_t _SLED) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SLED="));
				GSM_Serial->print(_SLED);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables the Ring Indicator pin response to a Socket Listen connect and, if enabled, the duration of the negative going pulse generated on receipt of connect.
			 * @param _Pulse_Duration Parameter
			 * 0 : RI disabled for Socket Listen connect 
			 * 50 - 1150 : RI enabled for Socket Listen connect
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool E2SLRI(const uint16_t _Pulse_Duration) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#E2SLRI="));
				GSM_Serial->print(_Pulse_Duration);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables the Ring Indicator pin response to one or more events. 
			 * If an event has been enabled, a negative going pulse is generated when event happens. 
			 * The duration of this pulse is determined by the value of <duration>.
			 * @param _Event_Mask Parameter
			 * 0 – disables all events hexadecimal number representing the list of events: Mode (same as AT#PSMRI=<duration>)
			 * 1 – Power Saving
			 * 2 – Socket Listen (same as AT#E2SLRI=<duration>)
			 * 4 – OTA firmware upgrade (same as AT#OTASETRI=<duration>)
			 * 8 – MT SMS has been received (same as AT#E2SMSRI=<duration>) 10 – +CREG will change status
			 * 20 – +CGREG will change status
			 * 40 – #QSS become 2 (SIM INSERTED and PIN UNLOCKED)
			 * 80 – MO SMS has been delivered
			 * 100 – Jamming Detection & Reporting (JDR)
			 * @param _Pulse_Duration Parameter
			 * 50 - 1150 : RI enabled for Socket Listen connect
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool E2RI(const uint8_t _Event_Mask, const uint16_t _Pulse_Duration) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#E2RI="));
				GSM_Serial->print(_Event_Mask);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Pulse_Duration);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief There are situations in which the presentation of the URCs controlled by either +CREG and +CGREG are slightly different from ETSI specifications.
			 * @param _REGMODE Parameter
			 * 0 : Basic operation mode
			 * 1 : Enhanced operation mode
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool REGMODE(const uint8_t _REGMODE) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#REGMODE="));
				GSM_Serial->print(_REGMODE);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set TXMON pin behavior.
			 * @param _TXMONMODE Parameter
			 * 0 : TXMON pin goes high when a call is started and it drops down when the call is ended. 
			 * It also goes high when a location update starts, and it drops down when the location 
			 * update procedure stops. Finally it goes high during SMS transmission and receiving. 
			 * Even if the TXMON in this case is set as GPIO in output, the read command AT#GPIO=5,2 
			 * returns gPIO:2,0, as the GPIO is in alternate mode.
			 * 1 : TXMON is set in alternate mode and the Timer unit controls its state. TXMON goes 
			 * high before power ramps start raising and drops down after power ramps stop falling down. 
			 * This behavior is repeated for every transmission burst.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool TXMONMODE(const uint8_t _TXMONMODE) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#TXMONMODE="));
				GSM_Serial->print(_TXMONMODE);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables the automatic band selection at power-on.
			 * @param _Mode Parameter
			 * 0 - disables automatic band selection at next power-up
			 * 1 - enables automatic band selection at next power-up; the automatic band selection stops as soon as a GSM cell is found (deprecated).
			 * 2 – enables automatic band selection in four bands (at 850/1900 and 900/1800); differently from previous settings it takes immediate effect 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool AUTOBND(const uint8_t _Mode) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#AUTOBND="));
				GSM_Serial->print(_Mode);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command causes the TA to return a numeric code in the format
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CEER(uint16_t & _Code) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#CEER"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Serial.println(Buffer_Variable);

				// Handle Variables
				uint8_t _Variable_Count = sscanf(Buffer_Variable, "#CEER: %03dOK", &_Code);

				// Control for Variable
				if (_Variable_Count == 1) return(true);

				// End Function
				return(false);


			}

			/**
			 * @brief Set command forces an attempt to select and register the GSM network operator.
			 * @param _Mode Parameter
			 * 0 - automatic choice (the parameter <oper> will be ignored) (factory default)
			 * 1 - manual choice unlocked (network is kept as long as available, then it can be changed with some other suited networks to guarantee the service)
			 * 2 - deregister from GSM network; the MODULE is kept unregistered until a +COPS with <mode>=0, 1, 4 or 5 is issued 
			 * 3 - set only <format> parameter (the parameter <oper> will be ignored)
			 * 4 - manual/automatic (<oper> field shall be present); if manual selection fails, automatic mode (<mode>=0) is entered
			 * 5 - manual choice locked (network is kept fixed, if the chosen network is not available, then the mobile has no service)
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool COPS(const uint8_t _Mode, const uint8_t _Format, const uint16_t _Operator) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 30000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+COPS="));
				GSM_Serial->print(_Mode);
				if (_Format != 99 or _Operator != 99) {
					GSM_Serial->print(F(","));
					GSM_Serial->print(_Format);
					GSM_Serial->print(F(","));
					GSM_Serial->print(_Operator);
				}
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables network registration reports depending on the parameter <mode>.
			 * @param _Mode Parameter
			 * 0 - Disable network registration unsolicited result code (factory default) 
			 * 1 - Enable network registration unsolicited result code
			 * 2 - Enable network registration unsolicited result code with network Cell identification data
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Set_CREG(const bool _Mode) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CREG="));
				GSM_Serial->print(_Mode);
				GSM_Serial->print(F("\r\n"));

				// Response Wait Delay
				delay(10);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Get command registration reports depending on the parameter <stat>.
			 * @param _Stat Parameter
			 * 0 - Not registered, ME is not currently searching a new operator to register to 
			 * 1 - Registered, home network
			 * 2 - Not registered, but ME is currently searching a new operator to register to 
			 * 3 - Registration denied
			 * 4 - Unknown
			 * 5 - Registered, roaming
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Get_CREG(uint8_t & _Mode, uint8_t & _Stat) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CREG?"));
				GSM_Serial->print(F("\r\n"));

				// Response Wait Delay
				delay(10);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Handle Variables
				sscanf(Buffer_Variable, "+CREG: %01d,%01dOK", &_Mode, &_Stat);

				// Handle Response
				if (_Stat == 0) return(true);
				if (_Stat == 1) return(true);
				if (_Stat == 2) return(true);
				if (_Stat == 3) return(true);
				if (_Stat == 4) return(true);
				if (_Stat == 5) return(true);

				// End Function
				return (false);

			}

			/**
			 * @brief Set command controls the presentation of an unsolicited result code
			 * @param _Mode Parameter
			 * 0 - disable network registration unsolicited result code
			 * 1 - enable network registration unsolicited result code; if there is a change in the terminal GPRS network registration status, it is issued the unsolicited result
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Set_CGREG(const bool _Mode) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGREG="));
				GSM_Serial->print(_Mode);
				GSM_Serial->print(F("\r\n"));

				// Response Wait Delay
				delay(10);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Get command controls the presentation of an unsolicited result code.
			 * @param _Stat Parameter
			 * 0 - Not registered, ME is not currently searching a new operator to register to 
			 * 1 - Registered, home network
			 * 2 - Not registered, but ME is currently searching a new operator to register to 
			 * 3 - Registration denied
			 * 4 - Unknown
			 * 5 - Registered, roaming
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Get_CGREG(uint8_t & _Mode, uint8_t & _Stat) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGREG?"));
				GSM_Serial->print(F("\r\n"));

				// Response Wait Delay
				delay(10);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Handle Variables
				sscanf(Buffer_Variable, "+CGREG: %01d,%01dOK", &_Mode, &_Stat);

				// Handle Response
				if (_Stat == 0) return(true);
				if (_Stat == 1) return(true);
				if (_Stat == 2) return(true);
				if (_Stat == 3) return(true);
				if (_Stat == 4) return(true);
				if (_Stat == 5) return(true);

				// End Function
				return (false);

			}

			/**
			 * @brief Set command specifies PDP context parameter values for a PDP context identified by the (local) context identification parameter, <cid>
			 * @param _Cid Parameter
			 * (PDP Context Identifier) numeric parameter which specifies a particular PDP context definition.
			 * @param _PDP_Type Parameter
			 * (Packet Data Protocol type) a string parameter which specifies the type of packet data protocol 
			 * "IP" - Internet Protocol
			 * @param _APN Parameter
			 * (Access Point Name) a string parameter which is a logical name that is used to select the GGSN or the external packet data network. If the value is empty (“”) or omitted, then the subscription value will be requested.
			 * @param _PDP_Addr Parameter
			 * a string parameter that identifies the terminal in the address space applicable to the PDP. The allocated address may be read using the +CGPADDR command.
			 * @param _D_Comp Parameter
			 * numeric parameter that controls PDP data compression
			 * 0 - off (default if value is omitted)
			 * 1 - on
			 * @param _H_Comp Parameter
			 * numeric parameter that controls PDP header compression
			 * 0 - off (default if value is omitted)
			 * 1 - on
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CGDCONT(const uint8_t _Cid, const char * _PDP_Type, const char * _APN, const char * _PDP_Addr, const bool _D_Comp, const bool _H_Comp) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGDCONT="));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F(",\""));
				GSM_Serial->print(_PDP_Type);
				GSM_Serial->print(F("\",\""));
				GSM_Serial->print(_APN);
				GSM_Serial->print(F("\",\""));
				GSM_Serial->print(_PDP_Addr);
				GSM_Serial->print(F("\","));
				GSM_Serial->print(_D_Comp);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_H_Comp);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command is used to activate or deactivate either the GSM context or the specified PDP context.
			 * @version 01.01.00
			 * @param _Cid Parameter
			 * PDP context identifier
			 * 0 - specifies the GSM context
			 * 1..5 - numeric parameter which specifies a particular PDP context definition
			 * @param _Stat Parameter
			 * 0 - deactivate the context
			 * 1 - activate the context
			 * @param _IP Parameter
			 * IP address obtained from the network.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Set_SGACT(const uint8_t _Cid, const bool _Stat, char * _IP) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 60000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SGACT="));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Stat);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(30);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Buffer Variable
				char _Buffer[20];
				memset(_Buffer, '\0', 20);

				// Control for Buffer
				for (uint8_t i = 0; i < Buffer.Read_Order; i++) {

					// Handle Data
					if (((Buffer_Variable[i] <= '9' and Buffer_Variable[i] >= '0') or Buffer_Variable[i] == '.')) {

						// Get Data
						_Buffer[Buffer.Data_Order] = Buffer_Variable[i] ;

						// Increase Data Order
						Buffer.Data_Order += 1;

					}

				}

				// Declare IP Segment Variables
				int IP_Segment[4];

				// IP : #SGACT: 178.242.19.187OK
	
				// Handle IP 
				uint8_t _Variable_Count = sscanf(_Buffer, "%d.%d.%d.%d", &IP_Segment[0], &IP_Segment[1], &IP_Segment[2], &IP_Segment[3]);
	
				// Control for IP
				if (_Variable_Count == 4) {

					// Handle TimeStamp
					sprintf(_IP, "%03hhu.%03hhu.%03hhu.%03hhu", IP_Segment[0], IP_Segment[1], IP_Segment[2], IP_Segment[3]);

					// End Function
					return (true);

				} else {

					// Handle TimeStamp
					sprintf(_IP, "%03hhu.%03hhu.%03hhu.%03hhu", 0, 0, 0, 0);

					// End Function
					return (false);

				}

			}

			/**
			 * @brief Execution command is used to get either the GSM context or the specified PDP context.
			 * @param _Cid Parameter
			 * PDP context identifier
			 * 0 - specifies the GSM context
			 * 1..5 - numeric parameter which specifies a particular PDP context definition
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Get_SGACT(const uint8_t _Cid) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SGACT?"));
				GSM_Serial->print(F("\r\n"));

				// Response Wait Delay
				delay(10);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Variable
				char _Stat_Response;

				// Handle Stat Response
				if (_Cid == 1) _Stat_Response = Buffer_Variable[10];
				if (_Cid == 2) _Stat_Response = Buffer_Variable[23];
				if (_Cid == 3) _Stat_Response = Buffer_Variable[36];

				// End Function
				if (_Stat_Response == '0') return(false);
				if (_Stat_Response == '1') return(true);

				// End Function
				return(false);

			}

			/**
			 * @brief Execution command is used to activate or deactivate the specified PDP context(s)
			 * @param _Cid Parameter
			 * a numeric parameter which specifies a particular PDP context definition
			 * 0 - specifies the GSM context
			 * 1..5 - numeric parameter which specifies a particular PDP context definition
			 * @param _Stat Parameter
			 * indicates the state of PDP context activation
			 * 0 - deactivate
			 * 1 - activate
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Set_CGACT(const uint8_t _Cid, const bool _State) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 150000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGACT="));
				GSM_Serial->print(_State);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command is used to activate or deactivate the specified PDP context(s)
			 * @param _Cid Parameter
			 * a numeric parameter which specifies a particular PDP context definition
			 * 0 - specifies the GSM context
			 * 1..5 - numeric parameter which specifies a particular PDP context definition
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Get_CGACT(const uint8_t _Cid) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGACT?"));
				GSM_Serial->print(F("\r\n"));

				// Response Wait Delay
				delay(10);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Variable
				char _Stat_Response;

				// Handle Stat Response
				if (_Cid == 1) _Stat_Response = Buffer_Variable[10];
				if (_Cid == 2) _Stat_Response = Buffer_Variable[23];
				if (_Cid == 3) _Stat_Response = Buffer_Variable[36];

				// End Function
				if (_Stat_Response == '0') return(false);
				if (_Stat_Response == '1') return(true);

				// End Function
				return(false);

			}

			/**
			 * @brief Execution command returns a list of PDP addresses for the specified context identifiers in the format:
			 * @param _Cid Parameter
			 * a numeric parameter which specifies a particular PDP context definition
			 * 0 - specifies the GSM context
			 * 1..5 - numeric parameter which specifies a particular PDP context definition
			 * @param _IP Parameter 
			 * a string that identifies the terminal in the address space applicable to the PDP
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CGPADDR(const uint8_t _Cid,  char * _IP) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CGPADDR="));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Clear Variables
				memset(_IP, '\0', 16);

				// Declare Handle Variable
				bool _Handle = false;

				// Control for Buffer
				for (uint8_t i = 0; i < Buffer.Read_Order; i++) {

					// Handle Data
					if (((Buffer_Variable[i] < 58 and Buffer_Variable[i] > 47) or Buffer_Variable[i] == 46) and _Handle == true) {

						// Get Data
						_IP[Buffer.Data_Order] = Buffer_Variable[i];

						// Increase Data Order
						Buffer.Data_Order++;

					}

					// Set Handle
					if (Buffer_Variable[i] == 34) _Handle = true;

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Set command enables/disables the ICMP Ping support.
			 * @param _Mode Parameter. 
			 * 0 - disable ICMP Ping support (default)
			 * 1 - enable firewall ICMP Ping support: the module is sending a proper
			 * ECHO_REPLY only to a subset of IP Addresses pinging it; this subset of IP
			 * Addresses has been previously specified through #FRWL (see) 
			 * 2 - enable free ICMP Ping support; the module is sending a proper
			 * ECHO_REPLY to every IP Address pinging it.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool ICMP(const uint8_t _Mode) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#ICMP="));
				GSM_Serial->print(_Mode);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command reports received signal quality indicators in the form.
			 * @param _RSSI Parameter. 
			 * Received signal strength indication
			 * 0 - (-113) dBm or less
			 * 1 - (-111) dBm
			 * 2..30 - (-109)dBm..(-53)dBm / 2 dBm per step 
			 * 31 - (-51)dBm or greater
			 * 99 - not known or not detectable		 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CSQ(uint8_t & _RSSI) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CSQ"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Clear Variables
				_RSSI = 0;

				// Declare Response Data
				char _CSQ[2]; 
				memset(_CSQ, '\0', 2);

				// Declare Handle Data
				bool _Handle = true;

				// Control for Buffer
				for (uint8_t i = 0; i < Buffer.Read_Order; i++) {

					// Handle Data
					if (Buffer_Variable[i] <= '9' and Buffer_Variable[i] >= '0' and _Handle == true) {

						// Get Data
						_CSQ[Buffer.Data_Order] = Buffer_Variable[i];

						// Increase Data Order
						Buffer.Data_Order++;

					}

					// Set Handle Pointer
					if (Buffer_Variable[i] == ',') _Handle = false;

				}

				// Set Signal Variable
				_RSSI = atoi(_CSQ);

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command reports information about serving cell.
			 * @version 01.01.00
			 * @param _Operator Parameter. 
			 * string representing the network operator in numeric format: 5 or 6 digits [country code (3) + network code (2 or 3)]
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SERVINFO(uint16_t & _Operator, uint16_t & _BARFCN, uint16_t & _dBM, uint16_t & _BSIC, uint16_t & _TA, uint16_t & _GPRS, char * _LAC) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SERVINFO"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// #SERVINFO: 3,-101,"Turkcell","28601",52,855E,03,1,,"II",01,6OK
				// 3,101,,28601,52,855,03,1,,,01,6

				// Handle Variables
				uint8_t _Variable_Count = sscanf(Buffer_Variable, "#SERVINFO: %d,-%d,\"Turkcell\",\"%d\",%d,%4c,%d,%d,,\"II\",01,6OK", &_BARFCN, &_dBM, &_Operator, &_BSIC, _LAC, &_TA, &_GPRS);

				// #SERVINFO: 3,-101,"Turkcell","28601",52,855E,03,1,,"II",01,6OK
				// #SERVINFO: <B-ARFCN>,<dBM>,<NetNameAsc>,<NetCode>,<BSIC>,<LAC>,<TA>,<GPRS>[,[<PB-ARFCN>],[<NOM>],<RAC>[,<PAT>]]

				// Handle Operator
				if (_Variable_Count == 7) {

					// End Function
					return (true);

				} else {

					// End Function
					return (false);

				}

			}

			/**
			 * @brief Execution command reports information about serving cell.
			 * @version 01.01.00
			 * @param _MONI_Data Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */	
			bool MONI(char * _LAC, char * _Cell_ID) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#MONI"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Define Variables
				int _BSIC;
				int _RxQual;

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Handle Variables
				uint8_t _Variable_Count = sscanf(Buffer_Variable, "#MONI: Turkcell BSIC:%d RxQual:%d LAC:%4c Id:%4c ARFCN:3 PWR:-94dbm TA:3OK", &_BSIC, &_RxQual, _LAC, _Cell_ID);

				// Control for Variable
				if (_Variable_Count == 4) {

					// End Function
					return(true);

				} else {

					// End Function
					return(false);

				}

			}

			/**
			 * @brief Set command sets one cell out of seven, in a the neighbor list of the serving cell including it, from which extract GSM-related information.
			 * @param _Cid Parameter
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Set_MONIZIP(const uint8_t _Cid = 0) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#MONIZIP="));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Set command sets one cell out of seven, in a the neighbour list of the serving cell including it, from which extract GSM-related information.
			 * @param _Operator 
			 * @param _BSIC 
			 * @param _QUAL 
			 * @param _LAC 
			 * @param _Cell_ID 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Get_MONIZIP(uint16_t & _Operator, char * _LAC, char * _Cell_ID, uint8_t & _dBM) {

				// Declare Variable Structure
				struct Operator_Structure {
					int 	Network_Code	= 0;
					int 	BSIC			= 0;
					int 	QUAL			= 0;
					char	LAC[5];
					char	CELL_ID[5];
					int		ARFCN			= 0;
					int		dBm				= 0;
					int		TIMADV			= 0;
				} Operator;

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 10000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#MONIZIP"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// #MONIZIP: 28601,23,0,855E,CCF3,60,-92,3OK
				// #MONIZIP: 28601,23,0,855E,CCF3,60,-93,0OK
				// #MONIZIP: 28601,23,0,855E,CCF3,60,-91,3OK

				// Handle Variables
				uint8_t _Variable_Count = sscanf(Buffer_Variable, "#MONIZIP: %05d,%02d,%01d,%04c,%04c,%02d,-%03d,%01d", &_Operator, &Operator.BSIC, &Operator.QUAL, _LAC, _Cell_ID, &Operator.ARFCN, &_dBM, &Operator.TIMADV);

				// Set Signal Strength
				Modem.Signal = Signal_Strength(_dBM);

				// Control for Variable
				if (_Variable_Count == 8) return(true);

				// End Function
				return(false);

			}

			/**
			 * @brief Set command sets one cell out of seven, in a the neighbour list of the serving cell including it, from which extract GSM-related information.
			 * @param _Response 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool All_MONIZIP(char * _Response) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#MONIZIP"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				strcpy(_Response, Buffer_Variable);

			}

			/**
			 * @brief his command enables and disables automatic time zone update via NITZ.
			 * @param _State Parameter. 
			 * 0 Disable automatic time zone update via NITZ (default) 
			 * 1 Enable automatic time zone update via NITZ		 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CTZU(const bool _State) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CTZU="));
				GSM_Serial->print(String(_State));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command enables/disables automatic date/time updating and Network Timezone unsolicited indication.
			 * @param _State Parameter. 
			 * 0 - disables automatic set (factory default)
			 * 1 - enables automatic set
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool NITZ(const bool _State) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#NITZ="));
				GSM_Serial->print(_State);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief This command permits to calculate and update date and time through NTP protocol(RFC2030), sending a request to a NTP server.
			 * @param _NTP_Addr Parameter. 
			 * Address of the NTP server, string type
			 * @param _NTP_Port Parameter. 
			 * NTP server port to contact
			 * @param _Update_Module_Clock Parameter. 
			 * 0 - no update module clock 
			 * 1 – update module clock
			 * @param _Time_Out Parameter. 
			 * Waiting timeout for server response in seconds
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out, uint8_t & _Year, uint8_t & _Month, uint8_t & _Day, uint8_t & _Hour, uint8_t & _Minute, uint8_t & _Second) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#NTP=\""));
				GSM_Serial->print(_NTP_Addr);
				GSM_Serial->print(F("\","));
				GSM_Serial->print(_NTP_Port);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Update_Module_Clock);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Time_Out);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// #NTP: 22/05/31,13:30:00OK

				// Handle Time
				_Year = (((Buffer_Variable[6] - 48) * 10) + (Buffer_Variable[7] - 48));
				_Month = (((Buffer_Variable[9] - 48) * 10) + (Buffer_Variable[10] - 48));
				_Day = (((Buffer_Variable[12] - 48) * 10) + (Buffer_Variable[13] - 48));
				_Hour = (((Buffer_Variable[15] - 48) * 10) + (Buffer_Variable[16] - 48));
				_Minute = (((Buffer_Variable[18] - 48) * 10) + (Buffer_Variable[19] - 48));
				_Second = (((Buffer_Variable[21] - 48) * 10) + (Buffer_Variable[22] - 48));

				// Control for Variables
				if (_Year > 24 and _Year < 22) return(false);	
				if (_Month > 12 and _Month < 0) return(false);	
				if (_Day > 31 and _Day < 0) return(false);	
				if (_Hour > 24 and _Hour < 0) return(false);	
				if (_Minute > 59 and _Minute < 0) return(false);	
				if (_Second > 59 and _Second < 0) return(false);	

				// End Function
				return (true);

			}

			/**
			 * @brief Set command sets the real-time clock of the ME.
			 * @param _Year Parameter. 
			 * @param _Month Parameter. 
			 * @param _Day Parameter. 
			 * @param _Hour Parameter. 
			 * @param _Minute Parameter. 
			 * @param _Second Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool CCLK(uint8_t & _Year, uint8_t & _Month, uint8_t & _Day, uint8_t & _Hour, uint8_t & _Minute, uint8_t & _Second) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT+CCLK?"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// +CCLK: "22/05/31,13:06:06+00"OK

				// Handle Variables
				sscanf(Buffer_Variable, "+CCLK: \"%02d/%02d/%02d,%02d:%02d:%02d+00\"OK", &_Year, &_Month, &_Day, &_Hour, &_Minute, &_Second);

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command controls the internal firewall settings.
			 * @param _Action Parameter. 
			 * 0 - remove selected chain
			 * 1 - add an ACCEPT chain
			 * 2 - remove all chains (DROP everything); <ip_addr> and <net_mask> has no meaning in this case.
			 * @param _IP_Addr Parameter. 
			 * Remote address to be added into the ACCEPT chain; string type
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool FRWL(const uint8_t _Action, const char *_IP_Addr) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#FRWL="));
				GSM_Serial->print(_Action);
				GSM_Serial->print(F(",\""));
				GSM_Serial->print(_IP_Addr);
				GSM_Serial->print(F("\",\"255.255.255.0\""));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief This command sets the parameters needed to the HTTP connection
			 * @param _HTTP_Server Parameter. 
			 * String parameter indicating the IP address of the HTTP server.
			 * @param _Port Parameter. 
			 * Numeric parameter indicating the TCP remote port of the HTTP server to connect to.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool HTTPCFG(const char *_HTTP_Server, const uint8_t _Port) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#HTTPCFG=1,\""));
				GSM_Serial->print(_HTTP_Server);
				GSM_Serial->print(F("\","));
				GSM_Serial->print(_Port);
				GSM_Serial->print(F(",0,\"\",\"\",0,30,1"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command performs a POST or PUT request to HTTP server and starts sending data to the server.
			 * @param _Prof_ID Parameter.
			 * Numeric parameter indicating the profile identifier.
			 * @param _Command Parameter. 
			 * Numeric parameter indicating the command requested to HTTP server:
			 * 0 – POST
			 * 1 – PUT
			 * @param _URL Parameter. 
			 * String parameter indicating the HTTP resource (uri), object of the request
			 * @param _Data Parameter. 
			 * Data to send.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool HTTPSND(const uint8_t _Prof_ID, const uint8_t _Command, const char *_URL, const char *_Data) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer_Set = {false, 0, 0, 20000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#HTTPSND="));
				GSM_Serial->print(_Prof_ID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Command);
				GSM_Serial->print(F(",\""));
				GSM_Serial->print(_URL);
				GSM_Serial->print(F("\","));
				GSM_Serial->print(String(_Data).length());
				GSM_Serial->print(F(",\"application/json\""));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer_Set.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer_Set.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer_Set.Read_Order - 2] == '>' and Buffer_Variable[Buffer_Set.Read_Order - 1] == '>' and Buffer_Variable[Buffer_Set.Read_Order] == '>') Buffer_Set.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer_Set.Read_Order] > 31 and Buffer_Variable[Buffer_Set.Read_Order] < 127) Buffer_Set.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer_Set.Time_Out) return(false);

				}

				// Send Delay
				delay(50);

				// Print Data
				GSM_Serial->print(_Data);

				// Send Delay
				delay(50);

				// Declare Buffer Object
				Serial_Buffer Buffer_Get = {false, 0, 0, 20000};

				// Declare Buffer
				memset(Buffer_Variable, '\0', 255);

				// Read Current Time
				Current_Time = millis();

				// Read UART Response
				while (!Buffer_Get.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer_Get.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer_Get.Read_Order - 1] == 'O' and Buffer_Variable[Buffer_Get.Read_Order] == 'K') Buffer_Get.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer_Get.Read_Order] > 31 and Buffer_Variable[Buffer_Get.Read_Order] < 127) Buffer_Get.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer_Get.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command permits the user to read data from HTTP server in response to a previous HTTP module request.
			 * @param _Prof_ID Parameter.
			 * Numeric parameter indicating the profile identifier.
			 * @param _Response Parameter.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool HTTPRCV(const uint8_t _Prof_ID, char * _Response) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 20000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#HTTPRCV="));
				GSM_Serial->print(_Prof_ID);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 2] == '<' and Buffer_Variable[Buffer.Read_Order - 1] == '<' and Buffer_Variable[Buffer.Read_Order] == '<') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Command Work Delay
				delay(10);

				// Declare Buffer Object
				Serial_Buffer Data_Buffer = {false, 0, 0, 20000};

				// Read Current Time
				Current_Time = millis();

				// Read UART Response
				while (!Data_Buffer.Response) {

					// Read Serial Char
					_Response[Data_Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (_Response[Data_Buffer.Read_Order] == '\n') Data_Buffer.Response = true;

					// Increase Read Order
					if (_Response[Data_Buffer.Read_Order] > 31 and _Response[Data_Buffer.Read_Order] < 127) Data_Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Data_Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command sets the socket configuration parameters.
			 * @param _Conn_ID Parameter. 
			 * Socket connection identifier
			 * @param _Cid Parameter. 
			 * PDP context identifier
			 * 0 - specifies the GSM context
			 * 1..5 - numeric parameter which specifies a particular PDP context definition
			 * @param _Pkt_Sz Parameter. 
			 * packet size to be used by the TCP/UDP/IP stack for data sending.
			 * 0 - select automatically default value(300).
			 * 1..1500 - packet size in bytes.
			 * @param _Max_To Parameter. 
			 * exchange timeout (or socket inactivity timeout); if there’s no data exchange within this timeout period the connection is closed.
			 * 0 - no timeout
			 * 1..65535 - timeout value in seconds (default 90 s.)
			 * @param _Conn_To Parameter. 
			 * connection timeout; if we can’t establish a connection to the remote within this timeout period, an error is raised.
			 * 10..1200 - timeout value in hundreds of milliseconds (default 600)
			 * @param _TX_To Parameter. 
			 * data sending timeout; after this period data are sent also if they’re less than max packet size.
			 * 0 - no timeout
			 * 1..255 - timeout value in hundreds of milliseconds (default 50) 
			 * 256 – set timeout value in 10 milliseconds
			 * 257 – set timeout value in 20 milliseconds
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SCFG="));
				GSM_Serial->print(_Conn_ID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Pkt_Sz);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Max_To);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Conn_To);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_TX_To);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command sets the socket configuration extended parameters.
			 * @param _Conn_ID Parameter. 
			 * socket connection identifier
			 * @param _Sr_Mode Parameter. 
			 * SRing unsolicited mode
			 * 0 - Normal (default):
			 * 1 – Data amount:
			 * 2 - Data view:
			 * 3 – Data view with UDP datagram informations
			 * @param _Recv_Data_Mode Parameter. 
			 * data view mode for received data
			 * 0- text mode (default)
			 * 1- hexadecimal mode
			 * @param _Keep_Alive Parameter. 
			 * Set the TCP Keepalive value in minutes
			 * 0 – Deactivated (default)
			 * 1 – 240 – Keepalive time in minutes
			 * @param _Listen_Auto_Rsp Parameter. 
			 * Set the listen auto-response mode, that affects the commands AT#SL and AT#SLUDP
			 * 0 - Deactivated (default)
			 * 1 – Activated
			 * @param _Send_Data_Mode Parameter. 
			 * data mode for sending data in command mode(AT#SSEND)
			 * 0 - data represented as text (default)
			 * 1 - data represented as sequence of hexadecimal numbers (from 00 to FF)
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode) {

				// <connId> - socket connection identifier (1..6)
				// <srMode> - SRing unsolicited mode
				// <recvDataMode> - data view mode for received data in command mode(AT#SRECV or <srMode> = 2)
				// <keepalive> - Set the TCP Keepalive value in minutes
				// <ListenAutoRsp> - Set the listen auto-response mode, that affects the commands AT#SL and AT#SLUDP
				// <sendDataMode> - data mode for sending data in command mode(AT#SSEND)

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SCFGEXT="));
				GSM_Serial->print(_Conn_ID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Sr_Mode);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Recv_Data_Mode);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Keep_Alive);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Listen_Auto_Rsp);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Send_Data_Mode);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command sets the socket configuration extended parameters for features not included in #SCFGEXT command.
			 * @param _Conn_ID Parameter. 
			 * @param _Buffer_Start Parameter. 
			 * @param _Abort_Conn_Attempt Parameter. 
			 * @param _SRing_Len Parameter. 
			 * @param _SRing_To Parameter. 
			 * @param _No_Carrier_Mode Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SCFGEXT2="));
				GSM_Serial->print(_Conn_ID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Buffer_Start);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Abort_Conn_Attempt);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_SRing_Len);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_SRing_To);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_No_Carrier_Mode);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Set command sets the socket configuration extended parameters for features not included in #SCFGEXT command nor in #SCFGEXT2 command.
			 * @param _Conn_ID Parameter. 
			 * @param _Imm_Rsp Parameter. 
			 * @param _Closure_Type_Cmd_Mode_Enabling Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SCFGEXT3="));
				GSM_Serial->print(_Conn_ID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Imm_Rsp);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Closure_Type_Cmd_Mode_Enabling);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Get Socket Status
			 * @param _Conn_ID Parameter. 
			 * @param _State Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SS(const uint8_t _ConnID, uint8_t & _State) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SS="));
				GSM_Serial->print(String(_ConnID));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Handle State
				_State = (Buffer_Variable[7] - 48);

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(26, 108, CYAN, "         ");
					if (_State == 0) Terminal_GSM.Text(26, 108, RED, "Closed   ");
					if (_State == 1) Terminal_GSM.Text(26, 108, WHITE, "Transfer ");
					if (_State == 2) Terminal_GSM.Text(26, 108, YELLOW, "Suspend  ");
					if (_State == 3) Terminal_GSM.Text(26, 108, YELLOW, "Suspend  ");
					if (_State == 4) Terminal_GSM.Text(26, 108, GREEN, "Listening");
					if (_State == 5) Terminal_GSM.Text(26, 108, WHITE, "Incoming ");
					if (_State == 6) Terminal_GSM.Text(26, 108, RED, "DNS      ");
					if (_State == 7) Terminal_GSM.Text(26, 108, WHITE, "Connecting");
				#endif

				// End Function
				return (true);

			}

			/**
			 * @brief Set Socket Listen
			 * @param _Conn_ID Parameter. 
			 * @param _Listen_State Parameter. 
			 * @param _Listen_Port Parameter. 
			 * @param _Closure_Type Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SL(const uint8_t _ConnID, const bool _Listen_State, const uint16_t _Listen_Port, const uint8_t _Closure_Type) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SL="));
				GSM_Serial->print(_ConnID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Listen_State);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Listen_Port);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Closure_Type);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief This command is used to close a socket.
			 * @param _Conn_ID Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SH(const uint8_t _ConnID) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SH="));
				GSM_Serial->print(String(_ConnID));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command resumes the direct interface to a socket connection which has been suspended by the escape sequence.
			 * @param _Conn_ID Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SO(const uint8_t _ConnID) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SO="));
				GSM_Serial->print(String(_ConnID));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command accepts an incoming socket connection after an URC
			 * @param _Conn_ID Parameter. 
			 * @param _ConnMode Parameter. 
			 * @param _Length Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 * TODO:Development
			 */
			bool SA(const uint8_t _ConnID, const uint8_t _ConnMode, uint16_t & _Length) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 15000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SA="));
				GSM_Serial->print(_ConnID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_ConnMode);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Read Current Time
				Current_Time = millis();

				// Declare Buffer
				memset(Buffer_Variable, '\0', 255);

				// Declare Buffer Object
				Serial_Buffer Buffer_Get = {false, 0, 0, 25000};

				// Declare Handle Variable
				bool Comma_Handle = false;

				// Declare Response Array
				char Length_Array[3];

				// Read UART Response
				while (!Buffer_Get.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer_Get.Read_Order] = GSM_Serial->read();

					// Get Length
					if (Comma_Handle) {

						// Get Length
						if (Buffer_Variable[Buffer_Get.Read_Order] > 47 and Buffer_Variable[Buffer_Get.Read_Order] < 58) {

							// Read Data	
							Length_Array[Buffer_Get.Data_Order] = Buffer_Variable[Buffer_Get.Read_Order];

							// Handle Data Order
							Buffer_Get.Data_Order += 1;

						}

						// Set Handle
						if (Buffer_Variable[Buffer_Get.Read_Order] == '\n') Buffer_Get.Response = true;

					}

					// Set Handle
					if (Buffer_Variable[Buffer_Get.Read_Order] == ',') Comma_Handle = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer_Get.Read_Order] > 31 and Buffer_Variable[Buffer_Get.Read_Order] < 127) Buffer_Get.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer_Get.Time_Out) return(false);

				}

				// Convert char to int
				_Length = atoi(Length_Array);

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command opens a remote connection via socket.
			 * @param _Cid Parameter. 
			 * Socket connection identifier 1..6
			 * @param _Protocol Parameter. 
			 * transmission protocol
			 * 0 - TCP
			 * 1 - UDP
			 * @param _Port Parameter. 
			 * remote host port to contact 1..65535
			 * @param _Closure_Type Parameter. 
			 * socket closure behaviour for TCP when remote host has closed
			 * 0 - local host closes immediately (default)
			 * 255 - local host closes after an AT#SH or immediately in case of an abortive disconnect from remote.
			 * @param _IPort Parameter. 
			 * UDP connections local port 1..65535
			 * @param _Conn_Mode Parameter. 
			 * Connection mode
			 * 0 - online mode connection (default) 
			 * 1 - command mode connection
			 * @param _IP Parameter. 
			 * address of the remote host, string type. This parameter can be either:
			 * - any valid IP address in the format: “xxx.xxx.xxx.xxx”
			 * - any host name to be solved with a DNS query
			 * @param _URL Parameter. 
			 * @param _Data Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SD(const uint8_t _Cid, const uint8_t _Protocol, const uint8_t _Port, const uint8_t _Closure_Type, uint16_t _IPort, const bool _Conn_Mode, const char *_IP) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 15000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SD="));
				GSM_Serial->print(_Cid);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Protocol);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Port);
				GSM_Serial->print(F(",\""));
				GSM_Serial->print(_IP);
				GSM_Serial->print(F("\","));
				GSM_Serial->print(_Closure_Type);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_IPort);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_Conn_Mode);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command permits, while the module is in command mode, to send data through a connected socket.
			 * @param _ConnID Parameter. 
			 * Socket connection identifier
			 * @param _Send_Type Parameter. 
			 * Pack Send type (Get / Post)
			 * @param _Response_Code Parameter. 
			 * HTTP Response Code
			 * @param _IP Parameter. 
			 * Server IP
			 * @param _URL Parameter. 
			 * Server End Point
			 * @param _Data_Pack Parameter. 
			 * Data Payload
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SSEND(const uint8_t _ConnID, const uint8_t _Send_Type, const uint16_t _Response_Code, const char * _IP, const char * _URL, const char * _Data_Pack) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer_Set = {false, 0, 0, 2000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SSEND="));
				GSM_Serial->print(_ConnID);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer_Set.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer_Set.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer_Set.Read_Order - 1] == '>' and Buffer_Variable[Buffer_Set.Read_Order] == ' ') Buffer_Set.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer_Set.Read_Order] > 31 and Buffer_Variable[Buffer_Set.Read_Order] < 127) Buffer_Set.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer_Set.Time_Out) return(false);

				}

				// Send Delay
				delay(10);

				// Print HTTP Header
				this->Send_Header(_Send_Type, _Response_Code, _IP, _URL, _Data_Pack);

				// Send Data Pack
				GSM_Serial->print(_Data_Pack);

				// Print End Char
				GSM_Serial->print((char)26);

				// Declare Buffer Object
				Serial_Buffer Buffer_Get = {false, 0, 0, 2000};

				// Declare Buffer
				memset(Buffer_Variable, '\0', 255);

				// Read Current Time
				Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer_Get.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer_Get.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer_Get.Read_Order - 1] == 'O' and Buffer_Variable[Buffer_Get.Read_Order] == 'K') Buffer_Get.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer_Get.Read_Order] > 31 and Buffer_Variable[Buffer_Get.Read_Order] < 127) Buffer_Get.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer_Get.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command permits the user to read data arrived through a connected socket.
			 * @param _Conn_ID Parameter. 
			 * Socket connection identifier
			 * @param _MaxByte Parameter. 
			 * max number of bytes to read 1..1500
			 * @param _Data Parameter. 
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SRECV(const uint8_t _ConnID, const uint16_t _MaxByte, char * _Data) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 5000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SRECV="));
				GSM_Serial->print(_ConnID);
				GSM_Serial->print(F(","));
				GSM_Serial->print(_MaxByte);
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer.Read_Order > 30 and Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Declare Data Handle Variable
				bool Data_Handle = false;

				// Declare Data Order
				int Data_Order = 0;

				// Control for Buffer
				for (uint16_t i = 10; i <= Buffer.Read_Order; i++) {

					// Handle JSON Data
					if (Buffer_Variable[i] == '{') Data_Handle = true;

					// Get Data
					if (Data_Handle) {
						_Data[Data_Order] = Buffer_Variable[i];
						Data_Order += 1;
					}

					// Handle JSON Data
					if (Buffer_Variable[i] == '}') Data_Handle = false;

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Execution command causes device detach from the network and shut down. Before definitive shut down an OK response is returned.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool SHDN(void) {

				// Clear UART Buffer
				Clear_UART_Buffer();

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 1000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Command Chain Delay (Advice by Telit)
				delay(20);

				// Send UART Command
				GSM_Serial->print(F("AT#SHDN"));
				GSM_Serial->print(F("\r\n"));

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Response Wait Delay
				delay(10);

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (Buffer_Variable[Buffer.Read_Order - 1] == 'O' and Buffer_Variable[Buffer.Read_Order] == 'K') Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// End Function
				return (true);

			}

			/**
			 * @brief Detect SRING Response.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Send_SRING(uint8_t & _Ring, uint16_t & _Length) {

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 25000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Declare Handle Variable
				bool Comma_Handle = false;

				// Declare Response Array
				char Length_Array[4];

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Get Length
					if (Comma_Handle) {

						// Get Length
						if (Buffer_Variable[Buffer.Read_Order] > 47 and Buffer_Variable[Buffer.Read_Order] < 58) {

							// Read Data	
							Length_Array[Buffer.Data_Order] = Buffer_Variable[Buffer.Read_Order];

							// Handle Data Order
							Buffer.Data_Order += 1;

						}

						// Set Handle
						if (Buffer_Variable[Buffer.Read_Order] == '\n') Buffer.Response = true;

					}

					// Set Handle
					if (Buffer_Variable[Buffer.Read_Order] == ',') Comma_Handle = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Convert char to int
				_Length = atoi(Length_Array);

				// Control for Socket ID
				_Ring = 0;
				if ((Buffer_Variable[7]) == '1') _Ring = 1;
				if ((Buffer_Variable[7]) == '2') _Ring = 2;
				if ((Buffer_Variable[7]) == '3') _Ring = 3;

				// End Function
				return(true);

			}

			/**
			 * @brief Detect SRING Response.
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Receive_SRING(uint8_t & _Ring) {

				// Declare Buffer Object
				Serial_Buffer Buffer = {false, 0, 0, 15000};

				// Declare Buffer
				char Buffer_Variable[255];
				memset(Buffer_Variable, '\0', 255);

				// Read Current Time
				const uint32_t Current_Time = millis();

				// Read UART Response
				while (!Buffer.Response) {

					// Read Serial Char
					Buffer_Variable[Buffer.Read_Order] = GSM_Serial->read();

					// Control for <OK> Response
					if (
						Buffer_Variable[Buffer.Read_Order - 7] == 'S' and 
						Buffer_Variable[Buffer.Read_Order - 6] == 'R' and 
						Buffer_Variable[Buffer.Read_Order - 5] == 'I' and 
						Buffer_Variable[Buffer.Read_Order - 4] == 'N' and 
						Buffer_Variable[Buffer.Read_Order - 3] == 'G' and 
						Buffer_Variable[Buffer.Read_Order - 2] == ':'
						) Buffer.Response = true;

					// Increase Read Order
					if (Buffer_Variable[Buffer.Read_Order] > 31 and Buffer_Variable[Buffer.Read_Order] < 127) Buffer.Read_Order += 1;

					// Handle for timeout
					if (millis() - Current_Time >= Buffer.Time_Out) return(false);

				}

				// Control for Socket ID
				_Ring = 0;
				if ((Buffer_Variable[7]) == '1') _Ring = 1;
				if ((Buffer_Variable[7]) == '2') _Ring = 2;
				if ((Buffer_Variable[7]) == '3') _Ring = 3;

				// End Function
				return(true);

			}

			/**
			 * @brief Send HTTP Pack Header
			 * @param _Header_Type Parameter
			 * 1 - Response
			 * 2 - Send
			 * @param _IP Parameter
			 * @param _URL Parameter
			 * @param _Data Parameter
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Send_Header(const uint8_t _Header_Type, const uint16_t _Response_Code, const char *_IP, const char *_URL, const char *_Data) {

				// Handle Type
				if (_Header_Type == 1) {

					// Select Response Code
					if (_Response_Code == 200) GSM_Serial->print(F("HTTP/1.1 200 OK\r\n"));
					if (_Response_Code == 202) GSM_Serial->print(F("HTTP/1.1 202 Accepted\r\n"));
					if (_Response_Code == 400) GSM_Serial->print(F("HTTP/1.1 400 Bad Request\r\n"));
					if (_Response_Code == 405) GSM_Serial->print(F("HTTP/1.1 405 Method Not Allowed\r\n"));
					if (_Response_Code == 406) GSM_Serial->print(F("HTTP/1.1 406 Not Acceptable\r\n"));

					// Print Connection Header
					GSM_Serial->print(F("Connection: close\r\n"));

					// Print Content Type
					GSM_Serial->print(F("Content-Type: application/json\r\n"));

					// Print User Agent
					GSM_Serial->print(F("User-Agent: STF-PowerStat\r\n"));

					// End of Header
					GSM_Serial->print(F("\r\n"));

					// End Function
					return(true);

				} else if (_Header_Type == 2) {

					// Print HTTP Header
					GSM_Serial->print(F("POST ")); GSM_Serial->print(_URL); GSM_Serial->print(F(" HTTP/1.1\r\n"));

					// Print Host
					GSM_Serial->print(F("Host: ")); GSM_Serial->print(_IP); GSM_Serial->print(F("\r\n"));

					// Print Content Length
					GSM_Serial->print(F("Content-Length: ")); GSM_Serial->print(String(_Data).length()); GSM_Serial->print(F("\r\n"));

					// Print Connection Header
					GSM_Serial->print(F("Connection: close\r\n"));

					// Print Content Type
					GSM_Serial->print(F("Content-Type: application/json\r\n"));

					// Print User Agent
					GSM_Serial->print(F("User-Agent: STF-PowerStat\r\n"));

					// End of Header
					GSM_Serial->print(F("\r\n"));

					// End Function
					return(true);

				}

				// End Function
				return(false);

			}

			/**
			 * @brief RSSI to Signal strength converter
			 * @param _CSQ RSSI
			 * @return uint8_t Signal Strength
			 */
			uint8_t Signal_Strength(uint16_t _dBm) {

				// Calculate Signal Strength
				if (_dBm >= -70) return(5);
				if (_dBm < -70 and _dBm >= -85) return(4);
				if (_dBm < -85 and _dBm >= -100) return(3);
				if (_dBm < -100) return(2);
				if (_dBm < -110) return(1);

				// End Function
				return(0);

			}

	};

	// RTC Class
	class xE910_RTC : private AT_Command_Set {

		public:

			// Define Time Structure
			struct Struct_Time {
				bool		Time_Update			= false;
				uint8_t 	Year				= 0;
				uint8_t 	Month				= 0;
				uint8_t 	Day					= 0;
				uint8_t 	Hour				= 0;
				uint8_t 	Minute				= 0;
				uint8_t 	Second				= 0;
			} Time;

			/**
			 * @brief Pair Modem RTC from Time Server
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Sync(void) {

				// Declare Watchdog Variable
				uint8_t _Error_WD = 0;

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(GSM_Console_Connect_X + 15, 42, WHITE, F("AT+CCLK"));
					Terminal_GSM.Text(GSM_Console_Connect_X + 15, 73, BLUE, F(" .. "));
				#endif

				// Process Command
				while (!this->Time.Time_Update) {

					// Send Command
					this->Time.Time_Update = CCLK(this->Time.Year, this->Time.Month, this->Time.Day, this->Time.Hour, this->Time.Minute, this->Time.Second);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(this->Time.Time_Update, GSM_Console_Connect_X + 15, 73);
				#endif

				// End Function
				return(this->Time.Time_Update);

			}

	};

	// Data Incoming Class
	class xE910_Incoming : private AT_Command_Set {

		private:

			// Define Socket Status Structure
			struct Socket_Status {
				uint8_t Conn_ID;
				uint8_t Cid = 1;
				uint8_t Server_Port = 80;
			} Parameter;

			/**
			 * @brief Handle JSON Pack Function
			 * @param _Data JSON Pack
			 * @return uint16_t Request Command
			 */
			uint16_t Handle_JSON_Request(const char *_Data) {

				// Declare Variable
				uint16_t Event = 0;

				// Declare JSON Object
				StaticJsonDocument<512> Incoming_JSON;

				// Deserialize the JSON document
				DeserializationError Error = deserializeJson(Incoming_JSON, _Data);

				// Handle JSON
				if (!Error) Event = Incoming_JSON["Request"]["Event"];

				// End Function
				return(Event);

			}

		public:

			xE910_Incoming(uint8_t _Conn_ID) : AT_Command_Set() {

				// Set Socket Port
				this->Parameter.Conn_ID = _Conn_ID;

			}

			/**
			 * @brief Configure Socket
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Configure(void) {

				// Declare Watchdog Variable
				uint8_t _Error_WD = 0;

				// Declare Response Status
				bool _Response = false;

					// Print Command State
					#ifdef GSM_Debug
						//Terminal_GSM.Text(Debug_Connect_X + 5, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = SCFG(this->Parameter.Conn_ID, this->Parameter.Cid, 1500, 90, 300, 50);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = SCFGEXT(this->Parameter.Cid, 1, 0, 1, 0, 0);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						//Terminal_GSM.OK_Decide(_Response, Debug_Connect_X + 5, Debug_Connect_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				// End Function
				return(false);

			}

			/**
			 * @brief Configure Socket for Listen
			 * @param _State Enable / Disable Socket Listen
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Listen(const bool _State) {

				// Declare Status Variable
				uint8_t Socket_Status;

				// Get Socket Status
				SS(this->Parameter.Conn_ID, Socket_Status);

				// Handle State
				if (_State) {

					// Control Current State
					if (Socket_Status != 4) SL(this->Parameter.Conn_ID, 1, this->Parameter.Server_Port, 255);

					// Command Delay
					delay(20);

					// Get Socket Status
					SS(this->Parameter.Conn_ID, Socket_Status);

					// Command Delay
					delay(20);

					// Control Socket
					if (Socket_Status != 4) return(false);

				} else {

					// Control Current State
					if (Socket_Status != 0) SL(this->Parameter.Conn_ID, 0, this->Parameter.Server_Port, 255);

					// Command Delay
					delay(20);

					// Get Socket Status
					SS(this->Parameter.Conn_ID, Socket_Status);

					// Command Delay
					delay(20);

					// Control Socket
					if (Socket_Status != 0) return(false);

				}

				// End Function
				return(true);

			}

			/**
			 * @brief Socket Answer Function
			 * @param _JSON_Data Incoming JSON Pack
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Answer(char * _JSON_Data) {

				// Declare RING
				uint8_t RING;

				// Get Ring Port
				Receive_SRING(RING);

				// Control for <SRING:n>
				if (RING == 1 or RING == 2 or RING == 3) {

					// Declare Request Length
					uint16_t Request_Length;

					// Answer Socket
					SA(RING, 1, Request_Length);

					// Get Request Data
					SRECV(RING, Request_Length, _JSON_Data);

					// End Function
					return(true);

				}

				// End Function
				return(false);

			}

			/**
			 * @brief Send Request Response Function
			 * @param _Data Sended Response Data Pack
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Response(uint16_t _Response_Code, char * _Data) {

				// Send Socket Answer
				if (SSEND(this->Parameter.Conn_ID, 1, _Response_Code, "", "", _Data)) {

					// Command Delay
					delay(20);

					// Close Socket
					if (SH(this->Parameter.Conn_ID)) {

						// Command Delay
						delay(20);

						// ReOpen Socket
						bool Socket_Open = this->Listen(true);

						// End Function
						return(Socket_Open);

					} else {

						// End Function
						return(false);

					}

				} else {

					// End Function
					return(false);

				}
				
			}

			/**
			 * @brief Get Server Command Batch Function
			 * @return uint16_t Server Request Command
			 */
			uint16_t Get(char * _Request) {

				// Declare Command Variable
				uint16_t Event;

				// Answer Socket Connection
				this->Answer(_Request);

				// Handle JSON Data
				Event = this->Handle_JSON_Request(_Request);

				// End Function
				return(Event);

			}

	};

	// Main GSM Class
	class xE910 : public xE910_Hardware, public AT_Command_Set {

		public:

			// Define Modem Status Structure
			struct Struct_Status {
				bool 		Initialize		 	= false;
				bool		Connection			= false;
			} Status;

			/**
			 * @brief Construct a new x E910 object
			 * @param _Serial GSM Connection Serial
			 */
			xE910(void) : AT_Command_Set() {

			}

			/**
			 * @brief GSM Modem Power Sequence
			 * @param _State Power State
			 * True : Turn ON Modem
			 * False : Turn OFF Modem
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Power(const bool _State) {

				// Handle State
				if (_State) {

					// Open Modem
					xE910_Hardware::Hardware_Status.Power = ON(true, true, true);

				} else {

					// ShutDown Modem
					xE910_Hardware::Hardware_Status.Power = OFF(true, true, true);

				}

				// End Function
				return(xE910_Hardware::Hardware_Status.Power);

			}

			/**
			 * @brief Initialize GSM Modem
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Initialize(void) {

				// Set Variable
				this->Status.Initialize = false;

				// Declare Watchdog Variable
				uint8_t _Error_WD = 0;

				// Declare Response Variable
				bool _Response = false;

				// Control for Power Monitor
				if (PowerMonitor()) {

					// Define Terminal Row Variable
					uint8_t GSM_Console_Initialize_ROW = GSM_Console_Initialize_X;

					// AT Command
					#ifdef _AT_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT();

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif
					
					// ATE Command
					#ifdef _AT_ATE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("ATE0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = ATE(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// CMEE Command
					#ifdef _AT_CMEE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CMEE=1"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CMEE(1);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// FCLASS Command
					#ifdef _AT_FCLASS_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+FCLASS=0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FCLASS(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// K Command
					#ifdef _AT_K_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT&K0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = K(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// CPIN Command
					#ifdef _AT_CPIN_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CPIN?"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CPIN(Modem.PIN);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// CGSN Command
					#ifdef _AT_CGSN_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CGSN"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CGSN(Modem.IMEI);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Console_Detail_X + 3, GSM_Console_Detail_Y + 20, CYAN, String(Modem.IMEI));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GSN Command
					#ifdef _AT_GSN_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GSN"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GSN(Modem.Serial_ID);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Console_Detail_X + 4, GSM_Console_Detail_Y + 25, CYAN, String(Modem.Serial_ID));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// ICCID Command
					#ifdef _AT_ICCID_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CCID"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CCID(Modem.ICCID);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Console_Detail_X + 5, GSM_Console_Detail_Y + 16, CYAN, String(Modem.ICCID));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GMI Command
					#ifdef _AT_GMI_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GMI"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GMI(Modem.Manufacturer);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Console_Detail_X, GSM_Console_Detail_Y + 34, CYAN, String(Modem.Manufacturer));
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GMM Command
					#ifdef _AT_GMM_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GMM"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GMM(Modem.Model);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Console_Detail_X + 1, GSM_Console_Detail_Y + 34, CYAN, String(Modem.Model));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GMR Command
					#ifdef _AT_GMR_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GMR"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GMR(Modem.Firmware);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Console_Detail_X + 2, GSM_Console_Detail_Y + 26, CYAN, String(Modem.Firmware));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// SLED Command
					#ifdef _AT_SLED_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#SLED=2"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = SLED(2);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// E2SLRI Command
					#ifdef _AT_E2SLRI_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#E2SLRI=50"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = E2SLRI(50);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// TXMONMODE Command
					#ifdef _AT_TXMONMODE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#TXMONMODE=1"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = TXMONMODE(1);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// Set Variable
					this->Status.Initialize = true;

				} else {

					// Power ON Modem
					this->Power(true);

				}

				// End Function
				return(this->Status.Initialize);
				
			}

			/**
			 * @brief Connect GSM Modem
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Online(void) {

				// Set Variable
				this->Status.Connection = false;

				// Declare Watchdog Variable
				uint8_t _Error_WD = 0;

				// Declare Response Status
				bool _Response = false;

				// Declare Time Variable
				uint32_t _Connection_Start_Time = 0;

				// Control for Initialization Monitor
				if (this->Status.Initialize) {

					// Set Variable
					this->Status.Connection = false;

					// Get Time
					_Connection_Start_Time = millis();

					// Define Terminal Row Variable
					uint8_t GSM_Console_Connect_ROW = GSM_Console_Connect_X;

					// REGMODE Command
					#ifdef _AT_REGMODE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#REGMODE=0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = REGMODE(_AT_REGMODE_mode_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;
						
					#endif

					// AUTOBND Command
					#ifdef _AT_AUTOBND_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#AUTOBND=1"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AUTOBND(_AT_AUTOBND_value_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// COPS Command
					#ifdef _AT_COPS_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+COPS=0,2,28601"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = COPS(_AT_COPS_mode_, _AT_COPS_format_, _AT_COPS_oper_); // 0,2,28601

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// CREG Command
					#ifdef _AT_CREG_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CREG=0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = Set_CREG(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Declare Response Status
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CREG?"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Declare Variable
							uint8_t _CREG_Connection_Mode = 0;
							uint8_t _CREG_Connection_Stat = 0;

							// Get CREG Status
							Get_CREG(_CREG_Connection_Mode, _CREG_Connection_Stat);

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, CYAN, F("    "));
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 32, RED, String(_CREG_Connection_Stat));
							#endif

							// Control for Connection
							if (_CREG_Connection_Stat == 3) {
								
								uint16_t _Error_Code;
								CEER(_Error_Code);

								Terminal_GSM.Text(27, 5, RED, F("CEER Code : "));
								Terminal_GSM.Text(27, 17, RED, String(_Error_Code));

							}

							// Control for Connection
							if (_CREG_Connection_Stat == 1 or _CREG_Connection_Stat == 5) {
								
								// Declare Response Status
								_Response = true;

							} else {

								// Wait Delay
								if (_CREG_Connection_Stat == 2) delay(1000);
								if (_CREG_Connection_Stat == 3) delay(10000);

								// Declare Response Status
								_Response = false;

							}

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(13, 113, CYAN, String((millis() - _Connection_Start_Time) / 1000));
							#endif

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 100) return(false);

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// CGREG Command
					#ifdef _AT_CGREG_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CGREG=0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = Set_CGREG(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Declare Response Status
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CGREG?"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Declare Variable
							uint8_t _CGREG_Connection_Mode = 0;
							uint8_t _CGREG_Connection_Stat = 0;

							// Get CREG Status
							Get_CGREG(_CGREG_Connection_Mode, _CGREG_Connection_Stat);

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, CYAN, F("    "));
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 32, RED, String(_CGREG_Connection_Stat));
							#endif

							// Control for Connection
							if (_CGREG_Connection_Stat == 3) {
								
								uint16_t _Error_Code;
								CEER(_Error_Code);

								Terminal_GSM.Text(27, 5, RED, F("CEER Code : "));
								Terminal_GSM.Text(27, 17, RED, String(_Error_Code));

							}

							// Control for Connection
							if (_CGREG_Connection_Stat == 1 or _CGREG_Connection_Stat == 5) {
								
								// Declare Response Status
								_Response = true;

							} else {

								// Wait Delay
								if (_CGREG_Connection_Stat == 2) delay(1000);
								if (_CGREG_Connection_Stat == 3) delay(10000);

								// Declare Response Status
								_Response = false;

							}

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(13, 113, CYAN, String((millis() - _Connection_Start_Time) / 1000));
							#endif

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 50) return(false);

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// CGDCONT Command Socket
					#ifdef _AT_CGDCONT_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT++CGDCONT=1,**,**,**,0,0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = CGDCONT(1, "IP", "mgbs", "0.0.0.0", 0, 0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// SGACT Command
					#ifdef _AT_SGACT_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#SGACT=1,1"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = Set_SGACT(1, 1, Modem.IP_Address);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(16, 102, CYAN, String(Modem.IP_Address));
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// FRWL Command 1
					#ifdef _AT_FRWL_1_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#FRWL=1,***"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 10, WHITE, String(_AT_FRWL_1_IP_));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FRWL(1, _AT_FRWL_1_IP_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// FRWL Command 2
					#ifdef _AT_FRWL_2_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#FRWL=1,***"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 10, WHITE, String(_AT_FRWL_2_IP_));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FRWL(1, _AT_FRWL_2_IP_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// FRWL Command 3
					#ifdef _AT_FRWL_3_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#FRWL=1,***"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 10, WHITE, String(_AT_FRWL_3_IP_));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FRWL(1, _AT_FRWL_3_IP_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// ICMP Command
					#ifdef _AT_ICMP_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#ICMP=1"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = ICMP(1);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// Set MONI Command
					#ifdef _AT_MONIZIP_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#MONIZIP"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = Get_MONIZIP(Modem.Operator, Modem.LAC, Modem.Cell_ID, Modem.dBm);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(17, 112, CYAN, String(Modem.LAC));
							Terminal_GSM.Text(18, 112, CYAN, String(Modem.Cell_ID));
							Terminal_GSM.Text(15, 112, CYAN, String(Modem.Operator));
							Terminal_GSM.Text(14, 115, CYAN, String(Modem.dBm));
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// Set Connection Time
					Modem.Connection_Time = ((millis() - _Connection_Start_Time) / 1000);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(13, 113, CYAN, String(Modem.Connection_Time));
					#endif

					// Set Variable
					this->Status.Connection = true;

				} else {

					// Initialise Modem
					this->Initialize();

				}

				// End Function
				return(this->Status.Connection);

			}

	};

	// Cloud Functions
	class PostOffice : public xE910 {

		// Private Functions
		private:

			// Define JSON Status Structure
			struct JSON_Device_Structure {

				// Define JSON Status Structure
				struct JSON_Info_Structure {
					char * Device_ID;
					float Temperature;
					float Humidity;
				} JSON_Info;

				// Define JSON Battery Structure
				struct JSON_Battery_Structure {
					float IV;
					float AC;
					float SOC;
					uint8_t Charge;
					float T;
					uint16_t FB;
					uint16_t IB;
				} JSON_Battery;

				// Define JSON Status Structure
				struct JSON_Status_Structure {
					uint16_t Device;
					uint16_t Fault;
				} JSON_Status;

				// Time Stamp
				char * Time_Stamp;

			} JSON_Data;

			// Define JSON
			String JSON_Pack;

			// Parse JSON Pack
			uint16_t Parse_JSON(uint8_t _Pack_Type) {

				// Clear Pack
				this->JSON_Pack = "";

				// Define Versions
				#ifndef __Hardware__
					#define __Hardware__ "00.00.00"
				#endif
				#ifndef __Firmware__
					#define __Firmware__ "00.00.00"
				#endif

				// Define JSON
				StaticJsonDocument<1024> JSON;

				// Set Device ID Variable
				if (_Pack_Type == Pack_Online) JSON[F("Command")] = F("STF:PowerStat.Online");
				if (_Pack_Type == Pack_Timed) JSON[F("Command")] = F("STF:PowerStat.Timed");
				if (_Pack_Type == Pack_Interrupt) JSON[F("Command")] = F("STF:PowerStat.Interrupt");
				if (_Pack_Type == Pack_Alarm) JSON[F("Command")] = F("STF:PowerStat.Alarm");
				if (_Pack_Type == Pack_Offline) JSON[F("Command")] = F("STF:PowerStat.Offline");

				// Define Device Section
				JsonObject JSON_Device = JSON.createNestedObject(F("Device"));

				// Define Device Section
				JsonObject JSON_Info= JSON_Device.createNestedObject(F("Info"));

				// Set Device ID Variable
				JSON_Info[F("ID")] = this->JSON_Data.JSON_Info.Device_ID;
				
				// Set Device Hardware Version Variable
				if (_Pack_Type == Pack_Online) JSON_Info[F("Hardware")] = F(__Hardware__);

				// Set Device Firmware Version Variable
				if (_Pack_Type == Pack_Online) JSON_Info[F("Firmware")] = F(__Firmware__);

				// Set Device Environment Variable
				JSON_Info[F("Temperature")] = this->JSON_Data.JSON_Info.Temperature;
				JSON_Info[F("Humidity")] = this->JSON_Data.JSON_Info.Humidity;

				// Define Power Section
				JsonObject JSON_Battery = JSON_Device["Power"].createNestedObject("Battery");

				// Set Battery Variables
				JSON_Battery[F("IV")] = this->JSON_Data.JSON_Battery.IV;
				JSON_Battery[F("AC")] = this->JSON_Data.JSON_Battery.AC;
				JSON_Battery[F("SOC")] = this->JSON_Data.JSON_Battery.SOC;
				JSON_Battery[F("Charge")] = this->JSON_Data.JSON_Battery.Charge;
				if (_Pack_Type == Pack_Online) JSON_Battery[F("T")] = this->JSON_Data.JSON_Battery.T;
				if (_Pack_Type == Pack_Online) JSON_Battery[F("FB")] = this->JSON_Data.JSON_Battery.FB;
				if (_Pack_Type == Pack_Online) JSON_Battery[F("IB")] = this->JSON_Data.JSON_Battery.IB;

				// Set MONI Command
				#ifdef _AT_MONIZIP_

					// Declare Watchdog Variable
					uint8_t _Error_WD = 0;

					// Set Response Variable
					bool _Response = false;

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = Get_MONIZIP(Modem.Operator, Modem.LAC, Modem.Cell_ID, Modem.dBm);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(17, 112, CYAN, String(Modem.LAC));
						Terminal_GSM.Text(18, 112, CYAN, String(Modem.Cell_ID));
						Terminal_GSM.Text(15, 112, CYAN, String(Modem.Operator));
						Terminal_GSM.Text(14, 115, CYAN, String(Modem.dBm));
					#endif

				#endif

				// Define GSM Section
				JsonObject JSON_GSM = JSON_Device["IoT"].createNestedObject(F("GSM"));

				// Get GSM Parameters
				if (_Pack_Type == Pack_Online) {

					// Define IoT Module
					JsonObject JSON_Module = JSON_GSM.createNestedObject(F("Module"));

					// Set IoT Parameters
					JSON_Module[F("Manufacturer")] = Modem.Manufacturer;
					JSON_Module[F("Model")] = Modem.Model;
					JSON_Module[F("Firmware")] = Modem.Firmware;
					JSON_Module[F("Serial")] = Modem.Serial_ID;
					JSON_Module[F("IMEI")] = Modem.IMEI;

					// Define SIM
					JsonObject JSON_SIM = JSON_GSM.createNestedObject(F("SIM"));

					// Set SIM Parameters
					JSON_SIM[F("SIM_Type")] = 1;
					JSON_SIM[F("ICCID")] = Modem.ICCID;

				}

				// Define GSM Operator Section
				JsonObject JSON_Operator = JSON_GSM.createNestedObject(F("Operator"));

				// Set Device GSM Connection Detail Section
				JSON_Operator[F("Code")] = Modem.Operator;
				JSON_Operator[F("dBm")] = Modem.dBm;
				JSON_Operator[F("LAC")] = Modem.LAC;
				JSON_Operator[F("Cell_ID")] = Modem.Cell_ID;

				// Define Data Section
				JsonObject JSON_Payload = JSON.createNestedObject(F("Payload"));

				// Set Device Time Variable
				JSON_Payload[F("TimeStamp")] = this->JSON_Data.Time_Stamp;

				// Set Device Status Variable
				JSON_Payload[F("Device")] = this->JSON_Data.JSON_Status.Device;

				// Set Device Fault Variable
				JSON_Payload[F("Fault")] = this->JSON_Data.JSON_Status.Fault;

				// Clear Unused Data
				JSON.garbageCollect();

				// Serialize JSON	
				uint16_t _JSON_Size = serializeJson(JSON, this->JSON_Pack);

				// End Function
				return(_JSON_Size);

			}

			/**
			 * @brief Handle Send Response
			 * @param _Data Received Response
			 * @return uint16_t Command
			 */		
			uint16_t Handle_JSON_Send_Response(const char *_Data) {

				// Declare JSON Object
				StaticJsonDocument<32> Incoming_JSON;

				// Deserialize the JSON document
				deserializeJson(Incoming_JSON, _Data);

				// Fetch values.
				uint16_t Event = Incoming_JSON["Event"];

				// End Function
				return(Event);

			}

		// Public Functions
		public:

			// PostOffice Constructor
			PostOffice(Stream &_Serial) : xE910() {

				// Set Serial Port
				GSM_Serial = & _Serial;

			}

			// Connect Cloud
			bool Connect(char * _Device_ID) {

				// Set Device ID
				this->JSON_Data.JSON_Info.Device_ID = _Device_ID;

				// Declare Watchdog Variable
				uint8_t _Error_WD = 0;

				// Declare Response Status
				bool _Response = false;

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(21, 108, YELLOW, F("........."));
				#endif

				// Process Command
				while (!_Response) {

					// Process Command
					_Response = SCFG(3, 1, 1500, 90, 300, 50);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// End Function
				if (!_Response) return (false);

				// Declare Watchdog Variable
				_Error_WD = 0;

				// Set Response Variable
				_Response = false;

				// Process Command
				while (!_Response) {

					// Process Command
					_Response = SCFGEXT(3, 1, 0, 1, 0, 0);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}
			
				// End Function
				if (!_Response) return (false);

				// Print Command State
				#ifdef GSM_Debug
					if (_Response) Terminal_GSM.Text(21, 108, GREEN, F("Connected"));
				#endif

				// End Function
				return(true);

			}

			// Set Environment Variables
			void Environment(float _Temperature, float _Humidity) {

				// Set Environment
				this->JSON_Data.JSON_Info.Temperature = _Temperature;
				this->JSON_Data.JSON_Info.Humidity = _Humidity;

			}

			// Set Battery Variables
			void Battery(float _IV, float _AC, float _SOC, uint8_t _Charge, float _T = 0, uint16_t _FB = 0, uint16_t _IB = 0) {

				// Set Battery Parameters
				this->JSON_Data.JSON_Battery.IV = _IV;
				this->JSON_Data.JSON_Battery.AC = _AC;
				this->JSON_Data.JSON_Battery.SOC = _SOC;
				this->JSON_Data.JSON_Battery.Charge = _Charge;
				this->JSON_Data.JSON_Battery.T = _T;			// Optional
				this->JSON_Data.JSON_Battery.FB = _FB;			// Optional
				this->JSON_Data.JSON_Battery.IB = _IB;			// Optional

			}

			// Set TimeStamp
			void TimeStamp(char * _TimeStamp) {

				// Set Time Stamp
				this->JSON_Data.Time_Stamp = _TimeStamp;

			}

			// Set Status
			void Status(uint16_t _Device, uint16_t _Fault) {

				// Set Device Status Variables
				this->JSON_Data.JSON_Status.Device = _Device;
				this->JSON_Data.JSON_Status.Fault = _Fault;

			}

			/**
			 * @brief Send Data Batch Function
			 * @param _Data Sended Data
			 * @param _Response Received Data
			 * @return uint16_t Server Request Command
			 */
			bool Send(uint8_t _Pack_Type) {

				// Parse JSON
				uint16_t _JSON_Size = this->Parse_JSON(Pack_Online);

				// Open Connection
				if (SD(3, 0, 80, 0, 88, 1, PostOffice_Server)) {

					// Clear UART Buffer
					Clear_UART_Buffer();

					// Declare Buffer Object
					Serial_Buffer Buffer_Set = {false, 0, 0, 2000};

					// Declare Buffer
					char Buffer_Variable[255];
					memset(Buffer_Variable, '\0', 255);

					// Command Chain Delay (Advice by Telit)
					delay(20);

					// Send UART Command
					GSM_Serial->print(F("AT#SSEND=3"));
					GSM_Serial->print(F("\r\n"));

					// Read Current Time
					uint32_t Current_Time = millis();

					// Response Wait Delay
					delay(10);

					// Read UART Response
					while (!Buffer_Set.Response) {

						// Read Serial Char
						Buffer_Variable[Buffer_Set.Read_Order] = GSM_Serial->read();

						// Control for <OK> Response
						if (Buffer_Variable[Buffer_Set.Read_Order - 1] == '>' and Buffer_Variable[Buffer_Set.Read_Order] == ' ') Buffer_Set.Response = true;

						// Increase Read Order
						if (Buffer_Variable[Buffer_Set.Read_Order] > 31 and Buffer_Variable[Buffer_Set.Read_Order] < 127) Buffer_Set.Read_Order += 1;

						// Handle for timeout
						if (millis() - Current_Time >= Buffer_Set.Time_Out) return(false);

					}

					// Send Delay
					delay(10);

					// Print HTTP Header
					GSM_Serial->print(F("POST ")); GSM_Serial->print(PostOffice_EndPoint); GSM_Serial->print(F(" HTTP/1.1\r\n"));
					GSM_Serial->print(F("Host: ")); GSM_Serial->print(PostOffice_Server); GSM_Serial->print(F("\r\n"));
					GSM_Serial->print(F("Content-Length: ")); GSM_Serial->print(_JSON_Size); GSM_Serial->print(F("\r\n"));
					GSM_Serial->print(F("Connection: close\r\n"));
					GSM_Serial->print(F("Content-Type: application/json\r\n"));
					GSM_Serial->print(F("User-Agent: PowerStat\r\n"));
					GSM_Serial->print(F("\r\n"));

					// Send Data Pack
	//				Serial.println(_JSON_Size);
					GSM_Serial->print(this->JSON_Pack);

					// Print End Char
					GSM_Serial->print((char)26);

					// Declare Buffer Object
					Serial_Buffer Buffer_Get = {false, 0, 0, 2000};

					// Declare Buffer
					memset(Buffer_Variable, '\0', 255);

					// Read Current Time
					Current_Time = millis();

					// Read UART Response
					while (!Buffer_Get.Response) {

						// Read Serial Char
						Buffer_Variable[Buffer_Get.Read_Order] = GSM_Serial->read();

						// Control for <OK> Response
						if (Buffer_Variable[Buffer_Get.Read_Order - 1] == 'O' and Buffer_Variable[Buffer_Get.Read_Order] == 'K') Buffer_Get.Response = true;

						// Increase Read Order
						if (Buffer_Variable[Buffer_Get.Read_Order] > 31 and Buffer_Variable[Buffer_Get.Read_Order] < 127) Buffer_Get.Read_Order += 1;

						// Handle for timeout
						if (millis() - Current_Time >= Buffer_Get.Time_Out) return(false);

					}

					// Response Wait Delay
					delay(10);

					// Declare Ring Status
					char _Response[32];
					uint8_t Ring_ID;
					uint16_t Length;
					uint16_t Response_Command;

					// Get Ring Port
					if (Send_SRING(Ring_ID, Length)) {

						// Get Request Data
						SRECV(3, Length, _Response);

						// Handle JSON
						Response_Command = this->Handle_JSON_Send_Response(_Response);

						// Close Socket
						SH(3);

						// End Function
						if (Response_Command == 200) return(true);
						
					}

					// End Function
					return(false);

				}

				// End Function
				return(false);

			}

	};

#endif /* defined(__Telit_xE910__) */