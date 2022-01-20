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

		// Command Set
		const bool _REGMODE = true;
		const bool _TXMONMODE = true;
		const bool _CREG = true;
		const bool _CGREG = true;
		const bool _SCFG1 = true;
		const bool _SCFG2 = true;
		const bool _SCFGEXT1 = true;
		const bool _SCFGEXT2 = true;
		const bool _CGDCONT = true;
		const bool _SERVINFO = true;
		const bool _CGACT = true;
		const bool _CGPADDR = true;
		const bool _HTTPCFG = true;
		const bool _ICMP = true;

		// REGMODE Command
		if (_REGMODE) {

			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			bool _Parameter_REGMODE = 1;

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT#REGMODE="));
			if (Debug_Mode) Serial.print(_Parameter_REGMODE);
			if (Debug_Mode) Serial.print(F("................................"));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.REGMODE(_Parameter_REGMODE);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (Debug_Mode) {

				// Control for Response				
				if (_Response) Serial.println(F("..[OK]"));
				if (!_Response) Serial.println(F("[FAIL]"));

			}
		
			// End Function
			if (!_Response) return (false);
			
		}

		// TXMONMODE Command
		if (_TXMONMODE) {

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
				if (_Response) Serial.println(F("..[OK]"));
				if (!_Response) Serial.println(F("[FAIL]"));

			}
		
			// End Function
			if (!_Response) return (false);
			
		}
		
		// Read Current Time
		uint32_t _CTime = millis();

		// CREG Command
		if (_CREG) {

			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			bool _Parameter_CREG_Mode = 0;

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
				if (_Response) Serial.println(F("..[OK]"));
				if (!_Response) Serial.println(F("[FAIL]"));

			}
		
			// End Function
			if (!_Response) return (false);

		}

		// CGREG Command
		if (_CGREG) {
		
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
				if (_Response) Serial.println(F("..[OK]"));
				if (!_Response) Serial.println(F("[FAIL]"));

			}
		
			// End Function
			if (!_Response) return (false);

			// Read Current Time
			uint32_t _CRTime = millis() - _CTime;
			GSM_AT.Connection_Time = uint8_t(_CRTime / 1000);

			// Print GSM Command
			if (Debug_Mode) {Serial.print(F("GSM Connection Time...........................[")); Serial.print(GSM_AT.Connection_Time); Serial.println(F("]"));}

		}

		// SCFG Command Socket 1
		if (_SCFG1) {
			
			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			const uint8_t _Parameter_SCFG_ConnID = 1;
			const uint8_t _Parameter_SCFG_Cid = 1;
			const uint16_t _Parameter_SCFG_Pkt_Sz = 1500;
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
			if (Debug_Mode) Serial.print(F(".................."));

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

		}

		// SCFG Command Socket 2
		if (_SCFG2) {
			
			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			const uint8_t _Parameter_SCFG_ConnID = 2;
			const uint8_t _Parameter_SCFG_Cid = 1;
			const uint16_t _Parameter_SCFG_Pkt_Sz = 1500;
			const uint16_t _Parameter_SCFG_Max_To = 90;
			const uint16_t _Parameter_SCFG_Conn_To = 300;
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
			if (Debug_Mode) Serial.print(F(".................."));

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

		}

		// SCFGEXT Command Socket 2
		if (_SCFGEXT2) {
		
			// Command Chain Delay (Advice by Telit)
			delay(20);

			// Declare Parameters
			const uint8_t _Parameter_SCFGEXT_ConnID = 2;
			const uint8_t _Parameter_SCFGEXT_SrMode = 1;
			const bool _Parameter_SCFGEXT_Recv_Data_Mode = false;
			const uint8_t _Parameter_SCFGEXT_Keep_Alieve = 1;
			const bool _Parameter_SCFGEXT_Listen_Auto_Response = false;
			const bool _Parameter_SCFGEXT_Send_Data_Mode = false;

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT#SCFGEXT="));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_ConnID);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_SrMode);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_Recv_Data_Mode);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_Keep_Alieve);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_Listen_Auto_Response);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_Send_Data_Mode);
			if (Debug_Mode) Serial.print(F("......................"));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.SCFGEXT(_Parameter_SCFGEXT_ConnID, _Parameter_SCFGEXT_SrMode, _Parameter_SCFGEXT_Recv_Data_Mode, _Parameter_SCFGEXT_Keep_Alieve, _Parameter_SCFGEXT_Listen_Auto_Response, _Parameter_SCFGEXT_Send_Data_Mode);

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

		// CGDCONT Command
		if (_CGDCONT) {
		
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

		}

		// SERVINFO Command
		if (_SERVINFO) {
		
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

		}

		// CGACT Command
		if (_CGACT) {
		
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

		}

		// CGPADDR Command
		if (_CGPADDR) {
		
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

		}

		// HTTPCFG Command
		if (_HTTPCFG) {
		
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

		}

		// ICMP Command
		if (_ICMP) {
		
			// Command Chain Delay (Advice by Telit)
			delay(30);

			// Declare Parameters
			const uint8_t _Parameter_ICMP_Mode = 1;

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) {
				Serial.print(F("AT#ICMP="));
				Serial.print(_Parameter_ICMP_Mode);
				Serial.print(F("..................................."));
			}

			// Process Command
			while (!_Response) {

				// Set HTTP Configuration
				_Response = GSM_AT.ICMP(_Parameter_ICMP_Mode);

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

		}

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
bool xE910_GSM::Send_Data_Pack(const String _Data) {

	// Declare Variables
	const bool _Protocol = 0;						// TCP Protocol
	const char _Service[15] = "/api/v1.1/p402";		// Service URL
	const uint8_t _TimeOut = 20;					// Time Out

	// Send Data
	if (GSM_AT.HTTPSND(1, _Protocol, _Service, _TimeOut, _Data) == true) {
		
		// Get Response
		bool _Response = GSM_AT.HTTPRCV(1);

		// End Function
		return(_Response);
		
	}

	// End Function
	return(false);

}
bool xE910_GSM::Socket_Answer(void) {

	// Interrupt Delay
	delay(10);

	// Control for Incoming Message
	if (GSM_Serial.available() > 0) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

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
		if (strstr(_Serial_Buffer, "SRING: 2") != NULL) {

			// Socket Answer Command
			uint16_t _Message_Length = GSM_AT.SA(2,1);

			// Reset Variable
			GSM_AT.Remote_Command = 0;

			// Socket Recieve Command
			GSM_AT.Remote_Command = GSM_AT.SRECV(2, _Message_Length);

			// Send Response Message
			GSM_AT.SSEND(2,SUCCESS);

			// Close Socket Command
			bool _SH = GSM_AT.SH(2);

			// ReOpen Socket Command
			bool _SL = GSM_AT.SL(2, 1, 80, 255); 

		} else {

			// Set Request Variable
			GSM_AT.Remote_Command = 0;

		}

	}

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

	// End Function
	return(false);

}
bool xE910_GSM::Connection_Control(void) {

	// Clear UART Buffer
	while (GSM_Serial.available() > 0) {
        
        GSM_Serial.read(); 
        delay(3);

    }

	// Send UART Command
	GSM_Serial.print(F("AT+CGATT?"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	char _Serial_Buffer[255];
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Set Response Loop Variable
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (98);

		// Increase Read Order
		_Read_Order++;

	}

	// Handle Response
	if (_Serial_Buffer[10] ==  48) return(false);
	if (_Serial_Buffer[10] ==  49) return(true);
	
}

/**************************************************
 * Variable Functions
 **************************************************/
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

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::ATE(const bool _ECHO) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("ATE"));
	GSM_Serial.print(String(_ECHO));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::CMEE(const uint8_t _CMEE) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CMEE="));
	GSM_Serial.print(String(_CMEE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::FCLASS(const uint8_t _FCLASS) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+FCLASS="));
	GSM_Serial.print(String(_FCLASS));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::K(const uint8_t _K) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT&K"));
	GSM_Serial.print(String(_K));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::CPIN(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CPIN?"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

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

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGSN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Clear Variables
	memset(IMEI, 0, 17);

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Control for Buffer
	for (uint8_t i = 0; i < 255; i++) {

		// Handle Data
		if (_Serial_Buffer[i] < 58 and _Serial_Buffer[i] > 47) {

			// Get Data
			IMEI[_Data_Order] = _Serial_Buffer[i];

			// Increase Data Order
			_Data_Order++;

		}

	}

	// End Function
	return(true);

}
bool xE910_AT::GSN(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GSN"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Clear Variables
	memset(Serial_Number, 0, 11);

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Control for Buffer
	for (uint8_t i = 0; i < 255; i++) {

		// Handle Data
		if (_Serial_Buffer[i] < 58 and _Serial_Buffer[i] > 47) {

			// Get Data
			Serial_Number[_Data_Order] = _Serial_Buffer[i];

			// Increase Data Order
			_Data_Order++;

		}

	}

	// End Function
	return(true);

}
bool xE910_AT::CCID(void) {

	// Control for SIM Module
	if (SIM_PIN_Status) {
		
		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT#CCID"));
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Clear Buffer Variable
		_Clear_Buffer_Variable();

		// Declare Loop Variable
		bool _RESPONSE = false;

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Read Current Time
		uint32_t _Time = millis();

		// Read UART Response
		while (!_RESPONSE) {

			// Read Serial Char
			_Serial_Buffer[_Read_Order] = GSM_Serial.read();

			// Control for Response
			if (strstr(_Serial_Buffer, "OK") != NULL) {

				// Handle Response
				_RESPONSE = true;

			} else {

				// Buffer Read Delay
				delay(2);

			}

			// Handle for timeout
			if (millis() - _Time >= 1000) return (false);

			// Increase Read Order
			_Read_Order++;

		}

		// Clear Variables
		memset(ICCID, 0, 21);

		// Declare Data Order Variable
		uint8_t _Data_Order = 0;

		// Control for Buffer
		for (uint8_t i = 0; i < 255; i++) {

			// Handle Data
			if (_Serial_Buffer[i] < 58 and _Serial_Buffer[i] > 47) {

				// Get Data
				ICCID[_Data_Order] = _Serial_Buffer[i];

				// Increase Data Order
				_Data_Order++;

			}

		}

		// End Function
		return(true);

	} else {

		// End Function
		return (false);

	}

}
bool xE910_AT::GMI(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMI"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

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

	// End Function
	return(true);

}
bool xE910_AT::GMM(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMM"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

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

	// End Function
	return(true);

}
bool xE910_AT::GMR(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+GMR"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Clear Variables
	memset(Modem_Firmware, 0, 10);

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Control for Buffer
	for (uint8_t i = 0; i < 255; i++) {

		// Handle Data
		if ((_Serial_Buffer[i] < 58 and _Serial_Buffer[i] > 47) or _Serial_Buffer[i] == 46) {

			// Get Data
			Modem_Firmware[_Data_Order] = _Serial_Buffer[i];

			// Increase Data Order
			_Data_Order++;

		}

	}

	// End Function
	return(true);

}
bool xE910_AT::CSQ(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CSQ"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 3000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Declare Response Data
	char _CSQ[2]; 

	// Handle RSSI
	_CSQ[0] = _Serial_Buffer[8];
	_CSQ[1] = _Serial_Buffer[9];
	
	// Set Signal Variable
	Signal_RSSI = atoi(_CSQ);

	// End Function
	return (true);

}
bool xE910_AT::SERVINFO(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SERVINFO"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 3000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Control Operator ID
	Operator = 99;													// Unknown Operator
	if (strstr(_Serial_Buffer, "28601") != NULL) Operator = 28601;	// Turkcell
	if (strstr(_Serial_Buffer, "28602") != NULL) Operator = 28602;	// Vodafone
	if (strstr(_Serial_Buffer, "28603") != NULL) Operator = 28603;	// Turk Telecom
	if (strstr(_Serial_Buffer, "28604") != NULL) Operator = 28604;	// Turk Telecom

	// End Function
	return (true);

}
bool xE910_AT::SLED(const uint8_t _SLED) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SLED="));
	GSM_Serial.print(String(_SLED));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::TXMONMODE(const uint8_t _TXMONMODE) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#TXMONMODE="));
	GSM_Serial.print(String(_TXMONMODE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::REGMODE(const uint8_t _REGMODE) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#REGMODE="));
	GSM_Serial.print(String(_REGMODE));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

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

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else if (strstr(_Serial_Buffer, "+CME") != NULL) {

			// End Function
			return (false);

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To) {

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

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode) {

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

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode) {

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

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling) {

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

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

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

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::CGPADDR(const uint8_t _Cid) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGPADDR="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Clear Variable
	memset(IP_Address, '\0', 255);

	// Control for Buffer
	for (uint8_t i = 0; i < 255; i++) {

		// Handle Data
		if ((_Serial_Buffer[i] < 58 and _Serial_Buffer[i] > 47) or _Serial_Buffer[i] == 46) {

			// Get Data
			IP_Address[_Data_Order] = _Serial_Buffer[i];

			// Increase Data Order
			_Data_Order++;

		}

	}

	// End Function
	return(true);

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

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#NITZ="));
	GSM_Serial.print(String(_State));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

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
		memset(_Response, '\0', sizeof(_Response));

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
bool xE910_AT::SD(const uint8_t _Cid, const uint8_t _Pro, const uint8_t _Port, const char *_IP, const uint8_t _Closure_Type, uint16_t _IPort, const bool _Conn_Mode, const char *_Data) {

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
	GSM_Serial.print(F("\","));
	GSM_Serial.print(String(_Closure_Type));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_IPort));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Conn_Mode));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _CONNECTED = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_CONNECTED) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "CONNECT") != NULL) {

			// Handle Response
			_CONNECTED = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 10000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Command Work Delay
	delay(30);

	// Send Headers
	_Request_Headers(String(_Data).length());

	// Send Data
	GSM_Serial.print(String(_Data));

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	_Read_Order = 0;

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "}\n") != NULL and GSM_Serial.available() == 0) {

			// Handle Response
			_RESPONSE = true;

		}

		// Handle for timeout
		if (millis() - _Time >= 15000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Command Work Delay
	delay(50);

	// Send Data
	GSM_Serial.print(F("+++"));

Serial.println(_Serial_Buffer);

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	_RESPONSE = false;

	// Declare Read Order Variable
	_Read_Order = 0;

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		}

		// Handle for timeout
		if (millis() - _Time >= 15000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
uint8_t xE910_AT::SS(const uint8_t _ConnID) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SS="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Control Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (98);

		// Increase Read Order
		_Read_Order++;

	}

	// Return Response
	return(String(_Serial_Buffer[14]).toInt());

}
bool xE910_AT::SL(const uint8_t _ConnID, const bool _Listen_State, const uint16_t _Listen_Port, const uint8_t _Closure_Type) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SL="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Listen_State));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Listen_Port));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_Closure_Type));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
uint16_t xE910_AT::SA(const uint8_t _ConnID, const uint8_t _ConnMode) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SA="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_ConnMode));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Control Variables
	bool _R = false;
	bool _N = false;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "SRING:") != NULL) {

			// Control for Char
			if (_Serial_Buffer[_Read_Order] == '\r') _R = true;
			if (_Serial_Buffer[_Read_Order] == '\n') _N = true;

			// Handle Response
			if (_R and _N) _RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (98);

		// Increase Read Order
		_Read_Order++;

	}

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Declare Handle Variable
	bool _Comma = false;

	// Declare Incomming Message Length Variable
	char _Incomming_Length[4];
	memset(_Incomming_Length, '\0', 4);

	// Control for Buffer
	for (uint8_t i = 0; i < 255; i++) {

		// Handle ,
		if (_Serial_Buffer[i] == 44) _Comma = true;

		// Handle Data
		if (_Comma == true and _Serial_Buffer[i] < 58 and _Serial_Buffer[i] > 47) {

			// Get Data
			_Incomming_Length[_Data_Order] = _Serial_Buffer[i];

			// Increase Data Order
			_Data_Order++;

		}

	}

	// Calculate Length
	uint8_t _Length = atoi(_Incomming_Length);
	if (_Length < 10) _Length = 100;	

	// End Function
	return(_Length);

}
uint16_t xE910_AT::SRECV(const uint8_t _ConnID, const uint16_t _MaxByte) {

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

	// Declare Loop Variable
	bool _SRECV = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SRECV="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_MaxByte));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Message Variable
	char _Message_Buffer[4];
	memset(_Message_Buffer, '\0' ,4);

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_SRECV) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "Event") != NULL) {

			// Handle Data
			if (_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) {

				// Get Data
				_Message_Buffer[_Data_Order] = _Serial_Buffer[_Read_Order];

				// Increase Data Order
				_Data_Order++;

			}

		} 

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Set Control Variable
			_SRECV = true;

		} else {

			// Buffer Read Delay
			delay(2);

			// Set Control Variable
			_SRECV = false;

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (99);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return(atol(_Message_Buffer));

}
bool xE910_AT::SSEND(const uint8_t _ConnID, const uint16_t _Response_Code) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SSEND="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _SEND = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_SEND) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "\r\n>") != NULL) {

			// Handle Response
			_SEND = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (98);

		// Increase Read Order
		_Read_Order++;

	}

	// Command Delay
	delay(30);

	// Print Response Headers
	_Response_Headers();

	// Print Response Message
	_Response_Message(200);

	// Declare Loop Variable
	_SEND = false;

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Read Order Variable
	_Read_Order = 0;

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_SEND) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_SEND = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (98);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return(true);

}
bool xE910_AT::SH(const uint8_t _ConnID) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

  	// Send UART Command
	GSM_Serial.print(F("AT#SH="));
	GSM_Serial.print(String(_ConnID));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Command Delay
	delay(50);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) _RESPONSE = true;

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::FRWL(const uint8_t _Action, const char *_IP_Addr, const char *_Net_Mask) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#FRWL="));
	GSM_Serial.print(String(_Action));
	GSM_Serial.print(F(",\""));
	GSM_Serial.print(String(_IP_Addr));
	GSM_Serial.print(F("\",\""));
	GSM_Serial.print(String(_Net_Mask));
	GSM_Serial.print(F("\""));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::HTTPCFG(const uint8_t _ProfID, const char *_HTTP_Server, const uint8_t _Port, const uint8_t _AuthType, const char *_Username, const char *_Password, const uint8_t _SSL, const uint8_t _TimeOut, const uint8_t _Cid) {

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

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::HTTPSND(const uint8_t _ProfID, const uint8_t _Command, const char *_URL, const uint8_t _TimeOut, const String _Data) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#HTTPSND="));
	GSM_Serial.print(String(_ProfID));
	GSM_Serial.print(F(",")); 
	GSM_Serial.print(String(_Command));
	GSM_Serial.print(F(",\"")); 
	GSM_Serial.print(String(_URL));
	GSM_Serial.print(F("\","));
	GSM_Serial.print(String(_Data.length()));
	GSM_Serial.print(F(",\"application/json\""));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, ">>>") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 10000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Send Delay
	delay(30);

	// Print Data
	GSM_Serial.print(String(_Data));

	// Send Delay
	delay(30);

	// Declare Loop Variable
	_RESPONSE = false;

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Read Order Variable
	_Read_Order = 0;

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::HTTPRCV(const uint8_t _ProfID) {

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "HTTPRING") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Command Work Delay
	delay(15);

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#HTTPRCV="));
	GSM_Serial.print(String(_ProfID));
	GSM_Serial.print(F("\r\n"));

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	_RESPONSE = false;

	// Declare Read Order Variable
	_Read_Order = 0;

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "<<<") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Command Work Delay
	delay(30);

	// Declare Response Variable
	memset(_Serial_Buffer, 0, 255);

	// Declare Loop Variable
	_RESPONSE = false;

	// Declare Read Order Variable
	_Read_Order = 0;

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "\n") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Command
	if (strstr(_Serial_Buffer, "{\"Event\":500}") != NULL) {

		// Set Response Variable
		Server_Response = 500;

		// End Function
		return (true);

	}

	// End Function
	return (false);

}
bool xE910_AT::E2SLRI(const uint16_t _Pulse_Duration) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#E2SLRI="));
	GSM_Serial.print(String(_Pulse_Duration));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 1000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

}
bool xE910_AT::ICMP(const uint8_t _Mode) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#ICMP="));
	GSM_Serial.print(String(_Mode));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Clear Buffer Variable
	_Clear_Buffer_Variable();

	// Declare Loop Variable
	bool _RESPONSE = false;

	// Declare Read Order Variable
	uint8_t _Read_Order = 0;

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_RESPONSE) {

		// Read Serial Char
		_Serial_Buffer[_Read_Order] = GSM_Serial.read();

		// Control for Response
		if (strstr(_Serial_Buffer, "OK") != NULL) {

			// Handle Response
			_RESPONSE = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= 5000) return (false);

		// Increase Read Order
		_Read_Order++;

	}

	// End Function
	return (true);

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
void xE910_AT::_Response_Headers(void) {

	// Print HTTP Header
	GSM_Serial.print(F("HTTP/1.1 200 OK\r\n"));
	GSM_Serial.print(F("Connection: close\r\n"));
	GSM_Serial.print(F("Content-Type: application/json\r\n"));
	GSM_Serial.print(F("User-Agent: STF-PowerStat\r\n"));
	GSM_Serial.print(F("\r\n"));

}
void xE910_AT::_Request_Headers(uint16_t _Size) {

	// Print HTTP Header
	GSM_Serial.print(F("POST /api/v1.1/p402 HTTP/1.1\r\n"));
	GSM_Serial.print(F("Host: 54.216.226.171\r\n"));

	GSM_Serial.print(F("Content-Length: "));
	GSM_Serial.print(String(_Size));
	GSM_Serial.print(F("\r\n"));

	GSM_Serial.print(F("Connection: keep-alive\r\n"));
	GSM_Serial.print(F("Content-Type: application/json\r\n"));
	GSM_Serial.print(F("User-Agent: STF-PowerStat\r\n"));
	GSM_Serial.print(F("\r\n"));

}
void xE910_AT::_Response_Message(const uint16_t _Response_Code) {

	// Print HTTP Header
	GSM_Serial.print(F("{\"Response\":{\"Event\":"));
	GSM_Serial.print(String(_Response_Code));
	GSM_Serial.print(F("}}"));

	// Print End Char
	GSM_Serial.write((char)26);

}
void xE910_AT::_Clear_Buffer_Variable(void) {

	// Declare Response Variable
	memset(_Serial_Buffer, '\0', 255);

}

// Define Library Class
xE910_GSM GSM;
xE910_HARDWARE GSM_HARDWARE;
xE910_AT GSM_AT;
xE910_RTC GSM_RTC;

// 1903