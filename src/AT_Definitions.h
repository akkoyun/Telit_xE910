#define _AT_
#define _AT_ATE_
#define _AT_CMEE_
#define _AT_FCLASS_
#define _AT_K_
#define _AT_SLED_
#define _AT_E2SLRI_

// RegMode Command
#define _AT_REGMODE_
#ifdef _AT_REGMODE_
	#define _AT_REGMODE_mode_ 0
#endif

// Cops Command
#define _AT_COPS_
#ifdef _AT_COPS_
	#define _AT_COPS_mode_ 0
	#define _AT_COPS_format_ 2
	#define _AT_COPS_oper_ 28601
#endif

// AutoBnd Command
#define _AT_AUTOBND_
#ifdef _AT_AUTOBND_
	#define _AT_AUTOBND_value_ 1
#endif

#define _AT_SCFG_Out_
#define _AT_SCFGEXT_Out_
#define _AT_SCFG_In_
#define _AT_SCFGEXT_In_




#define _AT_TXMONMODE_
#define _AT_CREG_
#define _AT_CGREG_
#define _AT_CGDCONT_
#define _AT_SGACT_
#define _AT_ICMP_
#define _AT_CPIN_
#define _AT_CGSN_
#define _AT_GSN_
#define _AT_GMI_
#define _AT_GMM_
#define _AT_GMR_
#define _AT_ICCID_
//#define _AT_SERVINFO_
#define _AT_MONIZIP_
#define _AT_CSQ_

// Firewall Parameters
#define _AT_FRWL_1_
#define _AT_FRWL_1_IP_						"213.14.250.214"

#define _AT_FRWL_2_
#define _AT_FRWL_2_IP_						"167.99.137.254"

#define _AT_FRWL_3_
#define _AT_FRWL_3_IP_						"176.240.98.205"
