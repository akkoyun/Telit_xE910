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
	GSM_HARDWARE.ShutDown(200);

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
			// Connection Parameters
			// **************************************************

			const bool 		_Parameter_TXMONMODE 			= 0;
			const bool		_Parameter_CREG_Mode			= true;
			const bool		_Parameter_CGREG_Mode			= true;
			const uint8_t 	_Parameter_CDGCONT_Cid 			= 1;
			const char 		*_Parameter_CGDCONT_PDP 		= "IP";
			const char 		*_Parameter_CGDCONT_APN 		= "mgbs";
			const uint8_t 	_Parameter_SCFG_ConnID 			= 1;
			const uint8_t 	_Parameter_SCFG_Cid 			= 1;
			const uint16_t 	_Parameter_SCFG_Pkt_Sz 			= 0;
			const uint16_t 	_Parameter_SCFG_Max_To 			= 0;
			const uint16_t 	_Parameter_SCFG_Conn_To 		= 150;
			const uint8_t 	_Parameter_SCFG_TX_To 			= 0;
			const uint8_t	_Parameter_SCFGEXT_ConnID		= 2;
			const uint8_t	_Parameter_SCFGEXT_SRing		= 1;
			const uint8_t	_Parameter_SCFGEXT_RcvMode		= 0;
			const uint8_t	_Parameter_SCFGEXT_KeepAlieve	= 0;
			const uint8_t	_Parameter_SCFGEXT_ListerRcp	= 0;
			const uint8_t	_Parameter_SCFGEXT_SendMode		= 0;
			const uint8_t 	_Parameter_SGACTCFG_Cid			= 1;
			const uint8_t 	_Parameter_SGACTCFG_Retry		= 5;
			const uint8_t 	_Parameter_SGACTCFG_Delay		= 180;
			const uint8_t 	_Parameter_SGACTCFG_UrcMode		= false;

			// **************************************************
			// TXMONMODE Command
			// **************************************************

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
			// CGDCONT Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CGDCONT="));
			if (Debug_Mode) Serial.print(_Parameter_CDGCONT_Cid);
			if (Debug_Mode) Serial.print(F(",\""));
			if (Debug_Mode) Serial.print(_Parameter_CGDCONT_PDP);
			if (Debug_Mode) Serial.print(F("\",\""));
			if (Debug_Mode) Serial.print(_Parameter_CGDCONT_APN);
			if (Debug_Mode) Serial.print(F("\""));
			if (Debug_Mode) Serial.print(F("...................."));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.CGDCONT(_Parameter_CDGCONT_Cid, _Parameter_CGDCONT_PDP, _Parameter_CGDCONT_APN);

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
			// SCFG Command
			// **************************************************

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
			if (Debug_Mode) Serial.print(F("......................."));

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
			// SCFGEXT Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT#SCFGEXT="));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_ConnID);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_SRing);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_RcvMode);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_KeepAlieve);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_ListerRcp);
			if (Debug_Mode) Serial.print(F(","));
			if (Debug_Mode) Serial.print(_Parameter_SCFGEXT_SendMode);
			if (Debug_Mode) Serial.print(F("......................"));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.SCFGEXT(_Parameter_SCFGEXT_ConnID, _Parameter_SCFGEXT_SRing, _Parameter_SCFGEXT_RcvMode, _Parameter_SCFGEXT_KeepAlieve, _Parameter_SCFGEXT_ListerRcp, _Parameter_SCFGEXT_SendMode);

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
			// SGACT Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+SGACT...................................."));

			// Process Command
			while (!_Response) {

				// Release IP if Exits
				GSM_AT.SGACT(1, false);

				// Get IP Address
				_Response = GSM_AT.SGACT(1, true);

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

	// End Function
	return(false);

}
bool xE910_GSM::Socket_Listen(void) {

}
bool xE910_GSM::RSSI_Refresh(void) {

	// Declare Response Status
	bool _Response = false;

	// **************************************************
	// CSQ Command
	// **************************************************

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Set Response Variable
	_Response = false;

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
	return (true);

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

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+NITZ....................................."));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.NITZ(true);

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

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+CTZU....................................."));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.CTZU(true);

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

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+NTP......................................"));

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = GSM_AT.NTP("85.199.214.98", 123, 1, 3);

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

	// Declare Data Order Variable
	uint8_t _Data_Order = 0;

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

		// Handle Data
		if (_Serial_Buffer[_Read_Order] < 58 and _Serial_Buffer[_Read_Order] > 47) {

			// Get Data
			_CSQ[_Data_Order] = _Serial_Buffer[_Read_Order];

			// Increase Data Order
			_Data_Order++;

		}

		// Increase Read Order
		_Read_Order++;

	}

	// Control for Response
	if (strstr(_Serial_Buffer, "OK") != NULL) {

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

	// Declare Function Variables
	bool _Control = false;
	uint8_t _Error_WD = 0;

	// Declare Stat Variable
	uint8_t _Stat = NOT_REGISTERED;

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Send AT+CREG=1 Connection Command
	while (!_Control) {

		// Clear UART Buffer
    	_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT+CREG="));
		GSM_Serial.print(_Mode);
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Handle Response
		if (_Response_Wait(_Response_Length, 500000)) {

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
				delay(3);

			}

			// Control for Response
			if (strstr(_Serial_Buffer, "OK") != NULL) _Control = true;

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

	// Send CREG: 1 Connection Command
	while (!_Control) {

		// Handle Response
		if (_Response_Wait(_Response_Length, 300000)) {

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
				delay(3);

			}

			// Control for Response
			if (strstr(_Serial_Buffer, "+CREG:") != NULL) {

				// Read Stat
				_Stat = _Serial_Buffer[9];

				// Handle Stat Variable
				if (_Stat == 48) {

					// Set Variable
					CREG_Status = NOT_REGISTERED;

					// Set Control Variable
					_Control = false;

				} // Not Registered [0]
				if (_Stat == 49) {

					// Set Variable
					CREG_Status = HOME_REGISTERED;

					// End Function
					return (true);

				} // Registered to Home Network [1]
				if (_Stat == 50) {

					// Set Variable
					CREG_Status = SEARCHING;

					// Set Control Variable
					_Control = false;

				} // Searching Network [2]
				if (_Stat == 51) {

					// Set Variable
					CREG_Status = DENIED;

					// Set Control Variable
					_Control = false;

				} // Registration Denied [3]
				if (_Stat == 52) {

					// Set Variable
					CREG_Status = UNKNOWN;

					// End Function
					return (true);

				} // Unknown Error [4]
				if (_Stat == 53) {

					// Set Variable
					CREG_Status = ROAMING_REGISTERED;

					// End Function
					return (false);

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
bool xE910_AT::CGREG(const bool _Mode) {

	// Control for CREG
	if (CREG_Status == HOME_REGISTERED or CREG_Status == ROAMING_REGISTERED) {
	
		// Declare Function Variables
		bool _Control = false;
		uint8_t _Error_WD = 0;

		// Declare Stat Variable
		uint8_t _Stat = NOT_REGISTERED;

    	// Declare Response Length
    	uint8_t _Response_Length = 6;

		// Send AT+CGREG=1 Connection Command
		while (!_Control) {

			// Clear UART Buffer
    		_Clear_UART_Buffer();

			// Send UART Command
			GSM_Serial.print(F("AT+CGREG="));
			GSM_Serial.print(_Mode);
			GSM_Serial.print(F("\r\n"));

			// Wait for UART Data Send
			GSM_Serial.flush();

			// Handle Response
			if (_Response_Wait(_Response_Length, 500000)) {

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
					delay(3);

				}

				// Control for Response
				if (strstr(_Serial_Buffer, "OK") != NULL) _Control = true;

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

		// Send CGREG: 1 Connection Command
		while (!_Control) {

			// Handle Response
			if (_Response_Wait(_Response_Length, 300000)) {

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
					delay(3);

				}

				// Control for Response
				if (strstr(_Serial_Buffer, "+CGREG:") != NULL) {

					// Read Stat
					_Stat = _Serial_Buffer[10];

					// Handle Stat Variable
					if (_Stat == 48) {

						// Set Variable
						CGREG_Status = NOT_REGISTERED;

						// Set Control Variable
						_Control = false;

					} // Not Registered [0]
					if (_Stat == 49) {

						// Set Variable
						CGREG_Status = HOME_REGISTERED;

						// End Function
						return (true);

					} // Registered to Home Network [1]
					if (_Stat == 50) {

						// Set Variable
						CGREG_Status = SEARCHING;

						// Set Control Variable
						_Control = false;

					} // Searching Network [2]
					if (_Stat == 51) {

						// Set Variable
						CGREG_Status = DENIED;

						// Set Control Variable
						_Control = false;

					} // Registration Denied [3]
					if (_Stat == 52) {

						// Set Variable
						CGREG_Status = UNKNOWN;

						// End Function
						return (true);

					} // Unknown Error [4]
					if (_Stat == 53) {

						// Set Variable
						CGREG_Status = ROAMING_REGISTERED;

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
bool xE910_AT::CGDCONT(const uint8_t _Cid, const char *_PDP_Type, const char *_APN) {

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CGDCONT="));
	GSM_Serial.print(String(_Cid));
	GSM_Serial.print(F(",\""));
	GSM_Serial.print(String(_PDP_Type));
	GSM_Serial.print(F("\",\""));
	GSM_Serial.print(String(_APN));
	GSM_Serial.print(F("\""));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 10000)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Clear Variable Array
		memset(_Response, 0, _Response_Length);

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
		if (strstr(_Response, "+CME") != NULL) return (false);
		if (strstr(_Response, "OK") != NULL) return (true);

		// End Function
		return (false);

    } else {

		// End Function
		return (false);

    }

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
bool xE910_AT::SGACT(const uint8_t _Cid = 1, const bool _Stat = true) {

	// Deactivate the Context
	if (!_Stat) {

    	// Declare Response Length
    	uint8_t _Response_Length = 6;

		// Clear UART Buffer
    	_Clear_UART_Buffer();

		// Send UART Command
		GSM_Serial.print(F("AT#SGACT="));
		GSM_Serial.print(String(_Cid));
		GSM_Serial.print(F(","));
		GSM_Serial.print(String(_Stat));
		GSM_Serial.print(F("\r\n"));

		// Wait for UART Data Send
		GSM_Serial.flush();

		// Handle Response
		if (_Response_Wait(_Response_Length, 150000)) {

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

    	}

		// End Function
		return (false);

	}
	
	// Activate the Context
	if (_Stat) {

		// Declare Function Variables
		bool _Control = false;
		uint8_t _Error_WD = 0;

    	// Declare Response Length
    	uint8_t _Response_Length = 6;

		// Clear UART Buffer
    	_Clear_UART_Buffer();

		// Send Connection Command
		while (!_Control) {

			// Clear UART Buffer
    		_Clear_UART_Buffer();

			// Send UART Command
			GSM_Serial.print(F("AT#SGACT="));
			GSM_Serial.print(String(_Cid));
			GSM_Serial.print(F(","));
			GSM_Serial.print(String(_Stat));
			GSM_Serial.print(F("\r\n"));

			// Wait for UART Data Send
			GSM_Serial.flush();

			// Handle Response
			if (_Response_Wait(_Response_Length, 150000)) {

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

    	// Declare Response Length
    	_Response_Length = 25;

		// Recieve Connection Command
		while (!_Control) {

			// Handle Response
			if (_Response_Wait(_Response_Length, 150000)) {

				// Declare Read Order Variable
				uint8_t _Read_Order = 0;
				uint8_t _Data_Order = 0;

				// Declare Response Variable
				char _Response[_Response_Length];

				// Read UART Response
				while (GSM_Serial.available() > 0) {

					// Read Serial Char
					_Response[_Read_Order] = GSM_Serial.read();

					// Handle Data
					if ((_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) or _Response[_Read_Order] == 46) {

						// Get Data
						IP_Address[_Data_Order] = _Response[_Read_Order];

						// Increase Data Order
						_Data_Order++;

					}

					// Increase Read Order
					_Read_Order++;

					// Stream Delay
					delay(3);

				}

				// Control for Command
				if (strstr(_Response, "#SGACT:") != NULL) {

					// Set Control Variable
					_Control = true;

				} else if (strstr(_Response, "+CME ERROR: 553") != NULL) {

					// Set Control Variable
					_Control = true;

				} else if (strstr(_Response, "+CME ERROR: 567") != NULL) {

					// Set Control Variable
					_Control = false;

				} else {

					// Set Control Variable
					_Control = false;

				}

			}

			// Count for Error
			_Error_WD++;

			// Handle for Error
			if (_Error_WD > 3) return (false);

		}

		// Control for Connection
		if (sizeof(IP_Address) >= 7) {

			// Set Variable
			Connection_Status = CONNECTED;

			// End Function
			return (true);

		} else {

			// Set Variable
			Connection_Status = NOT_CONNECTED;

			// End Function
			return (false);

		}
	
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