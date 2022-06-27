#ifndef __AT_Definition__
#define __AT_Definition__

	// Serial Communications Definations
	#define Serial_Terminal 			Serial
	#define Serial_GSM 					Serial3

	// At Command Set Settings
	#define _AT_
	#define _AT_ATE_
	#define _AT_CMEE_
	#define _AT_FCLASS_
	#define _AT_K_
	#define _AT_CPIN_
	#define _AT_CGSN_
	#define _AT_GSN_
	#define _AT_GMI_
	#define _AT_GMM_
	#define _AT_GMR_
	#define _AT_ICCID_
	#define _AT_SLED_
	#define _AT_E2SLRI_
	//#define _AT_E2RI_
	#define _AT_REGMODE_
	#define _AT_TXMONMODE_
	#define _AT_COPS_
	#define _AT_AUTOBND_
	#define _AT_CREG_
	#define _AT_CGREG_
	#define _AT_CGDCONT_
	#define _AT_SERVINFO_
	#define _AT_MONI_
	#define _AT_SGACT_
	#define _AT_CSQ_
	#define _AT_FRWL_
	#define _AT_ICMP_
	#define _AT_SCFG_2_
	#define _AT_SCFG_3_

	// AT Command Definitions
	#ifdef _AT_

		// ATE - Echo Configuration Parameters (Disable)
		#ifdef _AT_ATE_
			#define _AT_ATE_Parameter_			(bool)0
		#endif

		// CMEE - Numeric Error Format
		#ifdef _AT_CMEE_
			#define _AT_CMEE_Parameter_			(uint8_t)1
		#endif

		// FCLASS -
		#ifdef _AT_FCLASS_
			#define _AT_FCLASS_Parameter_		(uint8_t)0
		#endif

		// K - 
		#ifdef _AT_K_
			#define _AT_K_Parameter_			(uint8_t)0
		#endif

		// SLED -
		#ifdef _AT_SLED_
			#define _AT_SLED_Parameter			(uint8_t)2
		#endif

		// E2SLRI - 
		#ifdef _AT_E2SLRI_
			#define _AT_E2SLRI_Parameter_		(uint8_t)50
		#endif

		// E2RI - 
		#ifdef _AT_E2RI_
			#define _AT_E2RI_Parameter_Mask_		(uint8_t)12 // 0x12
			#define _AT_E2RI_Parameter_Duration_	(uint8_t)50
		#endif

		// REGMODE - 
		#ifdef _AT_REGMODE_
			#define _AT_REGMODE_Parameter_		(uint8_t)0
		#endif

		// TXMONMODE -
		#ifdef _AT_TXMONMODE_
			#define _AT_TXMONMODE_Parameter_	(uint8_t)1
		#endif

		// AUTOBND - 
		#ifdef _AT_AUTOBND_
			#define _AT_AUTOBND_Parameter		(uint8_t)1
		#endif

		// CREG -
		#ifdef _AT_CREG_
			#define _AT_CREG_Parameter			(uint8_t)0
		#endif

		// CGREG -
		#ifdef _AT_CGREG_
			#define _AT_CGREG_Parameter			(uint8_t)0
		#endif

		// CGDCONT APN Configuration Parameters
		#ifdef _AT_CGDCONT_
			#define _AT_CGDCONT_Parameter_Cid_			(uint8_t)1
			#define _AT_CGDCONT_Parameter_PDP_			"IP"
			#define _AT_CGDCONT_Parameter_APN_			"mgbs"
			#define _AT_CGDCONT_Parameter_PDP_Addr_		"0.0.0.0"
			#define _AT_CGDCONT_Parameter_DComp_		(bool)0
			#define _AT_CGDCONT_Parameter_HComp_		(bool)0
		#endif

		// SGACT Configuration Parameters
		#ifdef _AT_SGACT_
			#define _AT_SGACT_Parameter_Cid_			(uint8_t)1
			#define _AT_SGACT_Parameter_State_			(uint8_t)1
		#endif

		// ICMP -
		#ifdef _AT_ICMP_
			#define _AT_ICMP_Parameter_Mode_				(uint8_t)1
		#endif

		// Socket 2
		#ifdef _AT_SCFG_2_

			// SCFG - Socket Configuration Parameters
			#define _AT_SCFG2_Cid_				(uint8_t)1
			#define _AT_SCFG2_PktSz_			(uint16_t)1500
			#define _AT_SCFG2_MaxTo_			(uint8_t)90
			#define _AT_SCFG2_ConnTo_			(uint16_t)300
			#define _AT_SCFG2_TXTo_				(uint8_t)50

			// SCFGEXT2 - Socket Configuration Extended Parameters
			#define _AT_SCFGEXT2_SrMode_		(uint8_t)1
			#define _AT_SCFGEXT2_RcvDataMode_	(uint8_t)0
			#define _AT_SCFGEXT2_KeepAlive_		(uint8_t)1
			#define _AT_SCFGEXT2_AutoResp_		(uint8_t)0
			#define _AT_SCFGEXT2_SndDataMode_	(uint8_t)0

		#endif

		// Socket 3
		#ifdef _AT_SCFG_3_

			// SCFG - Socket Configuration Parameters
			#define _AT_SCFG3_Cid_				(uint8_t)1
			#define _AT_SCFG3_PktSz_			(uint16_t)1500
			#define _AT_SCFG3_MaxTo_			(uint8_t)90
			#define _AT_SCFG3_ConnTo_			(uint16_t)300
			#define _AT_SCFG3_TXTo_				(uint8_t)50

			// SCFGEXT2 - Socket Configuration Extended Parameters
			#define _AT_SCFGEXT3_SrMode_		(uint8_t)1
			#define _AT_SCFGEXT3_RcvDataMode_	(uint8_t)0
			#define _AT_SCFGEXT3_KeepAlive_		(uint8_t)1
			#define _AT_SCFGEXT3_AutoResp_		(uint8_t)0
			#define _AT_SCFGEXT3_SndDataMode_	(uint8_t)0

		#endif

	#endif

	// Define Console Library
	#include "Console_Parameters.h"
	#ifdef GSM_Debug

		#ifndef __Console__
			#include <Console.h>
		#endif

		Console Terminal_GSM(Serial_Terminal);

	#endif

	// Define JSON Handler
	#include <ArduinoJson.h>

	// Define Modem Hardware Library
	#ifndef __Modem_Hardware__
		#include "Modem_Hardware.h"
	#endif

	// Define Hardware Object
	Modem_Hardware GSM_Hardware;

	// Define AT Command Library
	#ifndef __AT_Command_Set__
		#include "AT_Command_Set.h"
	#endif

	// Define AT Command Object
	AT_Command_Set AT(Serial_GSM);

	// Define Socket Command Library
	#ifndef __GSM_Socket__
		#include "Socket.h"
	#endif

#endif /* defined(__AT_Definition__) */