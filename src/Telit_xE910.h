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

class xE910_AT {

	public:

		/**
	 	* @brief Command control variable structure.
	 	*/
		Command_Control_Struct Command_Control {0, 0, 0, 0, 0, 0, 0, 0, 0};

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

class xE910_GSM {

	public:

		void Initialize();

	private:
	
	
};






extern xE910_AT GSM_AT;
extern xE910_GSM GSM;

#endif /* defined(__Energy__) */
