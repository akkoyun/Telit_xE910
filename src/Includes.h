#ifndef __Telit_xE910_Includes__
#define __Telit_xE910_Includes__

	// Serial Communications Definations
	#define Serial_Terminal 			Serial

	// Define Terminal Defaults
	#define GSM_Debug
	#define CONSOLE_TEMPLATE_GE910

	// Define Console Library
	#ifdef GSM_Debug

		#ifndef __Console__
			#include <Console.h>
		#endif

		// Define Terminal Defaults
		#include "Terminal_Defaults.h"

		// Define Console Library
		Console Terminal_GSM(Serial_Terminal);

	#endif

	// Define Cloud Parameters
	#define PostOffice_Server 			"165.227.154.147"
	#define PostOffice_EndPoint 		"/"

	// Firewall Parameters
	#define _AT_FRWL_1_
	#define _AT_FRWL_1_IP_				"213.14.250.214"

	#define _AT_FRWL_2_
	#define _AT_FRWL_2_IP_				"167.99.137.254"

	#define _AT_FRWL_3_
	#define _AT_FRWL_3_IP_				"176.240.98.205"

	// Define Pack Type
	#define Pack_Online 				(uint8_t)1
	#define Pack_Timed 					(uint8_t)2
	#define Pack_Interrupt 				(uint8_t)3
	#define Pack_Alarm 					(uint8_t)4
	#define Pack_Offline 				(uint8_t)5

#endif