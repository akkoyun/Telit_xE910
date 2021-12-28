#ifndef __Telit_xE910_Structures__
#define __Telit_xE910_Structures__

/**
 * GSM serial communication channel definations. 
 * Modem communicates with microcontroller over UART interface.
 * version 01.00.00
 */
#define		GSM_Serial					Serial				/// GSM Serial communication defination.
#define		GSM_Serial_Baud				115200				/// GSM Serial terminal connection speed defination.

/**
 * Library global variables declarations.
 */
const char 	Version[9] 					= "01.00.03";		/// Library Version


/**
 * @brief Command control structure.
 */
struct Command_Control_Struct {
	bool								ATE;				/// ATE Command Control Variable
	bool								CMEE;				/// CMEE Command Control Variable
	bool								FCLASS;				/// FCLASS Command Control Variable
};






#endif
