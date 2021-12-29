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
const char 	Version[9] 					= "01.00.22";		/// Library Version
bool		Power_Monitor				= false;			/// GSM Power Monitor Signal
uint64_t 	IMEI						= 0;				/// IMEI Variable
uint32_t 	Serial_Number				= 0;				/// Serial Number Variable
uint64_t 	ICCID						= 0;				/// ICCID Variable
uint8_t 	Manufacturer 				= 0;				/// Modem Manufacturer Variable
uint8_t 	Model 						= 0;				/// Modem Model Variable
char		Modem_Firmware[10]			= "";				/// Modem Firmware Version Variable
uint8_t		CREG_Status					= 0;				/// CREG Status Variable
uint8_t		CGREG_Status				= 0;				/// CGREG Status Variable
uint8_t		SGACT_Status				= 0;				/// SGACT Status Variable
char		IP_Address[16]				= "";				/// IP Address Variable
bool		IP_Status					= false;			/// Connection Status

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
