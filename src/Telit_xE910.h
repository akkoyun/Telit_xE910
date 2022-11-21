#ifndef __Telit_xE910__
#define __Telit_xE910__

// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Define Library Includes
#ifndef __Telit_xE910_Includes__
	#include "Includes.h"
#endif

class Telit_xE910 {

	private:

	public:

		// Define Modem Status Structure
		struct Struct_Status {
			bool 		Initialize		 	= false;

			uint8_t 	Connection_Time		= 0;
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

			// IP Address Variable (xxx.xxx.xxx.xxx)
			char 		IP_Address[16];

			// Servinfo Variables
			uint16_t 	Operator			= 0;
			uint16_t 	BARFCN				= 0;
			uint16_t 	dBM					= 0;
			uint16_t 	BSIC				= 0;
			uint16_t 	TA					= 0;
			uint16_t 	GPRS				= 0;
			uint16_t	QUAL				= 0;
			char 		LAC[5];
			char 		Cell_ID[5];

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
				GSM_Modem.Status.Power = GSM_Modem.ON(Enable, Enable, Enable);

			} else {

				// ShutDown Modem
				GSM_Modem.Status.Power = GSM_Modem.OFF(Enable, Enable, Enable);

			}

			// End Function
			return(GSM_Modem.Status.Power);

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
			if (GSM_Modem.PowerMonitor()) {

				// Define Consol Row Variable
				uint8_t Console_Init_Row = GSM_Initialize_X;

				// AT Command
				#ifdef _AT_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT"));
						Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.AT();

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif

					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif
				
				// ATE Command
				#ifdef _AT_ATE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("ATE0"));
						Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.ATE(_AT_ATE_n_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// CMEE Command
				#ifdef _AT_CMEE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+CMEE=1"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CMEE(_AT_CMEE_n_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif

					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// FCLASS Command
				#ifdef _AT_FCLASS_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+FCLASS=0"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.FCLASS(_AT_FCLASS_n_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// K Command
				#ifdef _AT_K_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT&K0"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.K(_AT_K_n_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// CPIN Command
				#ifdef _AT_CPIN_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+CPIN"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// CGSN Command
				#ifdef _AT_CGSN_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+CGSN"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
						Terminal_GSM.Text(GSM_Detail_X + 4, GSM_Detail_Y + 22, CYAN, String(Modem.IMEI));
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// GSN Command
				#ifdef _AT_GSN_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+GSN"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
						Terminal_GSM.Text(GSM_Detail_X + 5, GSM_Detail_Y + 27, CYAN, String(Modem.Serial_ID));
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// ICCID Command
				#ifdef _AT_CCID_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+CCID"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
						Terminal_GSM.Text(GSM_Detail_X + 6, GSM_Detail_Y + 18, CYAN, String(Modem.ICCID));
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// GMI Command
				#ifdef _AT_GMI_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+GMI"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
						Terminal_GSM.Text(GSM_Detail_X + 1, GSM_Detail_Y + 36, CYAN, String(Modem.Manufacturer));
					#endif

					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// GMM Command
				#ifdef _AT_GMM_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+GMM"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
						Terminal_GSM.Text(GSM_Detail_X + 2, GSM_Detail_Y + 36, CYAN, String(Modem.Model));
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// GMR Command
				#ifdef _AT_GMR_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT+GMR"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
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
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
						Terminal_GSM.Text(GSM_Detail_X + 3, GSM_Detail_Y + 28, CYAN, String(Modem.Firmware));
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// SLED Command
				#ifdef _AT_SLED_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT#SLED=2"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.SLED(_AT_SLED_Mode_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// E2RI Command
				#ifdef _AT_E2RI_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT#E2RI=12,50"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.E2RI(_AT_E2RI_Parameter_event_mask_, _AT_E2RI_Parameter_duration_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// E2SLRI Command
				#ifdef _AT_E2SLRI_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT#E2SLRI=50"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.E2SLRI(_AT_E2SLRI_n_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// REGMODE Command
				#ifdef _AT_REGMODE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT#REGMODE=0"));
						Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.REGMODE(_AT_REGMODE_mode_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif

					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// TXMONMODE Command
				#ifdef _AT_TXMONMODE_

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y - 29, WHITE, F("AT#TXMONMODE=1"));
					Terminal_GSM.Text(Console_Init_Row, GSM_Initialize_Y, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.TXMONMODE(_AT_TXMONMODE_mode_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Console_Init_Row, GSM_Initialize_Y);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Init_Row += 1;

				#endif

				// Set Variable
				this->Status.Initialize = true;

			} else {

				// Power ON Modem
				this->Power(true);

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

			// Control for Initialization Monitor
			if (this->Status.Initialize) {

				// Define Consol Row Variable
				uint8_t Console_Connection_Row = GSM_Connection_X;

				// Set Variable
				this->Status.Connection = false;

				// COPS Command
				#ifdef _AT_COPS_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+COPS=0,2,28601"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

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

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// AUTOBND Command
				#ifdef _AT_AUTOBND_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT#AUTOBND=1"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.AUTOBND(_AT_AUTOBND_value);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// Get Time
				uint32_t _Connection_Start_Time = millis();

				// CREG Command
				#ifdef _AT_CREG_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CREG=0"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.Set_CREG(_AT_CREG_mode);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CREG? "));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, CYAN, F(" ** "));
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
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, CYAN, F("    "));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 34, CYAN, String(_CREG_Connection_Status));
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
							Terminal_GSM.Text(GSM_Connection_Detail_X + 1, GSM_Connection_Detail_Y + 33, CYAN, String((millis() - _Connection_Start_Time) / 1000));
						#endif

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 150) return(false);

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// CGREG Command
				#ifdef _AT_CGREG_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CGREG=0"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.Set_CGREG(_AT_CGREG_n_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CGREG? "));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, CYAN, F(" ** "));
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
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, CYAN, F("    "));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 34, CYAN, String(_CGREG_Connection_Status));
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
							Terminal_GSM.Text(GSM_Connection_Detail_X + 1, GSM_Connection_Detail_Y + 33, CYAN, String((millis() - _Connection_Start_Time) / 1000));
						#endif

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 50) return(false);

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// CGDCONT Command Socket
				#ifdef _AT_CGDCONT_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CGDCONT=1,\"IP\",\"mgbs\""));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.CGDCONT(_AT_CGDCONT_Parameter_cid_, _AT_CGDCONT_Parameter_pdp_, _AT_CGDCONT_Parameter_apn_, _AT_CGDCONT_Parameter_pdpaddr_, _AT_CGDCONT_Parameter_dcomp_, _AT_CGDCONT_Parameter_hcomp_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif
				
					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// SGACT Command
				#ifdef _AT_SGACT_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+SGACT=1,1"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.Set_SGACT(_AT_SGACT_Parameter_cid_, _AT_SGACT_Parameter_state_, this->Modem.IP_Address);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_Connection_Detail_X + 4, GSM_Connection_Detail_Y + 22, WHITE, String(this->Modem.IP_Address));
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// Set Connection Time
				this->Status.Connection_Time = ((millis() - _Connection_Start_Time) / 1000);

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(GSM_Connection_Detail_X + 1, GSM_Connection_Detail_Y + 33, CYAN, String((millis() - _Connection_Start_Time) / 1000));
				#endif

				// CSQ Command
				#ifdef _AT_CSQ_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CSQ"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

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
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						Terminal_GSM.Text(GSM_Connection_Detail_X + 2, GSM_Connection_Detail_Y + 35, WHITE, String(Signal_Strength(this->Modem.RSSI)));
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// FRWL Command
				#ifdef _AT_FRWL_

					// Define IP1
					#ifdef _AT_FRWL_IP1_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, "AT+FRWL=1," + String(_AT_FRWL_IP1_));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT.FRWL(1, _AT_FRWL_IP1_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						#endif

						// Handle Console Row
						Console_Connection_Row += 1;

						// End Function
						if (!_Response) return (false);

					#endif

					// Define IP2
					#ifdef _AT_FRWL_IP2_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, "AT+FRWL=1," + String(_AT_FRWL_IP2_));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT.FRWL(1, _AT_FRWL_IP2_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						#endif

						// Handle Console Row
						Console_Connection_Row += 1;

						// End Function
						if (!_Response) return (false);

					#endif

					// Define IP3
					#ifdef _AT_FRWL_IP3_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, "AT+FRWL=1," + String(_AT_FRWL_IP3_));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT.FRWL(1, _AT_FRWL_IP3_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						#endif

						// Handle Console Row
						Console_Connection_Row += 1;

						// End Function
						if (!_Response) return (false);

					#endif

					// Define IP4
					#ifdef _AT_FRWL_IP4_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, "AT+FRWL=1," + String(_AT_FRWL_IP4_));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT.FRWL(1, _AT_FRWL_IP4_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						#endif

						// Handle Console Row
						Console_Connection_Row += 1;

						// End Function
						if (!_Response) return (false);

					#endif

					// Define IP5
					#ifdef _AT_FRWL_IP5_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, "AT+FRWL=1," + String(_AT_FRWL_IP5_));
							Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT.FRWL(1, _AT_FRWL_IP5_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						#endif

						// Handle Console Row
						Console_Connection_Row += 1;

						// End Function
						if (!_Response) return (false);

					#endif

				#endif

				// ICMP Command
				#ifdef _AT_ICMP_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+ICMP=2"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.ICMP(_AT_ICMP_Parameter_mode_);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// End Function
					if (!_Response) return (false);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// Set MONI Command
				#ifdef _AT_MONI_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+MONI"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = AT.MONI(this->Modem.LAC, this->Modem.Cell_ID);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
						Terminal_GSM.Text(GSM_Connection_Detail_X + 5, GSM_Connection_Detail_Y + 32, WHITE, String(this->Modem.LAC));
						Terminal_GSM.Text(GSM_Connection_Detail_X + 6, GSM_Connection_Detail_Y + 32, WHITE, String(this->Modem.Cell_ID));
					#endif

					// End Function
					if (!_Response) return (false);

					// Handle Console Row
					Console_Connection_Row += 1;

				#endif

				// Set CCLK Command
				#ifdef _AT_CCLK_

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 2, WHITE, F("AT+CCLK"));
						Terminal_GSM.Text(Console_Connection_Row, GSM_Connection_Y + 33, BLUE, F(" .. "));
					#endif

					// Declare Watchdog Variable
					_Error_WD = 0;

					// Set Response Variable
					_Response = false;

					// Process Command
					while (!_Response) {

						// Send Command
						_Response = AT.CCLK(this->Time.Year, this->Time.Month, this->Time.Day, this->Time.Hour, this->Time.Minute, this->Time.Second);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.OK_Decide(_Response, Console_Connection_Row, GSM_Connection_Y + 33);
					#endif

					// End Function
					if (!_Response) return (false);

					// Set Variable
					this->Status.Time_Update = true;

				#endif

				// Set Variable
				this->Status.Connection = true;

			} else {

				// Initialise Modem
				this->Initialize();

			}

			// End Function
			return(this->Status.Connection);

		}

		/**
		 * @brief RSSI to Signal strength converter
		 * @param _CSQ RSSI
		 * @return uint8_t Signal Strength
		 */
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

		/**
		 * @brief Get Connection Details Function
		 */
		void Get_Connection_Detail(void) {

			// Declare Watchdog Variable
			uint8_t _Error_WD = 0;

			// Declare Response Status
			bool _Response = false;

			// CSQ Command
			#ifdef _AT_CSQ_

				// Declare Watchdog Variable
				_Error_WD = 0;

				// Set Response Variable
				_Response = false;

				// Clear RSSI
				this->Modem.RSSI = 0;

				// Process Command
				while (!_Response) {

					// Send Command
					_Response = AT.CSQ(this->Modem.RSSI);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(23, 115, CYAN, String(this->Modem.RSSI));
				#endif

			#endif
			
			// SERVINFO Command Socket
			#ifdef _AT_SERVINFO_

				// Declare Watchdog Variable
				_Error_WD = 0;

				// Set Response Variable
				_Response = false;

				// Process Command
				while (!_Response) {

					// Process Command
					_Response = AT.SERVINFO(this->Modem.Operator, this->Modem.BARFCN, this->Modem.dBM, this->Modem.BSIC, this->Modem.TA, this->Modem.GPRS, this->Modem.LAC);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(24, 112, CYAN, String(this->Modem.Operator));
				#endif

			#endif

			// MONI Command Socket
			#ifdef _AT_MONI_

				// Declare Watchdog Variable
				_Error_WD = 0;

				// Set Response Variable
				_Response = false;

				// Process Command
				while (!_Response) {

					// Process Command
					_Response = AT.MONI(this->Modem.LAC, this->Modem.Cell_ID);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

			#endif

			// Print Command State
			#ifdef GSM_Debug
				Terminal_GSM.Text(23, 115, CYAN, String(this->Modem.RSSI));
			#endif

		}

};

#endif /* defined(__Telit_xE910__) */