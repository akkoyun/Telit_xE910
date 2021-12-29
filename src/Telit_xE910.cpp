/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit GE910 Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <Telit_xE910.h>

bool xE910_HARDWARE::Communication(const bool _State) {

	// Enable Communication 
	if (_State) PORTJ &= 0b11101111;

	// Disable Communication
	if (_State) PORTJ |= 0b00010000;

	// Control Buffer Enable Signal
	if ((PINJ & (1 << PINJ4)) == (1 << PINJ4)) {
	
		// Bufer EN Signal High
		return(false);
	
	} else {
		
		// Bufer EN Signal LOW
		return(true);
	
	}

}
bool xE910_HARDWARE::PowerMonitor(void) {

	// Control for PWMon (PJ3)
	if ((PINJ & (1 << PINJ3)) == (1 << PINJ3)) {

		// Set Variable
		Power_Monitor = POWERED;

		// Power Monitor 3V3 HIGH
		return (true);

	} else {

		// Set Variable
		Power_Monitor = NOT_POWERED;

		// Power Monitor 3V3 LOW
		return (false);

	}

}
void xE910_HARDWARE::OnOff(const uint16_t _Time) {

	// Set On/Off Signal HIGH [PJ6]
	PORTJ |= 0b01000000;

	// Command Delay
	delay(_Time);

	// Set On/Off Signal LOW [PJ6]
	PORTJ &= 0b10111111;

}
void xE910_HARDWARE::ShutDown(const uint16_t _Time) {

	// Set Shut Down Signal HIGH [PJ5]
	PORTJ |= 0b00100000;

	// Command Delay
	delay(_Time);

	// Set Shut Down Signal LOW [PJ5]
	PORTJ &= 0b11011111;

}
void xE910_HARDWARE::Power_Switch(const bool _State) {

	// Set GSM Power Enable
	if (_State) PORTH |= 0b00000100;

	// Set GSM Power Disable
	if (!_State) PORTH &= 0b11111011;

}
void xE910_HARDWARE::LED(const bool _State) {

	// Set GSM LED Power Enable
	if (_State) PORTH &= 0b11101111;

	// Set GSM LED Power Disable
	if (!_State) PORTH |= 0b00010000;

}

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
bool xE910_AT::GMR(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 19;

	// Set Control Variable
	Command_Control.GMR = false;

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMR"));
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
		char _Modem_Firmware[10];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if ((_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) or _Response[_Read_Order] == 46) {

				// Get Data
				_Modem_Firmware[_Data_Order] = _Response[_Read_Order];

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

			// Set Firmware Variable
			memset(Modem_Firmware, 0, sizeof(Modem_Firmware));
			strcpy(Modem_Firmware, _Modem_Firmware);

			// Set Control Variable
			Command_Control.GMR = true;

			// End Function
			return (true);

		} else {

			// Set Firmware Variable
			memset(Modem_Firmware, 0, sizeof(Modem_Firmware));

			// Set Control Variable
			Command_Control.GMR = false;

			// End Function
			return (false);

		}

	} else {

		// Set Firmware Variable
		memset(Modem_Firmware, 0, sizeof(Modem_Firmware));

		// Set Control Variable
		Command_Control.GMR = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::SLED(const uint8_t _SLED) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.SLED = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SLED="));
	GSM_Serial.print(String(_SLED));
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
			Command_Control.SLED = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.SLED = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.SLED = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::TXMONMODE(const uint8_t _TXMONMODE) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.TXMONMODE = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#TXMONMODE="));
	GSM_Serial.print(String(_TXMONMODE));
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
			Command_Control.TXMONMODE = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.TXMONMODE = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.TXMONMODE = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::REGMODE(const uint8_t _REGMODE) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.REGMODE = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#REGMODE="));
	GSM_Serial.print(String(_REGMODE));
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
			Command_Control.REGMODE = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.REGMODE = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.REGMODE = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::CREG(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

	// Declare Function Variables
	bool _Control = false;
	uint8_t _Error_WD = 0;

	// Declare Stat Variable
	uint8_t _Stat = NOT_REGISTERED;

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.CREG = false;

	// Send AT+CREG=1 Connection Command
	while (!_Control) {

		// Clear UART Buffer
    	_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT+CREG=1"));
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
			if (strstr(_Response, "OK") != NULL) _Control = true;

	    } 

		// Count for Error
		_Error_WD++;

		// Handle for Error
		if (_Error_WD >= 10) return (false);
	
	}

	// Reset Control Variables
	_Control = false;
	_Error_WD = 0;
    _Response_Length = 12;

	// Command Chain Delay (Advice by Telit)
	delay(10);

	// Send CREG: 1 Connection Command
	while (!_Control) {

		// Handle Response
		if (_Response_Wait(_Response_Length, 600)) {

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
			if (strstr(_Response, "+CREG:") != NULL) {

				// Read Stat
				_Stat = _Response[9];

				// Handle Stat Variable
				if (_Stat == 48) {

					// Set Variable
					CREG_Status = NOT_REGISTERED;

					// Set Control Variable
					Command_Control.CREG = false;

					// Set Control Variable
					_Control = false;

				} // Not Registered [0]
				if (_Stat == 49) {

					// Set Variable
					CREG_Status = HOME_REGISTERED;

					// Set Control Variable
					Command_Control.CREG = true;

					// Set Control Variable
					_Control = true;

				} // Registered to Home Network [1]
				if (_Stat == 50) {

					// Set Variable
					CREG_Status = SEARCHING;

					// Set Control Variable
					Command_Control.CREG = false;

					// Set Control Variable
					_Control = false;

				} // Searching Network [2]
				if (_Stat == 51) {

					// Set Variable
					CREG_Status = DENIED;

					// Set Control Variable
					Command_Control.CREG = false;

					// Set Control Variable
					_Control = false;

				} // Registration Denied [3]
				if (_Stat == 52) {

					// Set Variable
					CREG_Status = UNKNOWN;

					// Set Control Variable
					Command_Control.CREG = false;

					// End Function
					return (false);

				} // Unknown Error [4]
				if (_Stat == 53) {

					// Set Variable
					CREG_Status = ROAMING_REGISTERED;

					// Set Control Variable
					Command_Control.CREG = true;

					// Set Control Variable
					_Control = true;

				} // Registered to Rooming Network [5]
			
			}

	    } 

		// Count for Error
		_Error_WD++;

		// Handle for Error
		if (_Error_WD >= 3) return (false);
	
	}

	// Control for Response
	if ((CREG_Status == HOME_REGISTERED) or (CREG_Status == ROAMING_REGISTERED)) return (true);

	// End Function
	return (false);

}
bool xE910_AT::CGREG(void) {

	// Control for CREG
	if (CREG_Status == HOME_REGISTERED or CREG_Status == ROAMING_REGISTERED) {
	
		// Command Chain Delay (Advice by Telit)
		delay(10);

		// Declare Function Variables
		bool _Control = false;
		uint8_t _Error_WD = 0;

		// Declare Stat Variable
		uint8_t _Stat = NOT_REGISTERED;

    	// Declare Response Length
    	uint8_t _Response_Length = 6;

		// Set Control Variable
		Command_Control.CGREG = false;

		// Send AT+CREG=1 Connection Command
		while (!_Control) {

			// Clear UART Buffer
    		_Clear_UART_Buffer();

			// Send UART Command
			GSM_Serial.print(F("AT+CGREG=1"));
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
				if (strstr(_Response, "OK") != NULL) _Control = true;

			} 

			// Count for Error
			_Error_WD++;

			// Handle for Error
			if (_Error_WD >= 10) return (false);
	
		}

		// Reset Control Variables
		_Control = false;
		_Error_WD = 0;
    	_Response_Length = 13;

		// Command Chain Delay (Advice by Telit)
		delay(10);

		// Send CREG: 1 Connection Command
		while (!_Control) {

			// Handle Response
			if (_Response_Wait(_Response_Length, 600)) {

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
				if (strstr(_Response, "+CGREG:") != NULL) {

					// Read Stat
					_Stat = _Response[10];

					// Handle Stat Variable
					if (_Stat == 48) {

						// Set Variable
						CGREG_Status = NOT_REGISTERED;

						// Set Control Variable
						Command_Control.CGREG = false;

						// Set Control Variable
						_Control = false;

					} // Not Registered [0]
					if (_Stat == 49) {

						// Set Variable
						CGREG_Status = HOME_REGISTERED;

						// Set Control Variable
						Command_Control.CGREG = true;

						// Set Control Variable
						_Control = true;

					} // Registered to Home Network [1]
					if (_Stat == 50) {

						// Set Variable
						CGREG_Status = SEARCHING;

						// Set Control Variable
						Command_Control.CGREG = false;

						// Set Control Variable
						_Control = false;

					} // Searching Network [2]
					if (_Stat == 51) {

						// Set Variable
						CGREG_Status = DENIED;

						// Set Control Variable
						Command_Control.CGREG = false;

						// Set Control Variable
						_Control = false;

					} // Registration Denied [3]
					if (_Stat == 52) {

						// Set Variable
						CGREG_Status = UNKNOWN;

						// Set Control Variable
						Command_Control.CGREG = false;

						// End Function
						return (false);

					} // Unknown Error [4]
					if (_Stat == 53) {

						// Set Variable
						CGREG_Status = ROAMING_REGISTERED;

						// Set Control Variable
						Command_Control.CGREG = true;

						// Set Control Variable
						_Control = true;

					} // Registered to Rooming Network [5]
			
				}

		    } 

			// Count for Error
			_Error_WD++;

			// Handle for Error
			if (_Error_WD >= 3) return (false);
	
		}

		// Control for Response
		if ((CGREG_Status == HOME_REGISTERED) or (CGREG_Status == ROAMING_REGISTERED)) return (true);

		// End Function
		return (false);

	}

	// End Function
	return (false);

}
bool xE910_AT::CGDCONT(const uint8_t _Cid, const char *_PDP_Type, const char *_APN, const char *_PDP_Addr, const bool _D_Comp, const bool _H_Comp) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.CGDCONT = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGDCONT="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F(",\""));
	GSM_Serial.print(String(_PDP_Type));
	GSM_Serial.print(F("\",\""));
	GSM_Serial.print(String(_APN));
	GSM_Serial.print(F("\",\""));
	GSM_Serial.print(String(_PDP_Addr));
	GSM_Serial.print(F("\","));
	GSM_Serial.print(String(_D_Comp));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_H_Comp));
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
			Command_Control.CGDCONT = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.CGDCONT = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.CGDCONT = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.SCFG = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SCFG="));
	GSM_Serial.print(String(_Conn_ID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Pkt_Sz));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Max_To));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Conn_To));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_TX_To));
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
			Command_Control.SCFG = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.SCFG = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.SCFG = false;

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
xE910_HARDWARE GSM_HARDWARE;
