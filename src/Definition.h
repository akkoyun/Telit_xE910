#ifndef __AT_Definition__
#define __AT_Definition__

// Serial Communications Definations
#define Serial_GSM 					Serial3

// At Command Set Settings
#define _AT_

// ATE - Echo Configuration Parameters (Disable)
#define _AT_ATE_
#ifdef _AT_ATE_
#define _AT_ATE_Parameter_			(bool)0
#endif

// CMEE - Numeric Error Format
#define _AT_CMEE_
#ifdef _AT_CMEE_
#define _AT_CMEE_Parameter_			(uint8_t)1
#endif

// FCLASS -
#define _AT_FCLASS_
#ifdef _AT_FCLASS_
#define _AT_FCLASS_Parameter_		(uint8_t)0
#endif

// K - 
#define _AT_K_
#ifdef _AT_K_
#define _AT_K_Parameter_			(uint8_t)0
#endif

// CPIN - 
#define _AT_CPIN_

// CGSN - 
#define _AT_CGSN_

// GSN - 
#define _AT_GSN_

// GMI - 
#define _AT_GMI_

// GMM - 
#define _AT_GMM_

// GMR - 
#define _AT_GMR_

// ICCID - 
#define _AT_ICCID_

// SLED -
#define _AT_SLED_
#ifdef _AT_SLED_
#define _AT_SLED_Parameter			(uint8_t)2
#endif

// E2SLRI - 
#define _AT_E2SLRI_
#ifdef _AT_E2SLRI_
#define _AT_E2SLRI_Parameter_				(uint8_t)50
#endif

// E2RI - 
//#define _AT_E2RI_
#ifdef _AT_E2RI_
	#define _AT_E2RI_Parameter_Mask_		(uint8_t)12 // 0x12
	#define _AT_E2RI_Parameter_Duration_	(uint8_t)50
#endif

// REGMODE - 
#define _AT_REGMODE_
#ifdef _AT_REGMODE_
#define _AT_REGMODE_Parameter_		(uint8_t)0
#endif

// TXMONMODE -
#define _AT_TXMONMODE_
#ifdef _AT_TXMONMODE_
#define _AT_TXMONMODE_Parameter_	(uint8_t)1
#endif

// COPS - 
#define _AT_COPS_

// AUTOBND - 
#define _AT_AUTOBND_
#ifdef _AT_AUTOBND_
	#define _AT_AUTOBND_Parameter		(uint8_t)1
#endif

// CREG -
#define _AT_CREG_
#ifdef _AT_CREG_
	#define _AT_CREG_Parameter			(uint8_t)0
#endif

// CGREG -
#define _AT_CGREG_
#ifdef _AT_CGREG_
	#define _AT_CGREG_Parameter			(uint8_t)0
#endif

// CGDCONT APN Configuration Parameters
#define _AT_CGDCONT_
#ifdef _AT_CGDCONT_
	#define _AT_CGDCONT_Parameter_Cid_			(uint8_t)1
	#define _AT_CGDCONT_Parameter_PDP_			"IP"
	#define _AT_CGDCONT_Parameter_APN_			"mgbs"
	#define _AT_CGDCONT_Parameter_PDP_Addr_		"0.0.0.0"
	#define _AT_CGDCONT_Parameter_DComp_		(bool)0
	#define _AT_CGDCONT_Parameter_HComp_		(bool)0
#endif

// SERVINFO -
#define _AT_SERVINFO_

// SGACT Configuration Parameters
#define _AT_SGACT_
#ifdef _AT_SGACT_
	#define _AT_SGACT_Parameter_Cid_			(uint8_t)1
	#define _AT_SGACT_Parameter_State_			(uint8_t)1
#endif

// CSQ -
#define _AT_CSQ_

// FRWL
#define _AT_FRWL_

// ICMP -
#define _AT_ICMP_
#ifdef _AT_ICMP_
	#define _AT_ICMP_Parameter_Mode_				(uint8_t)1
#endif

// Socket 2
#define _SCFG_2_
#ifdef _SCFG_2_

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
#define _SCFG_3_
#ifdef _SCFG_3_

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









// Debug Parameters
#define GSM_Debug
#ifdef GSM_Debug

	// Terminal Coordinates
	#define Debug_Initialize_X 14
	#define Debug_Initialize_Y 33

	#define Debug_Connect_X 14
	#define Debug_Connect_Y 73

	#define Debug_Boot_X 29
	#define Debug_Boot_Y 10

#endif






// Define Console Library
#ifndef __Console__
#include <Console.h>
#endif

// Define Modem Hardware Library
#ifndef __Modem_Hardware__
#include "Modem_Hardware.h"
#endif

// Define AT Command Library
#ifndef __AT_Command_Set__
#include "AT_Command_Set.h"
#endif

// Define JSON Handler
#include <ArduinoJson.h>

#endif /* defined(__AT_Definition__) */