#ifndef __Telit_xE910_Includes__
#define __Telit_xE910_Includes__

	// Define Terminal Defaults
	#include "Terminal_Defaults.h"

	// Define AT Command Defaults	
	#include "AT_Definitions.h"
	
	
	
	
	
	





	
	
	
	








	// Define Console Library
	#ifdef GSM_Debug

		#ifndef __Console__
			#include <Console.h>
		#endif

		// Define Console Library
		Console Terminal_GSM(Serial_Terminal);

	#endif

#endif