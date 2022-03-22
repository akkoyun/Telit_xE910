/* *******************************************************************************
 *  Copyright (C) 2014-2020 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit GE910 Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *********************************************************************************/

#include <Telit_xE910.h>

/**************************************************
 * Public Functions
 **************************************************/
bool Telit_xE910::Begin(void) {

	// GSM Module Port Definations
	DDRJ	&= 0b11111011;	PORTJ	|= 0b00000100;	// PJ2 - Input / Pull Down [GSM Ring]
	DDRJ	&= 0b11110111;	PORTJ	&= 0b11110111;	// PJ3 - Input / Pull Down [GSM Power Monitor]
	DDRJ	|= 0b00010000;	PORTJ	|= 0b00010000;	// PJ4 - Output / Pull Down [GSM Communication Enable]
	DDRJ	|= 0b00100000;	PORTJ	&= 0b11011111;	// PJ5 - Output / Pull Down [GSM Shut Down]
	DDRJ	|= 0b01000000;	PORTJ	&= 0b10111111;	// PJ6 - Output / Pull Down [GSM On Off]

	// Serial Begin
	_GSM_Serial.begin(115200);

	// Power On Modem
	bool _Power_Status = Power_ON(false, false, true);

	// Initialize Modem
	bool _Set_Status = Set_Modem();

	// End Function
	return(_Power_Status and _Set_Status);

}
bool Telit_xE910::Set_Modem(void) {

	// Declare Response Status
	bool _Response = false;

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Command Set
	struct _Command_Struct {
  		bool AT = true;
  		bool ATE = true;
  		bool CMEE = true;
  		bool FCLASS = true;
  		bool K = true;
  		bool CPIN = true;
  		bool CGSN = true;
  		bool GSN = true;
  		bool ICCID = true;
  		bool GMI = true;
  		bool GMM = true;
  		bool GMR = true;
  		bool SLED = true;
  		bool E2SLRI = true;
	} _Command;

	// Control for Power Monitor
	if (Modem.Power_Monitor) {

		// AT Command
		if (_Command.AT) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = AT();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}
		
			// End Function
			if (!_Response) return (false);

		}
		
		// ATE Command
		if (_Command.ATE) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = ATE(false);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// CMEE Command
		if (_Command.CMEE) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 1, Debug_Initialize_Y, BLUE, F(" .. "));
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
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 1, Debug_Initialize_Y);
			#endif

			// End Function
			if (!_Response) return (false);

		}

		// FCLASS Command
		if (_Command.FCLASS) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 2, Debug_Initialize_Y, BLUE, F(" .. "));
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
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 2, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// K Command
		if (_Command.K) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 3, Debug_Initialize_Y, BLUE, F(" .. "));
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
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 3, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// CPIN Command
		if (_Command.CPIN) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 4, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = CPIN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 4, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// CGSN Command
		if (_Command.CGSN) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 5, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = CGSN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 5, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// GSN Command
		if (_Command.GSN) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 6, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GSN();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 6, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// ICCID Command
		if (_Command.ICCID) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 7, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = CCID();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 7, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// GMI Command
		if (_Command.GMI) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 8, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GMI();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 8, Debug_Initialize_Y);
			#endif

			// End Function
			if (!_Response) return (false);

		}

		// GMM Command
		if (_Command.GMM) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 9, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GMM();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 9, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// GMR Command
		if (_Command.GMR) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 10, Debug_Initialize_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = GMR();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 10, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// SLED Command
		if (_Command.SLED) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 11, Debug_Initialize_Y, BLUE, F(" .. "));
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
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 11, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// E2SLRI Command
		if (_Command.E2SLRI) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Initialize_X + 12, Debug_Initialize_Y, BLUE, F(" .. "));
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
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Initialize_X + 12, Debug_Initialize_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// Set Variable
		Modem.Initialize_Status = true;

		// End Function
		return(true);

	}

	// End Function
	return(false);

}
bool Telit_xE910::Connect(void) { 

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Declare Response Status
	bool _Response = false;

	// Declare Time Variable
	uint32_t _Connection_Start_Time = 0;
	uint32_t _Connection_Time = 0;

	// Control for Initialization Monitor
	if (Modem.Initialize_Status) {

		// Command Set
		struct _Command_Struct {
			const bool REGMODE = true;
			const bool TXMONMODE = true;
			const bool CREG = true;
			const bool CGREG = true;
			const bool SCFG1 = true;
			const bool SCFG2 = true;
			const bool SCFGEXT2 = true;
			const bool CGDCONT = true;
			const bool SERVINFO = true;
			const bool SGACT = true;
			const bool HTTPCFG = true;
			const bool Firewall = true;
			const bool ICMP = true;
			const bool CSQ = true;
		} _Command;

		// REGMODE Command
		if (_Command.REGMODE) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = REGMODE(0);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X, Debug_Connect_Y);
			#endif

			// End Function
			if (!_Response) return (false);
			
		}

		// TXMONMODE Command
		if (_Command.TXMONMODE) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 1, Debug_Connect_Y, BLUE, F(" .. "));
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
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 1, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);
			
		}

		// CREG Command
		if (_Command.CREG) {

			// Get Connection Start Time
			_Connection_Start_Time = millis();

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 2, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = CREG(0);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 2, Debug_Connect_Y);
			#endif

			// Calculate Connection Time
			_Connection_Time += millis() - _Connection_Start_Time;		

			// End Function
			if (!_Response) return (false);

		}

		// CGREG Command
		if (_Command.CGREG) {

			// Get Connection Start Time
			_Connection_Start_Time = millis();

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 3, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = CGREG(0);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 3, Debug_Connect_Y);
			#endif

			// Calculate Connection Time
			_Connection_Time += millis() - _Connection_Start_Time;		

			// End Function
			if (!_Response) return (false);

		}

		// SCFG Command Socket 1
		if (_Command.SCFG1) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 4, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = SCFG(1, 1, 1500, 90, 600, 50);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 4, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// SCFG Command Socket 2
		if (_Command.SCFG2) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 5, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = SCFG(2, 1, 1500, 90, 300, 50);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 5, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// SCFGEXT Command Socket 2
		if (_Command.SCFGEXT2) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 6, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = SCFGEXT(2, 1, false, 1, false, false);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 6, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// CGDCONT Command
		if (_Command.CGDCONT) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 7, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Process Command
				_Response = CGDCONT(1, "IP", "mgbs", "0.0.0.0", false, false);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 7, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// SERVINFO Command
		if (_Command.SERVINFO) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 8, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = SERVINFO();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

				// Delay
				delay(100);

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 8, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// SGACT Command
		if (_Command.SGACT) {

			// Get Connection Start Time
			_Connection_Start_Time = millis();

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 9, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Get IP Address
				_Response = SGACT(1, true);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 1) break;

				// Delay
				delay(100);

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 9, Debug_Connect_Y);
			#endif
		
			// Calculate Connection Time
			_Connection_Time += millis() - _Connection_Start_Time;		

			// End Function
			if (!_Response) return (false);

		}

		// CSQ Command
		if (_Command.CSQ) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Process Command
			while (!_Response) {

				// Get RSSI 
				_Response = CSQ();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 1) break;

				// Delay
				delay(100);

			}

			// End Function
			if (!_Response) return (false);

		}

		// HTTPCFG Command
		if (_Command.HTTPCFG) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 10, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Set HTTP Configuration
				_Response = HTTPCFG("54.216.226.171", 80);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 1) break;

				// Delay
				delay(100);

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 10, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// ICMP Command
		if (_Command.ICMP) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 11, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Set HTTP Configuration
				_Response = ICMP(1);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 1) break;

				// Delay
				delay(100);

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 11, Debug_Connect_Y);
			#endif
		
			// End Function
			if (!_Response) return (false);

		}

		// Firewall Command
		if (_Command.Firewall) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Print Command State
			#ifdef Debug
			Terminal.Text(Debug_Connect_X + 12, Debug_Connect_Y, BLUE, F(" .. "));
			#endif

			// Process Command
			while (!_Response) {

				// Set Firewall Configuration
				_Response = Set_Firewall();

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 1) break;

				// Delay
				delay(100);

			}

			// Print Command State
			#ifdef Debug
			Terminal.OK_Decide(_Response, Debug_Connect_X + 12, Debug_Connect_Y);
			#endif

			// End Function
			if (!_Response) return (false);

		}

		// Calculate Connection Time
		Variables.Connection_Time = (_Connection_Start_Time / 1000);

		// End Function
		return(Modem.Connection_Status);

	}

}
bool Telit_xE910::Time_Update(void) {

	// Declare Response Status
	bool _Response = false;

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Control for Initialization Monitor
	if (Modem.Connection_Status) {

		// Command Set
		struct _Command_Struct {
			const bool NITZ = false;
			const bool CTZU = false;
			const bool NTP = true;
		} _Command;

		// NITZ Command
		if (_Command.NITZ) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = NITZ(true);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// End Function
			if (!_Response) return (false);

		}

		// CTZU Command
		if (_Command.CTZU) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = CTZU(true);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// End Function
			if (!_Response) return (false);

		}

		// NTP Command
		if (_Command.NTP) {

			// Declare Watchdog Variable
			_Error_WD = 0;

			// Set Response Variable
			_Response = false;

			// Process Command
			while (!_Response) {

				// Send Command
				_Response = NTP("time.google.com", 123, true, 3);

				// Set WD Variable
				_Error_WD++;

				// Control for WD
				if (_Error_WD > 5) break;

			}

			// Update Variable
			if (_Response) Modem.Time_Status = true;

			// End Function
			if (!_Response) return (false);

		}

		// End Function
		return(true);

	}

	// End Function
	return(false);

}
bool Telit_xE910::Get_RSSI(void) {

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Set Response Variable
	bool _Response = false;

	// Process Command
	while (!_Response) {

		// Get RSSI 
		_Response = CSQ();

		// Set WD Variable
		_Error_WD++;

		// Control for WD
		if (_Error_WD > 1) break;

		// Delay
		delay(100);

	}

	// End Function
	return(_Response);

}
uint8_t Telit_xE910::Get_Signal(void) {

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Set Response Variable
	bool _Response = false;

	// Process Command
	while (!_Response) {

		// Get RSSI 
		_Response = CSQ();

		// Set WD Variable
		_Error_WD++;

		// Control for WD
		if (_Error_WD > 1) break;

		// Delay
		delay(100);

	}

	// End Function
	return(_Signal_Strength(Variables.RSSI));

}
bool Telit_xE910::Connection_Control(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[50];

	// Send UART Command
	_GSM_Serial.print(F("AT+CGACT?"));
	_GSM_Serial.print(F("\r\n"));

	// Command Work Delay
	delay(15);

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(5);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Declare Status Variable
	uint8_t _State = _Buffer_Variable[10];

	// Control Response
	if (_State == 48) return(false);
	if (_State == 49) return(true);

	// End Function
	return(false);

}
bool Telit_xE910::Send_Data_Pack(const uint8_t _Pack_Type, const char *_Data) {

	// Declare Variable
	bool _Response = false;

	// Close Socket Command
	if (_Pack_Type != 1) GSM.Socket_Close();

	// Send Pack
	if (HTTPSND("/api/v1.1/p402", _Data)) _Response = HTTPRCV();

	// Open Socket Command
	if (_Pack_Type != 1) GSM.Socket_Open();

	// End Function
	return(_Response);

}
bool Telit_xE910::Set_Firewall(void) {

	// Declare Response Status
	bool _Response = false;

	// Declare Watchdog Variable
	uint8_t _Error_WD = 0;

	// Command Set
	struct _Command_Struct {
		const bool Firewall_1 = true;
		const bool Firewall_2 = false;
		const bool Firewall_3 = false;
	} _Command;

	// Firewall 1 Command
	if (_Command.Firewall_1) {

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Process Command
		while (!_Response) {

			// Send Command
			_Response = FRWL(1, "213.14.250.214");

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// End Function
		if (!_Response) return (false);

	}

	// Firewall 2 Command
	if (_Command.Firewall_2) {

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Process Command
		while (!_Response) {

			// Send Command
			_Response = FRWL(1, "83.160.73.106");

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// End Function
		if (!_Response) return (false);

	}

	// Firewall 3 Command
	if (_Command.Firewall_3) {

		// Declare Watchdog Variable
		_Error_WD = 0;

		// Set Response Variable
		_Response = false;

		// Process Command
		while (!_Response) {

			// Send Command
			_Response = FRWL(1, "213.127.108.158");

			// Set WD Variable
			_Error_WD++;

			// Control for WD
			if (_Error_WD > 5) break;

		}

		// End Function
		if (!_Response) return (false);

	}

	// End Function
	return(true);

}

// Socket Functions
bool Telit_xE910::Socket_Close(void) {

	// Declare Variable
	bool _Response = false;

	// Control Socket Connection
	uint8_t _SS = SS(2);

	// Open Socket
	if (_SS == 0) {

		// End Function
		return(true);

	} else {

		// Close Socket
		SL(2, 0, 80, 255);

		// Re Check Socket
		_SS = SS(2);

		// Control for Socket
		if (_SS == 0) return(true);

	}
	
	// End Function
	return(false);

}
bool Telit_xE910::Socket_Open(void) {

	// Declare Variable
	bool _Response = false;

	// Control Socket Connection
	uint8_t _SS = SS(2);

	// Open Socket
	if (_SS == 4) {

		// End Function
		return(true);

	} else {

		// Close Socket
		SL(2, 1, 80, 255);

		// Re Check Socket
		_SS = SS(2);

		// Control for Socket
		if (_SS == 4) return(true);

	}
	
	// End Function
	return(false);

}
uint8_t Telit_xE910::Socket_Status(void) {

	// Socket Listen
	uint8_t _Response = SS(2);

	// End Function
	return(_Response);

}
uint16_t Telit_xE910::Socket_Answer(void) {

	// Beep
	#ifdef Debug
		Terminal.Beep();
	#endif

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Read Current Time
	uint32_t _Time = millis();

	// Declare Control Variables
	bool _R = false;
	bool _N = false;

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, "SRING:") != NULL) {

			// Control for Char
			if (_Buffer_Variable[_Buffer.Read_Order] == '\r') _R = true;
			if (_Buffer_Variable[_Buffer.Read_Order] == '\n') _N = true;

			// Handle Response
			if (_R and _N) _Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Handle Response
	if (_Buffer.Response) {

		// Answer Socket
		uint16_t _Incomming_Size = SA(2,1);

		// Socket Recieve Command
		uint16_t Incomming_Command = SRECV(2, _Incomming_Size);

		// Debug Message
		#ifdef Debug
		Terminal.Text(17, 110, YELLOW, String(Incomming_Command));
		#endif

		// End Function
		return(Incomming_Command);

	}

}
bool Telit_xE910::Socket_Send(const char * _Data_Pack) {

	// Send Response Message
	bool _Response_Send = SSEND(2, _Data_Pack);
	
	// Close Socket
	bool _Response_Close = SH(2);

	// ReOpen Socket
	Socket_Open();

	// End Function
	return(_Response_Send and _Response_Close);

}

// Socket Functions
uint8_t Telit_xE910::Get_Connection_Status(void) {return(Modem.Connection_Status);}

/**************************************************
 * Private Functions
 **************************************************/

 // Hardware Functions
void Telit_xE910::Set_Communication(const bool _State) {

	// Enable Communication 
	if (_State) PORTJ &= 0b11101111;

	// Disable Communication
	if (!_State) PORTJ |= 0b00010000;

}
void Telit_xE910::Set_OnOff(const uint16_t _Time) {

	// Set On/Off Signal HIGH [PJ6]
	PORTJ |= 0b01000000;

	// Command Delay
	for (uint8_t i = 0; i < 100; i++) {

		// Calculate Delay (2000)
		uint8_t _Delay = _Time / 100;

		// Wait
		delay(_Delay); 

		// Terminal Bar
		#ifdef Debug
		Terminal.Text(Debug_Boot_X, Debug_Boot_Y + i, WHITE, F("▒"));
		#endif

	}

	// Set On/Off Signal LOW [PJ6]
	PORTJ &= 0b10111111;

	// Clear Bar
	#ifdef Debug
	for (uint8_t i = 0; i < 100; i++) Terminal.Text(Debug_Boot_X, Debug_Boot_Y + i, WHITE, F(" "));
	#endif

}
void Telit_xE910::Set_ShutDown(const uint16_t _Time) {

	// Set Shut Down Signal HIGH [PJ5]
	PORTJ |= 0b00100000;

	// Command Delay
	delay(_Time);

	// Set Shut Down Signal LOW [PJ5]
	PORTJ &= 0b11011111;

}
void Telit_xE910::Set_Power_Switch(const bool _State) {

	// Set GSM Power Enable
	if (_State) PORTH |= 0b00000100;

	// Set GSM Power Disable
	if (!_State) PORTH &= 0b11111011;

}
void Telit_xE910::Set_LED(const bool _State) {

	// Set GSM LED Power Enable
	if (_State) PORTH &= 0b11101111;

	// Set GSM LED Power Disable
	if (!_State) PORTH |= 0b00010000;

}
bool Telit_xE910::Get_PowerMonitor(void) {

	// Control for PWMon (PJ3)
	if ((PINJ & (1 << PINJ3)) == (1 << PINJ3)) {

		// Set Variable
		Modem.Power_Monitor = 1;

		// Power Monitor 3V3 HIGH
		return (true);

	} else {

		// Set Variable
		Modem.Power_Monitor = 0;

		// Power Monitor 3V3 LOW
		return (false);

	}

}

// Hardware Batch Functions
bool Telit_xE910::Power_ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

	// Send Shut Down Signal
	Set_ShutDown(500);

	// Enable GSM Modem Power Switch
	if (_Power_Switch) Set_Power_Switch(true);  
	
	// Enable GSM Modem LED Feed
	if (_LED_Switch) Set_LED(true);

	// Set Communication Signal LOW
	if (_Communication_Switch) Set_Communication(true);
	
	// Turn On Modem
	if (Get_PowerMonitor()) {

		// Set Variable
		Modem.Power_Monitor = 1;
		Modem.Connection_Status = 0;

		// End Function
		return (true);

	} else {

		// Send On Off Signal
		Set_OnOff(5000);

		// Control for PWMon (PH7)
		if (Get_PowerMonitor()) {

			// Set Variable
			Modem.Power_Monitor = 1;
			Modem.Connection_Status = 0;

			// End Function
			return (true);

		} else {

			// Send Shut Down Signal
			Set_ShutDown(200);

		}

	}

	// Set Variable
	Modem.Power_Monitor = 0;
	Modem.Connection_Status = 0;

	// End Function
	return (false);

}
bool Telit_xE910::Power_OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

	// Turn Off Modem
	if (Get_PowerMonitor()) Set_ShutDown(200);

	// Disable GSM LED Power
	if (_LED_Switch) Set_LED(false);

	// Disable GSM Modem Voltage Translator
	if (_Communication_Switch) Set_Communication(false);

	// Disable GSM Modem Main Power Switch
	if (_Power_Switch) Set_Power_Switch(false);

	// Set Variable
	Modem.Power_Monitor = 0;
	Modem.Connection_Status = 0;
	Modem.Initialize_Status = false;

	// Command Delay
	delay(1000);

	// End Function
	return (true);

}

// Initialize Commands
bool Telit_xE910::AT(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	const uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::ATE(const bool _ECHO) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("ATE"));
	_GSM_Serial.print(String(_ECHO));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::CMEE(const uint8_t _CMEE) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+CMEE="));
	_GSM_Serial.print(String(_CMEE));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::FCLASS(const uint8_t _FCLASS) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+FCLASS="));
	_GSM_Serial.print(String(_FCLASS));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::K(const uint8_t _K) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT&K"));
	_GSM_Serial.print(String(_K));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::CPIN(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+CPIN?"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Control for Response
	if (strstr(_Buffer_Variable, _READY_) != NULL) {

		// Set Control Variable
		Modem.SIM_Status = true;

	} else {

		Modem.SIM_Status = false;

	}

	// End Function
	return (Modem.SIM_Status);

}
bool Telit_xE910::CGSN(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+CGSN"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle Data
		if (_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) {

			// Get Data
			Variables.IMEI[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

	}

	// End Function
	return(true);

}
bool Telit_xE910::GSN(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+GSN"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Clear Variables
	memset(Variables.Serial_Number, 0, 11);

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle Data
		if (_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) {

			// Get Data
			Variables.Serial_Number[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

	}

	// End Function
	return(true);

}
bool Telit_xE910::CCID(void) {

	// Control for SIM Module
	if (Modem.SIM_Status) {
		
		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Declare Buffer Object
		Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

		// Declare Buffer
		char _Buffer_Variable[255];
		memset(_Buffer_Variable, '\0', 255);

		// Send UART Command
		_GSM_Serial.print(F("AT#CCID"));
		_GSM_Serial.print(F("\r\n"));

		// Read Current Time
		uint32_t _Time = millis();

		// Command Delay
		delay(20);

		// Read UART Response
		while (!_Buffer.Response) {

			// Read Serial Char
			_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

			// Control for Response
			if (strstr(_Buffer_Variable, _OK_) != NULL) {

				// Handle Response
				_Buffer.Response = true;

			} else {

				// Buffer Read Delay
				delay(2);

			}

			// Handle for timeout
			if (millis() - _Time >= _Buffer.Time_Out) return(false);

			// Increase Read Order
			_Buffer.Read_Order++;

		}

		// Clear Variables
		memset(Variables.ICCID, 0, 21);

		// Control for Buffer
		for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

			// Handle Data
			if (_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) {

				// Get Data
				Variables.ICCID[_Buffer.Data_Order] = _Buffer_Variable[i];

				// Increase Data Order
				_Buffer.Data_Order++;

			}

		}

		// End Function
		return(true);

	} else {

		// End Function
		return (false);

	}

}
bool Telit_xE910::GMI(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+GMI"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Control for Response
	if (strstr(_Buffer_Variable, "Telit") != NULL) {

		// Set Manufacturer Variable
		Variables.Manufacturer = 1;

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

	// End Function
	return(true);

}
bool Telit_xE910::GMM(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+GMM"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Control for Response
	if (strstr(_Buffer_Variable, "GE910-QUAD") != NULL) {

		// Set Model Variable
		Variables.Model = 1;

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

	// End Function
	return(true);

}
bool Telit_xE910::GMR(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+GMR"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Clear Variables
	memset(Variables.Firmware, '\0', 10);

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle Data
		if ((_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) or _Buffer_Variable[i] == 46) {

			// Get Data
			Variables.Firmware[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

	}

	// End Function
	return(true);

}
bool Telit_xE910::SLED(const uint8_t _SLED) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SLED="));
	_GSM_Serial.print(String(_SLED));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::E2SLRI(const uint16_t _Pulse_Duration) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#E2SLRI="));
	_GSM_Serial.print(String(_Pulse_Duration));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::TXMONMODE(const uint8_t _TXMONMODE) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#TXMONMODE="));
	_GSM_Serial.print(String(_TXMONMODE));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::REGMODE(const uint8_t _REGMODE) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#REGMODE="));
	_GSM_Serial.print(String(_REGMODE));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}

// Context Functions
bool Telit_xE910::CREG(const bool _Mode) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Set = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+CREG="));
	_GSM_Serial.print(_Mode);
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_Buffer_Set.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer_Set.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer_Set.Response = true;

		} else {

			// Buffer Read Delay
			delay(5);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Set.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Set.Read_Order++;

	}

	// Declare Watchdog Variables
	uint8_t _Error_WD = 0;

	// Reset CREG Variable
	Modem.CREG_Status = 0;

	// Define Control Variable
	bool _Control = false;

	// Control Loop
	while (!_Control) {

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Declare Buffer Object
		Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 5000};

		// Send UART Command
		_GSM_Serial.print(F("AT+CREG?\r\n"));

		// Declare Buffer
		char _Buffer_Get_Variable[255];
		memset(_Buffer_Get_Variable, '\0', 255);

		// Read Current Time
		_Time = millis();

		// Read UART Response
		while (!_Buffer_Get.Response) {

			// Read Serial Char
			_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

			// Control for Response
			if (strstr(_Buffer_Get_Variable, _OK_) != NULL) {

				// Handle Response
				_Buffer_Get.Response = true;

			} else {

				// Buffer Read Delay
				delay(5);

			}

			// Handle for timeout
			if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

			// Increase Read Order
			_Buffer_Get.Read_Order++;

		}

		// Set Comma Handle
		uint8_t _Stat_Position = 0;

		// Control for Buffer
		for (uint8_t i = 0; i < _Buffer_Get.Read_Order; i++) {if (_Buffer_Get_Variable[i] == 44) _Stat_Position = i;}

		// Declare Status Variable
		uint8_t _Stat = _Buffer_Get_Variable[_Stat_Position + 1];

		// Show Buffer
//		Serial.print(_Buffer_Get_Variable);
//		Serial.println(_Stat);

		// Handle Stat Variable
		if (_Stat == 48) {

			// Set Variable
			Modem.CREG_Status = 0;

			// Set Control Variable
			_Control = false;

			// Count for Error
			_Error_WD++;

			// Wait Delay
			delay(2000);

		} // Not Registered [0]
		if (_Stat == 49) {

			// Set Variable
			Modem.CREG_Status = 1;

			// Set Control Variable
			_Control = true;

		} // Registered to Home Network [1]
		if (_Stat == 50) {

			// Set Variable
			Modem.CREG_Status = 2;

			// Set Control Variable
			_Control = false;

			// Count for Error
			_Error_WD++;

			// Wait Delay
			delay(2000);

		} // 2 Network [2]
		if (_Stat == 51) {

			// Set Variable
			Modem.CREG_Status = 3;

			// Set Control Variable
			_Control = false;

			// Count for Error
			_Error_WD++;

			// Wait Delay
			delay(5000);

		} // Registration 3 [3]
		if (_Stat == 52) {

			// Set Variable
			Modem.CREG_Status = 4;

			// End Function
			return (false);

		} // 4 Error [4]
		if (_Stat == 53) {

			// Set Variable
			Modem.CREG_Status = 5;

			// Set Control Variable
			_Control = true;

		} // Registered to Rooming Network [5]

		// Handle for Error
		if (_Error_WD > 60) return(false);

	};

	// End Function
	return (true);

}
bool Telit_xE910::CGREG(const bool _Mode) {

	// Control for CREG
	if (Modem.CREG_Status == 1 or Modem.CREG_Status == 5) {
	
		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Declare Buffer Object
		Struct_Serial_Buffer _Buffer_Set = {false, 0, 0, 1000};

		// Declare Buffer
		char _Buffer_Set_Variable[255];
		memset(_Buffer_Set_Variable, '\0', 255);

		// Send UART Command
		_GSM_Serial.print(F("AT+CGREG="));
		_GSM_Serial.print(_Mode);
		_GSM_Serial.print(F("\r\n"));

		// Read Current Time
		uint32_t _Time = millis();

		// Read UART Response
		while (!_Buffer_Set.Response) {

			// Read Serial Char
			_Buffer_Set_Variable[_Buffer_Set.Read_Order] = _GSM_Serial.read();

			// Control for Response
			if (strstr(_Buffer_Set_Variable, _OK_) != NULL) {

				// Handle Response
				_Buffer_Set.Response = true;

			} else {

				// Buffer Read Delay
				delay(2);

			}

			// Handle for timeout
			if (millis() - _Time >= _Buffer_Set.Time_Out) return(false);

			// Increase Read Order
			_Buffer_Set.Read_Order++;

		}

		// Declare Watchdog Variables
		uint8_t _Error_WD = 0;

		// Reset CGREG Variable
		Modem.CGREG_Status = 0;

		// Define Control Variable
		bool _Control = false;

		// Control Loop
		while (!_Control) {

			// Clear UART Buffer
			_Clear_UART_Buffer();

			// Declare Buffer Object
			Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 5000};

			// Send UART Command
			_GSM_Serial.print(F("AT+CGREG?\r\n"));

			// Declare Buffer
			char _Buffer_Get_Variable[255];
			memset(_Buffer_Get_Variable, '\0', 255);

			// Command Work Delay
			delay(15);

			// Read Current Time
			_Time = millis();

			// Read UART Response
			while (!_Buffer_Get.Response) {

				// Read Serial Char
				_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

				// Control for Response
				if (strstr(_Buffer_Get_Variable, _OK_) != NULL) {

					// Handle Response
					_Buffer_Get.Response = true;

				} else {

					// Buffer Read Delay
					delay(5);

				}

				// Handle for timeout
				if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

				// Increase Read Order
				_Buffer_Get.Read_Order++;

			}

			// Set Comma Handle
			uint8_t _Stat_Position = 0;

			// Control for Buffer
			for (uint8_t i = 0; i < _Buffer_Get.Read_Order; i++) {if (_Buffer_Get_Variable[i] == 44) _Stat_Position = i;}

			// Declare Status Variable
			uint8_t _Stat = _Buffer_Get_Variable[_Stat_Position + 1];

			// Handle Stat Variable
			if (_Stat == 48) {

				// Set Variable
				Modem.CGREG_Status = 0;

				// Set Control Variable
				_Control = false;

				// Count for Error
				_Error_WD++;

				// Wait Delay
				delay(2000);

			} // Not Registered [0]
			if (_Stat == 49) {

				// Set Variable
				Modem.CGREG_Status = 1;

				// Set Control Variable
				_Control = true;

			} // Registered to Home Network [1]
			if (_Stat == 50) {

				// Set Variable
				Modem.CGREG_Status = 2;

				// Set Control Variable
				_Control = false;

				// Count for Error
				_Error_WD++;

				// Wait Delay
				delay(2000);

			} // 2 Network [2]
			if (_Stat == 51) {

				// Set Variable
				Modem.CGREG_Status = 3;

				// Set Control Variable
				_Control = false;

				// Count for Error
				_Error_WD++;

				// Wait Delay
				delay(5000);

			} // Registration 3 [3]
			if (_Stat == 52) {

				// Set Variable
				Modem.CGREG_Status = 4;

				// End Function
				return (false);

			} // 4 Error [4]
			if (_Stat == 53) {

				// Set Variable
				Modem.CGREG_Status = 5;

				// Set Control Variable
				_Control = true;

			} // Registered to Rooming Network [5]

			// Handle for Error
			if (_Error_WD > 60) return(false);

		}

		// End Function
		return (true);

	} else {

		// End Function
		return (false);

	}

}
bool Telit_xE910::CGDCONT(const uint8_t _Cid, const char *_PDP_Type, const char *_APN, const char *_PDP_Addr, const bool _D_Comp, const bool _H_Comp) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+CGDCONT="));
	_GSM_Serial.print(String(_Cid));
	_GSM_Serial.print(F(",\""));
	_GSM_Serial.print(String(_PDP_Type));
	_GSM_Serial.print(F("\",\""));
	_GSM_Serial.print(String(_APN));
	_GSM_Serial.print(F("\",\""));
	_GSM_Serial.print(String(_PDP_Addr));
	_GSM_Serial.print(F("\","));
	_GSM_Serial.print(String(_D_Comp));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_H_Comp));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

//		Terminal.Text(30, _Buffer.Read_Order, CYAN, String(_Buffer_Variable[_Buffer.Read_Order]));

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SGACT(const uint8_t _Cid, const bool _Stat) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Send UART Command
	_GSM_Serial.print(F("AT#SGACT="));
	_GSM_Serial.print(String(_Cid));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Stat));
	_GSM_Serial.print(F("\r\n"));

	// Command Delay
	delay(10);

	// Clear Variables
	memset(Variables.IP_Address, '\0', 17);

	// Handle Stat
	if (_Stat) {

		// Declare Buffer Object
		Struct_Serial_Buffer _Buffer_Connect = {false, 0, 0, 150000};

		// Declare Buffer
		char _Buffer_Connect_Variable[255];
		memset(_Buffer_Connect_Variable, '\0', 255);

		// Read Current Time
		uint32_t _Time = millis();

		// Read UART Response
		while (!_Buffer_Connect.Response) {

			// Read Serial Char
			_Buffer_Connect_Variable[_Buffer_Connect.Read_Order] = _GSM_Serial.read();

			// Control for Response
			if (strstr(_Buffer_Connect_Variable, _OK_) != NULL) {

				// Handle Response
				_Buffer_Connect.Response = true;

			} else {

				// Buffer Read Delay
				delay(5);

			}

			// Handle for timeout
			if (millis() - _Time >= 150000) return(false);

			// Increase Read Order
			_Buffer_Connect.Read_Order++;

		}

		// Control for Buffer
		for (uint8_t i = 0; i < _Buffer_Connect.Read_Order; i++) {

			// Handle Data
			if (((_Buffer_Connect_Variable[i] < 58 and _Buffer_Connect_Variable[i] > 47) or _Buffer_Connect_Variable[i] == 46)) {

				// Get Data
				Variables.IP_Address[_Buffer_Connect.Data_Order] = _Buffer_Connect_Variable[i];

				// Increase Data Order
				_Buffer_Connect.Data_Order++;

			}

		}

		if (sizeof(Variables.IP_Address) >= 7 and Modem.CREG_Status and Modem.CGREG_Status) {

			// Set Variable
			Modem.Connection_Status = true;

			// End Function
			return (true);

		} else {

			// Set Variable
			Modem.Connection_Status = false;

			// End Function
			return (false);

		}

	} else {

		// Declare Buffer Object
		Struct_Serial_Buffer _Buffer_DisConnect = {false, 0, 0, 150000};

		// Declare Buffer
		char _Buffer_DisConnectVariable[255];
		memset(_Buffer_DisConnectVariable, '\0', 255);

		// Read Current Time
		uint32_t _Time = millis();

		// Read UART Response
		while (!_Buffer_DisConnect.Response) {

			// Read Serial Char
			_Buffer_DisConnectVariable[_Buffer_DisConnect.Read_Order] = _GSM_Serial.read();

			// Control for Response
			if (strstr(_Buffer_DisConnectVariable, _OK_) != NULL) {

				// Handle Response
				_Buffer_DisConnect.Response = true;

			} else {

				// Buffer Read Delay
				delay(5);

			}

			// Handle for timeout
			if (millis() - _Time >= 1000) return(false);

			// Increase Read Order
			_Buffer_DisConnect.Read_Order++;

		}

		// End Function
		return(true);

	}

}
bool Telit_xE910::CGACT(const uint8_t _State, const bool _Cid) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Set = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Set_Variable[255];
	memset(_Buffer_Set_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+CGACT="));
	_GSM_Serial.print(String(_State));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Cid));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_Buffer_Set.Response) {

		// Read Serial Char
		_Buffer_Set_Variable[_Buffer_Set.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Set_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer_Set.Response = true;

		} else {

			// Buffer Read Delay
			delay(5);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Set.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Set.Read_Order++;

	}

	// Declare Watchdog Variables
	uint8_t _Error_WD = 0;

	// Define Control Variable
	bool _Control = false;

	// Control Loop
	while (!_Control) {

		// Clear UART Buffer
		_Clear_UART_Buffer();

		// Declare Buffer Object
		Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 5000};

		// Declare Buffer
		char _Buffer_Get_Variable[255];
		memset(_Buffer_Get_Variable, '\0', 255);

		// Send UART Command
		_GSM_Serial.print(F("AT+CGACT?"));
		_GSM_Serial.print(F("\r\n"));

		// Command Work Delay
		delay(15);

		// Read Current Time
		_Time = millis();

		// Read UART Response
		while (!_Buffer_Get.Response) {

			// Read Serial Char
			_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

			// Control for Response
			if (strstr(_Buffer_Get_Variable, _OK_) != NULL) {

				// Handle Response
				_Buffer_Get.Response = true;

			} else {

				// Buffer Read Delay
				delay(5);

			}

			// Handle for timeout
			if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

			// Increase Read Order
			_Buffer_Get.Read_Order++;

		}

		// Declare Status Variable
		uint8_t _State = _Buffer_Get_Variable[10];

//		Terminal.Text(40, 0, CYAN, String(_Error_WD));
//		Terminal.Text(41, 0, CYAN, String(_State));

		// Handle Stat Variable
		if (_State == 48) {

			// Set Control Variable
			_Control = false;

			// Count for Error
			_Error_WD++;

			// Wait Delay
			delay(2000);

		} // Deactivated [0]
		if (_State == 49) {

			// Set Control Variable
			_Control = true;

		} // Activated [1]

		// Handle for Error
		if (_Error_WD > 60) return(false);

	};

	// End Function
	return (true);

//	Terminal.Text(40, 0, CYAN, String(_Buffer_Variable));

}
bool Telit_xE910::CGPADDR(const uint8_t _Cid) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+CGPADDR="));
	_GSM_Serial.print(String(_Cid));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(5);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

//	Terminal.Text(40, 0, CYAN, String(_Buffer_Variable));

	// Clear Variables
	memset(Variables.IP_Address, '\0', 16);

	// Declare Handle Variable
	bool _Handle = false;

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle Data
		if (((_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) or _Buffer_Variable[i] == 46) and _Handle == true) {

			// Get Data
			Variables.IP_Address[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

		// Set Handle
		if (_Buffer_Variable[i] == 34) _Handle = true;

	}

	// End Function
	return(true);

}
bool Telit_xE910::ICMP(const uint8_t _Mode) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT#ICMP="));
	_GSM_Serial.print(String(_Mode));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::CSQ(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 3000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT+CSQ"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(50);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Clear Variables
	Variables.RSSI = 0;

	// Declare Response Data
	char _CSQ[2]; 

	// Declare Handle Data
	bool _Handle = true;

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle Data
		if (_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47 and _Handle == true) {

			// Get Data
			_CSQ[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

		// Set Handle Pointer
		if (_Buffer_Variable[i] == 44) _Handle = false;

	}

	// Set Signal Variable
	Variables.RSSI = atoi(_CSQ);

	// End Function
	return (true);

}
bool Telit_xE910::SERVINFO(void) {

	// Clear UART Buffer
	_Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SERVINFO"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(50);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Control Operator ID
	Variables.Operator = 0;														// 4 Operator
	if (strstr(_Buffer_Variable, "28601") != NULL) Variables.Operator = 28601;	// Turkcell
	if (strstr(_Buffer_Variable, "28602") != NULL) Variables.Operator = 28602;	// Vodafone
	if (strstr(_Buffer_Variable, "28603") != NULL) Variables.Operator = 28603;	// Turk Telecom
	if (strstr(_Buffer_Variable, "28604") != NULL) Variables.Operator = 28604;	// Turk Telecom

	// End Function
	return (true);

}

// Time Functions
bool Telit_xE910::CTZU(const bool _State) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+CTZU="));
	_GSM_Serial.print(String(_State));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(50);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

//	Terminal.Text(40, 0, CYAN, String(_Buffer_Variable));

	// End Function
	return (true);

}
bool Telit_xE910::NITZ(const bool _State) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT#NITZ="));
	_GSM_Serial.print(String(_State));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(50);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

//	Terminal.Text(40, 0, CYAN, String(_Buffer_Variable));

	// End Function
	return (true);

}
bool Telit_xE910::NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 10000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT#NTP=\""));
	_GSM_Serial.print(String(_NTP_Addr));
	_GSM_Serial.print(F("\","));
	_GSM_Serial.print(String(_NTP_Port));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Update_Module_Clock));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Time_Out));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(100);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Define Time Buffer Variables
	char _Serial_Time_Buffer[18];

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle Data
		if (_Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) {

			// Get Data
			_Serial_Time_Buffer[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

	}

	// Handle Time
	Time.Year = (((_Serial_Time_Buffer[0] - 48) * 10) + (_Serial_Time_Buffer[1] - 48));
	Time.Month = (((_Serial_Time_Buffer[2] - 48) * 10) + (_Serial_Time_Buffer[3] - 48));
	Time.Day = (((_Serial_Time_Buffer[4] - 48) * 10) + (_Serial_Time_Buffer[5] - 48));
	Time.Hour = (((_Serial_Time_Buffer[6] - 48) * 10) + (_Serial_Time_Buffer[7] - 48));
	Time.Minute = (((_Serial_Time_Buffer[8] - 48) * 10) + (_Serial_Time_Buffer[9] - 48));
	Time.Second = (((_Serial_Time_Buffer[10] - 48) * 10) + (_Serial_Time_Buffer[11] - 48));

	// Control for Variables
	if (Time.Year > 24 and Time.Year < 22) return(false);	
	if (Time.Month > 12 and Time.Month < 0) return(false);	
	if (Time.Day > 31 and Time.Day < 0) return(false);	
	if (Time.Hour > 24 and Time.Hour < 0) return(false);	
	if (Time.Minute > 59 and Time.Minute < 0) return(false);	
	if (Time.Second > 59 and Time.Second < 0) return(false);	

	// End Function
	return (true);

}
bool Telit_xE910::CCLK(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT+CCLK?"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(50);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Declare Buffer
	char _Time_Buffer[2];

	// Parse Year
	_Time_Buffer[0] = _Buffer_Variable[10];
	_Time_Buffer[1] = _Buffer_Variable[11];
	Time.Year = (uint8_t)atoi(_Time_Buffer);
	memset(_Time_Buffer, '\0', 2);

	// Parse Month
	_Time_Buffer[0] = _Buffer_Variable[13];
	_Time_Buffer[1] = _Buffer_Variable[14];
	if ((uint8_t)atoi(_Time_Buffer) > 12) Time.Month = 1;
	Time.Month = (uint8_t)atoi(_Time_Buffer);
	memset(_Time_Buffer, '\0', 2);

	// Parse Day
	_Time_Buffer[0] = _Buffer_Variable[16];
	_Time_Buffer[1] = _Buffer_Variable[17];
	if ((uint8_t)atoi(_Time_Buffer) > 31) Time.Day = 1;
	Time.Day = (uint8_t)atoi(_Time_Buffer);
	memset(_Time_Buffer, '\0', 2);

	// Parse Hour
	_Time_Buffer[0] = _Buffer_Variable[19];
	_Time_Buffer[1] = _Buffer_Variable[20];
	if ((uint8_t)atoi(_Time_Buffer) > 24) Time.Hour = 0;
	Time.Hour = (uint8_t)atoi(_Time_Buffer);
	memset(_Time_Buffer, '\0', 2);

	// Parse Minute
	_Time_Buffer[0] = _Buffer_Variable[22];
	_Time_Buffer[1] = _Buffer_Variable[23];
	if ((uint8_t)atoi(_Time_Buffer) > 60) Time.Minute = 0;
	Time.Minute = (uint8_t)atoi(_Time_Buffer);
	memset(_Time_Buffer, '\0', 2);

	// Parse Second
	_Time_Buffer[0] = _Buffer_Variable[25];
	_Time_Buffer[1] = _Buffer_Variable[26];
	if ((uint8_t)atoi(_Time_Buffer) > 60) Time.Second = 0;
	Time.Second = (uint8_t)atoi(_Time_Buffer);
	memset(_Time_Buffer, '\0', 2);

	// End Function
	return (true);

}

// Data Functions
bool Telit_xE910::FRWL(const uint8_t _Action, const char *_IP_Addr) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#FRWL="));
	_GSM_Serial.print(String(_Action));
	_GSM_Serial.print(F(",\""));
	_GSM_Serial.print(String(_IP_Addr));
	_GSM_Serial.print(F("\",\"255.255.255.0\""));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(50);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::HTTPCFG(const char *_HTTP_Server, const uint8_t _Port) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#HTTPCFG=1,\""));
	_GSM_Serial.print(String(_HTTP_Server));
	_GSM_Serial.print(F("\","));
	_GSM_Serial.print(String(_Port));
	_GSM_Serial.print(F(",0,\"\",\"\",0,20,1"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::HTTPSND(const char *_URL, const char *_Data) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Set = {false, 0, 0, 10000};

	// Declare Buffer
	char _Buffer_Set_Variable[255];
	memset(_Buffer_Set_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT#HTTPSND=1,0,\"")); 
	_GSM_Serial.print(String(_URL));
	_GSM_Serial.print(F("\","));
	_GSM_Serial.print(String(_Data).length());
	_GSM_Serial.print(F(",\"application/json\""));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer_Set.Response) {

		// Read Serial Char
		_Buffer_Set_Variable[_Buffer_Set.Read_Order] = _GSM_Serial.read();

//		Terminal.Text(50, _Buffer_Set.Read_Order, CYAN, String(_Buffer_Set_Variable[_Buffer_Set.Read_Order]));

		// Control for Response
		if (strstr(_Buffer_Set_Variable, _SEND_DATA_) != NULL) {

			// Handle Response
			_Buffer_Set.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Set.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Set.Read_Order++;

	}

	// Send Delay
	delay(50);

	// Print Data
	_GSM_Serial.print(String(_Data));

	// Send Delay
	delay(50);

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 10000};

	// Declare Buffer
	char _Buffer_Get_Variable[255];
	memset(_Buffer_Get_Variable, '\0', 255);

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_Buffer_Get.Response) {

		// Read Serial Char
		_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Get_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer_Get.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Get.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::HTTPRCV(void) {

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 10000};

	// Declare Buffer
	char _Buffer_Get_Variable[255];
	memset(_Buffer_Get_Variable, '\0', 255);

	// Read Current Time
	uint32_t _Time = millis();

	// Read UART Response
	while (!_Buffer_Get.Response) {

		// Read Serial Char
		_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Get_Variable, "HTTPRING") != NULL) {

			// Handle Response
			_Buffer_Get.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Get.Read_Order++;

	}

	// Command Work Delay
	delay(15);

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Read = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Read_Variable[255];
	memset(_Buffer_Read_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#HTTPRCV=1"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_Buffer_Read.Response) {

		// Read Serial Char
		_Buffer_Read_Variable[_Buffer_Read.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Read_Variable, _RECIEVE_DATA_) != NULL) {

			// Handle Response
			_Buffer_Read.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Read.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Read.Read_Order++;

	}

	// Command Work Delay
	delay(30);

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Data = {false, 0, 0, 15000};

	// Declare Buffer
	char _Buffer_Data_Variable[255];
	memset(_Buffer_Data_Variable, '\0', 255);

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_Buffer_Data.Response) {

		// Read Serial Char
		_Buffer_Data_Variable[_Buffer_Data.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Data_Variable, "\n") != NULL) {

			// Handle Response
			_Buffer_Data.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Data.Time_Out) return(0);

		// Increase Read Order
		_Buffer_Data.Read_Order++;

	}

	// Control for Command
	if (strstr(_Buffer_Data_Variable, _OK_RESPONSE_) != NULL) return (true);

	// End Function
	return (false);

}

// Socket Functions
bool Telit_xE910::SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SCFG="));
	_GSM_Serial.print(String(_Conn_ID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Cid));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Pkt_Sz));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Max_To));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Conn_To));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_TX_To));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode) {

	// <connId> - socket connection identifier (1..6)
	// <srMode> - SRing unsolicited mode
	// <recvDataMode> - data view mode for received data in command mode(AT#SRECV or <srMode> = 2)
	// <keepalive> - Set the TCP Keepalive value in minutes
	// <ListenAutoRsp> - Set the listen auto-response mode, that affects the commands AT#SL and AT#SLUDP
	// <sendDataMode> - data mode for sending data in command mode(AT#SSEND)

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SCFGEXT="));
	_GSM_Serial.print(String(_Conn_ID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Sr_Mode));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Recv_Data_Mode));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Keep_Alive));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Listen_Auto_Rsp));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Send_Data_Mode));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SCFGEXT2="));
	_GSM_Serial.print(String(_Conn_ID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Buffer_Start));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Abort_Conn_Attempt));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_SRing_Len));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_SRing_To));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_No_Carrier_Mode));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SCFGEXT3="));
	_GSM_Serial.print(String(_Conn_ID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Imm_Rsp));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Closure_Type_Cmd_Mode_Enabling));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SL(const uint8_t _ConnID, const bool _Listen_State, const uint16_t _Listen_Port, const uint8_t _Closure_Type) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SL="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Listen_State));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Listen_Port));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Closure_Type));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else if (strstr(_Buffer_Variable, "ERROR") != NULL) {

			// End Function
			return(false);

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
uint16_t Telit_xE910::SA(const uint8_t _ConnID, const uint8_t _ConnMode) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 15000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SA="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_ConnMode));
	_GSM_Serial.print(F("\r\n"));

	// Command Delay
	delay(20);

	// Read Current Time
	uint32_t _Time = millis();

	// Declare Control Variables
	bool _R = false;
	bool _N = false;

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, "SRING:") != NULL) {

			// Control for Char
			if (_Buffer_Variable[_Buffer.Read_Order] == '\r') _R = true;
			if (_Buffer_Variable[_Buffer.Read_Order] == '\n') _N = true;

			// Handle Response
			if (_R and _N) _Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Debug Message
//	Terminal.Text(30, 0, WHITE, String(F("AT#SA Response : "))); Terminal.Text(30, 17, CYAN, String(_Buffer_Variable));

	// Declare Handle Variable
	bool _Comma = false;

	// Declare Incomming Message Length Variable
	char _Incomming_Length[4];
	memset(_Incomming_Length, '\0', 4);

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle ,
		if (_Buffer_Variable[i] == 44) _Comma = true;

		// Handle Data
		if (_Comma == true and _Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) {

			// Get Data
			_Incomming_Length[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

	}

	// Debug Message
//	Terminal.Text(30, 0, WHITE, String(F("AT#SA Response : "))); Terminal.Text(30, 17, CYAN, String(_Buffer_Variable));
//	Terminal.Text(40, 0, YELLOW, String(_Incomming_Length));

	// End Function
	return(atoi(_Incomming_Length));

}
bool Telit_xE910::SH(const uint8_t _ConnID) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT#SH="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SO(const uint8_t _ConnID) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SO="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}
bool Telit_xE910::SD(const uint8_t _Cid, const uint8_t _Pro, const uint8_t _Port, const uint8_t _Closure_Type, uint16_t _IPort, const bool _Conn_Mode, const char *_IP, const char *_URL, const char *_Data) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Connect = {false, 0, 0, 10000};

	// Declare Buffer
	char _Buffer_Connect_Variable[255];
	memset(_Buffer_Connect_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SD="));
	_GSM_Serial.print(String(_Cid));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Pro));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Port));
	_GSM_Serial.print(F(",\""));
	_GSM_Serial.print(String(_IP));
	_GSM_Serial.print(F("\","));
	_GSM_Serial.print(String(_Closure_Type));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_IPort));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_Conn_Mode));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer_Connect.Response) {

		// Read Serial Char
		_Buffer_Connect_Variable[_Buffer_Connect.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Connect_Variable, "CONNECT") != NULL) {

			// Handle Response
			_Buffer_Connect.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Connect.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Connect.Read_Order++;

	}

	// Command Work Delay
	delay(30);

	// Print HTTP Header
	_GSM_Serial.print(F("POST ")); _GSM_Serial.print(String(_URL)); _GSM_Serial.print(F(" HTTP/1.1\r\n"));
	_GSM_Serial.print(F("Host: ")); _GSM_Serial.print(String(_IP)); _GSM_Serial.print(F("\r\n"));
	_GSM_Serial.print(F("Content-Length: ")); _GSM_Serial.print(String(_Data).length()); _GSM_Serial.print(F("\r\n"));
	_GSM_Serial.print(F("Connection: close\r\n"));
	_GSM_Serial.print(F("Content-Type: application/json\r\n"));
	_GSM_Serial.print(F("User-Agent: STF-PowerStat\r\n"));
	_GSM_Serial.print(F("\r\n"));

	// Send Data
	_GSM_Serial.print(String(_Data));

	// Command Work Delay
	delay(50);

	// Send Data
	_GSM_Serial.print(F("+++"));

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 10000};

	// Declare Buffer
	char _Buffer_Get_Variable[255];
	memset(_Buffer_Get_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Read Current Time
	_Time = millis();

	// Read UART Response
	while (!_Buffer_Get.Response) {

		// Read Serial Char
		_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Get_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer_Get.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Get.Read_Order++;

	}

	// End Function
	return (true);

}
uint8_t Telit_xE910::SS(const uint8_t _ConnID) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SS="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(9);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (_Buffer_Variable[9] - 48);

}
uint16_t Telit_xE910::SRECV(const uint8_t _ConnID, const uint16_t _MaxByte) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 5000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SRECV="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F(","));
	_GSM_Serial.print(String(_MaxByte));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(9);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Declare Handle Variable
	bool _Handle_Char = false;

	// Declare Incomming Message Length Variable
	char _Message_Buffer[4];

	// Control for Buffer
	for (uint8_t i = 0; i < _Buffer.Read_Order; i++) {

		// Handle ,
		if (_Buffer_Variable[i] == 123) _Handle_Char = true;

		// Handle Data
		if (_Handle_Char == true and _Buffer_Variable[i] < 58 and _Buffer_Variable[i] > 47) {

			// Get Data
			_Message_Buffer[_Buffer.Data_Order] = _Buffer_Variable[i];

			// Increase Data Order
			_Buffer.Data_Order++;

		}

	}

	// End Function
	return(atol(_Message_Buffer));

}
bool Telit_xE910::SSEND(const uint8_t _ConnID, const char * _Data_Pack) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 2000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Send UART Command
	_GSM_Serial.print(F("AT#SSEND="));
	_GSM_Serial.print(String(_ConnID));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(10);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, "\r\n>") != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// Command Delay
	delay(10);

	// Print HTTP Header
	_GSM_Serial.print(F("HTTP/1.1 200 OK\r\n"));
	_GSM_Serial.print(F("Connection: close\r\n"));
	_GSM_Serial.print(F("Content-Type: application/json\r\n"));
	_GSM_Serial.print(F("User-Agent: STF\r\n"));
	_GSM_Serial.print(F("\r\n"));

	// Send Data Pack
	_GSM_Serial.print(String(_Data_Pack));

	// Print End Char
	_GSM_Serial.print((char)26);

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer_Get = {false, 0, 0, 2000};

	// Declare Buffer
	char _Buffer_Get_Variable[255];
	memset(_Buffer_Get_Variable, '\0', 255);

	// Read UART Response
	while (!_Buffer_Get.Response) {

		// Read Serial Char
		_Buffer_Get_Variable[_Buffer_Get.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Get_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer_Get.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer_Get.Time_Out) return(false);

		// Increase Read Order
		_Buffer_Get.Read_Order++;

	}

	// End Function
	return(true);

}

// Hardware Functions
bool Telit_xE910::SHDN(void) {

	// Clear UART Buffer
    _Clear_UART_Buffer();

	// Declare Buffer Object
	Struct_Serial_Buffer _Buffer = {false, 0, 0, 1000};

	// Declare Buffer
	char _Buffer_Variable[255];
	memset(_Buffer_Variable, '\0', 255);

	// Command Delay
	delay(20);

	// Send UART Command
	_GSM_Serial.print(F("AT#SHDN"));
	_GSM_Serial.print(F("\r\n"));

	// Read Current Time
	uint32_t _Time = millis();

	// Command Delay
	delay(20);

	// Read UART Response
	while (!_Buffer.Response) {

		// Read Serial Char
		_Buffer_Variable[_Buffer.Read_Order] = _GSM_Serial.read();

		// Control for Response
		if (strstr(_Buffer_Variable, _OK_) != NULL) {

			// Handle Response
			_Buffer.Response = true;

		} else {

			// Buffer Read Delay
			delay(2);

		}

		// Handle for timeout
		if (millis() - _Time >= _Buffer.Time_Out) return(false);

		// Increase Read Order
		_Buffer.Read_Order++;

	}

	// End Function
	return (true);

}

// Tool Functions
void Telit_xE910::_Clear_UART_Buffer(void) {

	// Clear UART Buffer
	while (_GSM_Serial.available() > 0) {
        
        _GSM_Serial.read(); 
        delay(3);

    }

	// Command Chain Delay (Advice by Telit)
	delay(10);

}
uint8_t Telit_xE910::_Signal_Strength(uint16_t _CSQ) {

	// Declare RSSI Variable
	int16_t _RSSI = 0;

	// Calculate RSSI
	if (_CSQ == 0) _RSSI = -113;
	if (_CSQ == 1) _RSSI = -111;
	if (_CSQ >= 2 and _CSQ <= 30) _RSSI = -109 + ((_CSQ - 2) * 2);
	if (_CSQ >= 31) _RSSI = -51;

	// Calculate Signal Strength
	if (_RSSI >= -70) return(5);
	if (_RSSI < -70 and _RSSI >= -85) return(4);
	if (_RSSI < -85 and _RSSI >= -100) return(3);
	if (_RSSI < -100) return(2);
	if (_RSSI < -110) return(1);

	// End Function
	return(0);

}

// Define Library Class
Telit_xE910 GSM;

// 1903