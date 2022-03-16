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

// Define Library Structures
#ifndef __Telit_xE910_Structures__
#include <Telit_xE910_Structures.h>
#endif


class Telit_xE910 {

	private:

		// Stream Object Definition
		Stream *_GSM_Serial;

		// Hardware Functions
		void Set_Communication(const bool _State);
		void Set_OnOff(const uint16_t _Time, const bool _Terminal, const uint8_t _X, const uint8_t _Y);
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
		bool CGSN(void);
		bool GSN(void);
		bool CCID(void);
		bool GMI(void);
		bool GMM(void);
		bool GMR(void);
		bool CSQ(void);
		bool SERVINFO(void);
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
		bool FRWL(const uint8_t _Action, const char *_IP_Addr, const char *_Net_Mask);
		bool ICMP(const uint8_t _Mode);
		bool CTZU(const bool _State);
		bool NITZ(const bool _State);
		bool NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out);
		bool CCLK(void);
		bool SHDN(void);

		// Data Functions
		bool HTTPCFG(const uint8_t _ProfID, const char *_HTTP_Server, const uint8_t _Port, const uint8_t _AuthType, const char *_Username, const char *_Password, const uint8_t _SSL, const uint8_t _TimeOut, const uint8_t _Cid);
		bool HTTPSND(const uint8_t _ProfID, const uint8_t _Command, const char *_URL, const uint8_t _TimeOut, const String _Data);
		bool HTTPRCV(const uint8_t _ProfID);

		// Tool Functions
		void _Clear_UART_Buffer(void);	
		uint8_t _Signal_Strength(uint16_t _CSQ);

	public:

		// Define Struct
		Struct_Modem Modem {
			"02.00.00",			// Version
			false,				// Power Monitor
			false,				// SIM Status
			false,				// Initialize Status
			false,				// Connection Status
			false,				// CREG Status
			false				// CGREG Status
		};
		Struct_Variables Variables {
			"",					// IMEI Variable
			"",					// Serial Number Variable
			"",					// ICCID Variable
			0,					// Manufacturer Variable
			0,					// Model Variable
			"",					// Firmware Variable
			0,					// RSSI Variable
			0,					// Operator Variable
			0,					// Connection Time
			""					// IP Address
		};
		Struct_Time Time {0, 0, 0, 0, 0, 0};

		// Public Functions
		bool Begin(Stream &_Serial);
		bool Set_Modem(const bool _Terminal, const uint8_t _X, const uint8_t _Y);
		bool Connect(const bool _Terminal, const uint8_t _X, const uint8_t _Y);
		bool Time_Update(void);
		bool Get_RSSI(void);
		uint8_t Get_Signal(void);
		bool Connection_Control(void);
		bool Send_Data_Pack(const uint8_t _Pack_Type, const String _Data);

};

extern Telit_xE910 GSM;

#endif /* defined(__Telit_xE910__) */
