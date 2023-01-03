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

	// Define Library Includes
	#ifndef __Telit_AT_Command_Set__
		#include "AT_Commands/AT_Command_Set.h"
	#endif

	// Define JSON Handler
	#include <ArduinoJson.h>

	// Cloud Functions
	class PostOffice : public AT_Command_Set {

		// Private Functions
		private:

			// Define Modem Status Structure
			struct Hardware_Status {
				bool Power = false;
			} Hardware_Status;

			/**
			 * @brief Get Power Monitor
			 * @return true Modem Powered
			 * @return false Modem is not Powered
			 */
			bool PowerMonitor(void) {

				// Control for PWMon (PJ3)
				if ((PINJ & (1 << PINJ3)) == (1 << PINJ3)) {

					// Response Delay
					delay(10);

					// Power Monitor 3V3 HIGH
					return (true);

				} else {

					// Response Delay
					delay(10);

					// Power Monitor 3V3 LOW
					return (false);

				}

			}

			/**
			 * @brief Enable Communication Buffer.
			 * @param _Function 
			 * _GET_ (0) : Get Buffer State.
			 * _SET_ (1) : Set Buffer State.
			 * @param _State 
			 * true : Enable Buffer.
			 * false : Disable Buffer.
			 * @return true Buffer is Enabled.
			 * @return false Buffer is Disabled.
			 */
			void Communication(const bool _State) {

				// Enable Communication 
				if (_State) PORTJ &= 0b11101111;

				// Disable Communication
				if (!_State) PORTJ |= 0b00010000;

			}

			/**
			 * @brief On or Off Modem.
			 * @param _Time On/Off Time
			 */
			void OnOff(const uint16_t _Time) {

				// Set On/Off Signal HIGH [PJ6]
				PORTJ |= 0b01000000;

				// Command Delay
				for (uint8_t i = 0; i < 100; i++) {

					// Calculate Delay (2000)
					uint8_t _Delay = _Time / 100;

					// Terminal Bar
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_Console_Boot_X, GSM_Console_Boot_Y + i, WHITE, F("▒"));
					#endif

					// Wait
					delay(_Delay); 

				}

				// Set On/Off Signal LOW [PJ6]
				PORTJ &= 0b10111111;

				// Clear Bar
				#ifdef GSM_Debug
					for (uint8_t i = 0; i < 100; i++) Terminal_GSM.Text(GSM_Console_Boot_X, GSM_Console_Boot_Y + i, WHITE, F(" "));
				#endif

			}

			/**
			 * @brief ShutDown Modem
			 * @param _Time ShutDown Time
			 */
			void ShutDown(const uint16_t _Time) {

				// Set Shut Down Signal HIGH [PJ5]
				PORTJ |= 0b00100000;

				// Command Delay
				delay(_Time);

				// Set Shut Down Signal LOW [PJ5]
				PORTJ &= 0b11011111;

			}

			/**
			 * @brief Power Switch
			 * @param _State Switch State
			 */
			void Power_Switch(const bool _State) {

				// Set GSM Power Enable
				if (_State) PORTH |= 0b00000100;

				// Set GSM Power Disable
				if (!_State) PORTH &= 0b11111011;
			
			}

			/**
			 * @brief LED Power
			 * @param _State LED State
			 */
			void LED(const bool _State) {

				// Set GSM LED Power Enable
				if (_State) PORTH &= 0b11101111;

				// Set GSM LED Power Disable
				if (!_State) PORTH |= 0b00010000;

			}

			/**
			 * @brief Power on Sequence of Modem
			 * @param _Power_Switch Power Switch State
			 * @param _LED_Switch  LED Stat
			 * @param _Communication_Switch Communication State
			 * @return true Modem is ON
			 * @return false Modem is OFF
			 */
			bool ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

				// Send Shut Down Signal
//				this->ShutDown(500);

				// Enable GSM Modem Power Switch
				if (_Power_Switch) this->Power_Switch(true);  
				
				// Enable GSM Modem LED Feed
				if (_LED_Switch) this->LED(true);

				// Set Communication Signal LOW
				if (_Communication_Switch) this->Communication(true);
				
				// Turn On Modem
				if (this->PowerMonitor()) {

					// End Function
					return (true);

				} else {

					// Send On Off Signal
					this->OnOff(5000);

					// Control for PWMon (PH7)
					if (this->PowerMonitor()) {

						// End Function
						return(true);

					} else {

						// Send Shut Down Signal
						this->ShutDown(200);

					}

				}

				// End Function
				return (false);

			}

			/**
			 * @brief Power on Sequence of Modem
			 * @param _Power_Switch Power Switch State
			 * @param _LED_Switch  LED Stat
			 * @param _Communication_Switch Communication State
			 * @return true Modem is OFF
			 * @return false Modem is ON
			 */
			bool OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

				// Turn Off Modem
				if (this->PowerMonitor()) {

					// Turn Off Modem
					this->OnOff(3000);

					// Set Variable
					bool _Power = true;

					// Read Current Time
					const uint32_t _Current_Time = millis();

					// Control for Power Monitor
					while (_Power) {

						// Control for PowerMonitor
						if (!this->PowerMonitor()) {

							// Set Variable
							_Power = false;

							// Set Status Variable
							this->Hardware_Status.Power = false;

							// Disable GSM LED Power
							if (_LED_Switch) this->LED(false);

							// Disable GSM Modem Voltage Translator
							if (_Communication_Switch) this->Communication(false);

							// Disable GSM Modem Main Power Switch
							if (_Power_Switch) this->Power_Switch(false);

						}

						// Handle for timeout
						if (millis() - _Current_Time >= 15000) return(false);

					}
					
				} else {

					// Set Status Variable
					this->Hardware_Status.Power = false;

					// Disable GSM LED Power
					if (_LED_Switch) this->LED(false);

					// Disable GSM Modem Voltage Translator
					if (_Communication_Switch) this->Communication(false);

					// Disable GSM Modem Main Power Switch
					if (_Power_Switch) this->Power_Switch(false);

				}

				// Boot Delay
				delay(1000);

				// End Function
				return (true);

			}

			// Define Modem Status Structure
			struct Struct_Status {
				bool		PowerMon			= false;
				bool 		Initialize		 	= false;
				bool		Connection			= false;
			} Status;

			// Define JSON Status Structure
			struct JSON_Device_Structure {

				// Define JSON Status Structure
				struct JSON_Info_Structure {
					char * Device_ID;
					float Temperature;
					float Humidity;
				} JSON_Info;

				// Define JSON Battery Structure
				struct JSON_Battery_Structure {
					float IV;
					float AC;
					float SOC;
					uint8_t Charge;
					float T;
					uint16_t FB;
					uint16_t IB;
				} JSON_Battery;

				// Define JSON Status Structure
				struct JSON_Status_Structure {
					uint16_t Device;
					uint16_t Fault;
				} JSON_Status;

				// Time Stamp
				char * Time_Stamp;

			} JSON_Data;

			// Define JSON
			String JSON_Pack;

			// Parse JSON Pack
			uint16_t Parse_JSON(uint8_t _Pack_Type) {

				// Clear Pack
				this->JSON_Pack = "";

				// Define Versions
				#ifndef __Hardware__
					#define __Hardware__ "00.00.00"
				#endif
				#ifndef __Firmware__
					#define __Firmware__ "00.00.00"
				#endif

				// Define JSON
				StaticJsonDocument<1024> JSON;

				// Set Device ID Variable
				if (_Pack_Type == Pack_Online) JSON[F("Command")] = F("STF:PowerStat.Online");
				if (_Pack_Type == Pack_Timed) JSON[F("Command")] = F("STF:PowerStat.Timed");
				if (_Pack_Type == Pack_Interrupt) JSON[F("Command")] = F("STF:PowerStat.Interrupt");
				if (_Pack_Type == Pack_Alarm) JSON[F("Command")] = F("STF:PowerStat.Alarm");
				if (_Pack_Type == Pack_Offline) JSON[F("Command")] = F("STF:PowerStat.Offline");

				// Define Device Section
				JsonObject JSON_Device = JSON.createNestedObject(F("Device"));

				// Define Device Section
				JsonObject JSON_Info= JSON_Device.createNestedObject(F("Info"));

				// Set Device ID Variable
				JSON_Info[F("ID")] = this->JSON_Data.JSON_Info.Device_ID;
				
				// Set Device Hardware Version Variable
				if (_Pack_Type == Pack_Online) JSON_Info[F("Hardware")] = F(__Hardware__);

				// Set Device Firmware Version Variable
				if (_Pack_Type == Pack_Online) JSON_Info[F("Firmware")] = F(__Firmware__);

				// Set Device Environment Variable
				JSON_Info[F("Temperature")] = this->JSON_Data.JSON_Info.Temperature;
				JSON_Info[F("Humidity")] = this->JSON_Data.JSON_Info.Humidity;

				// Define Power Section
				JsonObject JSON_Battery = JSON_Device["Power"].createNestedObject("Battery");

				// Set Battery Variables
				JSON_Battery[F("IV")] = this->JSON_Data.JSON_Battery.IV;
				JSON_Battery[F("AC")] = this->JSON_Data.JSON_Battery.AC;
				JSON_Battery[F("SOC")] = this->JSON_Data.JSON_Battery.SOC;
				JSON_Battery[F("Charge")] = this->JSON_Data.JSON_Battery.Charge;
				if (_Pack_Type == Pack_Online) JSON_Battery[F("T")] = this->JSON_Data.JSON_Battery.T;
				if (_Pack_Type == Pack_Online) JSON_Battery[F("FB")] = this->JSON_Data.JSON_Battery.FB;
				if (_Pack_Type == Pack_Online) JSON_Battery[F("IB")] = this->JSON_Data.JSON_Battery.IB;

				// Set MONI Command
				#ifdef _AT_MONIZIP_

					// Declare Watchdog Variable
					uint8_t _Error_WD = 0;

					// Set Response Variable
					bool _Response = false;

					// Process Command
					while (!_Response) {

						// Process Command
						_Response = MONIZIP(Modem.Operator, Modem.LAC, Modem.Cell_ID, Modem.dBm);

						// Set WD Variable
						_Error_WD++;

						// Control for WD
						if (_Error_WD > 5) break;

					}

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_CellLAC_X, GSM_CellLAC_Y, CYAN, String(Modem.LAC));
						Terminal_GSM.Text(GSM_CellID_X, GSM_CellID_Y, CYAN, String(Modem.Cell_ID));
						Terminal_GSM.Text(GSM_Operator_X, GSM_Operator_Y, CYAN, String(Modem.Operator));
						Terminal_GSM.Text(GSM_RSSI_X, GSM_RSSI_Y, CYAN, String(Modem.dBm));
					#endif

				#endif

				// Define GSM Section
				JsonObject JSON_GSM = JSON_Device["IoT"].createNestedObject(F("GSM"));

				// Get GSM Parameters
				if (_Pack_Type == Pack_Online) {

					// Define IoT Module
					JsonObject JSON_Module = JSON_GSM.createNestedObject(F("Module"));

					// Set IoT Parameters
					JSON_Module[F("Manufacturer")] = Modem.Manufacturer;
					JSON_Module[F("Model")] = Modem.Model;
					JSON_Module[F("Firmware")] = Modem.Firmware;
					JSON_Module[F("Serial")] = Modem.Serial_ID;
					JSON_Module[F("IMEI")] = Modem.IMEI;

					// Define SIM
					JsonObject JSON_SIM = JSON_GSM.createNestedObject(F("SIM"));

					// Set SIM Parameters
					JSON_SIM[F("SIM_Type")] = 1;
					JSON_SIM[F("ICCID")] = Modem.ICCID;

				}

				// Define GSM Operator Section
				JsonObject JSON_Operator = JSON_GSM.createNestedObject(F("Operator"));

				// Set Device GSM Connection Detail Section
				JSON_Operator[F("Code")] = Modem.Operator;
				JSON_Operator[F("dBm")] = Modem.dBm;
				JSON_Operator[F("LAC")] = Modem.LAC;
				JSON_Operator[F("Cell_ID")] = Modem.Cell_ID;

				// Define Data Section
				JsonObject JSON_Payload = JSON.createNestedObject(F("Payload"));

				// Set Device Time Variable
				JSON_Payload[F("TimeStamp")] = this->JSON_Data.Time_Stamp;

				// Set Device Status Variable
				JSON_Payload[F("Device")] = this->JSON_Data.JSON_Status.Device;

				// Set Device Fault Variable
				JSON_Payload[F("Fault")] = this->JSON_Data.JSON_Status.Fault;

				// Clear Unused Data
				JSON.garbageCollect();

				// Serialize JSON	
				uint16_t _JSON_Size = serializeJson(JSON, this->JSON_Pack);

				// End Function
				return(_JSON_Size);

			}

			// Handle Send Response
			uint16_t Handle_JSON_Send_Response(const char *_Data) {

				// Declare JSON Object
				StaticJsonDocument<32> Incoming_JSON;

				// Deserialize the JSON document
				deserializeJson(Incoming_JSON, _Data);

				// Fetch values.
				uint16_t Event = Incoming_JSON["Event"];

				// End Function
				return(Event);

			}
			uint16_t Handle_JSON_Request(const char *_Data) {

				// Declare Variable
				uint16_t Event = 0;

				// Declare JSON Object
				StaticJsonDocument<512> Incoming_JSON;

				// Deserialize the JSON document
				DeserializationError Error = deserializeJson(Incoming_JSON, _Data);

				// Handle JSON
				if (!Error) Event = Incoming_JSON["Request"]["Event"];

				// End Function
				return(Event);

			}

			// Configure Socket for Listen
			bool Listen(const bool _State) {

				// Declare Status Variable
				uint8_t Socket_Status;

				// Get Socket Status
				SS(2, Socket_Status);

				// Handle State
				if (_State) {

					// Control Current State
					if (Socket_Status != 4) SL(2, 1, 80, 255);

					// Command Delay
					delay(20);

					// Get Socket Status
					SS(2, Socket_Status);

					// Command Delay
					delay(20);

					// Control Socket
					if (Socket_Status != 4) return(false);

				} else {

					// Control Current State
					if (Socket_Status != 0) SL(2, 0, 80, 255);

					// Command Delay
					delay(20);

					// Get Socket Status
					SS(2, Socket_Status);

					// Command Delay
					delay(20);

					// Control Socket
					if (Socket_Status != 0) return(false);

				}

				// End Function
				return(true);

			}

			// Define CallBack Functions
			void (*_Send_CallBack)(uint16_t);
			void (*_Device_Data_CallBack)();
			void (*_Payload_Data_CallBack)();
			void (*_Request_CallBack)(uint16_t, char*);

		// Public Functions
		public:

			// Define Time Structure
			struct Struct_Time {
				bool		Time_Update			= false;
				uint8_t 	Year				= 0;
				uint8_t 	Month				= 0;
				uint8_t 	Day					= 0;
				uint8_t 	Hour				= 0;
				uint8_t 	Minute				= 0;
				uint8_t 	Second				= 0;
			} Time;

			// PostOffice Constructor
			PostOffice(Stream &_Serial) : AT_Command_Set(_Serial) {

			}

			/*************************
			* PostOffice Callback Section
			*************************/

			// CallBack Definitions
			void Device_Data_CallBack(void (*_Device_Data_CallBack)()) {

				// Set CallBack Functions
				this->_Device_Data_CallBack = _Device_Data_CallBack;

			}
			void Payload_Data_CallBack(void (*_Payload_Data_CallBack)()) {

				// Set CallBack Functions
				this->_Payload_Data_CallBack = _Payload_Data_CallBack;

			}
			void PackSend_CallBack(void (*_Send_CallBack)(uint16_t)) {

				// Set CallBack Functions
				this->_Send_CallBack = _Send_CallBack;

			}
			void Request_CallBack(void (*_Command_CallBack)(uint16_t, char*)) {

				// Set CallBack Functions
				this->_Request_CallBack = _Command_CallBack;

			}

			/*************************
			* PostOffice GSM Section
			*************************/

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
					this->Hardware_Status.Power = ON(true, true, true);

				} else {

					// ShutDown Modem
					this->Hardware_Status.Power = OFF(true, true, true);

				}

				// End Function
				return(this->Hardware_Status.Power);

			}

			/**
			 * @brief Initialize GSM Modem
			 * @return true Function is success.
			 * @return false Function fail.
			 */
			bool Initialize(void) {

				// Define Terminal Row Variable
				uint8_t GSM_Console_Initialize_ROW = GSM_Console_Initialize_X;

				// Set Variable
				this->Status.Initialize = false;

				// Get PowerMon
				this->Status.PowerMon = this->PowerMonitor();

				// Declare Watchdog Variable
				uint8_t _Error_WD = 0;

				// Declare Response Variable
				bool _Response = false;

				// Control for Power Monitor
				if (this->Status.PowerMon) {

					// AT Command
					#ifdef _AT_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AT();

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// ATF Command
					#ifdef _AT_ATF_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT&F0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = ATF(1);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// ATE Command
					#ifdef _AT_ATE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("ATE0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = ATE(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// CMEE Command
					#ifdef _AT_CMEE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CMEE=1"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// FCLASS Command
					#ifdef _AT_FCLASS_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+FCLASS=0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// K Command
					#ifdef _AT_K_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT&K0"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// CPIN Command
					#ifdef _AT_CPIN_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CPIN?"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CPIN(Modem.PIN);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// CGSN Command
					#ifdef _AT_CGSN_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CGSN"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CGSN(Modem.IMEI);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_IMEI_X, GSM_IMEI_Y, CYAN, String(Modem.IMEI));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GSN Command
					#ifdef _AT_GSN_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GSN"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GSN(Modem.Serial_ID);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Serial_X, GSM_Serial_Y, CYAN, String(Modem.Serial_ID));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// ICCID Command
					#ifdef _AT_ICCID_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+CCID"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CCID(Modem.ICCID);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_ICCID_X, GSM_ICCID_Y, CYAN, String(Modem.ICCID));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GMI Command
					#ifdef _AT_GMI_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GMI"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GMI(Modem.Manufacturer);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Manufacturer_X, GSM_Manufacturer_Y, CYAN, String(Modem.Manufacturer));
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GMM Command
					#ifdef _AT_GMM_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GMM"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GMM(Modem.Model);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Model_X, GSM_Model_Y, CYAN, String(Modem.Model));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// GMR Command
					#ifdef _AT_GMR_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT+GMR"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = GMR(Modem.Firmware);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
							Terminal_GSM.Text(GSM_Firmware_X, GSM_Firmware_Y, CYAN, String(Modem.Firmware));
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// SLED Command
					#ifdef _AT_SLED_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#SLED=2"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// SLEDSAV Command
					#ifdef _AT_SLEDSAV_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#SLEDSAV"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = SLEDSAV();

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// E2SLRI Command
					#ifdef _AT_E2SLRI_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#E2SLRI=50"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

					#endif

					// TXMONMODE Command
					#ifdef _AT_TXMONMODE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y, WHITE, F("AT#TXMONMODE=1"));
							Terminal_GSM.Text(GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Initialize_ROW, GSM_Console_Initialize_Y + 29);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Initialize_ROW += 1;

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

				// Declare Time Variable
				uint32_t _Connection_Start_Time = 0;

				// Control for Initialization Monitor
				if (this->Status.Initialize) {

					// Set Variable
					this->Status.Connection = false;

					// Get Time
					_Connection_Start_Time = millis();

					// Define Terminal Row Variable
					uint8_t GSM_Console_Connect_ROW = GSM_Console_Connect_X;

					// REGMODE Command
					#ifdef _AT_REGMODE_

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#REGMODE=0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
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
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;
						
					#endif

					// AUTOBND Command
					#ifdef _AT_AUTOBND_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#AUTOBND=1"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = AUTOBND(1);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// COPS Command
					#ifdef _AT_COPS_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+COPS=0,2,28601"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = COPS(1, 2, 28601); // 0,2,28601

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// CREG Command
					#ifdef _AT_CREG_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CREG=0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = Set_CREG(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Declare Response Status
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CREG?"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Declare Variable
							uint8_t _CREG_Connection_Mode = 0;
							uint8_t _CREG_Connection_Stat = 0;

							// Get CREG Status
							Get_CREG(_CREG_Connection_Mode, _CREG_Connection_Stat);

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, CYAN, F("    "));
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 32, RED, String(_CREG_Connection_Stat));
							#endif

							// Control for Connection
							if (_CREG_Connection_Stat == 0) {

								// Declare Response Status
								_Response = false;

							} else if (_CREG_Connection_Stat == 1) {

								// Declare Response Status
								_Response = true;

							} else if (_CREG_Connection_Stat == 2) {

								// Declare Response Status
								_Response = false;

								// Wait Delay
								delay(1000);

							} else if (_CREG_Connection_Stat == 3) {

								// CEER Code Get
								uint16_t _Error_Code;
								CEER(_Error_Code);

								// Print Command State
								#ifdef GSM_Debug
									Terminal_GSM.Text(27, 5, RED, F("CEER Code : "));
									Terminal_GSM.Text(27, 17, RED, String(_Error_Code));
								#endif

								// Declare Response Status
								_Response = false;

								// Wait Delay
								delay(10000);

							} else if (_CREG_Connection_Stat == 4) {

								// Declare Response Status
								_Response = true;

							} else if (_CREG_Connection_Stat == 5) {

								// Declare Response Status
								_Response = true;

							}

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_ConnTime_X, GSM_ConnTime_Y, CYAN, String((millis() - _Connection_Start_Time) / 1000));
							#endif

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 200) return(false);

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// CGREG Command
					#ifdef _AT_CGREG_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CGREG=0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = Set_CGREG(0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Declare Response Status
						_Response = false;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CGREG?"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Process Command
						while (!_Response) {

							// Declare Variable
							uint8_t _CGREG_Connection_Mode = 0;
							uint8_t _CGREG_Connection_Stat = 0;

							// Get CREG Status
							Get_CGREG(_CGREG_Connection_Mode, _CGREG_Connection_Stat);

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, CYAN, F("    "));
								Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 32, RED, String(_CGREG_Connection_Stat));
							#endif

							// Control for Connection
							if (_CGREG_Connection_Stat == 3) {
								
								uint16_t _Error_Code;
								CEER(_Error_Code);

								Terminal_GSM.Text(27, 5, RED, F("CEER Code : "));
								Terminal_GSM.Text(27, 17, RED, String(_Error_Code));

							}

							// Control for Connection
							if (_CGREG_Connection_Stat == 1 or _CGREG_Connection_Stat == 5) {
								
								// Declare Response Status
								_Response = true;

							} else {

								// Wait Delay
								if (_CGREG_Connection_Stat == 2) delay(1000);
								if (_CGREG_Connection_Stat == 3) delay(10000);

								// Declare Response Status
								_Response = false;

							}

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_ConnTime_X, GSM_ConnTime_Y, CYAN, String((millis() - _Connection_Start_Time) / 1000));
							#endif

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 50) return(false);

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// CGDCONT Command Socket
					#ifdef _AT_CGDCONT_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT++CGDCONT=1,**,**,**,0,0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = CGDCONT(1, "IP", "mgbs", "0.0.0.0", 0, 0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif
					
						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// SGACT Command
					#ifdef _AT_SGACT_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#SGACT=1,1"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = Set_SGACT(1, 1, Modem.IP_Address);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_IP_X, GSM_IP_Y, CYAN, String(Modem.IP_Address));
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// SCFG (Send Port) Command
					#ifdef _AT_SCFG_Out_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#SCFG=3,1,1500,90,300,50"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = SCFG(3, 1, 1500, 90, 300, 50);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// SCFGEXT (Send Port) Command
					#ifdef _AT_SCFGEXT_Out_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#SCFGEXT=3,1,0,1,0,0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = SCFGEXT(3, 1, 0, 1, 0, 0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// SCFG (In Port) Command
					#ifdef _AT_SCFG_In_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#SCFG=2,1,1500,90,300,50"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = SCFG(2, 1, 1500, 90, 300, 50);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// SCFGEXT (In Port) Command
					#ifdef _AT_SCFGEXT_In_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#SCFGEXT=2,1,0,1,0,0"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = SCFGEXT(2, 1, 0, 1, 0, 0);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// FRWL Command 1
					#ifdef _AT_FRWL_1_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#FRWL=1,***"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 10, WHITE, String(_AT_FRWL_1_IP_));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FRWL(1, _AT_FRWL_1_IP_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// FRWL Command 2
					#ifdef _AT_FRWL_2_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#FRWL=1,***"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 10, WHITE, String(_AT_FRWL_2_IP_));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FRWL(1, _AT_FRWL_2_IP_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// FRWL Command 3
					#ifdef _AT_FRWL_3_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#FRWL=1,***"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 10, WHITE, String(_AT_FRWL_3_IP_));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = FRWL(1, _AT_FRWL_3_IP_);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// ICMP Command
					#ifdef _AT_ICMP_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#ICMP=1"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = ICMP(1);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// End Function
						if (!_Response) return (false);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// Set MONI Command
					#ifdef _AT_MONIZIP_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT#MONIZIP"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Process Command
							_Response = MONIZIP(Modem.Operator, Modem.LAC, Modem.Cell_ID, Modem.dBm);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_CellLAC_X, GSM_CellLAC_Y, CYAN, String(Modem.LAC));
							Terminal_GSM.Text(GSM_CellID_X, GSM_CellID_Y, CYAN, String(Modem.Cell_ID));
							Terminal_GSM.Text(GSM_Operator_X, GSM_Operator_Y, CYAN, String(Modem.Operator));
							Terminal_GSM.Text(GSM_RSSI_X, GSM_RSSI_Y, CYAN, String(Modem.dBm));
							Terminal_GSM.OK_Decide(_Response, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// Set CCLK Command
					#ifdef _AT_CCLK_

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y, WHITE, F("AT+CCLK"));
							Terminal_GSM.Text(GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31, BLUE, F(" .. "));
						#endif

						// Declare Watchdog Variable
						_Error_WD = 0;

						// Set Response Variable
						_Response = false;

						// Process Command
						while (!_Response) {

							// Send Command
							_Response = CCLK(this->Time.Year, this->Time.Month, this->Time.Day, this->Time.Hour, this->Time.Minute, this->Time.Second);

							// Set WD Variable
							_Error_WD++;

							// Control for WD
							if (_Error_WD > 5) break;

						}

						// Set Control Parameter
						this->Time.Time_Update = _Response;

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.OK_Decide(this->Time.Time_Update, GSM_Console_Connect_ROW, GSM_Console_Connect_Y + 31);
						#endif

						// End Function
						if (!_Response) return (false);

						// Set Variable
						GSM_Console_Connect_ROW += 1;

					#endif

					// Set Connection Time
					Modem.Connection_Time = ((millis() - _Connection_Start_Time) / 1000);

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(13, 113, CYAN, String(Modem.Connection_Time));
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

			/*************************
			* PostOffice Cloud Section
			*************************/

			// Connect Cloud
			bool Online(char * _Device_ID) {

				// Set Device ID
				this->JSON_Data.JSON_Info.Device_ID = _Device_ID;

				// Control for Connection
				if (this->Status.Connection) {

					// Listen Port
					bool _Response = this->Listen(true);

					// Print Command State
					#ifdef GSM_Debug
						if (_Response) Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Cloud Online"));
					#endif

					// End Function
					return(true);

				} else {

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, RED, F("No Connection"));
					#endif

					// End Function
					return(false);

				}

				// End Function
				return(false);

			}

			// Send Data Batch Function
			bool Send(uint8_t _Pack_Type) {

				// Control for Connection
				if (this->Status.Connection) {

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Data Initialize"));
					#endif

					// Get Data CallBack
					_Device_Data_CallBack();
					_Payload_Data_CallBack();

					// Print Command State
					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("JSON Pack       "));
					#endif

					// Parse JSON
					uint16_t _JSON_Size = this->Parse_JSON(Pack_Online);

					// Open Connection
					if (SD(3, 0, 80, 0, 88, 1, PostOffice_Server)) {

						// Clear UART Buffer
						Clear_UART_Buffer();

						// Declare Buffer Object
						Serial_Buffer Buffer_Set = {false, 0, 0, 2000};

						// Declare Buffer
						char Buffer_Variable[255];
						memset(Buffer_Variable, '\0', 255);

						// Command Chain Delay (Advice by Telit)
						delay(20);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Sending pack    "));
						#endif

						// Send UART Command
						GSM_Serial->print(F("AT#SSEND=3"));
						GSM_Serial->print(F("\r\n"));

						// Read Current Time
						uint32_t Current_Time = millis();

						// Response Wait Delay
						delay(10);

						// Read UART Response
						while (!Buffer_Set.Response) {

							// Read Serial Char
							Buffer_Variable[Buffer_Set.Read_Order] = GSM_Serial->read();

							// Control for <OK> Response
							if (Buffer_Variable[Buffer_Set.Read_Order - 1] == '>' and Buffer_Variable[Buffer_Set.Read_Order] == ' ') Buffer_Set.Response = true;

							// Increase Read Order
							if (Buffer_Variable[Buffer_Set.Read_Order] > 31 and Buffer_Variable[Buffer_Set.Read_Order] < 127) Buffer_Set.Read_Order += 1;

							// Handle for timeout
							if (millis() - Current_Time >= Buffer_Set.Time_Out) return(false);

						}

						// Send Delay
						delay(10);

						// Print HTTP Header
						GSM_Serial->print(F("POST ")); GSM_Serial->print(PostOffice_EndPoint); GSM_Serial->print(F(" HTTP/1.1\r\n"));
						GSM_Serial->print(F("Host: ")); GSM_Serial->print(PostOffice_Server); GSM_Serial->print(F("\r\n"));
						GSM_Serial->print(F("Content-Length: ")); GSM_Serial->print(_JSON_Size); GSM_Serial->print(F("\r\n"));
						GSM_Serial->print(F("Connection: close\r\n"));
						GSM_Serial->print(F("Content-Type: application/json\r\n"));
						GSM_Serial->print(F("User-Agent: PowerStat\r\n"));
						GSM_Serial->print(F("\r\n"));

						// Send Data Pack
						GSM_Serial->print(this->JSON_Pack);

						// Print End Char
						GSM_Serial->print((char)26);

						// Declare Buffer Object
						Serial_Buffer Buffer_Get = {false, 0, 0, 2000};

						// Declare Buffer
						memset(Buffer_Variable, '\0', 255);

						// Read Current Time
						Current_Time = millis();

						// Read UART Response
						while (!Buffer_Get.Response) {

							// Read Serial Char
							Buffer_Variable[Buffer_Get.Read_Order] = GSM_Serial->read();

							// Control for <OK> Response
							if (Buffer_Variable[Buffer_Get.Read_Order - 1] == 'O' and Buffer_Variable[Buffer_Get.Read_Order] == 'K') Buffer_Get.Response = true;

							// Increase Read Order
							if (Buffer_Variable[Buffer_Get.Read_Order] > 31 and Buffer_Variable[Buffer_Get.Read_Order] < 127) Buffer_Get.Read_Order += 1;

							// Handle for timeout
							if (millis() - Current_Time >= Buffer_Get.Time_Out) return(false);

						}

						// Response Wait Delay
						delay(10);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Pack Sended     "));
						#endif

						// Declare Ring Status
						uint8_t _Ring_ID;
						uint16_t _Length;

						// Get Ring Port
						if (Send_SRING(_Ring_ID, _Length)) {

							// Declare Response Status
							char _Response[32];
							uint16_t _Response_Command;

							// Get Request Data
							SRECV(3, _Length, _Response);

							// Handle JSON
							_Response_Command = this->Handle_JSON_Send_Response(_Response);

							// Close Socket
							SH(3);

							// Send Data CallBack
							_Send_CallBack(_Response_Command);

							// Print Command State
							#ifdef GSM_Debug
								Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Response Received"));
							#endif

							// Port Control
							this->Listen(true);

							// End Function
							if (_Response_Command == 200) return(true);
							
						}

					}

					// Port Control
					this->Listen(true);


				}

				// End Function
				return(false);

			}

			// Get Server Command Function
			void Get(void) {

				// Control for Connection
				if (this->Status.Connection) {

					// Declare Variable
					char _JSON_Data[50];

					// Declare Request Length
					uint16_t _Request_Length;

					// Handle Ring
					if (Receive_SRING()) {

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Ring             "));
						#endif

						// Answer Socket
						SA(2, 1, _Request_Length);

						// Get Request Data
						SRECV(2, _Request_Length, _JSON_Data);

						// Handle JSON Data
						uint16_t _Event = this->Handle_JSON_Request(_JSON_Data);

						// Print Command State
						#ifdef GSM_Debug
							Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("                 "));
							Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, String(_Event));
						#endif

						// Send Data CallBack
						_Request_CallBack(_Event, _JSON_Data);

					}

				}

			}

			// Send Request Response Function
			bool Response(uint16_t _Response_Code, char * _Data) {

				// Send Socket Answer
				if (SSEND(2, 1, _Response_Code, "", "", _Data)) {

					#ifdef GSM_Debug
						Terminal_GSM.Text(GSM_PostOfficeStatus_X, GSM_PostOfficeStatus_Y, GREEN, F("Response Sended  "));
					#endif

					// Command Delay
					delay(20);

					// Close Socket
					if (SH(2)) {

						// Command Delay
						delay(20);

						// ReOpen Socket
						bool Socket_Open = this->Listen(true);

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

			/*************************
			* PostOffice Data Section
			*************************/

			// Set Environment Variables
			void Environment(float * _Temperature, float * _Humidity) {

				// Set Environment
				this->JSON_Data.JSON_Info.Temperature = *_Temperature;
				this->JSON_Data.JSON_Info.Humidity = *_Humidity;

			}

			// Set Battery Variables
			void Battery(float *_IV, float *_AC, float *_SOC, uint8_t *_Charge, float *_T = 0, uint16_t *_FB = 0, uint16_t *_IB = 0) {

				// Set Battery Parameters
				this->JSON_Data.JSON_Battery.IV = *_IV;
				this->JSON_Data.JSON_Battery.AC = *_AC;
				this->JSON_Data.JSON_Battery.SOC = *_SOC;
				this->JSON_Data.JSON_Battery.Charge = *_Charge;
				this->JSON_Data.JSON_Battery.T = *_T;			// Optional
				this->JSON_Data.JSON_Battery.FB = *_FB;			// Optional
				this->JSON_Data.JSON_Battery.IB = *_IB;			// Optional

			}

			// Set TimeStamp
			void TimeStamp(char * _TimeStamp) {

				// Set Time Stamp
				this->JSON_Data.Time_Stamp = _TimeStamp;

			}

			// Set Status
			void SetStatus(uint16_t _Device, uint16_t _Fault) {

				// Set Device Status Variables
				this->JSON_Data.JSON_Status.Device = _Device;
				this->JSON_Data.JSON_Status.Fault = _Fault;

			}

	};

#endif /* defined(__Telit_xE910__) */