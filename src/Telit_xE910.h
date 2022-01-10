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

// Define PGMspace Function
#include <avr/pgmspace.h>

// Define Library Structures
#ifndef __Telit_xE910_Structures__
#include <Telit_xE910_Structures.h>
#endif

/**
 * @brief GSM Modem global function class
 * @version 01.00.00
 */
class xE910_GSM {

	public:

		/**
		* Library global variables declarations.
		*/
		const char 	Version[9] 					= "01.01.08";		// Library Version
		bool		Debug_Mode					= false;			// Debug Mode Variable

		/**
		 * @brief Power ON procedure of GSM modem.
		 * @param _Power_Switch Enable GSM modem main power switch.
		 * @param _LED_Switch Enable GSM modem LED inducator power switch.
		 * @param _Communication_Switch Enable GSM modem voltage translator.
		 * @version 01.00.00
		 */
		bool Power_ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch);

		/**
		 * @brief Power OFF procedure of GSM modem.
		 * @param _Power_Switch Disable GSM modem main power switch.
		 * @param _LED_Switch Disable GSM modem LED inducator power switch.
		 * @param _Communication_Switch Disable GSM modem voltage translator.
		 * @param _Clear_Variables Clear library control variables.
		 * @version 01.00.00
		 */
		bool Power_OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch, const bool _Clear_Variables);

		/**
		 * @brief GSM Modem initialize function
		 * @param _Debug Enable debug mode 
		 * @version 01.00.01
		 */
		bool Begin(const bool _Debug);

		/**
		 * @brief GSM Modem connection function
		 * @version 01.00.00
		 */
		bool Connect(void);

		/**
		 * @brief Socket listen function
		 * @version 01.00.00
		 */
		bool Socket_Listen(void);

		/**
		 * @brief RSSI function
		 * @version 01.00.00
		 */
		bool RSSI_Refresh(void);

		/**
		 * @brief Returns GSM modem manufacturer name (string)
		 */
		String Manufacturer(void);
		
		/**
		 * @brief Returns GSM modem Model name (string)
		 */
		String Model(void);

		/**
		 * @brief Returns GSM operator name (string)
		 */
		String Operator(void);

		/**
		 * @brief Returns GSM modem firmware name (string)
		 */
		String Firmware(void);

		/**
		 * @brief Returns GSM modem IMEI (string)
		 */
		String IMEI(void);

		/**
		 * @brief Returns GSM modem serial id (string)
		 */
		String Serial_ID(void);

		/**
		 * @brief Returns GSM modem ICCID (string)
		 */
		String ICCID(void);

		String IP(void);
		uint8_t Signal_Strength(void);

	private:
	
	
};

/**
 * @brief GSM Modem hardware function class.
 * @version 01.00.00
 */
class xE910_HARDWARE {

	public:

		/**
 		* Library global variables declarations.
		 */
		bool		Power_Monitor				= false;			/// GSM Power Monitor Signal

		/**
		 * @brief Enable or disable voltage translator buffer.
		 * @version 01.00.00
		 * @param _State Buffer state
		 * @return true - Buffer enabled
		 * @return false - Buffer disabled
		 */
		bool Communication(const bool _State);

		/**
		 * @brief Controls GSM modem power monitor signal.
		 * @version 01.00.00
		 * @return true - GSM powered
		 * @return false - GSM not powered
		 */
		bool PowerMonitor(void);

		/**
		 * @brief On or off GSM modem.
		 * @version 01.00.00
		 * @param _Time Signal time
		 */
		void OnOff(const uint16_t _Time);

		/**
		 * @brief Shut down GSM modem.
		 * @version 01.00.00
		 * @param _Time Signal time
		 */
		void ShutDown(const uint16_t _Time);

		/**
		 * @brief GSM modem main power switch control.
		 * @version 01.00.00
		 * @param _State Power state
		 */
		void Power_Switch(const bool _State);

		/**
		 * @brief GSM modem LED inducators power switch control.
		 * @version 01.00.00
		 * @param _State LED state
		 */
		void LED(const bool _State);

	private:

};

/**
 * @brief GSM Modem AT command set function class.
 * @version 01.00.00
 */
class xE910_AT {

	public:

		/**
 		* Library global variables declarations.
		 */
		char		IMEI[17]					= "";				// IMEI Variable
		char		Serial_Number[11]			= "";				// Serial Number Variable
		char		ICCID[21]					= "";				// ICCID Variable
		uint8_t 	Manufacturer 				= 0;				// Modem Manufacturer Variable
		uint8_t 	Model 						= 0;				// Modem Model Variable
		char		Modem_Firmware[10]			= "";				// Modem Firmware Version Variable
		bool		Initialization_Status		= false;			// Initialization Status
		bool		SIM_PIN_Status				= false;			// SIM Pin Status

		uint8_t		Connection_Time				= 0;				// CREG and CGREG time Variable
		uint16_t 	Operator 					= 0;				// Operator Variable
		uint8_t 	Signal_RSSI 				= 0;				// Signal Variable
		uint8_t		CREG_Status					= 0;				// CREG Status Variable
		uint8_t		CGREG_Status				= 0;				// CGREG Status Variable
		uint8_t		SGACT_Status				= 0;				// SGACT Status Variable
		char		IP_Address[16]				= "";				// IP Address Variable
		bool		Connection_Status			= false;			// Connection Status

		uint8_t 	RTC_Day						= 29;				// Day Variable
		uint8_t 	RTC_Month					= 10;				// Month Variable
		uint16_t 	RTC_Year					= 1923;				// Year Variable
		uint8_t 	RTC_Hour					= 0;				// Hour Variable
		uint8_t 	RTC_Minute					= 0;				// Minute Variable
		uint8_t 	RTC_Second					= 0;				// Second Variable

		/**
		 * @brief AT Function
		 * @details AT Command : AT\r\n (4 Byte)
		 * @details AT Response : AT\r\n\r\nOK\r\n (10 Byte)
		 * 
		 * @version 01.00.02
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool AT(void);

		/**
		 * @brief Set function to enable or disable the command echo.
		 * @details AT Command : ATE[<n>]\r\n (6 Byte)
		 * @details AT Response : ATE[<n>]\r\n\r\nOK\r\n (12 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @param _ECHO Command echo state
		 * TRUE : Echo ON [Factory Default]
		 * FALSE : Echo OFF
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool ATE(const bool _ECHO);

		/**
		 * @brief Set command enables/disables the report of result code.
		 * @details AT Command : AT+CMEE[<n>]\r\n (10 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @param _CMEE Enable flag
		 * 0 - disable +CME ERROR:<err> reports, use only ERROR report.
		 * 1 - enable +CME ERROR:<err> reports, with <err> in numeric format 
		 * 2 - enable +CME ERROR: <err> reports, with <err> in verbose format
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CMEE(const uint8_t _CMEE);

		/**
		 * @brief Set command sets the wireless module in specified connection 
		 * mode (data, fax, voice), hence all the calls done afterwards 
		 * will be data or voice.
		 * @details AT Command : AT+FCLASS[<n>]\r\n (12 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 *  
		 * @param _FCLASS Parameter flag
		 * 0 - data
		 * 1 - fax class 1 
		 * 8 - voice
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool FCLASS(const uint8_t _FCLASS);

		/**
		 * @brief Set command controls the RS232 flow control behaviour.
		 * @details AT Command : AT&K[<n>]\r\n (7 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @param _K Parameter flag
		 * 0 - no flow control
		 * 1 - hardware mono-directional flow control (only CTS active)
		 * 2 - software mono-directional flow control (XON/XOFF)
		 * 3 - hardware bi-directional flow control (both RTS/CTS active) (factory default) 
		 * 4 - software bi-directional with filtering (XON/XOFF)
		 * 5 - pass through: software bi-directional without filtering (XON/XOFF)
		 * 6 - both hardware bi-directional flow control (both RTS/CTS active) and 
		 * software bi-directional flow control (XON/XOFF) with filtering
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool K(const uint8_t _K);

		/**
		 * @brief Set command sends to the device a password which is necessary 
		 * before it can be operated (SIM PIN, SIM PUK, PH-SIM PIN, etc.).
		 * If the PIN required is SIM PUK or SIM PUK2, the <newpin> is required. 
		 * This second pin, <newpin> will replace the old pin in the SIM.
		 * The command may be used to change the SIM PIN by sending it with 
		 * both parameters <pin> and <newpin>.
		 * @details AT Command : AT+CPIN?\r\n (10 Byte)
		 * @details AT Response : \r\n+CPIN: READY\r\n\r\nOK\r\n (22 Byte)
		 * 
		 * @version 01.00.01
		 *  
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CPIN(void);

		/**
		 * @brief Execution command returns the product serial number, identified 
		 * as the IMEI of the mobile, without command echo.
		 * @details AT Command : AT+CGSN\r\n (9 Byte)
		 * @details AT Response : \r\n351732055366390\r\n\r\nOK\r\n (25 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CGSN(void);

		/**
		 * @brief Execution command returns the device board serial number.
		 * @details AT Command : AT+GSN\r\n (8 Byte)
		 * @details AT Response : \r\n0000328245\r\n\r\nOK\r\n (20 Byte)
		 * 
		 * @version 01.00.03
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool GSN(void);

		/**
		 * @brief Execution command reads on SIM the ICCID (card identification 
		 * number that provides a unique identification number for the SIM)
		 * @details AT Command : AT#CCID\r\n (9 Byte)
		 * @details AT Response : \r\n#CCID: 8990011916180288209\r\n\r\nOK\r\n (36 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CCID(void);

		/**
		 * @brief Execution command returns the manufacturer identification.
		 * @details AT Command : AT+GMI\r\n (8 Byte)
		 * @details AT Response : \r\nTelit\r\n\r\nOK\r\n (15 Byte)
		 * 
		 * 0 - No manufacturer 
		 * 1 - "Telit"
		 * 
		 * @version 01.00.01
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool GMI(void);

		/**
		 * @brief Execution command returns the model identification.
		 * @details AT Command : AT+GMM\r\n (8 Byte)
		 * @details AT Response : \r\nGE910-QUAD\r\n\r\nOK\r\n (20 Byte)
		 * 
		 * 0 - No model
		 * 1 - "GE910-QUAD"
		 * 
		 * @version 01.00.01
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool GMM(void);

		/**
		 * @brief Execution command returns the model identification.
		 * @details AT Command : AT+GMR\r\n (8 Byte)
		 * @details AT Response : \r\n13.00.007\r\n\r\nOK\r\n (19 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool GMR(void);

		/**
		 * @brief Execution command reports received signal quality indicators in the form.
		 * @details AT Command : AT+CSQ\r\n (8 Byte)
		 * @details AT Response : \r\n+CSQ: xx,x\r\n\r\nOK\r\n (19 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CSQ(void);

		/**
		 * @brief Execution command reports information about serving cell, in the format.
		 * @details AT Command : AT#SERVINFO\r\n (13 Byte)
		 * @details AT Response : \r\n#SERVINFO: 3,-81,"Turkcell","28601",52,855E,04,1,,"II",01,6\r\n\r\nOK\r\n (69 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SERVINFO(void);

		/**
		 * @brief Set command sets the behaviour of the STAT_LED GPIO
		 * @details AT Command : AT#SLED=[<n>]\r\n (11 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @param _SLED Enable flag
		 * 0 - GPIO tied Low
		 * 1 - GPIO tied High
		 * 2 - GPIO handled by Module Software
		 * 3 - GPIO is turned on and off alternatively
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SLED(const uint8_t _SLED);

		/**
		 * @brief Set TXMON pin behaviour.
		 * @details AT Command : AT#TXMONMODE=[<n>]\r\n (16 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @param _TXMONMODE Enable flag
		 * 0 - TXMON pin goes high when a call is started and it drops down when
		 * the call is ended. It also goes high when a location update starts, 
		 * and it drops down when the location update procedure stops. Finally 
		 * it goes high during SMS transmission and receiving. Even if the 
		 * TXMON in this case is set as GPIO in output, the read command 
		 * AT#GPIO=5,2 returns #GPIO:2,0, as the GPIO is in alternate mode.
		 * 1 - TXMON is set in alternate mode and the Timer unit controls its state. 
		 * TXMON goes high before power ramps start raising and drops down after 
		 * power ramps stop falling down. This behaviour is repeated for every 
		 * transmission burst.
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool TXMONMODE(const uint8_t _TXMONMODE);

		/**
		 * @brief There are situations in which the presentation of the URCs controlled by 
		 * either +CREG and +CGREG are slightly different from ETSI specifications. 
		 * We identified this behaviour and decided to maintain it as default for backward 
		 * compatibility issues, while we’re offering a more formal ‘Enhanced Operation Mode’ 
		 * through #REGMODE.
		 * @details AT Command : AT#REGMODE=[<mode>]\r\n (14 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * 0 - basic operation mode
		 * 1 - enhanced operation mode
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool REGMODE(const uint8_t _REGMODE);

		/**
		 * @brief Set command enables/disables network registration 
		 * reports depending on the parameter <mode>.
		 * @details AT Command : AT+CREG=[<mode>]\r\n (11 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 02.00.01
		 * 
		 * @param _Mode 
		 * 
		 * 0 - disable network registration unsolicited result code (factory default) 
		 * 1 - enable network registration unsolicited result code
		 * 2 - enable network registration unsolicited result code with network Cell identification data
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CREG(const bool _Mode);

		/**
		 * @brief Set command controls the presentation of an unsolicited result code
		 * @details AT Command : AT+CGREG=[<mode>]\r\n (11 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 02.00.01
		 * 
		 * @param _Mode 
		 * 
		 * 0 - disable network registration unsolicited result code
		 * 1 - enable network registration unsolicited result code; if there is a change in the
		 * terminal GPRS network registration status, it is issued the unsolicited result code.
		 * 2 - enable network registration and location information unsolicited result code; 
		 * if there is a change of the network cell, it is issued the unsolicited result code.
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CGREG(const bool _Mode);

		/**
		 * @brief Set command specifies PDP context parameter values for a 
		 * PDP context identified by the (local) context identification parameter, <cid>
		 * @details AT Command : AT+CGDCONT=[<cid>],[<PDP_type>],[<APN>],[<PDP_addr>],[<d_comp>],[<h_comp>]\r\n (37 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @param _Cid (PDP Context Identifier) 
		 * numeric parameter which specifies a particular PDP context definition.
		 * @param _PDP_Type (Packet Data Protocol type) 
		 * a string parameter which specifies the type of packet data protocol
		 * @param _APN (Access Point Name) 
		 * a string parameter which is a logical name that is used to select the 
		 * GGSN or the external packet data network. If the value is empty (“”) 
		 * or omitted, then the subscription value will be requested.
		 * @param _PDP_Addr 
		 * a string parameter that identifies the terminal in the address space 
		 * applicable to the PDP. The allocated address may be read using the +CGPADDR command.
		 * @param _D_Comp numeric parameter that controls PDP data compression
		 * 0 - off (default if value is omitted)
		 * 1 - on
		 * @param _H_Comp numeric parameter that controls PDP header compression
		 * 0 - off (default if value is omitted)
		 * 1 - on
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CGDCONT(const uint8_t _Cid, const char *_PDP_Type, const char *_APN, const char *_PDP_Addr, const bool _D_Comp, const bool _H_Comp);
		
		/**
		 * @brief Set command sets the socket configuration parameters.
		 * @details AT Command : AT#SCFG=[<conn_id>],[<cid>],[<pktsz>],[<maxto>],[<connto>],[<txto>]\r\n (27 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @param _Conn_ID socket connection identifier (1-6)
		 * @param _Cid PDP context identifier
		 * 0 - specifies the GSM context
		 * 1..5 - numeric parameter which specifies a particular PDP context definition
		 * @param _Pkt_Sz packet size to be used by the TCP/UDP/IP stack for data sending.
		 * 0 - select automatically default value(300).
		 * 1..1500 - packet size in bytes.
		 * @param _Max_To exchange timeout (or socket inactivity timeout); if there’s 
		 * no data exchange within this timeout period the connection is closed.
		 * 0 - no timeout
		 * 1..65535 - timeout value in seconds (default 90 s.)
		 * @param _Conn_To connection timeout; if we can’t establish a connection to the remote
		 * within this timeout period, an error is raised.
		 * 10..1200 - timeout value in hundreds of milliseconds (default 600)
		 * @param _TX_To data sending timeout; after this period data are sent also 
		 * if they’re less than max packet size.
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SCFG(const uint8_t _Conn_ID, const uint8_t _Cid, const uint16_t _Pkt_Sz, const uint16_t _Max_To, const uint16_t _Conn_To, const uint8_t _TX_To);

		/**
		 * @brief Set command sets the socket configuration extended parameters.
		 * @details AT Command : AT#SCFGEXT=[<conn_id>],[<srmode>],[<recvdatamode>],[<keepalive>],[<listenautorsp>],[<senddatamode>]\r\n (24 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @param _Conn_ID socket connection identifier (1-6)
		 * @param _Sr_Mode SRing unsolicited mode
		 * 0 - Normal (default)
		 * 1 – Data amount
		 * 2 - Data view
		 * 3 – Data view with UDP datagram informations
		 * @param _Recv_Data_Mode data view mode for received data
		 * 0- text mode (default)
		 * 1- hexadecimal mode
		 * @param _Keep_Alive Set the TCP Keepalive value in minutes
		 * 0 – Deactivated (default)
		 * 1 – 240 – Keepalive time in minutes
		 * @param _Listen_Auto_Rsp Set the listen auto-response mode, that affects the commands AT#SL and AT#SLUDP
		 * 0 - Deactivated (default)
		 * 1 – Activated
		 * @param _Send_Data_Mode data mode for sending data in command mode(AT#SSEND)
		 * 0 - data represented as text (default)
		 * 1 - data represented as sequence of hexadecimal numbers (from 00 to FF)
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SCFGEXT(const uint8_t _Conn_ID, const uint8_t _Sr_Mode, const uint8_t _Recv_Data_Mode, const uint8_t _Keep_Alive, const uint8_t _Listen_Auto_Rsp, const uint8_t _Send_Data_Mode);

		/**
		 * @brief Set command sets the socket configuration extended parameters 
		 * for features not included in #SCFGEXT command.
		 * @details AT Command : AT#SCFGEXT2=[<conn_id>],[<bufferstart>],[<abortconnattemp>],[<sringlen>],[<sringto>],[<nocarriermode>]\r\n (26 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @param _Conn_ID socket connection identifier (1-6)
		 * @param _Buffer_Start Set the sending timeout method based on new data received from the serial port. 
		 * (<txTo> timeout value is set by #SCFG command) Restart of transmission timer will be done when new 
		 * data are received from the serial port.
		 * 0 - old behaviour for transmission timer
		 * 1 - new behaviour for transmission timer
		 * @param _Abort_Conn_Attempt Enable connection attempt(#SD/#SKTD/#SKTOP) abort before 
		 * CONNECT(online mode) or OK(command mode)
		 * 0 – Not possible to interrupt connection attempt
		 * 1 – It is possible to interrupt the connection attempt
		 * @param _SRing_Len this parameter sets the length of data received in one 
		 * SRING URC in sring mode 2 or 3 ( see AT#SCFGEXT )
		 * 0 – factory default, means 64 bytes
		 * 1 – means that the length is equal to the maximum TCP payload size accepted 
		 * in download in case of TCP connections, same as 0 in case of UDP connections
		 * @param _SRing_To this parameter sets the delay among one SRING URC and 
		 * the other, in sring mode 2 or 3 ( see AT#SCFGEXT )
		 * 0 – factory default, means 10 hundreds of milliseconds 
		 * 1..10: value in hundreds of milliseconds
		 * @param _No_Carrier_Mode this parameter permits to choose NO CARRIER 
		 * indication format when the socket is closed as follows
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SCFGEXT2(const uint8_t _Conn_ID, const uint8_t _Buffer_Start, const uint8_t _Abort_Conn_Attempt, const uint8_t _SRing_Len, const uint8_t _SRing_To, const uint8_t _No_Carrier_Mode);

		/**
		 * @brief Set command sets the socket configuration extended parameters 
		 * for features not included in #SCFGEXT command nor in #SCFGEXT2 command.
		 * @details AT Command : AT#SCFGEXT3=[<conn_id>],[<immrsp>],[<closuretypecmdmodeenabling>]\r\n (19 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @param _Conn_ID socket connection identifier (1-6)
		 * @param _Imm_Rsp Enables AT#SD command mode immediate response
		 * 0 – factory default, means that AT#SD in command mode (see AT#SD) returns after the socket is connected
		 * 1 – means that AT#SD in command mode returns immediately. Then the state of the connection can be read by the AT command AT#SS
		 * @param _Closure_Type_Cmd_Mode_Enabling Setting this parameter, successive #SD or #SL with 
		 * <closureType> parameter 255 setting takes effect in command mode. It has been introduced 
		 * due to retrocompatibility reason regarding <closureType> behaviour in command mode.
		 * 0 – #SD or #SL <closureType> 255 in command mode has no effect 
		 * 1 – #SD or #SL <closureType> 255 in command mode takes effect
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SCFGEXT3(const uint8_t _Conn_ID, const uint8_t _Imm_Rsp, const uint8_t _Closure_Type_Cmd_Mode_Enabling);

		/**
		 * @brief Execution command is used to activate or deactivate either the 
		 * GSM context or the specified PDP context.
		 * @details AT Command : AT#SGACT=<cid>,<stat>,[<userid>],[<password>]\r\n (20 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.01
		 * 
		 * @param _Cid PDP context identifier
		 * 0 - specifies the GSM context
		 * 1..5 - numeric parameter which specifies a particular PDP context definition
		 * @param _Stat status identifier
		 * 0 - deactivate the context
		 * 1 - activate the context
		 * @param _User_ID string type, used only if the context requires it
		 * @param _Password string type, used only if the context requires it
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SGACT(const uint8_t _Cid);

		/**
		 * @brief Execution command is used to enable or disable the automatic activation/reactivation of the 
		 * context for the specified PDP context, to set the maximum number of attempts and to set the delay 
		 * between an attempt and the next one. The context is activated automatically after every GPRS Attach 
		 * or after a NW PDP CONTEXT deactivation if at least one IPEasy socket is configured to this context.
		 * 
		 * @version 01.00.00
		 * 
		 * @param _Cid PDP context identifier
		 * 0 - specifies the GSM context
		 * 1..5 - numeric parameter which specifies a particular PDP context definition
		 * @param _Retry numeric parameter which specifies the maximum number of context activation attempts 
		 * in case of activation failure. The value belongs to the following range: 0 - 15
		 * 0 - disable the automatic activation/reactivation of the context (default)
		 * @param _Delay numeric parameter which specifies the delay in seconds between an attempt 
		 * and the next one. The value belongs to the following range: 180 - 3600
		 * @param _UrcMode URC presentation mode
		 * 0 - disable unsolicited result code (default)
		 * 1 - enable unsolicited result code, after an automatic activation/reactivation, 
		 * of the local IP address obtained from the network.
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SGACTCFG(const uint8_t _Cid, const uint8_t _Retry, const uint16_t _Delay, const bool _UrcMode);

	bool CGACT(const uint8_t _State, const bool _Cid);
	bool CGPADDR(const uint8_t _Cid);

		/**
		 * @brief This command enables and disables automatic time zone update via NITZ.
		 * @details AT Command : AT+CTZU=[<state>]\r\n (9 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @param _State Parameter
		 * 0 Disable automatic time zone update via NITZ (default) 
		 * 1 Enable automatic time zone update via NITZ
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CTZU(const bool _State);

		/**
		 * @brief Set command enables/disables automatic date/time updating and Network Timezone 
		 * unsolicited indication. Date and time information can be sent by the network after 
		 * GSM registration or after GPRS attach.
		 * @details AT Command : AT+NITZ=[<state>]\r\n (11 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @param _State Parameter
		 * 0 - disables automatic set (factory default)
		 * 1 - enables automatic set
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool NITZ(const bool _State);

		/**
		 * @brief This command permits to calculate and update date and time through 
		 * NTP protocol(RFC2030), sending a request to a NTP server.
		 * @details AT Command : AT#NTP="<NTPserver>",<NTPport>,<updatemodule>,<timeout>\r\n (x Byte)
		 * @details AT Response : \r\n#NTP: 20/10/16,08:55:58\r\nOK\r\n (31 byte)
		 * 
		 * @param _NTP_Addr address of the NTP server, string type.
		 * @param _NTP_Port NTP server port to contact
		 * @param _Update_Module_Clock 
		 * 0 - no update module clock 
		 * 1 – update module clock
		 * @param _Time_Out waiting timeout for server response in seconds (1-10 sec)
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool NTP(const char *_NTP_Addr, const uint8_t _NTP_Port, const bool _Update_Module_Clock, const uint8_t _Time_Out);

		/**
		 * @brief Set command sets the real-time clock of the ME.
		 * @details AT Command : AT+CCLK?\r\n (10 Byte)
		 * @details AT Response : \r\n+CCLK: "21/12/30,12:37:34"\r\nOK\r\n (34 byte)
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CCLK(void);

		/**
		 * @brief Execution command causes device detach from the network and shut down. 
		 * Before definitive shut down an OK response is returned.
		 * @details AT Command : AT#SHDN\r\n (9 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool SHDN(void);
		
		bool SD(const uint8_t _Cid, const uint8_t _Pro, const uint8_t _Port, const char *_IP);
	
		/**
		 * @brief This command sets the parameters needed to the HTTP connection.
		 * 
		 * @version 01.00.00
		 * 
		 * @param _ProfID Numeric parameter indicating the profile identifier.
		 * @param _HTTP_Server String parameter indicating the IP address of the HTTP server.
		 * @param _Port Numeric parameter indicating the TCP remote port of the HTTP server to connect to.
		 * @param _AuthType Numeric parameter indicating the HTTP authentication type.
		 * 0 – no authentication (default) 
		 * 1 – basic authentication
		 * @param _Username String parameter indicating authentication user identification string for HTTP.
		 * @param _Password String parameter indicating authentication password for HTTP.
		 * @param _SSL Numeric parameter indicating if the SSL encryption is enabled.
		 * 0 – SSL encryption disabled (default) 
		 * 1 – SSL encryption enabled
		 * @param _TimeOut Numeric parameter indicating the time interval in seconds to wait for receiving data from HTTP server.
		 * Range: (1- 65535). Default: 120.
		 * @param _Cid Numeric parameter indicating the PDP Context Identifier.
		 * Range: (1-5). Default: 1
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool HTTPCFG(const uint8_t _ProfID, const char *_HTTP_Server, const uint8_t _Port, const uint8_t _AuthType, const char *_Username, const char *_Password, const uint8_t _SSL, const uint8_t _TimeOut, const uint8_t _Cid);
		
	private:

		/**
		 * @brief Clear GSM serial communication buffer.
		 * @version 01.00.00
		 */
		void _Clear_UART_Buffer(void);	

		/**
 		* @brief Waits serial stream message for specified length.
 		* @version 01.00.00
 		* @param _Length - Message length to wait.
 		* @param _TimeOut - Wait time out.
 		* @return true - Message recieved.
 		* @return false - Message can not recieved.
 		*/
		bool _Response_Wait(uint16_t _Length, uint32_t _TimeOut);

};

/**
 * @brief GSM Modem RTC function class.
 * @version 01.00.00
 */
class xE910_RTC {

	public:

		bool Time_Update(void);

		uint16_t Year(void);
		uint8_t Month(void);
		uint8_t Day(void);
		uint8_t Hour(void);
		uint8_t Minute(void);
		uint8_t Second(void);


	private:

};

extern xE910_GSM GSM;
extern xE910_HARDWARE GSM_HARDWARE;
extern xE910_AT GSM_AT;
extern xE910_RTC GSM_RTC;

#endif /* defined(__Telit_xE910__) */
