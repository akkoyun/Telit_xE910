/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit GE910 Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <Telit_xE910.h>

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

			// Set Control Variables
			GSM_AT.Command_Control.ATE 			= false;
			GSM_AT.Command_Control.CCLK			= false;
			GSM_AT.Command_Control.CGDCONT		= false;
			GSM_AT.Command_Control.CGREG		= false;
			GSM_AT.Command_Control.CGSN			= false;
			GSM_AT.Command_Control.CMEE			= false;
			GSM_AT.Command_Control.CPIN			= false;
			GSM_AT.Command_Control.CREG			= false;
			GSM_AT.Command_Control.CSQ			= false;
			GSM_AT.Command_Control.CTZU			= false;
			GSM_AT.Command_Control.FCLASS		= false;
			GSM_AT.Command_Control.GMI			= false;
			GSM_AT.Command_Control.GMM			= false;
			GSM_AT.Command_Control.GMR			= false;
			GSM_AT.Command_Control.GSN			= false;
			GSM_AT.Command_Control.ICCID		= false;
			GSM_AT.Command_Control.K			= false;
			GSM_AT.Command_Control.NITZ			= false;
			GSM_AT.Command_Control.NTP			= false;
			GSM_AT.Command_Control.REGMODE		= false;
			GSM_AT.Command_Control.SCFG			= false;
			GSM_AT.Command_Control.SCFGEXT2		= false;
			GSM_AT.Command_Control.SCFGEXT3		= false;
			GSM_AT.Command_Control.SCFGEXT		= false;
			GSM_AT.Command_Control.SERVINFO		= false;
			GSM_AT.Command_Control.SGACT		= false;
			GSM_AT.Command_Control.SHDN			= false;
			GSM_AT.Command_Control.SLED			= false;
			GSM_AT.Command_Control.TXMONMODE	= false;

		}

		// End Function
		return (true);

}
bool xE910_GSM::Initialize(const bool _Debug) {

	// Set Global Variable
	Debug_Mode = _Debug;

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
			while (!GSM_AT.Command_Control.AT) {

				// Process Command
				GSM_AT.AT();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.AT) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.AT) return (false);

			// **************************************************
			// ATE Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("ATE=0......................................."));

			// Process Command
			while (!GSM_AT.Command_Control.ATE) {

				// Process Command
				GSM_AT.ATE(_Parameter_ATE);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.ATE) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.ATE) return (false);

			// **************************************************
			// CMEE Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CMEE=1..................................."));

			// Process Command
			while (!GSM_AT.Command_Control.CMEE) {

				// Process Command
				GSM_AT.CMEE(_Parameter_CMEE);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CMEE) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CMEE) return (false);

			// **************************************************
			// FCLASS Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+FCLASS=0................................."));

			// Process Command
			while (!GSM_AT.Command_Control.FCLASS) {

				// Process Command
				GSM_AT.FCLASS(_Parameter_FCLASS);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.FCLASS) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.FCLASS) return (false);

			// **************************************************
			// K Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT&K0......................................."));

			// Process Command
			while (!GSM_AT.Command_Control.K) {

				// Process Command
				GSM_AT.K(_Parameter_K);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.K) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.K) return (false);

			// **************************************************
			// CPIN Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CPIN?...................................."));

			// Process Command
			while (!GSM_AT.Command_Control.CPIN) {

				// Process Command
				GSM_AT.CPIN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CPIN) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CPIN) return (false);

			// **************************************************
			// CGSN Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CGSN....................................."));

			// Process Command
			while (!GSM_AT.Command_Control.CGSN) {

				// Process Command
				GSM_AT.CGSN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CGSN) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CGSN) return (false);

			// **************************************************
			// GSN Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GSN......................................"));

			// Process Command
			while (!GSM_AT.Command_Control.GSN) {

				// Process Command
				GSM_AT.GSN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.GSN) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.GSN) return (false);

			// **************************************************
			// ICCID Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CCID....................................."));

			// Process Command
			while (!GSM_AT.Command_Control.ICCID) {

				// Process Command
				GSM_AT.CCID();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.ICCID) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.ICCID) return (false);

			// **************************************************
			// GMI Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GMI......................................"));

			// Process Command
			while (!GSM_AT.Command_Control.GMI) {

				// Process Command
				GSM_AT.GMI();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.GMI) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.GMI) return (false);

			// **************************************************
			// GMM Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GMM......................................"));

			// Process Command
			while (!GSM_AT.Command_Control.GMM) {

				// Process Command
				GSM_AT.GMM();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.GMM) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.GMM) return (false);

			// **************************************************
			// GMR Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+GMR......................................"));

			// Process Command
			while (!GSM_AT.Command_Control.GMR) {

				// Process Command
				GSM_AT.GMR();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.GMR) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.GMR) return (false);

			// **************************************************
			// SLED Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+SLED=2..................................."));

			// Process Command
			while (!GSM_AT.Command_Control.SLED) {

				// Process Command
				GSM_AT.SLED(_Parameter_SLED);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.SLED) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.SLED) return (false);

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

	// Control for Initialization Monitor
	if (GSM_AT.Initialization_Status) {

			// **************************************************
			// Connection Parameters
			// **************************************************
			const bool 		_Parameter_TXMONMODE 			= 0;
			const uint8_t 	_Parameter_CDGCONT_Cid 			= 1;
			const char 		*_Parameter_CGDCONT_PDP 		= "IP";
			const char 		*_Parameter_CGDCONT_APN 		= "mgbs";
			const char 		*_Parameter_CGDCONT_PDP_Addr 	= "";
			const bool 		_Parameter_CGDCONT_D_Comp 		= true;
			const bool 		_Parameter_CGDCONT_H_Comp 		= true;
			const uint8_t 	_Parameter_SCFG_ConnID 			= 0;
			const uint8_t 	_Parameter_SCFG_Cid 			= 1;
			const uint16_t 	_Parameter_SCFG_Pkt_Sz 			= 300;
			const uint16_t 	_Parameter_SCFG_Max_To 			= 90;
			const uint16_t 	_Parameter_SCFG_Conn_To 		= 600;
			const uint8_t 	_Parameter_SCFG_TX_To 			= 50;

			// **************************************************
			// TXMONMODE Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT#TXMONMODE="));
			if (Debug_Mode) Serial.print(_Parameter_TXMONMODE);
			if (Debug_Mode) Serial.print(F(".............................."));

			// Process Command
			while (!GSM_AT.Command_Control.TXMONMODE) {

				// Process Command
				GSM_AT.TXMONMODE(_Parameter_TXMONMODE);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.TXMONMODE) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.TXMONMODE) return (false);

			// **************************************************
			// CREG Command
			// **************************************************

			// Read Current Time
			uint32_t _CTime = millis();

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CREG=1,1................................."));

			// Process Command
			while (!GSM_AT.Command_Control.CREG) {

				// Process Command
				GSM_AT.CREG();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CREG) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CREG) return (false);

			// **************************************************
			// CGREG Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CGREG=1,1................................"));

			// Process Command
			while (!GSM_AT.Command_Control.CGREG) {

				// Process Command
				GSM_AT.CGREG();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CGREG) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CGREG) return (false);

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

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+CGDCONT.................................."));

			// Process Command
			while (!GSM_AT.Command_Control.CGDCONT) {

				// Process Command
				GSM_AT.CGDCONT(_Parameter_CDGCONT_Cid, _Parameter_CGDCONT_PDP, _Parameter_CGDCONT_APN, _Parameter_CGDCONT_PDP_Addr, _Parameter_CGDCONT_D_Comp, _Parameter_CGDCONT_H_Comp);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CGDCONT) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CGDCONT) return (false);

			// **************************************************
			// SCFG Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+SCFG....................................."));

			// Process Command
			while (!GSM_AT.Command_Control.SCFG) {

				// Process Command
				GSM_AT.SCFG(_Parameter_SCFG_ConnID, _Parameter_SCFG_Cid, _Parameter_SCFG_Pkt_Sz, _Parameter_SCFG_Max_To, _Parameter_SCFG_Conn_To, _Parameter_SCFG_TX_To);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.SCFG) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.SCFG) return (false);

			// **************************************************
			// SGACT Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (Debug_Mode) Serial.print(F("AT+SGACT...................................."));

			// Process Command
			while (!GSM_AT.Command_Control.SGACT) {

				// Process Command
				GSM_AT.SGACT(1, false, "", "");
				GSM_AT.SGACT(1, true, "", "");

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.SGACT) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.SGACT) return (false);



	}







}
bool xE910_GSM::Socket_Listen(void) {

}
bool xE910_GSM::RSSI_Refresh(void) {

	// **************************************************
	// CPIN Command
	// **************************************************

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Set Control Variable
	GSM_AT.Command_Control.CSQ = false;

	// Command Debug
	if (Debug_Mode) Serial.print(F("AT+CSQ......................................"));

	// Process Command
	while (!GSM_AT.Command_Control.CSQ) {

		// Process Command
		GSM_AT.CSQ();

		// Set WD Variable
		_Error_WD++;

		// Control for WD
		if (_Error_WD > 5) break;

	}

	// Print Command State
	if (GSM_AT.Command_Control.CSQ) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
	// End Function
	if (!GSM_AT.Command_Control.CSQ) return (false);

}

bool xE910_RTC::Time_Update(void) {

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Control for Power Monitor
	if (GSM_HARDWARE.PowerMonitor()) {

		// Control for Connection
		if (GSM_AT.Connection_Status) {
		
			// **************************************************
			// NITZ Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+NITZ....................................."));

			// Process Command
			while (!GSM_AT.Command_Control.NITZ) {

				// Process Command
				GSM_AT.NITZ(true);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.NITZ) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.NITZ) return (false);

			// **************************************************
			// CTZU Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+CTZU....................................."));

			// Process Command
			while (!GSM_AT.Command_Control.CTZU) {

				// Process Command
				GSM_AT.CTZU(true);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.CTZU) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.CTZU) return (false);

			// **************************************************
			// NTP Command
			// **************************************************

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Command Debug
			if (GSM.Debug_Mode) Serial.print(F("AT+NTP......................................"));

			// Process Command
			while (!GSM_AT.Command_Control.NTP) {

				// Process Command
				GSM_AT.NTP("85.199.214.98", 123, 1, 3);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			if (GSM_AT.Command_Control.NTP) {Serial.println(F("..[OK]"));} else {Serial.println(F("[FAIL]"));}
		
			// End Function
			if (!GSM_AT.Command_Control.NTP) return (false);

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

bool xE910_AT::AT(void) {

    // Declare Response Length
    uint8_t _Response_Length = 10;

	// Set Control Variable
	Command_Control.AT = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT"));
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
			Command_Control.AT = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.AT = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.AT = false;

		// End Function
		return (false);

    }

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
bool xE910_AT::SHDN(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.SHDN = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SHDN"));
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
			Command_Control.SHDN = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.SHDN = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.SHDN = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::CMEE(const uint8_t _CMEE) {

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

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

				// Get Data
				IMEI[_Data_Order] = _Response[_Read_Order];

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

			// Set Control Variable
			Command_Control.CGSN = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.CGSN = false;

			// End Function
			return (false);

		}

	} else {

		// Set Control Variable
		Command_Control.CGSN = false;

		// End Function
		return (false);

    }
}
bool xE910_AT::GSN(void) {

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

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

				// Get Data
				Serial_Number[_Data_Order] = _Response[_Read_Order];

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


			// Set Control Variable
			Command_Control.GSN = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.GSN = false;

			// End Function
			return (false);

		}

	} else {

		// Set Control Variable
		Command_Control.GSN = false;

		// End Function
		return (false);

    }
}
bool xE910_AT::CCID(void) {

	// Control for SIM Module
	if (Command_Control.CPIN) {
		
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

			// Read UART Response
			while (GSM_Serial.available() > 0) {

				// Read Serial Char
				_Response[_Read_Order] = GSM_Serial.read();

				// Handle Data
				if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

					// Get Data
					ICCID[_Data_Order] = _Response[_Read_Order];

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

				// Set Control Variable
				Command_Control.ICCID = true;

				// End Function
				return (true);

			} else {

				// Set Control Variable
				Command_Control.ICCID = false;

				// End Function
				return (false);

			}


		} else {

			// Set Control Variable
			Command_Control.ICCID = false;

			// End Function
			return (false);
		
		}

	} else {


		// End Function
		return (false);

	}

}
bool xE910_AT::GMI(void) {

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

		// Clear Variable Array
		memset(Modem_Firmware, 0, 10);

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
				Modem_Firmware[_Data_Order] = _Response[_Read_Order];

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

			// Set Control Variable
			Command_Control.GMR = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.GMR = false;

			// End Function
			return (false);

		}

	} else {

		// Set Control Variable
		Command_Control.GMR = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::CSQ(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

   	// Declare Response Length
   	uint8_t _Response_Length = 19;

	// Set Control Variable
	Command_Control.CSQ = false;

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CSQ"));
	GSM_Serial.print(F("\r\n"));

	// Wait for UART Data Send
	GSM_Serial.flush();

	// Handle Response
	if (_Response_Wait(_Response_Length, 1000)) {

		// Declare Read Order Variable
		uint8_t _Read_Order = 0;
		uint8_t _Data_Order = 0;

		// Declare Response Variable
		char _Response[_Response_Length];

		// Declare Response Data
		char _CSQ[2]; 

		// Read UART Response
		while (GSM_Serial.available() > 0) {

			// Read Serial Char
			_Response[_Read_Order] = GSM_Serial.read();

			// Handle Data
			if (_Response[_Read_Order] < 58 and _Response[_Read_Order] > 47) {

				// Get Data
				_CSQ[_Data_Order] = _Response[_Read_Order];

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

			// Set Signal Variable
			Signal_RSSI = atoi(_CSQ);

			// Set Control Variable
			Command_Control.CSQ = true;

			// End Function
			return (true);

		} else {

			// Set Signal Variable
			Signal_RSSI = 0;

			// Set Control Variable
			Command_Control.CSQ = false;

			// End Function
			return (false);

		}


	} else {

		// Set Signal Variable
		Signal_RSSI = 0;

		// Set Control Variable
		Command_Control.CSQ = false;

		// End Function
		return (false);
		
	}

}
bool xE910_AT::SERVINFO(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

   	// Declare Response Length
   	uint8_t _Response_Length = 60;

	// Set Control Variable
	Command_Control.SERVINFO = false;

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#SERVINFO"));
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
			delayMicroseconds(500);

		}

		// Control for Response
		if (strstr(_Response, "#SERVINFO:") != NULL) {

			// Control Operator ID
			Operator = 99;												// Unknown Operator
			if (strstr(_Response, "28601") != NULL) Operator = 28601;	// Turkcell
			if (strstr(_Response, "28602") != NULL) Operator = 28602;	// Vodafone
			if (strstr(_Response, "28603") != NULL) Operator = 28603;	// Turk Telecom
			if (strstr(_Response, "28604") != NULL) Operator = 28604;	// Turk Telecom

			// Set Control Variable
			Command_Control.SERVINFO = true;

			// End Function
			return (true);

		} else {

			// Set Signal Variable
			Operator = 0;

			// Set Control Variable
			Command_Control.SERVINFO = false;

			// End Function
			return (false);

		}


	} else {

		// Set Signal Variable
		Operator = 0;

		// Set Control Variable
		Command_Control.SERVINFO = false;

		// End Function
		return (false);
		
	}

}
bool xE910_AT::SLED(const uint8_t _SLED) {

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
		if (_Response_Wait(_Response_Length, (uint32_t)300000)) {

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
			if (_Response_Wait(_Response_Length, (uint32_t)300000)) {

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
	GSM_Serial.print(F("\""));
	GSM_Serial.print(F("\r\n"));

	/*
	if (_PDP_Addr != "") {
		GSM_Serial.print(F("\",\""));
		GSM_Serial.print(String(_PDP_Addr));
		GSM_Serial.print(F("\","));
	} else {
		GSM_Serial.print(F("\","));
		GSM_Serial.print(F(","));
	}
	
	GSM_Serial.print(String(_D_Comp));
	GSM_Serial.print(F(","));
	GSM_Serial.print(String(_H_Comp));
	GSM_Serial.print(F("\r\n"));
	*/

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
bool xE910_AT::SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.SCFGEXT = false;

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
			Command_Control.SCFGEXT = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.SCFGEXT = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.SCFGEXT = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.SCFGEXT2 = false;

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
			Command_Control.SCFGEXT2 = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.SCFGEXT2 = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.SCFGEXT2 = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.SCFGEXT3 = false;

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
			Command_Control.SCFGEXT3 = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.SCFGEXT3 = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.SCFGEXT3 = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::SGACT(const uint8_t _Cid, const bool _Stat, const char *_User_ID, const char *_Password) {

	// Set Control Variable
	Command_Control.SGACT = false;

	// Deactivate the Context
	if (!_Stat) {

		// Command Chain Delay (Advice by Telit)
		delay(10);

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
				delayMicroseconds(500);

			}

			// Control for Response
			if (strstr(_Response, "OK") != NULL) {

				// Set Control Variable
				Command_Control.SGACT = true;

				// End Function
				return (true);

			} else {

				// Set Control Variable
				Command_Control.SGACT = false;

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

			// Check Username and Password
			if (_User_ID != "" and _Password != "") {

				// Send Username and Password		
				GSM_Serial.print(F(",\""));
				GSM_Serial.print(String(_User_ID));
				GSM_Serial.print(F("\",\""));
				GSM_Serial.print(String(_Password));
				GSM_Serial.print(F("\""));

			}

			// Send Command Termination
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
					delayMicroseconds(500);

				}

				// Control for Command
				if (strstr(_Response, "#SGACT:") != NULL) {

					// Set Control Variable
					Command_Control.SGACT = true;

					// Set Control Variable
					_Control = true;

				} else if (strstr(_Response, "+CME ERROR: 553") != NULL) {

					// Set Control Variable
					Command_Control.SGACT = true;

					// Set Control Variable
					_Control = true;

				} else {

					// Set Control Variable
					Command_Control.SGACT = false;

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
bool xE910_AT::CTZU(const bool _State) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.CTZU = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CTZU="));
	GSM_Serial.print(String(_State));
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
			Command_Control.CTZU = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.CTZU = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.CTZU = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::NITZ(const bool _State) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 6;

	// Set Control Variable
	Command_Control.NITZ = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT#NITZ="));
	GSM_Serial.print(String(_State));
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
			Command_Control.NITZ = true;

			// End Function
			return (true);

		} else {

			// Set Control Variable
			Command_Control.NITZ = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.NITZ = false;

		// End Function
		return (false);

    }

}
bool xE910_AT::NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 31;

	// Set Control Variable
	Command_Control.NTP = false;

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
			Command_Control.NTP = true;

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

			// Set Control Variable
			Command_Control.NTP = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.NTP = false;

		// End Function
		return (false);

    }
	
}
bool xE910_AT::CCLK(void) {

	// Command Chain Delay (Advice by Telit)
	delay(10);

    // Declare Response Length
    uint8_t _Response_Length = 34;

	// Set Control Variable
	Command_Control.CCLK = false;

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	GSM_Serial.print(F("AT+CCLK?"));
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
			Command_Control.CCLK = true;

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

			// Set Control Variable
			Command_Control.CCLK = false;

			// End Function
			return (false);

		}

    } else {

		// Set Control Variable
		Command_Control.CCLK = false;

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



// Define Library Class
xE910_GSM GSM;
xE910_HARDWARE GSM_HARDWARE;
xE910_AT GSM_AT;
xE910_RTC GSM_RTC;
