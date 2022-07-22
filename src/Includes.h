#ifndef __Telit_xE910_Includes__
#define __Telit_xE910_Includes__

	// Define JSON Handler
	#include <ArduinoJson.h>

	// Define AT Definition
	#include "Definition.h"

	// Define Console Library
	#ifdef GSM_Debug

		// Define Console Parameters
		#include "Console_Parameters.h"

		#ifndef __Console__
			#include <Console.h>
		#endif

		// Define Console Library
		Console Terminal_GSM(Serial_Terminal);

	#endif

	// Define Modem Hardware Library
	#include "Modem_Hardware.h"

	// Define AT Command Library
	#include "AT_Command_Set.h"

	// Define Socket Command Library
	#ifndef __GSM_Socket__
		#include "Socket.h"
	#endif

	// Define Hardware Object
	Modem_Hardware GSM_Hardware;

	// Define AT Command Object
	AT_Command_Set AT(Serial_GSM);

#endif