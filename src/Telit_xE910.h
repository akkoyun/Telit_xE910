/* *******************************************************************************
 *  Copyright (C) 2014-2022 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: Telit xE910 GSM Library.
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *
 *********************************************************************************/

#ifndef __Telit_xE910__
#define __Telit_xE910__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define AT Definition
#ifndef __AT_Definition__
#include "Definition.h"
#endif

// Define Objects
Modem_Hardware Hardware;
AT_Command_Set AT(Serial_GSM);

class Telit_xE910{

	private:

		// Define Modem Status Structure
		struct Struct_Status {
			bool 		Power 				= false;
			uint8_t 	Connection_Time		= 0;
			bool 		Initialize		 	= false;
			bool		Connection			= false;
			bool		Socket_Config		= false;
			bool		Time_Update			= false;
			uint8_t		Socket				= 0;
		} Status;

		// Define Modem Structure
		struct Struct_Modem {
			uint8_t 	PIN 				= 0;
			char 		IMEI[17];
			char 		Serial_ID[11]; 
			char 		ICCID[21];
			uint8_t 	Manufacturer 		= 0;
			uint8_t 	Model 				= 0;
			char 		Firmware[10];
			uint8_t 	RSSI				= 0;
			uint16_t 	Operator			= 0;
			char 		IP_Address[16];
		} Modem;

		// Define Time Structure
		struct Struct_Time {
			uint8_t 	Year				= 0;
			uint8_t 	Month				= 0;
			uint8_t 	Day					= 0;
			uint8_t 	Hour				= 0;
			uint8_t 	Minute				= 0;
			uint8_t 	Second				= 0;
		} Time;

	public:

		/**
		 * @brief Construct a new Telit_xE910 object
		 */
		Telit_xE910(void) {
			
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
				this->Status.Power = Hardware.ON(true, true, true);

			} else {

				// ShutDown Modem
				this->Status.Power = Hardware.OFF(true, true, true);

			}

			// End Function
			return(this->Status.Power);

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
			if (Hardware.PowerMonitor()) {

				// AT Command
				#ifdef _AT_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.AT();

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}
				
					// End Function
					if (!_Response) return (false);

				#endif
				
				// ATE Command
				#ifdef _AT_ATE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Initialize_X, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.ATE(_AT_ATE_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// CMEE Command
				#ifdef _AT_CMEE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 1, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CMEE(_AT_CMEE_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 1, Debug_Initialize_Y);
					#endif

					// End Function
					if (!_Response) return (false);

				#endif

				// FCLASS Command
				#ifdef _AT_FCLASS_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 2, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.FCLASS(_AT_FCLASS_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 2, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// K Command
				#ifdef _AT_K_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 3, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.K(_AT_K_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 3, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// CPIN Command
				#ifdef _AT_CPIN_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 4, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CPIN(this->Modem.PIN);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 4, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// CGSN Command
				#ifdef _AT_CGSN_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 5, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CGSN(this->Modem.IMEI);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X + 5, Debug_Initialize_Y);
						Terminal.Text(8, 102, CYAN, String(Modem.IMEI));
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// GSN Command
				#ifdef _AT_GSN_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 6, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.GSN(this->Modem.Serial_ID);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X + 6, Debug_Initialize_Y);
						Terminal.Text(9, 107, CYAN, String(Modem.Serial_ID));
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// ICCID Command
				#ifdef _AT_ICCID_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 7, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CCID(this->Modem.ICCID);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X + 7, Debug_Initialize_Y);
						Terminal.Text(10, 98, CYAN, String(Modem.ICCID));
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// GMI Command
				#ifdef _AT_GMI_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 8, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.GMI(this->Modem.Manufacturer);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X + 8, Debug_Initialize_Y);
						Terminal.Text(5, 116, CYAN, String(Modem.Manufacturer));
					#endif

					// End Function
					if (!_Response) return (false);

				#endif

				// GMM Command
				#ifdef _AT_GMM_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 9, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.GMM(this->Modem.Model);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X + 9, Debug_Initialize_Y);
						Terminal.Text(6, 116, CYAN, String(Modem.Model));
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// GMR Command
				#ifdef _AT_GMR_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 10, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.GMR(this->Modem.Firmware);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Initialize_X + 10, Debug_Initialize_Y);
						Terminal.Text(7, 108, CYAN, String(Modem.Firmware));
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// SLED Command
				#ifdef _AT_SLED_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 11, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.SLED(_AT_SLED_Parameter);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 11, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// E2RI Command
				#ifdef _AT_E2RI_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 12, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.E2RI(_AT_E2RI_Parameter_Mask_, _AT_E2RI_Parameter_Duration_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 12, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// E2SLRI Command
				#ifdef _AT_E2SLRI_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Initialize_X + 12, Debug_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.E2SLRI(_AT_E2SLRI_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Initialize_X + 12, Debug_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// REGMODE Command
				#ifdef _AT_REGMODE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.REGMODE(_AT_REGMODE_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Connect_X, Debug_Connect_Y);
					#endif

					// End Function
					if (!_Response) return (false);
					
				#endif

				// TXMONMODE Command
				#ifdef _AT_TXMONMODE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal.Text(Debug_Connect_X + 1, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.TXMONMODE(_AT_TXMONMODE_Parameter_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Connect_X + 1, Debug_Connect_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);
					
				#endif

				// Set Variable
				this->Status.Initialize = true;

			}

			// End Function
			return(this->Status.Initialize);
			
		}

		/**
		 * @brief Connect GSM Modem
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool Connect(void) {

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

				// COPS Command
				#ifdef _AT_COPS_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.COPS(0, 2, 28601);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

				#endif

				// AUTOBND Command
				#ifdef _AT_AUTOBND_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.AUTOBND(_AT_AUTOBND_Parameter);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

				#endif

				// CREG Command
				#ifdef _AT_CREG_

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 2, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.Set_CREG(_AT_CREG_Parameter);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 2, Debug_Connect_Y, CYAN, F(" ** "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Declare Response Status
					_Response = false;

					// Process Command
					while (!_Response) {

						// Declare Variable
						uint8_t _CREG_Connection_Status = 0;

						// Get CREG Status
						AT.Get_CREG(_CREG_Connection_Status);

						// Print Command State
						#ifdef GSM_Debug
							Terminal.Text(Debug_Connect_X + 2, Debug_Connect_Y, CYAN, F("    "));
							Terminal.Text(Debug_Connect_X + 2, Debug_Connect_Y + 1, CYAN, String(_CREG_Connection_Status));
						#endif

						// Control for Connection
						if (_CREG_Connection_Status == 1 or _CREG_Connection_Status == 5) {
							
							// Declare Response Status
							_Response = true;

						} else {

							// Wait Delay
							if (_CREG_Connection_Status == 2) delay(1000);
							if (_CREG_Connection_Status == 3) delay(10000);

							// Declare Response Status
							_Response = false;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal.Text(13, 113, CYAN, String((millis() - _Connection_Start_Time) / 1000));
						#endif

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 150) return(false);

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Connect_X + 2, Debug_Connect_Y);
					#endif

				#endif

				// CGREG Command
				#ifdef _AT_CGREG_

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 3, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.Set_CGREG(_AT_CGREG_Parameter);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 3, Debug_Connect_Y, CYAN, F(" ** "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Declare Response Status
					_Response = false;

					// Process Command
					while (!_Response) {

						// Declare Variable
						uint8_t _CGREG_Connection_Status = 0;

						// Get CREG Status
						AT.Get_CGREG(_CGREG_Connection_Status);

						// Print Command State
						#ifdef GSM_Debug
							Terminal.Text(Debug_Connect_X + 3, Debug_Connect_Y, CYAN, F("    "));
							Terminal.Text(Debug_Connect_X + 3, Debug_Connect_Y + 1, CYAN, String(_CGREG_Connection_Status));
						#endif

						// Control for Connection
						if (_CGREG_Connection_Status == 1 or _CGREG_Connection_Status == 5) {
							
							// Declare Response Status
							_Response = true;

						} else {

							// Wait Delay
							if (_CGREG_Connection_Status == 2) delay(1000);
							if (_CGREG_Connection_Status == 3) delay(10000);

							// Declare Response Status
							_Response = false;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal.Text(13, 113, CYAN, String((millis() - _Connection_Start_Time) / 1000));
						#endif

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 50) return(false);

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal.OK_Decide(_Response, Debug_Connect_X + 3, Debug_Connect_Y);
					#endif

				#endif

				// CGDCONT Command Socket
				#ifdef _AT_CGDCONT_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 7, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.CGDCONT(_AT_CGDCONT_Parameter_Cid_, _AT_CGDCONT_Parameter_PDP_, _AT_CGDCONT_Parameter_APN_, _AT_CGDCONT_Parameter_PDP_Addr_, _AT_CGDCONT_Parameter_DComp_, _AT_CGDCONT_Parameter_HComp_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Connect_X + 7, Debug_Connect_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// SGACT Command
				#ifdef _AT_SGACT_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 9, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.Set_SGACT(_AT_SGACT_Parameter_Cid_, _AT_SGACT_Parameter_State_, this->Modem.IP_Address);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(16, 102, CYAN, String(this->Modem.IP_Address));
						Terminal.OK_Decide(_Response, Debug_Connect_X + 9, Debug_Connect_Y);
					#endif

				#endif

				// SERVINFO Command Socket
				#ifdef _AT_SERVINFO_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 8, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.SERVINFO(this->Modem.Operator);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Connect_X + 8, Debug_Connect_Y);
						Terminal.Text(15, 112, CYAN, String(this->Modem.Operator));
					#endif
				
					// End Function
					if (!_Response) return (false);

				#endif

				// CSQ Command
				#ifdef _AT_CSQ_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CSQ(this->Modem.RSSI);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(14, 115, CYAN, String(this->Modem.RSSI));
					#endif

				#endif

				// FRWL Command
				#ifdef _AT_FRWL_

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 12, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.FRWL(1, "213.14.250.214");

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.FRWL(1, "83.160.73.106");

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.FRWL(1, "77.175.255.33");

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Connect_X + 12, Debug_Connect_Y);
					#endif

				#endif

				// ICMP Command
				#ifdef _AT_ICMP_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal.Text(Debug_Connect_X + 11, Debug_Connect_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.ICMP(_AT_ICMP_Parameter_Mode_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal.OK_Decide(_Response, Debug_Connect_X + 11, Debug_Connect_Y);
					#endif

				#endif

				// Set Connection Time
				this->Status.Connection_Time = ((millis() - _Connection_Start_Time) / 1000);

				// Print Command State
				#ifdef GSM_Debug
					Terminal.Text(13, 113, CYAN, String(this->Status.Connection_Time));
				#endif

				// Set Variable
				this->Status.Connection = true;

			}

			// End Function
			return(this->Status.Connection);

		}

		/**
		 * @brief Get Time Variables from GSM
		 * @return true Function is success.
		 * @return false Function fail.
		 */		
		bool Time_Update(void) {

			// Declare Watchdog Variable
			uint8_t _Error_WD = 0;

			// Declare Response Status
			bool _Response = false;

			// Control for Initialization Monitor
			if (this->Status.Connection) {

				// Process Command
				while (!_Response) {

					// Send Command
//					_Response = AT.NTP(_AT_NTP_Server_, _AT_NTP_Port_, true, _AT_NTP_TO_, this->Time.Year, this->Time.Month, this->Time.Day, this->Time.Hour, this->Time.Minute, this->Time.Second);
					_Response = AT.CCLK(this->Time.Year, this->Time.Month, this->Time.Day, this->Time.Hour, this->Time.Minute, this->Time.Second);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// Print Command State
				#ifdef GSM_Debug

					// Handle TimeStamp
					char _Response_Time_Array[21];	// 2022-03-24  10:36:00
					sprintf(_Response_Time_Array, "20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu", this->Time.Year, this->Time.Month, this->Time.Day, this->Time.Hour, this->Time.Minute, this->Time.Second);

					Terminal.Text(2, 98, CYAN, String(_Response_Time_Array));

				#endif

				// End Function
				if (!_Response) return (false);

				// Set Variable
				this->Status.Time_Update = true;

				// End Function
				return(this->Status.Time_Update);

			}

			// End Function
			return(true);

		}

		bool Socket_Config(const uint8_t _Cid) {

			// Declare Watchdog Variable
			uint8_t _Error_WD = 0;

			// Declare Response Status
			bool _Response = false;

			// Control for Initialization Monitor
			if (this->Status.Connection) {

				// Handle Socket ID
				if (_Cid == 2) {

					// SCFG Command Socket 2
					#ifdef _SCFG_2_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal.Text(Debug_Connect_X + 5, Debug_Connect_Y, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = AT.SCFG(2, _AT_SCFG2_Cid_, _AT_SCFG2_PktSz_, _AT_SCFG2_MaxTo_, _AT_SCFG2_ConnTo_, _AT_SCFG2_TXTo_);

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
							_Response = AT.SCFGEXT(2, _AT_SCFGEXT2_SrMode_, _AT_SCFGEXT2_RcvDataMode_, _AT_SCFGEXT2_KeepAlive_, _AT_SCFGEXT2_AutoResp_, _AT_SCFGEXT2_SndDataMode_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}
			
						// Print Command State
						#ifdef GSM_Debug
							Terminal.OK_Decide(_Response, Debug_Connect_X + 5, Debug_Connect_Y);
						#endif
					
						// End Function
						if (!_Response) return (false);

					#endif

				} else if (_Cid == 3) {

					// SCFG Command Socket 3
					#ifdef _SCFG_3_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal.Text(Debug_Connect_X + 5, Debug_Connect_Y, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = AT.SCFG(3, _AT_SCFG3_Cid_, _AT_SCFG3_PktSz_, _AT_SCFG3_MaxTo_, _AT_SCFG3_ConnTo_, _AT_SCFG3_TXTo_);

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
							_Response = AT.SCFGEXT(3, _AT_SCFGEXT3_SrMode_, _AT_SCFGEXT3_RcvDataMode_, _AT_SCFGEXT3_KeepAlive_, _AT_SCFGEXT3_AutoResp_, _AT_SCFGEXT3_SndDataMode_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}
			
						// Print Command State
						#ifdef GSM_Debug
							Terminal.OK_Decide(_Response, Debug_Connect_X + 5, Debug_Connect_Y);
						#endif
					
						// End Function
						if (!_Response) return (false);

					#endif

				}

				// End Function
				return(true);

			}

			// End Function
			return(false);

		}










		bool Socket_Send_Pack(const uint8_t _Conn_ID, const char *_IP, const char *_URL, const char *_Data, char *_Response) {

			// Declare Status Variable
			uint8_t Socket_Status;

			// Open Connection
			if (AT.SD(_Conn_ID, 0, 80, 0, 88, 1, _IP)) {

				// Send Data Pack
				AT.SSEND(_Conn_ID, 2, _IP, _URL, _Data);

				// Declare Ring Status
				uint8_t Ring_ID;
				uint16_t Length;

				// Get Ring Port
				if (AT.Send_SRING(Ring_ID, Length)) {

					// Get Request Data
					AT.SRECV(Ring_ID, Length, _Response);

					// Close Socket
					AT.SH(Ring_ID);

					// End Function
					return(true);
					
				}

				// End Function
				return(false);

			}

			// End Function
			return(false);

		}



		uint16_t Handle_JSON_Send_Response(const char *_Data) {

			// Declare JSON Object
			StaticJsonDocument<40> Incomming_JSON;

			// Deserialize the JSON document
			deserializeJson(Incomming_JSON, _Data);

			// Fetch values.
			uint16_t Event = Incomming_JSON["Event"];

			// End Function
			return(Event);

		}
		uint16_t Handle_JSON_Request(const char *_Data) {

			// Declare JSON Object
			StaticJsonDocument<40> Incomming_JSON;

			// Deserialize the JSON document
			deserializeJson(Incomming_JSON, _Data);

			// Fetch values.
			uint16_t Event = Incomming_JSON["Request"]["Event"];

			// End Function
			return(Event);

		}



		bool Socket(const uint8_t _ConnID, const bool _State, const uint8_t _Port) {

			// Control for Initialization Monitor
			if (this->Status.Connection) {

			
				// Get Socket State
				AT.SS(_ConnID, this->Status.Socket);

				// Handle State
				if (_State) {

					// Control Current State
					if (this->Status.Socket != 4) AT.SL(_ConnID, 1, _Port, 255);

					// Command Delay
					delay(20);

					// Get Socket State
					AT.SS(_ConnID, this->Status.Socket);

					// Command Delay
					delay(20);

					// Control Socket
					if (this->Status.Socket != 4) return(false);

				} else {

					// Control Current State
					if (this->Status.Socket != 0) AT.SL(_ConnID, 0, _Port, 255);

					// Command Delay
					delay(20);

					// Get Socket State
					AT.SS(_ConnID, this->Status.Socket);

					// Command Delay
					delay(20);

					// Control Socket
					if (this->Status.Socket != 0) return(false);

				}

				// Print Command State
				#ifdef GSM_Debug
					Terminal.Text(17, 108, CYAN, "         ");
					if (this->Status.Socket == 0) Terminal.Text(17, 108, CYAN, "Closed   ");
					if (this->Status.Socket == 1) Terminal.Text(17, 108, CYAN, "Transfer ");
					if (this->Status.Socket == 2) Terminal.Text(17, 108, CYAN, "Suspend  ");
					if (this->Status.Socket == 3) Terminal.Text(17, 108, CYAN, "Suspend  ");
					if (this->Status.Socket == 4) Terminal.Text(17, 108, CYAN, "Listening");
					if (this->Status.Socket == 5) Terminal.Text(17, 108, CYAN, "Incomming");
					if (this->Status.Socket == 6) Terminal.Text(17, 108, CYAN, "DNS      ");
					if (this->Status.Socket == 7) Terminal.Text(17, 108, CYAN, "Conecting");
				#endif

				// End Function
				return(true);

			}

			// End Function
			return(false);

		}
		bool Socket_Answer(char * _JSON_Data) {

			// Declare RING
			uint8_t RING;
			
			// Get Ring Port
			AT.Recieve_SRING(RING);

			// Control for <SRING:n>
			if (RING == 1 or RING == 2 or RING == 3) {

				// Declare Request Length
				uint16_t Request_Length;

				// Answer Socket
				AT.SA(RING, 1, Request_Length);

				// Get Request Data
				AT.SRECV(RING, Request_Length, _JSON_Data);

				// End Function
				return(true);

			}

			// End Function
			return(false);

		}
		bool Socket_Send_Response(const uint8_t _ConnID, char * _JSON_Data) {

			// Send Socket Answer
			if (AT.SSEND(_ConnID, 1, "", "", _JSON_Data)) {

				// Command Delay
				delay(20);

				// Close Socket
				if (AT.SH(_ConnID)) {

					// Command Delay
					delay(20);

					// ReOpen Socket
					bool Socket_Open = this->Socket(_ConnID, true, 80);

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
		uint8_t Socket_Status(const uint8_t _Conn_ID) {

			// Declare Status Variable
			uint8_t Socket_Status;

			// Get Socket Status
			AT.SS(_Conn_ID, Socket_Status);

			// End Function
			return(Socket_Status);

		}



		uint8_t Signal_Strength(uint16_t _CSQ) {

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

};

#endif /* defined(__Telit_xE910__) */