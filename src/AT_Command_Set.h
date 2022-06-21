#ifndef __AT_Command_Set__
#define __AT_Command_Set__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

class AT_Command_Set {
	
	
	private:

		/**
		 * @brief GSM Serial Stream Definition
		 */
		Stream * GSM_Serial;

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

	public:

		/**
		 * @brief Construct a new at command set object
		 * @param _Serial GSM Serial Object
		 */
		AT_Command_Set(Stream &_Serial) {

			// Set Serial Port
			GSM_Serial = & _Serial;

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
		 * @param _CMEE Parameter. 0:Disable, 1:Numeric Format, 2:Verbos Format
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
		 * @brief Set command controls the RS232 flow control behaviour.
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
		 * @brief Set command sets the behaviour of the STAT_LED GPIO.
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
		 * @brief Set TXMON pin behaviour.
		 * @param _TXMONMODE Parameter
		 * 0 : TXMON pin goes high when a call is started and it drops down when the call is ended. 
		 * It also goes high when a location update starts, and it drops down when the location 
		 * update procedure stops. Finally it goes high during SMS transmission and receiving. 
		 * Even if the TXMON in this case is set as GPIO in output, the read command AT#GPIO=5,2 
		 * returns gPIO:2,0, as the GPIO is in alternate mode.
		 * 1 : TXMON is set in alternate mode and the Timer unit controls its state. TXMON goes 
		 * high before power ramps start raising and drops down after power ramps stop falling down. 
		 * This behaviour is repeated for every transmission burst.
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
			GSM_Serial->print(F(","));
			GSM_Serial->print(_Format);
			GSM_Serial->print(F(","));
			GSM_Serial->print(_Operator);
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
		bool Get_CREG(uint8_t & _Stat) {

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

			// Set Comma Handle
			uint8_t _Stat_Position = 0;

			// Control for Buffer
			for (uint8_t i = 0; i < Buffer.Read_Order; i++) {if (Buffer_Variable[i] == ',') _Stat_Position = i + 1;}

			// Declare Status Variable
			_Stat = Buffer_Variable[_Stat_Position] - 48;

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
		bool Get_CGREG(uint8_t & _Stat) {

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

			// Set Comma Handle
			uint8_t _Stat_Position = 0;

			// Control for Buffer
			for (uint8_t i = 0; i < Buffer.Read_Order; i++) {if (Buffer_Variable[i] == ',') _Stat_Position = i + 1;}

			// Declare Status Variable
			_Stat = Buffer_Variable[_Stat_Position] - 48;

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

			// Control for Buffer
			for (uint8_t i = 0; i < Buffer.Read_Order; i++) {

				// Handle Data
				if (((Buffer_Variable[i] <= '9' and Buffer_Variable[i] >= '0') or Buffer_Variable[i] == '.')) {

					// Get Data
					_IP[Buffer.Data_Order] = Buffer_Variable[i] ;

					// Increase Data Order
					Buffer.Data_Order += 1;

				}

			}

			// Control for IP
			if (Buffer.Data_Order >= 7) {

				// End Function
				return (true);

			} else {

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
		 * 1 - enable firewalled ICMP Ping support: the module is sending a proper
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
		 * @param _Operator Parameter. 
		 * string representing the network operator in numeric format: 5 or 6 digits [country code (3) + network code (2 or 3)]
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool SERVINFO(uint16_t & _Operator) {

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

			// Clear Variables
			_Operator = 0;

			// Control for Buffer
			for (uint8_t i = 0; i < Buffer.Read_Order; i++) {

				// Control Operator ID
				if (Buffer_Variable[i - 2] == '6' and Buffer_Variable[i - 1] == '0' and Buffer_Variable[i] == '1') _Operator = 28601;
				if (Buffer_Variable[i - 2] == '6' and Buffer_Variable[i - 1] == '0' and Buffer_Variable[i] == '2') _Operator = 28602;
				if (Buffer_Variable[i - 2] == '6' and Buffer_Variable[i - 1] == '0' and Buffer_Variable[i] == '3') _Operator = 28603;
				if (Buffer_Variable[i - 2] == '6' and Buffer_Variable[i - 1] == '0' and Buffer_Variable[i] == '4') _Operator = 28604;

			}

			// Handle Operator
			if (_Operator == 28601 or _Operator == 28602 or _Operator == 28603 or _Operator == 28604) return(true);

			// End Function
			return (false);

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

			// Handle Time
			_Year = (((Buffer_Variable[8] - 48) * 10) + (Buffer_Variable[9] - 48));
			_Month = (((Buffer_Variable[11] - 48) * 10) + (Buffer_Variable[12] - 48));
			_Day = (((Buffer_Variable[14] - 48) * 10) + (Buffer_Variable[15] - 48));
			_Hour = (((Buffer_Variable[17] - 48) * 10) + (Buffer_Variable[18] - 48));
			_Minute = (((Buffer_Variable[20] - 48) * 10) + (Buffer_Variable[21] - 48));
			_Second = (((Buffer_Variable[23] - 48) * 10) + (Buffer_Variable[24] - 48));

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
				Terminal.Text(26, 108, CYAN, "         ");
				if (_State == 0) Terminal.Text(26, 108, RED, "Closed   ");
				if (_State == 1) Terminal.Text(26, 108, WHITE, "Transfer ");
				if (_State == 2) Terminal.Text(26, 108, YELLOW, "Suspend  ");
				if (_State == 3) Terminal.Text(26, 108, YELLOW, "Suspend  ");
				if (_State == 4) Terminal.Text(26, 108, GREEN, "Listening");
				if (_State == 5) Terminal.Text(26, 108, WHITE, "Incomming");
				if (_State == 6) Terminal.Text(26, 108, RED, "DNS      ");
				if (_State == 7) Terminal.Text(26, 108, WHITE, "Conecting");
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
		 * TODO:Develeopment
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
		 * @param _Conn_ID Parameter. 
		 * Socket connection identifier
		 * @param _Data_Pack Parameter. 
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool SSEND(const uint8_t _ConnID, const uint8_t _Send_Type, const char * _IP, const char * _URL, const char * _Data_Pack) {

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
			this->Send_Header(_Send_Type, _IP, _URL, _Data_Pack);

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

			// Control for Buffer
			for (uint8_t i = 0; i < Buffer.Read_Order - 2; i++) {

				// Handle JSON Data
				if (Buffer_Variable[i] == '{') Data_Handle = true;

				// Get Data
				if (Data_Handle) {
					_Data[Buffer.Data_Order] = Buffer_Variable[i];
					Buffer.Data_Order += 1;
				}

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
			Serial_Buffer Buffer = {false, 0, 0, 15000};

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
		bool Recieve_SRING(uint8_t & _Ring) {

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
		bool Send_Header(const uint8_t _Header_Type, const char *_IP, const char *_URL, const char *_Data) {

			// Handle Type
			if (_Header_Type == 1) {

				// Print HTTP Header
				GSM_Serial->print(F("HTTP/1.1 200 OK\r\n"));
				GSM_Serial->print(F("Connection: close\r\n"));
				GSM_Serial->print(F("Content-Type: application/json\r\n"));
				GSM_Serial->print(F("User-Agent: STF\r\n"));
				GSM_Serial->print(F("\r\n"));

				// End Function
				return(true);

			} else if (_Header_Type == 2) {

				// Print HTTP Header
				GSM_Serial->print(F("POST ")); GSM_Serial->print(_URL); GSM_Serial->print(F(" HTTP/1.1\r\n"));
				GSM_Serial->print(F("Host: ")); GSM_Serial->print(_IP); GSM_Serial->print(F("\r\n"));
				GSM_Serial->print(F("Content-Length: ")); GSM_Serial->print(String(_Data).length()); GSM_Serial->print(F("\r\n"));
				GSM_Serial->print(F("Connection: close\r\n"));
				GSM_Serial->print(F("Content-Type: application/json\r\n"));
				GSM_Serial->print(F("User-Agent: STF-PowerStat\r\n"));
				GSM_Serial->print(F("\r\n"));

				// End Function
				return(true);

			}

			// End Function
			return(false);

		}

};

#endif /* defined(__AT_Command_Set__) */