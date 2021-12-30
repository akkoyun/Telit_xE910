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
 * @brief Command control structure.
 */
struct Command_Control_Struct {
	bool								ATE;				/// ATE Command Control Variable
	bool								CMEE;				/// CMEE Command Control Variable
	bool								FCLASS;				/// FCLASS Command Control Variable
	bool								K;					/// K Command Control Variable
	bool								CPIN;				/// CPIN Command Control Variable
	bool								CGSN;				/// CGSN Command Control Variable
	bool								GSN;				/// GSN Command Control Variable
	bool								ICCID;				/// ICCID Command Control Variable
	bool								GMI;				/// GMI Command Control Variable
	bool								GMM;				/// GMM Command Control Variable
	bool								GMR;				/// GMR Command Control Variable
	bool								SLED;				/// SLED Command Control Variable
	bool								TXMONMODE;			/// TXMONMODE Command Control Variable
	bool								REGMODE;			/// REGMODE Command Control Variable
	bool								CREG;				/// CREG Command Control Variable
	bool								CGREG;				/// CGREG Command Control Variable
	bool								CGDCONT;			/// CGDCONT Command Control Variable
	bool								SCFG;				/// SCFG Command Control Variable
	bool								SCFGEXT;			/// SCFGEXT Command Control Variable
	bool								SCFGEXT2;			/// SCFGEXT2 Command Control Variable
	bool								SCFGEXT3;			/// SCFGEXT3 Command Control Variable
	bool								SGACT;				/// SGACT Command Control Variable
	bool								CSQ;				/// CSQ Command Control Variable
	bool								SERVINFO;			/// SERVINFO Command Control Variable
	bool								CTZU;				/// CTZU Command Control Variable
	bool								NITZ;				/// NITZ Command Control Variable
};

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

#endif
