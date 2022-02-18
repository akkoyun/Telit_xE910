#ifndef __Telit_xE910_Structures__
#define __Telit_xE910_Structures__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Define VT100 Terminal
#ifndef __Console__
#include <Console.h>
#endif

// Define PGMspace Function
#include <avr/pgmspace.h>

// Include String Library
#include <string.h>

/**
 * GSM serial communication channel definations. 
 * Modem communicates with microcontroller over UART interface.
 * version 01.00.00
 */
#define		GSM_Serial					Serial3				/// GSM Serial communication defination.
#define		GSM_Serial_Baud				115200				/// GSM Serial terminal connection speed defination.

#define		Command_Delay				10

/**
 * @brief CREG and CGREG status defination structure
 */
enum GSM_Stat {
	NOT_REGISTERED						= 0,				// 0
	HOME_REGISTERED						= 1,				// 1
	SEARCHING							= 2,				// 2
	DENIED								= 3,				// 3
	UNKNOWN								= 4,				// 4
	ROAMING_REGISTERED					= 5					// 5
};

/**
 * @brief Library functions state defination structure
 */
enum Function_State {
	DISABLE								= 0,				// False
	ENABLE								= 1					// True
};

/**
 * @brief Library functions power state defination structure
 */
enum Power_State {
	NOT_POWERED							= 0,				// False
	POWERED								= 1					// True
};

/**
 * @brief GPRS connection state defination structure
 */
enum Connection_State {
	NOT_CONNECTED						= 0,				// No IP Address
	CONNECTED							= 1					// Valid IP Address
};

/**
 * @brief Server Communication Command Set
 */
enum Command_Set {
	HARD_RESET							= 100,				// Command Succesful
	SUCCESS								= 200,				// Command Succesful
	ALLREADY_STARTED					= 201,
	ALLREADY_STOPPED					= 202,
	ERROR_MP							= 203,
	ERROR_TH							= 204,
	ERROR_PHASE							= 205,
	PUMP_START							= 256,				// Pump Start Command
	PUMP_STOP							= 257,				// Pump Stop Command
	STATUS_UPDATE						= 262,				// Status Uğdate
	ILLEGAL								= 999
};

#endif
