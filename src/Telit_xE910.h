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

		void Initialize();
		void Power();

	private:
	
	
};

/**
 * @brief GSM Modem hardware function class.
 * @version 01.00.00
 */
class xE910_HARDWARE {

	public:

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
	 	* @brief Command control variable structure.
	 	*/
		Command_Control_Struct Command_Control {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		/**
		 * @brief Set function to enable or disable the command echo.
		 * @details AT Command : ATE[<n>]\r\n (6 Byte)
		 * @details AT Response : ATE[<n>]\r\n\r\nOK\r\n (12 Byte)
		 * 
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool GMR(void);

		/**
		 * @brief Set command sets the behaviour of the STAT_LED GPIO
		 * @details AT Command : AT#SLED=[<n>]\r\n (11 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
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
		 * @version 01.00.00
		 * 
		 * 0 - disable network registration unsolicited result code (factory default) 
		 * 1 - enable network registration unsolicited result code
		 * 2 - enable network registration unsolicited result code with network Cell identification data
		 * 
		 * @return true - Command successful
		 * @return false - Command fails
		 */
		bool CREG(void);

		/**
		 * @brief Set command controls the presentation of an unsolicited result code
		 * @details AT Command : AT+CGREG=[<mode>]\r\n (11 Byte)
		 * @details AT Response : \r\nOK\r\n (6 Byte)
		 * 
		 * @version 01.00.00
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
		bool CGREG(void);

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

extern xE910_AT GSM_AT;
extern xE910_HARDWARE GSM_HARDWARE;
extern xE910_GSM GSM;

#endif /* defined(__Telit_xE910__) */
