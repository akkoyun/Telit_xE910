#ifndef __GSM_Socket__
#define __GSM_Socket__

// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Define Objects
AT_Command_Set _AT(Serial_GSM);

// Data Incoming Class
class GSM_Socket_Incoming {

	private:

		// Define Socket Status Structure
		struct Socket_Status {
			
			// Incoming Socket Port Definations
			uint8_t Port = 2;
			uint8_t Cid = 1;
			uint8_t Server_Port = 80;

			// SCFG Parameters
			struct SCFG_Structure {
				uint16_t PktSize = 1500;
				uint8_t MaxTO = 90;
				uint16_t ConnTO = 300;
				uint8_t TxTO = 50;
			} SCFG;
			
			// SCFGEXT Parameters
			struct SCFGEXT_Structure {
				uint8_t SrMode = 1;
				uint8_t RecvDataMode = 0;
				uint8_t KeepAlive = 1;
				uint8_t AutoResp = 0;
				uint8_t SendDataMode = 0;
			} SCFGEXT;

		} Parameter;

		/**
		 * @brief Handle JSON Pack Function
		 * @param _Data JSON Pack
		 * @return uint16_t Request Command
		 */
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

	public:

		/**
		 * @brief Construct a new gsm socket incoming object
		 * @param _Port Socket Port
		 */
		GSM_Socket_Incoming(uint8_t _Port) {

			// Set Socket Port
			this->Parameter.Port = _Port;

		}

		/**
		 * @brief Configure Socket
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool Configure(void) {

			// Declare Watchdog Variable
			uint8_t _Error_WD = 0;

			// Declare Response Status
			bool _Response = false;

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(Debug_Connect_X + 5, Debug_Connect_Y, BLUE, F(" .. "));
				#endif

				// Process Command
				while (!_Response) {

					// Process Command
					_Response = _AT.SCFG(this->Parameter.Port, this->Parameter.Cid, this->Parameter.SCFG.PktSize, this->Parameter.SCFG.MaxTO, this->Parameter.SCFG.ConnTO, this->Parameter.SCFG.TxTO);

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
					_Response = _AT.SCFGEXT(this->Parameter.Cid, this->Parameter.SCFGEXT.SrMode, this->Parameter.SCFGEXT.RecvDataMode, this->Parameter.SCFGEXT.KeepAlive, this->Parameter.SCFGEXT.AutoResp, this->Parameter.SCFGEXT.SendDataMode);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Debug_Connect_X + 5, Debug_Connect_Y);
				#endif
			
				// End Function
				if (!_Response) return (false);

			// End Function
			return(false);

		}

		/**
		 * @brief Configure Socket for Listen
		 * @param _State Enable / Disable Socket Listen
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool Listen(const bool _State) {

			// Declare Status Variable
			uint8_t Socket_Status;

			// Get Socket Status
			_AT.SS(this->Parameter.Port, Socket_Status);

			// Handle State
			if (_State) {

				// Control Current State
				if (Socket_Status != 4) _AT.SL(this->Parameter.Port, 1, this->Parameter.Server_Port, 255);

				// Command Delay
				delay(20);

				// Get Socket Status
				_AT.SS(this->Parameter.Port, Socket_Status);

				// Command Delay
				delay(20);

				// Control Socket
				if (Socket_Status != 4) return(false);

			} else {

				// Control Current State
				if (Socket_Status != 0) _AT.SL(this->Parameter.Port, 0, this->Parameter.Server_Port, 255);

				// Command Delay
				delay(20);

				// Get Socket Status
				_AT.SS(this->Parameter.Port, Socket_Status);

				// Command Delay
				delay(20);

				// Control Socket
				if (Socket_Status != 0) return(false);

			}

			// End Function
			return(true);

		}

		/**
		 * @brief Socket Answer Function
		 * @param _JSON_Data Incoming JSON Pack
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool Answer(char * _JSON_Data) {

			// Declare RING
			uint8_t RING;

			// Get Ring Port
			_AT.Receive_SRING(RING);

			// Control for <SRING:n>
			if (RING == 1 or RING == 2 or RING == 3) {

				// Declare Request Length
				uint16_t Request_Length;

				// Answer Socket
				_AT.SA(RING, 1, Request_Length);

				// Get Request Data
				_AT.SRECV(RING, Request_Length, _JSON_Data);

				// End Function
				return(true);

			}

			// End Function
			return(false);

		}

		/**
		 * @brief Send Request Response Function
		 * @param _Data Sended Response Data Pack
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool Response(uint16_t _Response_Code, char * _Data) {

			// Send Socket Answer
			if (_AT.SSEND(this->Parameter.Port, 1, _Response_Code, "", "", _Data)) {

				// Command Delay
				delay(20);

				// Close Socket
				if (_AT.SH(this->Parameter.Port)) {

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

		/**
		 * @brief Get Server Command Batch Function
		 * @return uint16_t Server Request Command
		 */
		uint16_t Get(char * _Request) {

			// Declare Command Variable
			uint16_t Event;

			// Answer Socket Connection
			this->Answer(_Request);

			// Handle JSON Data
			Event = this->Handle_JSON_Request(_Request);

			// End Function
			return(Event);

		}

};

// Data Send Class
class GSM_Socket_Outgoing {

	private:

		// Define Socket Status Structure
		struct Socket_Status {
			uint8_t Port = 3;
			uint8_t Cid = 1;

			// Server Parameters
			struct Server_Structure {
				char * Address;
				char * EndPoint;
				uint8_t Port = 80;
			} Server;

			// SCFG Parameters
			struct SCFG_Structure {
				uint16_t PktSize = 1500;
				uint8_t MaxTO = 90;
				uint16_t ConnTO = 300;
				uint8_t TxTO = 50;
			} SCFG;

			// SCFGEXT Parameters
			struct SCFGEXT_Structure {
				uint8_t SrMode = 1;
				uint8_t RecvDataMode = 0;
				uint8_t KeepAlive = 1;
				uint8_t AutoResp = 0;
				uint8_t SendDataMode = 0;
			} SCFGEXT;

		} Parameter;

		/**
		 * @brief Handle Send Response
		 * @param _Data Received Response
		 * @return uint16_t Command
		 */		
		uint16_t Handle_JSON_Send_Response(const char *_Data) {

			// Declare JSON Object
			StaticJsonDocument<128> Incoming_JSON;

			// Deserialize the JSON document
			deserializeJson(Incoming_JSON, _Data);

			// Fetch values.
			uint16_t Event = Incoming_JSON["Event"];

			// End Function
			return(Event);

		}

	public:

		/**
		 * @brief Construct a new gsm socket outgoing object
		 * @param _Port Socket Port
		 * @param _Server Server Address (IP)
		 * @param _End_Point Server URL (EndPoint)
		 */
		GSM_Socket_Outgoing(uint8_t _Port, char * _Server, char * _End_Point) {

			// Set Socket Port
			this->Parameter.Port = _Port;

			// Set Socket Server
			this->Parameter.Server.Address = _Server;

			// Set Socket URL
			this->Parameter.Server.EndPoint = _End_Point;

		}

		/**
		 * @brief Configure Socket
		 * @return true Function is success.
		 * @return false Function fail.
		 */
		bool Configure(void) {

			// Declare Watchdog Variable
			uint8_t _Error_WD = 0;

			// Declare Response Status
			bool _Response = false;

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.Text(Debug_Connect_X + 5, Debug_Connect_Y, BLUE, F(" .. "));
				#endif

				// Process Command
				while (!_Response) {

					// Process Command
					_Response = _AT.SCFG(this->Parameter.Port, this->Parameter.Cid, this->Parameter.SCFG.PktSize, this->Parameter.SCFG.MaxTO, this->Parameter.SCFG.ConnTO, this->Parameter.SCFG.TxTO);

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
					_Response = _AT.SCFGEXT(this->Parameter.Cid, this->Parameter.SCFGEXT.SrMode, this->Parameter.SCFGEXT.RecvDataMode, this->Parameter.SCFGEXT.KeepAlive, this->Parameter.SCFGEXT.AutoResp, this->Parameter.SCFGEXT.SendDataMode);

					// Set WD Variable
					_Error_WD++;

					// Control for WD
					if (_Error_WD > 5) break;

				}

				// Print Command State
				#ifdef GSM_Debug
					Terminal_GSM.OK_Decide(_Response, Debug_Connect_X + 5, Debug_Connect_Y);
				#endif
			
				// End Function
				if (!_Response) return (false);

			// End Function
			return(false);

		}

		/**
		 * @brief Send Data Batch Function
		 * @param _Data Sended Data
		 * @param _Response Received Data
		 * @return uint16_t Server Request Command
		 */
		uint16_t Send(const char *_Data, char *_Response) {

			// Open Connection
			if (_AT.SD(this->Parameter.Port, 0, this->Parameter.Server.Port, 0, 88, 1, this->Parameter.Server.Address)) {

				// Send Data Pack
				_AT.SSEND(this->Parameter.Port, 2, 0, this->Parameter.Server.Address, this->Parameter.Server.EndPoint, _Data);

				// Declare Ring Status
				uint8_t Ring_ID;
				uint16_t Length;
				uint16_t Response_Command;

				// Get Ring Port
				if (_AT.Send_SRING(Ring_ID, Length)) {

					// Get Request Data
					_AT.SRECV(Ring_ID, Length, _Response);

					// Handle JSON
					Response_Command = this->Handle_JSON_Send_Response(_Response);

					// Close Socket
					_AT.SH(Ring_ID);

					// End Function
					return(Response_Command);
					
				}

				// End Function
				return(0);

			}

			// End Function
			return(0);

		}

};

#endif