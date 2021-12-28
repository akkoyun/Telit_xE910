/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit GE910 Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <Telit_xE910.h>

bool xE910_AT::ATE(const bool _ECHO) {

    // Declare Response Length
    uint8_t _Response_Length = 12;

	// Set Control Variable
	Command_Control.ATE = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("ATE"));
	GSM_Serial.print(String(_ECHO));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Set Control Variable
			Command_Control.ATE = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.ATE = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.ATE = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::CMEE(const uint8_t _CMEE) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.CMEE = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CMEE="));
	GSM_Serial.print(String(_CMEE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Set Control Variable
			Command_Control.CMEE = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.CMEE = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.CMEE = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::FCLASS(const uint8_t _FCLASS) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.FCLASS = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+FCLASS="));
	GSM_Serial.print(String(_FCLASS));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Set Control Variable
			Command_Control.FCLASS = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.FCLASS = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.FCLASS = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::K(const uint8_t _K) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.K = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT&K"));
	GSM_Serial.print(String(_K));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Set Control Variable
			Command_Control.K = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.K = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.K = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::CPIN(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 22;

	// Set Control Variable
	Command_Control.CPIN = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CPIN?"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "READY") != NULL) {

			// Set Control Variable
			Command_Control.CPIN = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.CPIN = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.CPIN = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::CGSN(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 25;

	// Set Control Variable
	Command_Control.CGSN = false;

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGSN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;
		uint8_t _Data_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];
		char _IMEI[18];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

				// Get Data
				_IMEI[_Data_Order] = _Response[_Read_Order];

				// Increase Data Order
				_Data_Order++;

			}

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Set IMEI Variable
			IMEI = atoi(_IMEI);

			// Set Control Variable
			Command_Control.CGSN = true;

			// End Function
			return (true);

		} else {

			// Set IMEI Variable
			IMEI = 0;

			// Set Control Variable
			Command_Control.CGSN = false;

			// End Function
			return (false);

		}

	} else {

		// Set IMEI Variable
		IMEI = 0;

		// Set Control Variable
		Command_Control.CGSN = false;

		// End Function
		return (false);

    }
}
bool xE910_AT::GSN(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 20;

	// Set Control Variable
	Command_Control.GSN = false;

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GSN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;
		uint8_t _Data_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];
		char _Serial_Number[10];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

				// Get Data
				_Serial_Number[_Data_Order] = _Response[_Read_Order];

				// Increase Data Order
				_Data_Order++;

			}

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Set IMEI Variable
			Serial_Number = atoi(_Serial_Number);

			// Set Control Variable
			Command_Control.GSN = true;

			// End Function
			return (true);

		} else {

			// Set IMEI Variable
			Serial_Number = 0;

			// Set Control Variable
			Command_Control.GSN = false;

			// End Function
			return (false);

		}

	} else {

		// Set IMEI Variable
		Serial_Number = 0;

		// Set Control Variable
		Command_Control.GSN = false;

		// End Function
		return (false);

    }
}
bool xE910_AT::CCID(void) {

	// Control for SIM Module
	if (Command_Control.CPIN) {
		
		// Command Chain Delay (Advice by Telit)
		delay(10);

    	// Declare Response Length
    	uint8_t _Response_Length = 36;

		// Set Control Variable
		Command_Control.ICCID = false;

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT#CCID"));
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Handle Response
		if (_Response_Wait(_Response_Length, 500)) {

			// Declare Read Order Variable
			uint8_t _Read_Order = 0;
			uint8_t _Data_Order = 0;

			// Declare Response Variable
			char _Response[_Response_Length];
			char _ICCID[20];

			// Read UART Response
			while (GSM_Serial.available() > 0) {

				// Read Serial Char
				_Response[_Read_Order] = GSM_Serial.read();

				// Handle Data
				if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

					// Get Data
					_ICCID[_Data_Order] = _Response[_Read_Order];

					// Increase Data Order
					_Data_Order++;

				}

				// Increase Read Order
				_Read_Order++;

				// Stream Delay
				delayMicroseconds(500);

			}

			// Control for Response
			if (strstr(_Response, "OK") != NULL) {

				// Set ICCID Variable
				ICCID = atoi(_ICCID);

				// Set Control Variable
				Command_Control.ICCID = true;

				// End Function
				return (true);

			} else {

				// Set IMEI Variable
				ICCID = 0;

				// Set Control Variable
				Command_Control.ICCID = false;

				// End Function
				return (false);

			}


		} else {

			// Set IMEI Variable
			ICCID = 0;

			// Set Control Variable
			Command_Control.ICCID = false;

			// End Function
			return (false);
		
		}

	} else {

		// Set IMEI Variable
		ICCID = 0;

		// End Function
		return (false);

	}

}
bool xE910_AT::GMI(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 15;

	// Set Control Variable
	Command_Control.GMI = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMI"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "Telit") != NULL) {

			// Set Manufacturer Variable
			Manufacturer = 1;

			// Set Control Variable
			Command_Control.GMI = true;

			// End Function
			return (true);

		} else {

			// Set Manufacturer Variable
			Manufacturer = 0;

			// Set Control Variable
			Command_Control.GMI = false;

			// End Function
			return (false);

		}

    } else {

		// Set Manufacturer Variable
		Manufacturer = 0;

		// Set Control Variable
		Command_Control.GMI = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::GMM(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 20;

	// Set Control Variable
	Command_Control.GMM = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMM"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 500)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "GE910-QUAD") != NULL) {

			// Set Model Variable
			Model = 1;

			// Set Control Variable
			Command_Control.GMM = true;

			// End Function
			return (true);

		} else {

			// Set Model Variable
			Model = 0;

			// Set Control Variable
			Command_Control.GMM = false;

			// End Function
			return (false);

		}

    } else {

		// Set Model Variable
		Model = 0;

		// Set Control Variable
		Command_Control.GMM = false;

		// End Function
		return (false);

    }

}














void xE910_AT::_Clear_UART_Buffer(void) {

	while (GSM_Serial.available() > 0) {
        
        GSM_Serial.read(); 
        delay(3);

    }

}
bool xE910_AT::_Response_Wait(uint16_t _Length, uint32_t _TimeOut) {

	// Read Current Time
	uint32_t _Time = millis();

	// Wait for UART Available
	while ((uint16_t)GSM_Serial.available() < _Length) {

		// Wait delay
		delay(3);

		// Handle for timeout
		if (millis() - _Time >= _TimeOut) return (false);

	}

	// End Function
	return (true);

}


void xE910_GSM::Initialize(void) {

}


// Define Library Class
xE910_AT GSM_AT;
xE910_GSM GSM;