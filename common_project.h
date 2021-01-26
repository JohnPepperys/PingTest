/*  --------------------------------- 
#//Project:		start_project
#//File:		commom_project.h
#//Date:		01.10.2015
#//Author:		T.O.
#//Version:		1.1

#//
#//	Common header for project
#------------------------------------*/

#include <stdio.h>
#include <time.h>

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#include <windows.h>


// -------------------------------- define for ERRORs ------------------------
#define MAXERRORS			15
// коды ошибок
#define ERR_SUCCESS			0
#define ERR_NULLSTR			1
#define ERR_OPENFILE		2
#define ERR_READFILE		3
#define ERR_BAD_PARAM		4
#define ERR_CONFIG_PARAM	5
#define ERR_SHORTCONFFILE	6
#define ERR_XMLFILE			7
#define ERR_NORULES			8

#define ERR_COMMANDLINE		9
#define ERR_OPENDRIVER		10
#define ERR_CONNECTDRIVER			11

#define ERR_CREATEDIRECTORY			12
#define ERR_TOO_MANY_REQ_PARAM		13
#define ERR_CREATELOGFILE			14
#define ERR_EMPTYERROR				15


// ---------- define for LOG subsystem ------------------------------------
#define LOG_MAX_MESSAGE_LEN				1024
#define LOG_FILE_NAME					"ping_test_log.log"
#define MAX_DIR_FILE_LEN				256


// ---------- define for ping subsystem -----------------------------------
#define  PN_DESTIPADDR					"192.168.4.23"
#define  PN_IP_ADDRESS_LEN				16
#define  PN_MAC_ADDRESS_LEN				18
#define  PN_ETHER_ADDRESS_LEN			6


// ---------- define for loading configure subsystem ----------------------
#define CFG_CONFIG_FILE					"ping_test.cfg"
#define CFG_MAX_LEN_CONFIG_STRING		300

#define CFG_VERBOSE_BEGIN				1
#define CFG_TIMOUT_BEGIN				6000
#define CFG_PING_PERIOD_BEGIN			20
#define CFG_IP_TARGET_BEGIN				"192.168.4.254"
#define CFG_LOG_FILENAME_BEGIN			"ping_test.log"
#define CFG_RESULT_FILENAME_BEGIN		"ping_result.log"
#define CFG_TEST_COUNT_BEGIN			3

#define CFG_TIMOUT_MIN					500
#define CFG_TIMOUT_MAX					100000
#define CFG_PING_PERIOD_MIN				4				
#define CFG_PING_PERIOD_MAX				3600
#define CFG_TEST_COUNT_MIN				1
#define CFG_TEST_COUNT_MAX				20

#define CFG_MAX_IP_TARGET				4
// --------------------- struct defines --------------------
typedef struct main_config_param
{
	int size;							// struct size
	int verbose;						// verbose = 1 - debug print on, verbose = 0 - debug print off
	int timout;							// ping timeout in miliseconds
	int ping_period;					// time between each ping in seconds
	int test_count;						// count of success/fail operation
	unsigned char ip_target[CFG_MAX_IP_TARGET][PN_IP_ADDRESS_LEN];			// ip adress
	unsigned char log_filename[MAX_DIR_FILE_LEN];		// filename for file where is log
	unsigned char result_logfile[MAX_DIR_FILE_LEN];		// filename for file where is result
	int ip_target_on[CFG_MAX_IP_TARGET];				// element of massive make = 1, if define it target
	
} MAIN_CONFIG, * PMAIN_CONFIG;


// ------------------------ external data ------------------------------
extern unsigned char *errmessages[];
extern MAIN_CONFIG cfg_Maindata;

// ------------------------ exernal functions --------------------------
extern void cfg_myCopyParameters( int pf );

// ------------------- end of file   common_project.h ---------------------