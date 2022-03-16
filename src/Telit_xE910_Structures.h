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

struct Struct_Modem {
	const char Version[9];
	bool Power_Monitor;
	bool SIM_Status;
	bool Initialize_Status;
	bool Connection_Status;
	bool CREG_Status;
	bool CGREG_Status;
};

struct Struct_Variables {
	char IMEI[17];				// IMEI Variable
	char Serial_Number[11];		// Serial Number Variable
	char ICCID[21];				// ICCID Variable
	uint8_t Manufacturer;		// Manufacturer Variable	
	uint8_t Model;				// Model Variable
	char Firmware[10];			// Modem Firmware Version Variable
	uint8_t RSSI;				// Signal Variable
	uint16_t Operator;			// Operator Variable
	uint8_t Connection_Time;	// Connection Time
	char IP_Address[16];		// IP Addres
};

struct Struct_Time {
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
};

struct Struct_Serial_Buffer {
	bool Response;
	uint8_t Read_Order;
	uint8_t Data_Order;
	char Buffer[255];
	uint16_t Time_Out;
};

enum Power_Enum {
	NOT_POWERED = 0,
	POWERED = 1
};

enum Connection_Enum {
	NOT_CONNECTED = 0,
	CONNECTED = 1
};

enum Stat_Enum {
	NOT_REGISTERED = 0,
	HOME_REGISTERED = 1,
	SEARCHING = 2,
	DENIED = 3,
	UNKNOWN	= 4,
	ROAMING_REGISTERED = 5
};

#endif
