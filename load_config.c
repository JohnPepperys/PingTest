/* --------------------------------- 
#//Project:		Ping Testing
#//File:		load_config.c
#//Date:		01.10.2015
#//Author:		T.O.
#//Version:		1.1

#//
#//
#------------------------------------*/


#include "common_project.h"
#include <sys/stat.h>

#define CFG_COL_USING_WORDS   10

MAIN_CONFIG cfg_Maindata;
MAIN_CONFIG temp_data;
unsigned char * cfg_main_element[] =
{
	"verbose", 
	"timeout",
	"ping_period",
	"ip_address_target1",
	"log_filename",
	"result_filename",
	"test_count",
	"ip_address_target2",
	"ip_address_target3",
	"ip_address_target4",
};
unsigned char cfg_num_elements[CFG_COL_USING_WORDS];

void cfg_InitConfigSubsystem( void )
{
	memset( &cfg_Maindata, 0, sizeof( MAIN_CONFIG ) );
	memset( &temp_data, 0, sizeof( MAIN_CONFIG ) );
	memset( cfg_num_elements, 0, CFG_COL_USING_WORDS );
	cfg_Maindata.size = sizeof( MAIN_CONFIG );
	cfg_Maindata.verbose = CFG_VERBOSE_BEGIN;
	cfg_Maindata.timout = CFG_TIMOUT_BEGIN;
	cfg_Maindata.test_count = CFG_TEST_COUNT_BEGIN;
	cfg_Maindata.ping_period = CFG_PING_PERIOD_BEGIN;
	safe_strncpy( cfg_Maindata.ip_target[0], CFG_IP_TARGET_BEGIN, sizeof( CFG_IP_TARGET_BEGIN ) );
	cfg_Maindata.ip_target_on[0] = 1;
	safe_strncpy( cfg_Maindata.log_filename, CFG_LOG_FILENAME_BEGIN, sizeof( CFG_LOG_FILENAME_BEGIN ) );
	safe_strncpy( cfg_Maindata.result_logfile, CFG_RESULT_FILENAME_BEGIN, sizeof( CFG_RESULT_FILENAME_BEGIN ) );
}


// ---------------------------------------------------------------------


int cfg_ReadConfig( int printflag )
{
	// printflag == 0 - no debug printf, printflag == 1 - only debug print, printflag == 2 - only log print,  printflag == 3 - both log and debug print

FILE *cfgFile = NULL;
unsigned char tempstr[CFG_MAX_LEN_CONFIG_STRING];
struct _stat fileinfo;
int strlen, colstr, k;
unsigned char logmess[2*CFG_MAX_LEN_CONFIG_STRING];
	
	// check input param
	if( printflag < 0 || printflag > 3 )
	{
		if( (printflag & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Error in input param\n" );
		}
		return ERR_NULLSTR;
	}
	
	memset( &temp_data, 0, sizeof(MAIN_CONFIG) );
	// read data about file
	if( (strlen = _stat( CFG_CONFIG_FILE, &fileinfo ) ) != 0 )
	{
		if( (printflag & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Error in read file %s struct decription\n", CFG_CONFIG_FILE );
		}
		if( (printflag & 2) > 0 )
		{
			sprintf( logmess, "Error read file: %s", CFG_CONFIG_FILE );
			logMessage( logmess, strlength( logmess )+1 );
		}
		return ERR_READFILE;
	}

	// open file for work with it
	if( (cfgFile = fopen( CFG_CONFIG_FILE, "r" )) == NULL )
	{
		if( (printflag & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Error in open cfg file %s.\n", CFG_CONFIG_FILE );
		}
		if( (printflag & 2) > 0 )
		{
			sprintf( logmess, "Error read file: %s", CFG_CONFIG_FILE );
			logMessage( logmess, strlength( logmess )+1 );
		}
		return ERR_OPENFILE;
	}
	else
	{
		// read each string from file
		for( colstr = 0; strlen < fileinfo.st_size; colstr++ )
		{
			memset( tempstr, 0, CFG_MAX_LEN_CONFIG_STRING );
			if( ( fgets( tempstr, CFG_MAX_LEN_CONFIG_STRING, cfgFile ) ) == NULL )
			{
				fclose( cfgFile );
				// printf();;
				if( (printflag & 1) > 0 )
				{
					printf( "\tcfg_ReadConfig. Error in read string %d from file %s\n", colstr, CFG_CONFIG_FILE );
				}
				return ERR_READFILE;
			}
			else
			{
				if( strlength( tempstr ) >=  CFG_MAX_LEN_CONFIG_STRING )
				{
					fclose( cfgFile );
					if( (printflag & 1) > 0 )
					{
						printf( "\tcfg_ReadConfig. More length of string %d from file %s.\n", colstr, CFG_CONFIG_FILE );
					}
					return ERR_EMPTYERROR;
				}
				strlen += strlength( tempstr ) + 1;
				//printf( "\tbefore analys str: -%s-  len: %d\n", tempstr, strlength(tempstr) );
				k = cfg_AnalysString( tempstr, strlength(tempstr)+1, printflag );
				if( (k != ERR_SUCCESS) && (k != ERR_NULLSTR) )
				{
					if( (printflag & 1) > 0 )
					{
						printf( "\tcfg_ReadConfig. %s: %s\n", errmessages[k], tempstr );
					}
					if( (printflag & 2) > 0 )
					{
						sprintf( logmess, "Error read config file: %s. Error in string: %s", CFG_CONFIG_FILE, tempstr );
						logMessage( logmess, strlength( logmess )+1 );
					}
					return k;
				}
			}
			
		}
	}

	// check ip - address to differents in it
	if( cfg_CheckDiffIPaddr( printflag ) != ERR_SUCCESS )
	{
		return ERR_CONFIG_PARAM;
	} 
	//printf( "qqqq! func cfg_CheckDiffIPaddr return %d\n", cfg_CheckDiffIPaddr( printflag ) );
	
	if( (printflag & 1) > 0 )
	{
		printf( "cfg_ReadConfig. Read config from file: %s, filelen: %d,   load %d byte,   %d string\n", 
				CFG_CONFIG_FILE, fileinfo.st_size, strlen, colstr );
	}
	if( (printflag & 2) > 0 )
	{
		sprintf( logmess, "Read config success. Read config from file: %s, filelen: %d,   load %d byte,   %d string.", 
				CFG_CONFIG_FILE, fileinfo.st_size, strlen, colstr );
		logMessage( logmess, strlength( logmess )+1 );
	}
	
	// copy param from temp to global struct
	cfg_myCopyParameters( printflag );
	
	return ERR_SUCCESS;
}


// ---------------------------------------------------------------------------------------------------

int cfg_AnalysString( unsigned char *str, int slen, int printflag )
{
	// check input parameters
	if( str == NULL )
	{
		return ERR_NULLSTR;
	}
	if( (slen < 3) || (slen > CFG_MAX_LEN_CONFIG_STRING ) )
	{
		return ERR_NULLSTR;
	}
	
	// -------- analys string ----------
	
	// check on commented string
	if( str[0] == '#' )
	{
		return ERR_SUCCESS;
	}

	// exclude space symbol
	rmspacestr( str, &slen );
	// printf( "len: %d   str: -%s-   fsym %d\n", slen, str, str[0] );
	return cfg_AnalysStringParameter( str, slen, printflag );
}

// ---------------------------------------------------------------------------------------------------

int cfg_AnalysStringParameter( unsigned char *str, int slen, int printflag )
{

unsigned char temp[CFG_MAX_LEN_CONFIG_STRING];
unsigned char pvalue[CFG_MAX_LEN_CONFIG_STRING];
int i, k, j, vallen = 0;

	// check input parameters
	if( str == NULL )
	{
		return ERR_NULLSTR;
	}
	if( (slen < 3) || (slen > CFG_MAX_LEN_CONFIG_STRING ) )
	{
		return ERR_NULLSTR;
	}

	memset( temp, 0, CFG_MAX_LEN_CONFIG_STRING );
	memset( pvalue, 0, CFG_MAX_LEN_CONFIG_STRING );
	// exlude part of string before '='
	for( i = 0; i < slen; i++ )
	{
		if( str[i] == '=' )
		{
			break;
		}
		temp[i] = str[i];
	}
	
	// compare first part with using words
	for( k = 0; k < CFG_COL_USING_WORDS; k++ )
	{
		//printf( "\tcfg_AnalysStringParameter. str1: -%s-, l1: %d,  str2: -%s-, l2: %d\n", temp, strlength(temp), cfg_main_element[k], strlength( cfg_main_element[k] ) );
		if( mystrncmp( temp, strlength(temp), cfg_main_element[k], strlength( cfg_main_element[k] ) ) == ERR_SUCCESS )
		{
			if( cfg_num_elements[k] > 0 )
			{
				// printf - такой параметр уже был
				return ERR_CONFIG_PARAM;
			}
			else
			{
				i += 1;
				for( j = 0; i < slen; i++, j++ )
				{
					pvalue[j] = str[i];
				}
				// printf( "\t\tcfg_AnalysStringParameter. j: %d,   pval: -%s-,  num k: %d\n", j, pvalue, k );
				return cfg_AnalysValueParameter( pvalue, j, k, printflag );
			}
		}
	}
	if( k == CFG_COL_USING_WORDS )
	{
		// printf - нет такого параметра
		return ERR_BAD_PARAM;
	}
	
	return ERR_SUCCESS;
}


// ------------------------------------------------------------------------------------------------------ 


int cfg_AnalysValueParameter( unsigned char * pvalue, int vallen, int valnum, int printflag )
{
int i, k;
	
	// check input parameters
	if( pvalue == NULL )
	{
		return ERR_NULLSTR;
	}
	if( (vallen < 1) || ( vallen > (CFG_MAX_LEN_CONFIG_STRING-2) ) )
	{
		return ERR_NULLSTR;
	}

/*	// delete '"' from string 	
	if( pvalue[0] == '"' )
	{
		pvalue = &pvalue[1];
	}
	if( pvalue[vallen-2] == '"' )
	{
		pvalue[vallen-2] = 0;
		vallen--;
	} */
	
	switch( valnum )
	{
		case 0:		// verbose
			if( mystrtoint( pvalue, vallen, &i ) != ERR_SUCCESS )
			{
				return ERR_CONFIG_PARAM;
			}
			if( ( i > -1 ) &&  ( i < 2 ) )
			{
				temp_data.verbose = i;
			}
			else
			{
				return ERR_CONFIG_PARAM;
			}
			cfg_num_elements[valnum]++;
			break;
			
		case 1:		// timout
			if( mystrtoint( pvalue, vallen, &i ) != ERR_SUCCESS )
			{
				return ERR_CONFIG_PARAM;
			}
			if( ( i >= CFG_TIMOUT_MIN ) && ( i <= CFG_TIMOUT_MAX ) )
			{
				cfg_num_elements[valnum]++;
				temp_data.timout = i;
			}
			else
			{
				return ERR_CONFIG_PARAM;
			}
			break;
			
		case 2:		// ping_period
			if( mystrtoint( pvalue, vallen, &i ) != ERR_SUCCESS )
			{
				return ERR_CONFIG_PARAM;
			}
			if( ( i >= CFG_PING_PERIOD_MIN ) && ( i <= CFG_PING_PERIOD_MAX ) )
			{
				temp_data.ping_period = i;
				cfg_num_elements[valnum]++;
			}
			else
			{
				return ERR_CONFIG_PARAM;
			}
			break;
			
		case 3:		// ip_address_target 1
			if( vallen > PN_IP_ADDRESS_LEN )
			{
				return ERR_CONFIG_PARAM;
			}
			else
			{
				if( verifyip( pvalue, vallen, NULL ) != ERR_SUCCESS )
				{
					return ERR_CONFIG_PARAM;
				}
				else
				{
					safe_strncpy( temp_data.ip_target[0], pvalue, vallen ); 
				}
				cfg_num_elements[valnum]++;
			}
			break;
			
		case 4:		// log_filename	
			if( vallen < MAX_DIR_FILE_LEN )
			{
				safe_strncpy( temp_data.log_filename, pvalue, vallen );
				cfg_num_elements[valnum]++;
			}
			else
			{
				return ERR_CONFIG_PARAM;
			}
			break;
			
		case 5:		// result_filename
			if( vallen < MAX_DIR_FILE_LEN )
			{
				safe_strncpy( temp_data.result_logfile, pvalue, vallen );
				cfg_num_elements[valnum]++;
			}
			else
			{
				return ERR_CONFIG_PARAM;
			}
			break;
		
		case 6:
			if( mystrtoint( pvalue, vallen, &i ) != ERR_SUCCESS )
			{
				return ERR_CONFIG_PARAM;
			}
			if( ( i >= CFG_TEST_COUNT_MIN ) && ( i <= CFG_TEST_COUNT_MAX ) )
			{
				temp_data.test_count = i;
				cfg_num_elements[valnum]++;
			}
			else
			{
				return ERR_CONFIG_PARAM;
			}
			break;

		case 7:		// ip_address_target 2
			if( vallen > PN_IP_ADDRESS_LEN )
			{
				return ERR_CONFIG_PARAM;
			}
			else
			{
				if( verifyip( pvalue, vallen, NULL ) != ERR_SUCCESS )
				{
					return ERR_CONFIG_PARAM;
				}
				else
				{
					safe_strncpy( temp_data.ip_target[1], pvalue, vallen ); 
				}
				cfg_num_elements[valnum]++;
			}
			break;
			
		case 8:		// ip_address_target 3
			if( vallen > PN_IP_ADDRESS_LEN )
			{
				return ERR_CONFIG_PARAM;
			}
			else
			{
				if( verifyip( pvalue, vallen, NULL ) != ERR_SUCCESS )
				{
					return ERR_CONFIG_PARAM;
				}
				else
				{
					safe_strncpy( temp_data.ip_target[2], pvalue, vallen ); 
				}
				cfg_num_elements[valnum]++;
			}
			break;
			
		case 9:		// ip_address_target 4
			if( vallen > PN_IP_ADDRESS_LEN )
			{
				return ERR_CONFIG_PARAM;
			}
			else
			{
				if( verifyip( pvalue, vallen, NULL ) != ERR_SUCCESS )
				{
					return ERR_CONFIG_PARAM;
				}
				else
				{
					safe_strncpy( temp_data.ip_target[3], pvalue, vallen ); 
				}
				cfg_num_elements[valnum]++;
			}
			break;
			
		default:
			return ERR_CONFIG_PARAM;
	}
	
	return ERR_SUCCESS;
}

// ---------------------------------------------------------------- 

void cfg_myCopyParameters( int pf )
{

unsigned char logmess[2*CFG_MAX_LEN_CONFIG_STRING];
// copy elements from temp struct to global parameters struct
	if( cfg_num_elements[0] > 0 )
	{
		cfg_Maindata.verbose = temp_data.verbose;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %d\n", cfg_main_element[0], temp_data.verbose );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %d", cfg_main_element[0], temp_data.verbose );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[1] > 0 )
	{
		cfg_Maindata.timout = temp_data.timout;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %d\n", cfg_main_element[1], temp_data.timout );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %d", cfg_main_element[1], temp_data.timout );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[2] > 0 )
	{
		cfg_Maindata.ping_period = temp_data.ping_period;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %d\n", cfg_main_element[2], temp_data.ping_period );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %d", cfg_main_element[2], temp_data.ping_period );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[3] > 0 )
	{
		safe_strncpy( cfg_Maindata.ip_target[0], temp_data.ip_target[0], PN_IP_ADDRESS_LEN );
		cfg_Maindata.ip_target_on[0] = 1;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[3], cfg_Maindata.ip_target[0] );
			//printf( "ipadd: %d %d %d %d %d\n", cfg_Maindata.ip_target[0], cfg_Maindata.ip_target[1], cfg_Maindata.ip_target[2], cfg_Maindata.ip_target[3], cfg_Maindata.ip_target[4] );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[3], cfg_Maindata.ip_target[0] );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[4] > 0 )
	{
		safe_strncpy( cfg_Maindata.log_filename, temp_data.log_filename, MAX_DIR_FILE_LEN );
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[4], cfg_Maindata.log_filename );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[4], cfg_Maindata.log_filename );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[5] > 0 )
	{
		safe_strncpy( cfg_Maindata.result_logfile, temp_data.result_logfile, MAX_DIR_FILE_LEN );
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[5], cfg_Maindata.result_logfile );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[5], cfg_Maindata.result_logfile );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[6] > 0 )
	{
		cfg_Maindata.test_count = temp_data.test_count;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[6], cfg_Maindata.test_count );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[6], cfg_Maindata.test_count );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[7] > 0 )
	{
		safe_strncpy( cfg_Maindata.ip_target[1], temp_data.ip_target[1], PN_IP_ADDRESS_LEN );
		cfg_Maindata.ip_target_on[1] = 1;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[7], cfg_Maindata.ip_target[1] );
			//printf( "ipadd: %d %d %d %d %d\n", cfg_Maindata.ip_target[0], cfg_Maindata.ip_target[1], cfg_Maindata.ip_target[2], cfg_Maindata.ip_target[3], cfg_Maindata.ip_target[4] );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[7], cfg_Maindata.ip_target[1] );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[8] > 0 )
	{
		safe_strncpy( cfg_Maindata.ip_target[2], temp_data.ip_target[2], PN_IP_ADDRESS_LEN );
		cfg_Maindata.ip_target_on[2] = 1;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[8], cfg_Maindata.ip_target[2] );
			//printf( "ipadd: %d %d %d %d %d\n", cfg_Maindata.ip_target[0], cfg_Maindata.ip_target[1], cfg_Maindata.ip_target[2], cfg_Maindata.ip_target[3], cfg_Maindata.ip_target[4] );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[8], cfg_Maindata.ip_target[2] );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	if( cfg_num_elements[9] > 0 )
	{
		safe_strncpy( cfg_Maindata.ip_target[3], temp_data.ip_target[3], PN_IP_ADDRESS_LEN );
		cfg_Maindata.ip_target_on[3] = 1;
		if( (pf & 1) > 0 )
		{
			printf( "\tcfg_ReadConfig. Loading parametr %s, value: %s\n", cfg_main_element[9], cfg_Maindata.ip_target[3] );
			//printf( "ipadd: %d %d %d %d %d\n", cfg_Maindata.ip_target[0], cfg_Maindata.ip_target[1], cfg_Maindata.ip_target[2], cfg_Maindata.ip_target[3], cfg_Maindata.ip_target[4] );
		}
		if( (pf & 2) > 0 )
		{
			sprintf( logmess, "\tReading config. Loading parametr %s, value: %s", cfg_main_element[9], cfg_Maindata.ip_target[3] );
			logMessage( logmess, strlength( logmess )+1 );
		}
	}
	cfg_Maindata.size = sizeof( MAIN_CONFIG );
	
}

// --------------------------------------------------------------------------------------


// function check that ip address need be different
int cfg_CheckDiffIPaddr( int pf )
{
int i, res;
unsigned char logmess[2*CFG_MAX_LEN_CONFIG_STRING];

	// check input param
	if( pf < 0 || pf > 3 )
	{
		printf( "\tcfg_CheckDiffIPaddr. Error in input param pf\n" );
		return ERR_NULLSTR;
	}
	
	// check 1;
	for( i = 1; i < CFG_MAX_IP_TARGET; i++ )
	{
		if( temp_data.ip_target[0][1] != 0 )
		{	
			if( mystrncmp( temp_data.ip_target[0], PN_IP_ADDRESS_LEN-1, temp_data.ip_target[i], PN_IP_ADDRESS_LEN-1 ) == ERR_SUCCESS )
			{
				if( (pf & 1) > 0 )
				{
					printf( "\tcfg_CheckDiffIPaddr. Error: ip_address_target1 equal ip_address_target%d in cfg file. Program abort.\n", i+1 );
				}
				if( (pf & 2) > 0 )
				{
					sprintf( logmess, "\tReading config. Error: ip_address_target1 equal ip_address_target%d in cfg file. Program abort.", i+1 );
					logMessage( logmess, strlength( logmess )+1 );
				}
				return ERR_CONFIG_PARAM;
			}
		}
	}
	
	// check 2:
	for( i = 2; i < CFG_MAX_IP_TARGET; i++ )
	{
		if( temp_data.ip_target[1][1] != 0 )
		{	
			//printf( "\tCheck. i: %d,   -%s-,  -%s- res: %d\n", i, temp_data.ip_target[1], temp_data.ip_target[i], res  );
			if( mystrncmp( temp_data.ip_target[1], PN_IP_ADDRESS_LEN-1, temp_data.ip_target[i], PN_IP_ADDRESS_LEN-1 ) == ERR_SUCCESS )
			{
				if( (pf & 1) > 0 )
				{
					printf( "\tcfg_CheckDiffIPaddr. Error: ip_address_target2 equal ip_address_target%d in cfg file. Program abort.\n", i+1 );
				}
				if( (pf & 2) > 0 )
				{
					sprintf( logmess, "\tReading config. Error: ip_address_target2 equal ip_address_target%d in cfg file. Program abort.", i+1 );
					logMessage( logmess, strlength( logmess )+1 );
				}
				return ERR_CONFIG_PARAM;
			}
		}
	}
	
	// check 3:
	if( temp_data.ip_target[2][1] != 0 )
	{	
		if( mystrncmp( temp_data.ip_target[2], PN_IP_ADDRESS_LEN-1, temp_data.ip_target[3], PN_IP_ADDRESS_LEN-1 ) == ERR_SUCCESS )
		{
			if( (pf & 1) > 0 )
			{
				printf( "\tcfg_CheckDiffIPaddr. Error: ip_address_target3 equal ip_address_target4 in cfg file. Program abort.\n" );
			}
			if( (pf & 2) > 0 )
			{
				sprintf( logmess, "\tReading config. Error: ip_address_target3 equal ip_address_target4 in cfg file. Program abort." );
				logMessage( logmess, strlength( logmess )+1 );
			}
			return ERR_CONFIG_PARAM;
		}
	}

	return ERR_SUCCESS;
}


// ---------------- END OF FILE: load_config.c -------------------