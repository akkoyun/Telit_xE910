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

// Define VT100 Terminal
#ifndef __Console__
#include <Console.h>
#endif

class Telit_xE910 {

	private:

		// Stream Object Definition
		Stream *_GSM_Serial;

		// Define Modem Structure
		bool _Debug_Mon = false;

		// Define Modem Structure
		struct _Struct_Modem {
			const char Version[9]		= "02.00.00";	// Version
			bool Power_Monitor			= false;		// Power Monitor
			bool SIM_Status				= false;		// SIM Status
			bool Initialize_Status		= false;		// Initialize Status
			bool Connection_Status		= false;		// Connection Status
			bool CREG_Status			= false;		// CREG Status
			bool CGREG_Status			= false;		// CGREG Status
		} Modem;

		// Define Variable Structure
		struct _Struct_Variables {
//			char IMEI[17]				= "";	// IMEI Variable
//			char Serial_Number[11]		= "";	// Serial Number Variable
//			char ICCID[21]				= "";	// ICCID Variable
//			uint8_t Manufacturer		= 0;	// Manufacturer Variable	
//			uint8_t Model				= 0;	// Model Variable
//			char Firmware[10]			= "";	// Modem Firmware Version Variable
//			uint8_t RSSI				= 0;	// Signal Variable
//			uint16_t Operator			= 0;	// Operator Variable
//			uint8_t Connection_Time		= 0;	// Connection Time
			char IP_Address[16]			= "";	// IP Addres
		} Variables;

		// Define Time Structure
		struct _Struct_Time {
			uint8_t Year				= 0;
			uint8_t Month				= 0;
			uint8_t Day					= 0;
			uint8_t Hour				= 0;
			uint8_t Minute				= 0;
			uint8_t Second				= 0;
		} Time;

		// Define Serial Buffer Structure
		struct Struct_Serial_Buffer {
			bool Response;
			uint8_t Read_Order;
			uint8_t Data_Order;
			char Buffer[255];
			uint16_t Time_Out;
		};

		// Hardware Functions
		void Set_Communication(const bool _State);
		void Set_OnOff(const uint16_t _Time);
		void Set_ShutDown(const uint16_t _Time);
		void Set_Power_Switch(const bool _State);
		void Set_LED(const bool _State);
		bool Get_PowerMonitor(void);

		// Hardware Batch Functions
		bool Power_ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch);
		bool Power_OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch);

		// AT Command Set
		bool AT(void);
		bool ATE(const bool _ECHO);
		bool CMEE(const uint8_t _CMEE);
		bool FCLASS(const uint8_t _FCLASS);
		bool K(const uint8_t _K);
		bool CPIN(void);
		bool CGSN(char * _IMEI);
		bool GSN(char * _Serial_Number);
		bool CCID(char * _ICCID);
		bool GMI(uint8_t &_Manufacturer);
		bool GMM(uint8_t &_Model);
		bool GMR(char * _Firmware);
		bool CSQ(uint8_t &_RSSI);
		bool SERVINFO(uint16_t &_Operator);
		bool SLED(const uint8_t _SLED);
		bool E2SLRI(const uint16_t _Pulse_Duration);
		bool TXMONMODE(const uint8_t _TXMONMODE);
		bool REGMODE(const uint8_t _REGMODE);
		bool CREG(const bool _Mode);
		bool CGREG(const bool _Mode);
		bool CGDCONT(const uint8_t _Cid, const char *_PDP_Type, const char *_APN, const char *_PDP_Addr, const bool _D_Comp, const bool _H_Comp);
		bool SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To);
		bool SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode);
		bool SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode);
		bool SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling);
		bool CGACT(const uint8_t _State, const bool _Cid);
		bool CGPADDR(const uint8_t _Cid);
		bool FRWL(const uint8_t _Action, const char *_IP_Addr);
		bool ICMP(const uint8_t _Mode);
		bool CTZU(const bool _State);
		bool NITZ(const bool _State);
		bool NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out);
		bool CCLK(void);
		bool SHDN(void);
		bool SGACT(const uint8_t _Cid, const bool _Stat);

		// Data Functions
		bool HTTPCFG(const char *_HTTP_Server, const uint8_t _Port);
		bool HTTPSND(const char *_URL, const char *_Data);
		bool HTTPRCV(void);

		// Socket Functions
		bool SL(const uint8_t _ConnID, const bool _Listen_State, const uint16_t _Listen_Port, const uint8_t _Closure_Type);
		uint16_t SA(const uint8_t _ConnID, const uint8_t _ConnMode);
		bool SH(const uint8_t _ConnID);
		bool SO(const uint8_t _ConnID);
		bool SD(const uint8_t _Cid, const uint8_t _Pro, const uint8_t _Port, const uint8_t _Closure_Type, uint16_t _IPort, const bool _Conn_Mode, const char *_IP, const char *_URL, const char *_Data);
		uint8_t SS(const uint8_t _ConnID);
		uint16_t SRECV(const uint8_t _ConnID, const uint16_t _MaxByte);
		bool SSEND(const uint8_t _ConnID, const char * _Data_Pack);

		// Tool Functions
		void _Clear_UART_Buffer(void);	
		uint8_t _Signal_Strength(uint16_t _CSQ);

	public:

		// Public Functions
		bool Begin(Stream &_Serial, const bool _Debug_Monitor);
		bool Set_Modem(void);
		bool Connect(uint8_t &_Connection_Time);
		uint8_t Get_RSSI(void);
		uint8_t Get_Signal(void);
		bool Connection_Control(void);
		bool Send_Data_Pack(const uint8_t _Pack_Type, const char *_Data);
		bool Set_Firewall(void);

		// Socket Functions
		bool Socket_Close(void);
		bool Socket_Open(void);
		bool Socket_Listen(void);
		uint8_t Socket_Status(void);
		uint16_t Socket_Answer(void);
		bool Socket_Send(char * _Data_Pack);

		// Variable Functions
		char * Get_IMEI(void);
		char * Get_Serial_ID(void);
		char * Get_ICCID(void);
		uint8_t Get_Manufacturer(void);
		uint8_t Get_Model(void);
		char * Get_Firmware(void);
		uint16_t Get_Operator(void);
		char * Get_IP_Address(void);

		// Time Funcrions
		uint8_t Get_Year(void);
		uint8_t Get_Month(void);
		uint8_t Get_Day(void);
		uint8_t Get_Hour(void);
		uint8_t Get_Minute(void);
		uint8_t Get_Second(void);

		// Status Functions
		uint8_t Get_Connection_Status(void);

};

extern Telit_xE910 GSM;

#endif /* defined(__Telit_xE910__) */
