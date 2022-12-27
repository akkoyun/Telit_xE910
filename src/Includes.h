#ifndef __Telit_xE910_Includes__
#define __Telit_xE910_Includes__

	// Define Terminal Defaults
	#include "Terminal_Defaults.h"

	// Define AT Command Defaults	
	#include "AT_Definitions.h"
	



	// Define Versions
	#ifndef __Hardware__
		#define __Hardware__ "00.00.00"
	#endif
	#ifndef __Firmware__
		#define __Firmware__ "00.00.00"
	#endif


	// Define Cloud Server
	#ifndef Cloud_Server
		#define Cloud_Server "165.227.154.147"
	#endif
	#ifndef Cloud_EndPoint
		#define Cloud_EndPoint "/"
	#endif
	
	// Define Pack Type
	#define Pack_Online 1
	#define Pack_Timed 2
	#define Pack_Interrupt 3
	#define Pack_Alarm 4
	#define Pack_Offline 5
	
	

	
	
	
	








	// Define Console Library
	#ifdef GSM_Debug

		#ifndef __Console__
			#include <Console.h>
		#endif

		// Define Console Library
		Console Terminal_GSM(Serial_Terminal);

	#endif

#endif