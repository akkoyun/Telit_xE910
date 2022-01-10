/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit GE910 Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <Telit_xE910.h>

/**************************************************
 * Batch Functions
 **************************************************/

bool xE910_GSM::Power_ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

	// Send Shut Down Signal
	GSM_HARDWARE.ShutDown(500);

	// Serial communication activate
	GSM_Serial.begin(GSM_Serial_Baud);

	// Enable GSM Modem Power Switch
	if (_Power_Switch) GSM_HARDWARE.Power_Switch(true);  
	
	// Enable GSM Modem LED Feed
	if (_LED_Switch) GSM_HARDWARE.LED(true);

	// Set Communication Signal LOW
	if (_Communication_Switch) GSM_HARDWARE.Communication(true);

	// GSM Boot Delay
	delay(2000);
	
	// Turn On Modem
	if (GSM_HARDWARE.PowerMonitor()) {

		// Set Variable
		GSM_HARDWARE.Power_Monitor = POWERED;
		GSM_AT.Connection_Status = NOT_CONNECTED;

		// End Function
		return (true);

	} else {

		// Send On Off Signal
		GSM_HARDWARE.OnOff(5000);

		// Control for PWMon (PH7)
		if (GSM_HARDWARE.PowerMonitor()) {

			// Set Variable
			GSM_HARDWARE.Power_Monitor = POWERED;
			GSM_AT.Connection_Status = NOT_CONNECTED;

			// End Function
			return (true);

		} else {

			// Send Shut Down Signal
			GSM_HARDWARE.ShutDown(200);

		}

	}

	// Set Variable
	GSM_HARDWARE.Power_Monitor = NOT_POWERED;
	GSM_AT.Connection_Status = NOT_CONNECTED;

	// End Function
	return (false);

}
bool xE910_GSM::Power_OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch, const bool _Clear_Variables) {

		// Turn Off Modem
		if (GSM_HARDWARE.PowerMonitor()) {

			// Power OFF GSM Modem
			GSM_HARDWARE.ShutDown(200);

			// Disable GSM LED Power
			if (_LED_Switch) GSM_HARDWARE.LED(false);

			// Disable GSM Modem Voltage Translator
			if (_Communication_Switch) GSM_HARDWARE.Communication(false);

			// Disable GSM Modem Main Power Switch
			if (_Power_Switch) GSM_HARDWARE.Power_Switch(false);

			// Command Delay
			delay(1000);

		}

		// Clear Library Variables
		if (_Clear_Variables) {

			// Set Variable
			GSM_HARDWARE.Power_Monitor = NOT_POWERED;
			GSM_AT.Connection_Status = NOT_CONNECTED;

		}

		// End Function
		return (true);

}
bool xE910_GSM::Begin(const bool _Debug) {

	// GSM Module Port Definations
	DDRJ	&= 0b11111011;	PORTJ	|= 0b00000100;	// PJ2 - Input / Pull Down [GSM Ring]
	DDRJ	&= 0b11110111;	PORTJ	&= 0b11110111;	// PJ3 - Input / Pull Down [GSM Power Monitor]
	DDRJ	|= 0b00010000;	PORTJ	|= 0b00010000;	// PJ4 - Output / Pull Down [GSM Communication Enable]
	DDRJ	|= 0b00100000;	PORTJ	&= 0b11011111;	// PJ5 - Output / Pull Down [GSM Shut Down]
	DDRJ	|= 0b01000000;	PORTJ	&= 0b10111111;	// PJ6 - Output / Pull Down [GSM On Off]

	// Set Global Variable
	Debug_Mode = _Debug;
	
	// Declare Response Status
	bool _Response = false;

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Power On Modem
    GSM.Power_ON(false, false, true);

	// Control for Power Monitor
	if (GSM_HARDWARE.PowerMonitor()) {

		// **************************************************
		// Command Parameters
		// **************************************************
		const bool 		_Parameter_ATE		 			= false;	// Echo Off
		const uint8_t 	_Parameter_CMEE 				= 1;		// Numeric Error Format
		const uint8_t 	_Parameter_FCLASS 				= 0;		// Connection Type
		const uint8_t 	_Parameter_K	 				= 0;		// Set Flow Control Off
		const uint8_t 	_Parameter_SLED	 				= 2;		// Set LED for Connection Type

		// Control for Connection
		if (!GSM_AT.Connection_Status) {
		
			// **************************************************
			// AT Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT.........................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.AT();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// ATE Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("ATE=0......................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.ATE(_Parameter_ATE);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// CMEE Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CMEE=1..................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.CMEE(_Parameter_CMEE);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// FCLASS Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+FCLASS=0................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.FCLASS(_Parameter_FCLASS);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// K Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT&K0......................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.K(_Parameter_K);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// CPIN Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CPIN?...................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.CPIN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// CGSN Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CGSN....................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.CGSN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// GSN Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GSN......................................"));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.GSN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// ICCID Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CCID....................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.CCID();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// GMI Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GMI......................................"));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.GMI();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// GMM Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GMM......................................"));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.GMM();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// GMR Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GMR......................................"));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.GMR();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// SLED Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+SLED=2..................................."));

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSM_AT.SLED(_Parameter_SLED);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// Initialize Complate
			// **************************************************

			// Set Variable
			GSM_AT.Initialization_Status = true;

			// End Function
			return(true);

		}

		// End Function
		return(false);

	}

	// End Function
	return(false);

}
bool xE910_GSM::Connect(void) { 

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Declare Response Status
	bool _Response = false;

	// Control for Initialization Monitor
	if (GSM_AT.Initialization_Status) {

		// **************************************************
		// TXMONMODE Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		bool _Parameter_TXMONMODE = 1;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT#TXMONMODE="));
		if (Debug_Mode) Serial.print(_Parameter_TXMONMODE);
		if (Debug_Mode) Serial.print(F(".............................."));

		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.TXMONMODE(_Parameter_TXMONMODE);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// CREG Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		bool _Parameter_CREG_Mode = 0;

		// Read Current Time
		uint32_t _CTime = millis();

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT+CREG="));
		if (Debug_Mode) Serial.print(_Parameter_CREG_Mode);
		if (Debug_Mode) Serial.print(F("..................................."));

		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.CREG(_Parameter_CREG_Mode);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// CGREG Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		bool _Parameter_CGREG_Mode = 0;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT+CGREG="));
		if (Debug_Mode) Serial.print(_Parameter_CGREG_Mode);
		if (Debug_Mode) Serial.print(F(".................................."));

		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.CGREG(_Parameter_CGREG_Mode);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// Read Current Time
		uint32_t _CRTime = millis() - _CTime;
		GSM_AT.Connection_Time = uint8_t(_CRTime / 1000);

		// Print GSM Command
		if (Debug_Mode) {Serial.print(F("GSM Connection Time...........................[")); Serial.print(GSM_AT.Connection_Time); Serial.println(F("]"));}

		// **************************************************
		// SCFG Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		const uint8_t _Parameter_SCFG_ConnID = 1;
		const uint8_t _Parameter_SCFG_Cid = 1;
		const uint16_t _Parameter_SCFG_Pkt_Sz = 300;
		const uint16_t _Parameter_SCFG_Max_To = 90;
		const uint16_t _Parameter_SCFG_Conn_To = 600;
		const uint8_t _Parameter_SCFG_TX_To = 50;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT#SCFG="));
		if (Debug_Mode) Serial.print(_Parameter_SCFG_ConnID);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SCFG_Cid);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SCFG_Pkt_Sz);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SCFG_Max_To);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SCFG_Conn_To);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SCFG_TX_To);
		if (Debug_Mode) Serial.print(F("..................."));

		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.SCFG(_Parameter_SCFG_ConnID, _Parameter_SCFG_Cid, _Parameter_SCFG_Pkt_Sz, _Parameter_SCFG_Max_To, _Parameter_SCFG_Conn_To, _Parameter_SCFG_TX_To);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);
		
		// **************************************************
		// CGDCONT Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		const uint8_t _Parameter_CDGCONT_Cid = 1;
		const char _Parameter_CGDCONT_PDP[3] = "IP";
		const char _Parameter_CGDCONT_APN[5] = "mgbs";
		const char _Parameter_CDGCONT_PDP_Addr[8] = "0.0.0.0";
		const bool _Parameter_CDGCONT_D_Comp = false;
		const bool _Parameter_CDGCONT_H_Comp = false;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) {

			Serial.print(F("AT+CGDCONT="));
			Serial.print(_Parameter_CDGCONT_Cid);
			Serial.print(F(",\""));
			Serial.print(_Parameter_CGDCONT_PDP);
			Serial.print(F("\",\""));
			Serial.print(_Parameter_CGDCONT_APN);
			Serial.print(F("\",\""));
			Serial.print(_Parameter_CDGCONT_PDP_Addr);
			Serial.print(F("\","));
			Serial.print(_Parameter_CDGCONT_D_Comp);
			Serial.print(F(","));
			Serial.print(_Parameter_CDGCONT_H_Comp);
			Serial.print(F("......"));
			
		}
		
		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.CGDCONT(_Parameter_CDGCONT_Cid, _Parameter_CGDCONT_PDP, _Parameter_CGDCONT_APN, _Parameter_CDGCONT_PDP_Addr, _Parameter_CDGCONT_D_Comp, _Parameter_CDGCONT_H_Comp);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// SERVINFO Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT#SERVIFO.................................."));

		// Process Command
		while (!_Response) {

			// Send Command
			_Response = GSM_AT.SERVINFO();

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// SGACTCFG Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		const uint8_t _Parameter_SGACTCFG_Cid = 1;
		const uint8_t _Parameter_SGACTCFG_Retry = 5;
		const uint8_t _Parameter_SGACTCFG_Delay = 180;
		const uint8_t _Parameter_SGACTCFG_UrcMode = false;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT#SGACTCFG="));
		if (Debug_Mode) Serial.print(_Parameter_SGACTCFG_Cid);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SGACTCFG_Retry);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SGACTCFG_Delay);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_SGACTCFG_UrcMode);
		if (Debug_Mode) Serial.print(F("......................."));

		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.SGACTCFG(_Parameter_SGACTCFG_Cid, _Parameter_SGACTCFG_Retry, _Parameter_SGACTCFG_Delay, _Parameter_SGACTCFG_UrcMode);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// CGACT Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		const bool _Parameter_CGACT_State = true;
		const uint8_t _Parameter_CGACT_Cid = 1;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT+CGACT="));
		if (Debug_Mode) Serial.print(_Parameter_CGACT_State);
		if (Debug_Mode) Serial.print(F(","));
		if (Debug_Mode) Serial.print(_Parameter_CGACT_Cid);
		if (Debug_Mode) Serial.print(F("................................"));

		// Process Command
		while (!_Response) {

			// Get IP Address
			_Response = GSM_AT.CGACT(_Parameter_CGACT_State, _Parameter_CGACT_Cid);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 1) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// CGPADDR Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		const uint8_t _Parameter_CGPADDR_Cid = 1;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT+CGPADDR="));
		if (Debug_Mode) Serial.print(_Parameter_CGPADDR_Cid);
		if (Debug_Mode) Serial.print(F("................................"));

		// Process Command
		while (!_Response) {

			// Get IP Address
			_Response = GSM_AT.CGPADDR(_Parameter_CGPADDR_Cid);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 1) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// HTTPCFG Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		const uint8_t _Parameter_HTTPCFG_ProfID = 1;
		const char *_Parameter_HTTPCFG_Server = "54.216.226.171";
		const uint8_t _Parameter_HTTPCFG_Port = 80;
		const uint8_t _Parameter_HTTPCFG_AuthType = 0;
		const char *_Parameter_HTTPCFG_Username = "";
		const char *_Parameter_HTTPCFG_Password = "";
		const uint8_t _Parameter_HTTPCFG_SSL = 0;
		const uint8_t _Parameter_HTTPCFG_TimeOut = 20;
		const uint8_t _Parameter_HTTPCFG_Cid = 1;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) {
			Serial.print(F("AT#HTTPCFG="));
			Serial.print(_Parameter_HTTPCFG_ProfID);
			Serial.print(F(",\""));
			Serial.print(F("*****"));
			Serial.print(F("\","));
			Serial.print(_Parameter_HTTPCFG_Port);
			Serial.print(F(","));
			Serial.print(_Parameter_HTTPCFG_AuthType);
			Serial.print(F(",\""));
			Serial.print(_Parameter_HTTPCFG_Username);
			Serial.print(F("\",\""));
			Serial.print(_Parameter_HTTPCFG_Password);
			Serial.print(F("\","));
			Serial.print(_Parameter_HTTPCFG_SSL);
			Serial.print(F(","));
			Serial.print(_Parameter_HTTPCFG_TimeOut);
			Serial.print(F(","));
			Serial.print(_Parameter_HTTPCFG_Cid);
			Serial.print(F("......"));
		}

		// Process Command
		while (!_Response) {

			// Set HTTP Configuration
			_Response = GSM_AT.HTTPCFG(_Parameter_HTTPCFG_ProfID, _Parameter_HTTPCFG_Server, _Parameter_HTTPCFG_Port, _Parameter_HTTPCFG_AuthType, _Parameter_HTTPCFG_Username, _Parameter_HTTPCFG_Password, _Parameter_HTTPCFG_SSL, _Parameter_HTTPCFG_TimeOut, _Parameter_HTTPCFG_Cid);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 1) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);

		// **************************************************
		// E2SLRI Command
		// **************************************************

		// Command Chain Delay (Advice by Telit)
		delay(20);

		// Declare Parameters
		uint16_t _Parameter_E2SLRI_Pulse = 50;

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Command Debug
		if (Debug_Mode) Serial.print(F("AT#E2SLRI="));
		if (Debug_Mode) Serial.print(_Parameter_E2SLRI_Pulse);
		if (Debug_Mode) Serial.print(F("................................"));

		// Process Command
		while (!_Response) {

			// Process Command
			_Response = GSM_AT.E2SLRI(_Parameter_E2SLRI_Pulse);

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// Print Command State
		if (Debug_Mode) {

			// Control for Response				
			if (_Response) {
				
				Serial.println(F("..[OK]"));
				
			} else {
				
				Serial.println(F("[FAIL]"));
				
			}

		}
	
		// End Function
		if (!_Response) return (false);






		// **************************************************
		// Control for IP Address
		// **************************************************

		if (sizeof(GSM_AT.IP_Address) >= 7) {

			// Set Variable
			GSM_AT.Connection_Status = CONNECTED;

			// End Function
			return (true);

		} else {

			// Set Variable
			GSM_AT.Connection_Status = NOT_CONNECTED;

			// End Function
			return (false);

		}

	}

	// End Function
	return(false);

}
bool xE910_GSM::Send_Data_Pack(const char *_Data) {

	// Send Data
	GSM_AT.HTTPSND(1, 0, "/api/v1.1/p402",20, _Data);
	
	// Get Response
	GSM_AT.HTTPRCV(1);

	// End Function
	return(true);

}
bool xE910_GSM::Socket_Listen(void) {

}
bool xE910_GSM::RSSI_Refresh(void) {

	// Declare Response Status
	bool _Response = false;

	// **************************************************
	// CSQ Command
	// **************************************************

	// Command Chain Delay (Advice by Telit)
	delay(20);

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Command Debug
	if (Debug_Mode) Serial.print(F("AT+CSQ......................................"));

	// Process Command
	while (!_Response) {

		// Process Command
		_Response = GSM_AT.CSQ();

		// Set WD Variable
		_Error_WD++;

		// Control for WD
		if (_Error_WD > 5) break;

	}

	// Print Command State
	if (Debug_Mode) {

		// Control for Response				
		if (_Response) {
			
			Serial.println(F("..[OK]"));
			
		} else {
			
			Serial.println(F("[FAIL]"));
			
		}

	}

	// End Function
	if (!_Response) return (false);


}

/**************************************************
 * Variable Functions
 **************************************************/

String xE910_GSM::Manufacturer(void) {

	if (GSM_AT.Manufacturer == 1) {

		return("Telit");

	} else {

		return("Nan Defined");

	}

}
String xE910_GSM::Model(void) {

	if (GSM_AT.Model == 1) {

		return("TeGE910 QUADlit");

	} else {

		return("Nan Defined");

	}

}
String xE910_GSM::Operator(void) {

	if (GSM_AT.Operator == 28601) {

		return("Turkcell");

	} else if (GSM_AT.Operator == 28602) {

		return("Vodafone");

	} else if (GSM_AT.Operator == 28603) {

		return("Turk Telekom");

	} else if (GSM_AT.Operator == 28604) {

		return("Turk Telekom");

	} else {

		return("Nan Turkish Operator");

	}

}
String xE910_GSM::Firmware(void) {

	return(GSM_AT.Modem_Firmware);

}
String xE910_GSM::IMEI(void) {

	return(GSM_AT.IMEI);

}
String xE910_GSM::Serial_ID(void) {

	return(GSM_AT.Serial_Number);

}
String xE910_GSM::ICCID(void) {

	return(GSM_AT.ICCID);

}
String xE910_GSM::IP(void) {

	return(GSM_AT.IP_Address);

}
uint8_t xE910_GSM::Signal_Strength(void) {

	// Declare RSSI Variable
	int16_t _RSSI = 0;

	// Calculate RSSI
	if (GSM_AT.Signal_RSSI == 0) _RSSI = -113;
	if (GSM_AT.Signal_RSSI == 1) _RSSI = -111;
	if (GSM_AT.Signal_RSSI >= 2 and GSM_AT.Signal_RSSI <= 30) _RSSI = -109 + ((GSM_AT.Signal_RSSI - 2) * 2);
	if (GSM_AT.Signal_RSSI >= 31) _RSSI = -51;
	
	// Calculate Signal Strength
	if (_RSSI >= -70) return(4);
	if (_RSSI < -70 and _RSSI >= -85) return(3);
	if (_RSSI < -85 and _RSSI >= -100) return(2);
	if (_RSSI < -100) return(1);
	if (_RSSI < -110) return(0);

	// End Function
	return(0);

}

/**************************************************
 * Time Functions
 **************************************************/

bool xE910_RTC::Time_Update(void) {

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Declare Response Status
	bool _Response = false;

	// Control for Power Monitor
	if (GSM_HARDWARE.PowerMonitor()) {

		// Control for Connection
		if (GSM_AT.Connection_Status) {
		
			// **************************************************
			// NITZ Command
			// **************************************************

			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			const bool _Parameter_NITZ_State = true;

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT#NITZ="));
			if (GSM.Debug_Mode) Serial.print(_Parameter_NITZ_State);
			if (GSM.Debug_Mode) Serial.print(F("..................................."));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.NITZ(_Parameter_NITZ_State);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM.Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// CTZU Command
			// **************************************************

			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			const bool _Parameter_CTZU_State = true;

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+CTZU="));
			if (GSM.Debug_Mode) Serial.print(_Parameter_CTZU_State);
			if (GSM.Debug_Mode) Serial.print(F("..................................."));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.CTZU(_Parameter_CTZU_State);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM.Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// NTP Command
			// **************************************************

			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			const char _Parameter_NTP_Addr[14] = "85.199.214.98";
			const uint16_t _Parameter_NTP_Port = 123;
			const bool _Parameter_NTP_Update = true;
			const uint8_t _Parameter_NTP_TimeOut = 3;

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT#NTP=\""));
			if (GSM.Debug_Mode) Serial.print(_Parameter_NTP_Addr);
			if (GSM.Debug_Mode) Serial.print(F("\","));
			if (GSM.Debug_Mode) Serial.print(_Parameter_NTP_Port);
			if (GSM.Debug_Mode) Serial.print(F(","));
			if (GSM.Debug_Mode) Serial.print(_Parameter_NTP_Update);
			if (GSM.Debug_Mode) Serial.print(F(","));
			if (GSM.Debug_Mode) Serial.print(_Parameter_NTP_TimeOut);
			if (GSM.Debug_Mode) Serial.print(F(".............."));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.NTP(_Parameter_NTP_Addr, _Parameter_NTP_Port, _Parameter_NTP_Update, _Parameter_NTP_TimeOut);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM.Debug_Mode) {

				// Control for Response				
				if (_Response) {
					
					Serial.println(F("..[OK]"));
					
				} else {
					
					Serial.println(F("[FAIL]"));
					
				}

			}
		
			// End Function
			if (!_Response) return (false);

			// **************************************************
			// Time Update Complate
			// **************************************************

			// End Function
			return(true);

		}

		// End Function
		return(false);

	}

	// End Function
	return(false);

}
uint16_t xE910_RTC::Year(void) {

	return(GSM_AT.RTC_Year);

}
uint8_t xE910_RTC::Month(void) {

	return(GSM_AT.RTC_Month);

}
uint8_t xE910_RTC::Day(void) {

	return(GSM_AT.RTC_Day);

}
uint8_t xE910_RTC::Hour(void) {

	return(GSM_AT.RTC_Hour);

}
uint8_t xE910_RTC::Minute(void) {

	return(GSM_AT.RTC_Minute);

}
uint8_t xE910_RTC::Second(void) {

	return(GSM_AT.RTC_Second);

}

/**************************************************
 * Hardware Functions
 **************************************************/

bool xE910_HARDWARE::Communication(const bool _State) {

	// Enable Communication 
	if (_State) PORTJ &= 0b11101111;

	// Disable Communication
	if (!_State) PORTJ |= 0b00010000;

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

	// Debug Mode
	if (GSM.Debug_Mode) Serial.print(F("Power On Modem.")); // 30 dot

	// Set On/Off Signal HIGH [PJ6]
	PORTJ |= 0b01000000;

	// Command Delay
	if (GSM.Debug_Mode) {for (uint8_t i = 1; i < 30; i++) {delay(_Time / 30); Serial.print(F("."));}}

	// Set On/Off Signal LOW [PJ6]
	PORTJ &= 0b10111111;

	// Print Status
	if (GSM.Debug_Mode) Serial.println(F("..[OK]"));

	// Print Divider
    if (GSM.Debug_Mode) Serial.println("--------------------------------------------------");

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

/**************************************************
 * AT Command Set Functions
 **************************************************/

bool xE910_AT::AT(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(10);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::ATE(const bool _ECHO) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("ATE"));
	GSM_Serial.print(String(_ECHO));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CMEE(const uint8_t _CMEE) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CMEE="));
	GSM_Serial.print(String(_CMEE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::FCLASS(const uint8_t _FCLASS) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+FCLASS="));
	GSM_Serial.print(String(_FCLASS));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::K(const uint8_t _K) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT&K"));
	GSM_Serial.print(String(_K));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CPIN(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CPIN?"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "READY") != NULL) {

		// Set Control Variable
		SIM_PIN_Status = true;

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CGSN(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGSN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Clear Variables
	memset(_Serial_Buffer, 0, GSM_Serial.available());
	memset(IMEI, 0, 17);

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Handle Data
		if (_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) {

			// Get Data
			IMEI[_Data_Order] = _Serial_Buffer[_Read_Order];

			// Increase Data Order
			_Data_Order++;

		}

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::GSN(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GSN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Clear Variables
	memset(_Serial_Buffer, 0, GSM_Serial.available());
	memset(Serial_Number, 0, 11);

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Handle Data
		if (_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) {

			// Get Data
			Serial_Number[_Data_Order] = _Serial_Buffer[_Read_Order];

			// Increase Data Order
			_Data_Order++;

		}

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CCID(void) {

	// Control for SIM Module
	if (SIM_PIN_Status) {
		
		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Data Order Variable
		uint8_t _Data_Order = 0;

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT#CCID"));
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Command Work Delay
		delay(15);

		// Declare Response Variable
		char _Serial_Buffer[GSM_Serial.available()];

		// Clear Variables
		memset(_Serial_Buffer, 0, GSM_Serial.available());
		memset(ICCID, 0, 21);

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if (_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) {

				// Get Data
				ICCID[_Data_Order] = _Serial_Buffer[_Read_Order];

				// Increase Data Order
				_Data_Order++;

			}

			// Increase Read Order
			_Read_Order++;

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::GMI(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMI"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Clear Variables
	memset(_Serial_Buffer, 0, GSM_Serial.available());

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "Telit") != NULL) {

		// Set Manufacturer Variable
		Manufacturer = 1;

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::GMM(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMM"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Clear Variables
	memset(_Serial_Buffer, 0, GSM_Serial.available());

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "GE910-QUAD") != NULL) {

		// Set Model Variable
		Model = 1;

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::GMR(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMR"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Clear Variables
	memset(_Serial_Buffer, 0, GSM_Serial.available());
	memset(Modem_Firmware, 0, 10);

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Handle Data
		if ((_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) or _Serial_Buffer[_Read_Order] == 46) {

			// Get Data
			Modem_Firmware[_Data_Order] = _Serial_Buffer[_Read_Order];

			// Increase Data Order
			_Data_Order++;

		}

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CSQ(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Response Data
	char _CSQ[2]; 

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CSQ"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// Handle RSSI
		if (_Serial_Buffer[8] > 47 and _Serial_Buffer[8] < 58) _CSQ[0] = _Serial_Buffer[8];
		if (_Serial_Buffer[9] > 47 and _Serial_Buffer[9] < 58) _CSQ[1] = _Serial_Buffer[9];
		
		// Set Signal Variable
		Signal_RSSI = atoi(_CSQ);

		// End Function
		return (true);

	} else {

		// Set Signal Variable
		Signal_RSSI = 0;

		// End Function
		return (false);

	}

	// End Function
	return(false);

}
bool xE910_AT::SERVINFO(void) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SERVINFO"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "#SERVINFO:") != NULL) {

		// Control Operator ID
		Operator = 99;													// Unknown Operator
		if (strstr(_Serial_Buffer, "28601") != NULL) Operator = 28601;	// Turkcell
		if (strstr(_Serial_Buffer, "28602") != NULL) Operator = 28602;	// Vodafone
		if (strstr(_Serial_Buffer, "28603") != NULL) Operator = 28603;	// Turk Telecom
		if (strstr(_Serial_Buffer, "28604") != NULL) Operator = 28604;	// Turk Telecom

		// End Function
		return (true);

	} else {

		// Set Signal Variable
		Operator = 0;

		// End Function
		return (false);

	}

}
bool xE910_AT::SLED(const uint8_t _SLED) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SLED="));
	GSM_Serial.print(String(_SLED));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::TXMONMODE(const uint8_t _TXMONMODE) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#TXMONMODE="));
	GSM_Serial.print(String(_TXMONMODE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::REGMODE(const uint8_t _REGMODE) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#REGMODE="));
	GSM_Serial.print(String(_REGMODE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Serial_Buffer[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delay(1);

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::CREG(const bool _Mode) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CREG="));
	GSM_Serial.print(_Mode);
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	uint8_t _Serial_Buffer_Size = GSM_Serial.available();
	char _Serial_Buffer[_Serial_Buffer_Size];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// Declare Watchdog Variables
		uint8_t _Error_WD = 0;

		// Control Loop
		while (CREG_Status != HOME_REGISTERED) {

			// Handle for Error
			if (_Error_WD >= 60) return (false);

			// Declare Read Order Variable
			_Read_Order = 0;

			// Clear UART Buffer
			_Clear_UART_Buffer();

			// Send UART Command
			GSM_Serial.print(F("AT+CREG?\r\n"));

			// Wait for UART Data Send
			GSM_Serial.flush();

			// Command Work Delay
			delay(15);

			// Clear Response Variable
			memset(_Serial_Buffer, 0, _Serial_Buffer_Size);

			// Read UART Response
			while (GSM_Serial.available() > 0) {

				// Read Serial Char
				_Serial_Buffer[_Read_Order] = GSM_Serial.read();

				// Increase Read Order
				_Read_Order++;

			}

			// Control for Response
			if (strstr(_Serial_Buffer, "OK") != NULL) {

				// Declare Status Variable
				uint8_t _Status = _Serial_Buffer[11];

				// Handle Stat Variable
				if (_Status == 48) CREG_Status = NOT_REGISTERED; 		// Not Registered [0]
				if (_Status == 49) CREG_Status = HOME_REGISTERED;		// Registered to Home Network [1]
				if (_Status == 50) CREG_Status = SEARCHING;				// Searching Network [2]
				if (_Status == 51) CREG_Status = DENIED;				// Registration Denied [3]
				if (_Status == 52) CREG_Status = UNKNOWN;				// Unknown Error [4]
				if (_Status == 53) CREG_Status = ROAMING_REGISTERED;	// Registered to Rooming Network [5]

			} else {

				// End Function
				return (false);

			}

			// Wait Delay
			if (CREG_Status != HOME_REGISTERED) delay(2000);

			// Count for Error
			_Error_WD++;

			// Control for Response
			if ((CREG_Status == HOME_REGISTERED) or (CREG_Status == ROAMING_REGISTERED)) return (true);

		}

		// End Function
		return (false);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CGREG(const bool _Mode) {

	// Control for CREG
	if (CREG_Status == HOME_REGISTERED or CREG_Status == ROAMING_REGISTERED) {
	
		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT+CGREG="));
		GSM_Serial.print(_Mode);
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Command Work Delay
		delay(15);

		// Declare Response Variable
		uint8_t _Serial_Buffer_Size = GSM_Serial.available();
		char _Serial_Buffer[_Serial_Buffer_Size];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Declare Watchdog Variables
			uint8_t _Error_WD = 0;

			// Control Loop
			while (CGREG_Status != HOME_REGISTERED) {

				// Handle for Error
				if (_Error_WD >= 60) return (false);

				// Declare Read Order Variable
				_Read_Order = 0;

				// Clear UART Buffer
				_Clear_UART_Buffer();

				// Send UART Command
				GSM_Serial.print(F("AT+CGREG?\r\n"));

				// Wait for UART Data Send
				GSM_Serial.flush();

				// Command Work Delay
				delay(15);

				// Clear Response Variable
				memset(_Serial_Buffer, 0, _Serial_Buffer_Size);

				// Read UART Response
				while (GSM_Serial.available() > 0) {

					// Read Serial Char
					_Serial_Buffer[_Read_Order] = GSM_Serial.read();

					// Increase Read Order
					_Read_Order++;

				}

				// Control for Response
				if (strstr(_Serial_Buffer, "OK") != NULL) {

					// Declare Status Variable
					uint8_t _Status = _Serial_Buffer[12];

					// Handle Stat Variable
					if (_Status == 48) CGREG_Status = NOT_REGISTERED; 		// Not Registered [0]
					if (_Status == 49) CGREG_Status = HOME_REGISTERED;		// Registered to Home Network [1]
					if (_Status == 50) CGREG_Status = SEARCHING;			// Searching Network [2]
					if (_Status == 51) CGREG_Status = DENIED;				// Registration Denied [3]
					if (_Status == 52) CGREG_Status = UNKNOWN;				// Unknown Error [4]
					if (_Status == 53) CGREG_Status = ROAMING_REGISTERED;	// Registered to Rooming Network [5]

				} else {

					// End Function
					return (false);

				}

				// Wait Delay
				if (CGREG_Status != HOME_REGISTERED) delay(2000);

				// Count for Error
				_Error_WD++;

				// Control for Response
				if ((CGREG_Status == HOME_REGISTERED) or (CGREG_Status == ROAMING_REGISTERED)) return (true);

			}

			// End Function
			return (false);

		} else {

			// End Function
			return (false);

		}
	
	}

	// End Function
	return (false);

}
bool xE910_AT::CGDCONT(const uint8_t _Cid, const char *_PDP_Type, const char *_APN, const char *_PDP_Addr, const bool _D_Comp, const bool _H_Comp) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

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

	// Command Work Delay
	delay(25);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "+CME") != NULL) return (false);
	if (strstr(_Serial_Buffer, "OK") != NULL) return (true);

	// End Function
	return (false);

}
bool xE910_AT::SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

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
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SCFGEXT="));
	GSM_Serial.print(String(_Conn_ID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Sr_Mode));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Recv_Data_Mode));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Keep_Alive));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Listen_Auto_Rsp));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Send_Data_Mode));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SCFGEXT2="));
	GSM_Serial.print(String(_Conn_ID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Buffer_Start));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Abort_Conn_Attempt));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_SRing_Len));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_SRing_To));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_No_Carrier_Mode));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SCFGEXT3="));
	GSM_Serial.print(String(_Conn_ID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Imm_Rsp));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Closure_Type_Cmd_Mode_Enabling));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::SGACT(const uint8_t _Cid = 1) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SGACT="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String("0"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(30);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// Command Delay
		delay(100);

		// Declare Read Order Variable
		_Read_Order = 0;

		// Declare Data Order Variable
		uint8_t _Data_Order = 0;

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT#SGACT="));
		GSM_Serial.print(String(_Cid));
		GSM_Serial.print(F(","));
		GSM_Serial.print(String("1"));
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Read Current Time
		uint32_t _Time = millis();

		// Wait for UART Available
		while (GSM_Serial.available() < 19) {

			// Wait delay
			delay(5);

			// Handle for timeout
			if (millis() - _Time >= 150000) return (false);

		}

		// Command Delay
		delay(20);

		// Declare Response Variable
		uint8_t _Serial_Buffer_Size = GSM_Serial.available();
		char _Serial_Buffer[_Serial_Buffer_Size];

		// Clear IP Variable
		memset(IP_Address, 0, 16);

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if ((_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) or _Serial_Buffer[_Read_Order] == 46) {

				// Get Data
				IP_Address[_Data_Order] = _Serial_Buffer[_Read_Order];

				// Increase Data Order
				_Data_Order++;

			}

			// Increase Read Order
			_Read_Order++;

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "553") != NULL) return (true);
		if (strstr(_Serial_Buffer, "555") != NULL) return (false);
		if (strstr(_Serial_Buffer, "OK") != NULL) return (true);

		// End Function
		return (false);

	} 

	// End Function
	return (false);

}
bool xE910_AT::SGACTCFG(const uint8_t _Cid = 1, const uint8_t _Retry = 0, const uint16_t _Delay = 180, const bool _UrcMode = 0) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SGACTCFG="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Retry));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Delay));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_UrcMode));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }


}
bool xE910_AT::CGACT(const uint8_t _State, const bool _Cid) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGACT="));
	GSM_Serial.print(String(_State));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(20);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CGPADDR(const uint8_t _Cid) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGPADDR="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(30);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Handle Data
		if (_Read_Order >= 13 and (_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) or _Serial_Buffer[_Read_Order] == 46) {

			// Get Data
			IP_Address[_Data_Order] = _Serial_Buffer[_Read_Order];

			// Increase Data Order
			_Data_Order++;

		}

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::CTZU(const bool _State) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CTZU="));
	GSM_Serial.print(String(_State));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::NITZ(const bool _State) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#NITZ="));
	GSM_Serial.print(String(_State));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out) {

    // Declare Response Length
    uint8_t _Response_Length = 31;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#NTP=\""));
	GSM_Serial.print(String(_NTP_Addr));
	GSM_Serial.print(F("\","));
	GSM_Serial.print(String(_NTP_Port));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Update_Module_Clock));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Time_Out));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 10000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Declare Buffer
			char _Buffer[2];

			// Parse Year
			_Buffer[0] = _Response[8];
			_Buffer[1] = _Response[9];
			RTC_Year = 2000 + (uint8_t)atoi(_Buffer);

			// Parse Month
			_Buffer[0] = _Response[11];
			_Buffer[1] = _Response[12];
			RTC_Month = (uint8_t)atoi(_Buffer);
			if (RTC_Month > 12) RTC_Month = 1;

			// Parse Day
			_Buffer[0] = _Response[14];
			_Buffer[1] = _Response[15];
			RTC_Day = (uint8_t)atoi(_Buffer);
			if (RTC_Day > 31) RTC_Day = 1;

			// Parse Hour
			_Buffer[0] = _Response[17];
			_Buffer[1] = _Response[18];
			RTC_Hour = (uint8_t)atoi(_Buffer);
			if (RTC_Hour > 24) RTC_Hour = 0;

			// Parse Minute
			_Buffer[0] = _Response[20];
			_Buffer[1] = _Response[21];
			RTC_Minute = (uint8_t)atoi(_Buffer);
			if (RTC_Minute > 60) RTC_Minute = 0;

			// Parse Second
			_Buffer[0] = _Response[23];
			_Buffer[1] = _Response[24];
			RTC_Second = (uint8_t)atoi(_Buffer);
			if (RTC_Second > 60) RTC_Second = 0;

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }
	
}
bool xE910_AT::CCLK(void) {

    // Declare Response Length
    uint8_t _Response_Length = 34;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CCLK?"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

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
			delay(3);

		}

		// Control for Response
		if (strstr(_Response, "OK") != NULL) {

			// Declare Buffer
			char _Buffer[2];

			// Parse Year
			_Buffer[0] = _Response[10];
			_Buffer[1] = _Response[11];
			RTC_Year = 2000 + (uint8_t)atoi(_Buffer);

			// Parse Month
			_Buffer[0] = _Response[13];
			_Buffer[1] = _Response[14];
			RTC_Month = (uint8_t)atoi(_Buffer);
			if (RTC_Month > 12) RTC_Month = 1;

			// Parse Day
			_Buffer[0] = _Response[16];
			_Buffer[1] = _Response[17];
			RTC_Day = (uint8_t)atoi(_Buffer);
			if (RTC_Day > 31) RTC_Day = 1;

			// Parse Hour
			_Buffer[0] = _Response[19];
			_Buffer[1] = _Response[20];
			RTC_Hour = (uint8_t)atoi(_Buffer);
			if (RTC_Hour > 24) RTC_Hour = 0;

			// Parse Minute
			_Buffer[0] = _Response[22];
			_Buffer[1] = _Response[23];
			RTC_Minute = (uint8_t)atoi(_Buffer);
			if (RTC_Minute > 60) RTC_Minute = 0;

			// Parse Second
			_Buffer[0] = _Response[25];
			_Buffer[1] = _Response[26];
			RTC_Second = (uint8_t)atoi(_Buffer);
			if (RTC_Second > 60) RTC_Second = 0;

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::SHDN(void) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SHDN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Serial_Buffer[_Response_Length];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Stream Delay
			delay(1);

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// End Function
			return (true);

		} else {

			// End Function
			return (false);

		}

    } else {

		// End Function
		return (false);

    }

}
bool xE910_AT::SD(const uint8_t _Cid, const uint8_t _Pro, const uint8_t _Port, const char *_IP) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Connection Variable
	bool _Connected = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SD="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Pro));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Port));
	GSM_Serial.print(F(",\""));
	GSM_Serial.print(String(_IP));
	GSM_Serial.print(F("\""));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(5);

	// Declare Watchdog Variables
	uint8_t _Error_WD = 0;

	// Control Loop
	while (!_Connected) {

		// Handle for Error
		if (_Error_WD >= 60) return (false);

		// Declare Response Variable
		char _Serial_Buffer[GSM_Serial.available()];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "CONNECT") != NULL) {

			// Set Control Variable
			_Connected = true;

		} else {

			// Connection Delay
			delay(500);

			// Set Control Variable
			_Connected = false;

			// Count for Error
			_Error_WD++;

		}

	}

	// End Function
	return(true);

}
uint8_t xE910_AT::SS(const uint8_t _ConnID) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SS="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(20);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		if (_Serial_Buffer[9] == 48) return(0);
		if (_Serial_Buffer[9] == 49) return(1);
		if (_Serial_Buffer[9] == 50) return(2);
		if (_Serial_Buffer[9] == 51) return(3);
		if (_Serial_Buffer[9] == 52) return(4);
		if (_Serial_Buffer[9] == 53) return(5);
		if (_Serial_Buffer[9] == 54) return(6);
		if (_Serial_Buffer[9] == 55) return(7);
		return (99);

	} else {

		// End Function
		return (99);

	}

	// End Function
	return(99);

}
bool xE910_AT::HTTPCFG(const uint8_t _ProfID, const char *_HTTP_Server, const uint8_t _Port, const uint8_t _AuthType, const char *_Username, const char *_Password, const uint8_t _SSL, const uint8_t _TimeOut, const uint8_t _Cid) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#HTTPCFG="));
	GSM_Serial.print(String(_ProfID));
	GSM_Serial.print(F(",\""));
	GSM_Serial.print(String(_HTTP_Server));
	GSM_Serial.print(F("\","));
	GSM_Serial.print(String(_Port));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_AuthType));
	GSM_Serial.print(F(",\""));
	GSM_Serial.print(String(_Username));
	GSM_Serial.print(F("\",\""));
	GSM_Serial.print(String(_Password));
	GSM_Serial.print(F("\","));
	GSM_Serial.print(String(_SSL));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_TimeOut));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(20);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::HTTPSND(const uint8_t _ProfID, const uint8_t _Command, const char *_URL, const uint8_t _TimeOut, const char *_Data) {

	// Declare Loop Variable
	bool _Command_Loop_Control = false;

	// Declare Watchdog Variables
	uint8_t _Error_WD = 0;

	// Command Send Loop
	while (!_Command_Loop_Control) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT#HTTPSND="));
		GSM_Serial.print(_ProfID);
		GSM_Serial.print(F(",")); 
		GSM_Serial.print(_Command);
		GSM_Serial.print(F(",\"")); 
		GSM_Serial.print(String(_URL));
		GSM_Serial.print(F("\","));
		GSM_Serial.print(strlen(_Data));
		GSM_Serial.print(F(",\"application/json\""));
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Read Current Time
		uint32_t _Time = millis();

		// Wait for UART Available
		while ((uint16_t)GSM_Serial.available() < 5) {

			// Wait delay
			delay(10);

			// Handle for timeout
			if (millis() - _Time >= 20000) return (false);

		}

		// Declare Response Variable
		char _Serial_Buffer[GSM_Serial.available()];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

		}

		// Control for Response
		if (strstr(_Serial_Buffer, ">>>") != NULL) {

			// Set Loop Variable
			_Command_Loop_Control = true;

		} else {

			// Handle for Error
			if (_Error_WD > 5) return (false);

			// Command Resend Delay
			delay(1000);

		}

		// Count for Error
		_Error_WD++;

	}
	
	// Send Delay
	delay(50);

	// Reset Loop Variable
	_Command_Loop_Control = false;

	// Print Data
	GSM_Serial.print(String(_Data));

	// Send Delay
	delay(50);

	// Reset Watchdog Variables
	_Error_WD = 0;

	// Command Send Loop
	while (!_Command_Loop_Control) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Read Current Time
		uint32_t _Time = millis();

		// Wait for UART Available
		while ((uint16_t)GSM_Serial.available() < 5) {

			// Wait delay
			delay(10);

			// Handle for timeout
			if (millis() - _Time >= 20000) return (false);

		}

		// Declare Response Variable
		char _Serial_Buffer[GSM_Serial.available()];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Set Loop Variable
			_Command_Loop_Control = true;

		} else {

			// Handle for Error
			if (_Error_WD > 5) return (false);

			// Command Resend Delay
			delay(500);

		}

		// Count for Error
		_Error_WD++;

	}

	// End Function
	return (true);





}
bool xE910_AT::HTTPRCV(const uint8_t _ProfID) {

	// Declare Loop Variable
	bool _Command_Loop_Control = false;

	// Declare Watchdog Variables
	uint8_t _Error_WD = 0;

	// Command Send Loop
	while (!_Command_Loop_Control) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Wait delay
		delay(20);

		// Declare Response Variable
		char _Serial_Buffer[50];

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Increase Read Order
			_Read_Order++;

			// Wait delay
			delay(2);

		}

		// Control for Response
		if (strstr(_Serial_Buffer, "HTTPRING") != NULL) {

			// Command Work Delay
			delay(15);

			// Clear UART Buffer
			_Clear_UART_Buffer();

			// Send UART Command
			GSM_Serial.print(F("AT#HTTPRCV="));
			GSM_Serial.print(String(_ProfID));
			GSM_Serial.print(F("\r\n"));

			// Wait for UART Data Send
			GSM_Serial.flush();

			// Declare Loop Variable
			bool _Response_Loop_Control = false;

			// Reset Read Order Variable
			_Read_Order = 0;

			// Declare Data Order Variable
			uint8_t _Data_Order = 0;

			// Handle Variable Defination
			bool _Handle = false;

			// Command Send Loop
			while (!_Response_Loop_Control) {

				// Handle for Error
				if (_Error_WD > 50) return (false);

				// Control for Response Length
				if (GSM_Serial.available() > 20) _Response_Loop_Control = true;
				
				// Wait delay
				delay(100);

				// Count for Error
				_Error_WD++;

			}

			// Declare Response Variable
			char _Response_Buffer[50];

			// Declare Response Variable
			char _Response[20];

			// Read UART Response
			while (GSM_Serial.available() > 0) {

				// Read Serial Char
				_Response_Buffer[_Read_Order] = GSM_Serial.read();

				// Handle Response
				if (_Response_Buffer[_Read_Order] == 123) _Handle = true;

				// Response String
				if (_Handle) _Response[_Data_Order] += _Serial_Buffer[_Read_Order];

				// Increase Read Order
				_Data_Order++;

				// Handle Response
				if (_Response_Buffer[_Read_Order] == 125) _Handle = false;

				// Increase Read Order
				_Read_Order++;

			}

			// Control for Command
			if (strstr(_Response, "500") != NULL) return (true);

			// End Function
			return (false);

		} else {

			// Handle for Error
			if (_Error_WD > 10) return (false);

			// Command Resend Delay
			delay(500);

		}

		// Count for Error
		_Error_WD++;

	}

	// End Function
	return (false);

}
bool xE910_AT::E2SLRI(const uint16_t _Pulse_Duration) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#E2SLRI="));
	GSM_Serial.print(String(_Pulse_Duration));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Work Delay
	delay(15);

	// Declare Response Variable
	char _Serial_Buffer[GSM_Serial.available()];

	// Read UART Response
	while (GSM_Serial.available() > 0) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}

/**************************************************
 * Private Functions 
 **************************************************/

void xE910_AT::_Clear_UART_Buffer(void) {

	while (GSM_Serial.available() > 0) {
        
        GSM_Serial.read(); 
        delay(3);

    }

	// Command Chain Delay (Advice by Telit)
	delay(Command_Delay);

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

// Define Library Class
xE910_GSM GSM;
xE910_HARDWARE GSM_HARDWARE;
xE910_AT GSM_AT;
xE910_RTC GSM_RTC;

// 1903