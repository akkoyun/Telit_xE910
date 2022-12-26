#ifndef __Telit_xE910_Includes__
#define __Telit_xE910_Includes__

	// Define Terminal Defaults
	#include "Terminal_Defaults.h"

	// Define AT Command Defaults	
	#include "AT_Definitions.h"
	




	// Define Cloud Server
	#ifndef Cloud_Server
		#define Cloud_Server "54.216.226.171"
	#endif
	#ifndef Cloud_EndPoint
		#define Cloud_EndPoint "/api/v1.1/p402"
	#endif
	
	
	
	



	
	
	
	








	// Define Console Library
	#ifdef GSM_Debug

		#ifndef __Console__
			#include <Console.h>
		#endif

		// Define Console Library
		Console Terminal_GSM(Serial_Terminal);

	#endif

#endif