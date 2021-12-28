/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit GE910 Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <Telit_xE910.h>

bool xE910_AT::ATE(const bool _n) {

    // Declare Response Length
    uint8_t _Response_Length = 12;

	// Set Control Variable
	Command_Control.ATE = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("ATE"));
	GSM_Serial.print(String(_n));
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