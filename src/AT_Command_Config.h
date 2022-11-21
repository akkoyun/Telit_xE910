// Include AT Command Index
#include "AT_Command_Index.h"

// ATE
#ifdef _AT_ATE_

	/*
		ATE=n - Set command enables/disables the command echo.
		0 - disables command echo
		1 - enables command echo (factory default), hence command sent to the device are echoed back to the DTE before the response is given.
	*/
	#ifndef _AT_ATE_n_
		#define _AT_ATE_n_					(bool)0
	#endif

#endif

// CMEE
#ifdef _AT_CMEE_

	/*
		AT+CMEE=n - Set command enables/disables the report of result code.
		0 - disable +CME ERROR:<err> reports, use only ERROR report.
		1 - enable +CME ERROR:<err> reports, with <err> in numeric format 
		2 - enable +CME ERROR: <err> reports, with <err> in verbose format
	*/
	#ifndef _AT_CMEE_n_
		#define _AT_CMEE_n_					(uint8_t)1
	#endif

#endif

// FCLASS
#ifdef _AT_FCLASS_
	
	/*
		AT+FCLASS=n - Set command sets the wireless module in specified connection mode (data, fax, voice), hence all the calls done afterwards will be data or voice.
		0 - data
		1 - fax class 1 
		8 - voice
	*/
	#ifndef _AT_FCLASS_n_
		#define _AT_FCLASS_n_				(uint8_t)0
	#endif

#endif

// K 
#ifdef _AT_K_
	
	/*
		AT&Kn - Set command controls the RS232 flow control behaviour.
		0 - no flow control
		1 - hardware mono-directional flow control (only CTS active)
		2 - software mono-directional flow control (XON/XOFF)
		3 - hardware bi-directional flow control (both RTS/CTS active) (factory default) 
		4 - software bi-directional with filtering (XON/XOFF)
		5 - pass through: software bi-directional without filtering (XON/XOFF)
		6 - both hardware bi-directional flow control (both RTS/CTS active) and software bi-directional flow control (XON/XOFF) with filtering
	*/
	#ifndef _AT_K_n_
		#define _AT_K_n_					(uint8_t)0
	#endif

#endif

// SLED
#ifdef _AT_SLED_

	/*
		AT#SLED - Set command sets the behaviour of the STAT_LED GPIO
		0 - GPIO tied Low
		1 - GPIO tied High
		2 - GPIO handled by Module Software
		3 - GPIO is turned on and off alternatively
	*/
	#ifndef _AT_SLED_Mode_
		#define _AT_SLED_Mode_				(uint8_t)2
	#endif

#endif

// E2SLRI
#ifdef _AT_E2SLRI_

	/*
		AT#E2SLRI=n - Set command enables/disables the Ring Indicator pin response to a Socket Listen connect and, if enabled, the duration of the negative going pulse generated on receipt of connect.
		0 - RI disabled for Socket Listen connect (factory default)
		50..1150 - RI enabled for Socket Listen connect; a negative going pulse is generated on receipt of connect and <n> is the duration in ms of this pulse.
	*/
	#ifndef _AT_E2SLRI_n_
		#define _AT_E2SLRI_n_				(uint8_t)50
	#endif

#endif

// E2RI
#ifdef _AT_E2RI_

	/*
		AT#E2RI=mask,duration

		Mask
		0 – disables all events hexadecimal number representing the list of events: Mode (same as AT#PSMRI=<duration>)
		1 – Power Saving
		2 – Socket Listen (same as AT#E2SLRI=<duration>)
		4 – OTA firmware upgrade (same as AT#OTASETRI=<duration>)
		8 – MT SMS has been received (same as AT#E2SMSRI=<duration>) 
		10 – +CREG will change status
		20 – +CGREG will change status
		40 – #QSS become 2 (SIM INSERTED and PIN UNLOCKED)
		80 – MO SMS has been delivered
		100 – Jamming Detection & Reporting (JDR)

		Duration
		50..1150 - the duration in ms of the pulse generated
	*/
	#ifndef _AT_E2RI_Parameter_event_mask_
		#define _AT_E2RI_Parameter_event_mask_	(uint8_t)12 // 0x12
	#endif
	#ifndef _AT_E2RI_Parameter_duration_
		#define _AT_E2RI_Parameter_duration_	(uint8_t)50
	#endif

#endif

// REGMODE
#ifdef _AT_REGMODE_

	/*
		AT#REGMODE=mode - There are situations in which the presentation of the URCs controlled by either +CREG and +CGREG are slightly different from ETSI specifications. We identified this behaviour and decided to maintain it as default for backward compatibility issues, while we’re offering a more formal ‘Enhanced Operation Mode’ through #REGMODE.
		0 - basic operation mode
		1 - enhanced operation mode
	*/
	#ifndef _AT_REGMODE_mode_
		#define _AT_REGMODE_mode_			(uint8_t)0
	#endif

#endif

// TXMONMODE
#ifdef _AT_TXMONMODE_

	/*
		AT#TXMONMODE=mode - Set TXMON pin behaviour.
		0 - TXMON pin goes high when a call is started and it drops down when the call is ended. It also goes high when a location update starts, and it drops down when the location update procedure stops. Finally it goes high during SMS transmission and receiving. Even if the TXMON in this case is set as GPIO in output, the read command AT#GPIO=5,2 returns #GPIO:2,0, as the GPIO is in alternate mode.
		1 - TXMON is set in alternate mode and the Timer unit controls its state. TXMON goes high before power ramps start raising and drops down after power ramps stop falling down. This behaviour is repeated for every transmission burst.
	*/
	#ifndef _AT_TXMONMODE_mode_
		#define _AT_TXMONMODE_mode_			(uint8_t)1
	#endif

#endif

// AUTOBND
#ifdef _AT_AUTOBND_

	/*
		AT#AUTOBND=value - Set command enables/disables the automatic band selection at power-on.
		0 - disables automatic band selection at power-on (default for all products) 
		1 - enables automatic band selection at power-on; +COPS=0 is necessary condition to effectively have automatic band selection at next power-on; the automatic band selection stops as soon as a GSM cell is found.
	*/
	#ifndef _AT_AUTOBND_value
		#define _AT_AUTOBND_value			(uint8_t)1
	#endif

#endif

// CREG
#ifdef _AT_CREG_

	/*
		AT+CREG=mode - Set command enables/disables network registration reports depending on the parameter <mode>.
		0 - disable network registration unsolicited result code (factory default) 
		1 - enable network registration unsolicited result code
		2 - enable network registration unsolicited result code with network Cell identification data
	*/
	#ifndef _AT_CREG_mode
		#define _AT_CREG_mode				(uint8_t)0
	#endif

#endif

// CGREG
#ifdef _AT_CGREG_

	/*
		AT+CGREG=n - Set command controls the presentation of an unsolicited result code
		0 - disable network registration unsolicited result code
		1 - enable network registration unsolicited result code; if there is a change in the terminal GPRS network registration status, it is issued the unsolicited result code:
		2 - enable network registration and location information unsolicited result code

		0 - not registered, terminal is not currently searching a new operator to register to
		1 - registered, home network
		2 - not registered, but terminal is currently searching a new operator to register to
		3 - registration denied 
		4 - unknown
		5 - registered, roaming
	*/
	#ifndef _AT_CGREG_n_
		#define _AT_CGREG_n_				(uint8_t)0
	#endif

#endif

// CGDCONT
#ifdef _AT_CGDCONT_

	/*
	AT+CGDCONT=cid,pdp,apn,pdpaddr,dcomp,hcomp - Set command specifies PDP context parameter values for a PDP context identified by the (local) context identification parameter, <cid>
	<cid> - (PDP Context Identifier) numeric parameter which specifies a particular PDP context definition.
		1..max - where the value of max is returned by the Test command
	<PDP_type> - (Packet Data Protocol type) a string parameter which specifies the type of packet data protocol 
		"IP" - Internet Protocol
	<APN> - (Access Point Name) a string parameter which is a logical name that is used to select the GGSN or the external packet data network. If the value is empty (“”) or omitted, then the subscription value will be requested. 
	<PDP_addr> - a string parameter that identifies the terminal in the address space applicable to the PDP. The allocated address may be read using the +CGPADDR command.
	<d_comp> - numeric parameter that controls PDP data compression
		0 - off (default if value is omitted)
		1 - on
	<h_comp> - numeric parameter that controls PDP header compression
		0 - off (default if value is omitted)
		1 - on
	*/
	#ifndef _AT_CGDCONT_Parameter_cid_
		#define _AT_CGDCONT_Parameter_cid_			(uint8_t)1
	#endif
	#ifndef _AT_CGDCONT_Parameter_pdp_
		#define _AT_CGDCONT_Parameter_pdp_			"IP"
	#endif
	#ifndef _AT_CGDCONT_Parameter_apn_
		#define _AT_CGDCONT_Parameter_apn_			"mgbs"
	#endif
	#ifndef _AT_CGDCONT_Parameter_pdpaddr_
		#define _AT_CGDCONT_Parameter_pdpaddr_		"0.0.0.0"
	#endif
	#ifndef _AT_CGDCONT_Parameter_dcomp_
		#define _AT_CGDCONT_Parameter_dcomp_		(bool)0
	#endif
	#ifndef _AT_CGDCONT_Parameter_hcomp_
		#define _AT_CGDCONT_Parameter_hcomp_		(bool)0
	#endif

#endif

// SGACT
#ifdef _AT_SGACT_

	/*
		AT#SGACT=cid,stst - Execution command is used to activate or deactivate either the GSM context or the specified PDP context.
		<cid> - PDP context identifier
		0 - specifies the GSM context
		1..5 - numeric parameter which specifies a particular PDP context definition
		<stat>
		0 - deactivate the context
		1 - activate the context
	*/
	#ifndef _AT_SGACT_Parameter_cid_
		#define _AT_SGACT_Parameter_cid_			(uint8_t)1
	#endif
	#ifndef _AT_SGACT_Parameter_state_
		#define _AT_SGACT_Parameter_state_			(uint8_t)1
	#endif

#endif

// ICMP
#ifdef _AT_ICMP_
	
	/*
	AT#ICMP=mode - Set command enables/disables the ICMP Ping support.
	0 - disable ICMP Ping support (default)
	1 - enable firewalled ICMP Ping support: the module is sending a proper ECHO_REPLY only to a subset of IP Addresses pinging it; this subset of IP Addresses has been previously specified through #FRWL (see) 
	2 - enable free ICMP Ping support; the module is sending a proper ECHO_REPLY to every IP Address pinging it.
	*/
	#ifndef _AT_ICMP_Parameter_mode_
		#define _AT_ICMP_Parameter_mode_				(uint8_t)1
	#endif

#endif

// FRWL
#ifdef _AT_FRWL_
	
	/*
	AT#FRWL=action,IP - Execution command controls the internal firewall settings.
		
		<action> - command action
		0 - remove selected chain
		1 - add an ACCEPT chain
		2 - remove all chains (DROP everything); <ip_addr> and <net_mask> has no meaning in this case.

		<ip_addr> - remote address to be added into the ACCEPT chain; string type, it can be any valid IP address in the format: xxx.xxx.xxx.xxx

	*/
	#define _AT_FRWL_IP1_				"213.14.250.214"
	#define _AT_FRWL_IP2_				"167.99.137.254"
	#define _AT_FRWL_IP3_				"176.240.98.205"
//	#define _AT_FRWL_IP4_				""
//	#define _AT_FRWL_IP5_				""

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

