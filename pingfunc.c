/* --------------------------------- 
#//Project:		PinTest - Ping Testing
#//File:		pingfunc.c
#//Date:		27.01.2021
#//Author:		T.O.
#//Version:		1.2

#//
#//
#------------------------------------*/


/* --------------------------------- 
#//Project:		PinTest - Ping Testing
#//File:		pingfunc.c
#//Date:		27.01.2021
#//Author:		T.O.
#//Version:		1.2

#//
#//
#------------------------------------*/


#include "common_project.h"


#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

FILE *pingFile = NULL, *resultFile = NULL;
char current_dir[1024];
int dir_level = 0;
// --------------------------------------------------- 


void pn_ChangeCurrentDirectory(void)
{

char new_directory[1024];
char new_file_name[1024];
int i;
DWORD res1;
BOOL flag;

	memset(current_dir, 0, 1024);
	memset(new_directory, 0, 1024);
	memset(new_file_name, 0, 1024);
	
	res1 = GetCurrentDirectory( sizeof(current_dir), current_dir );
	if( res1 == 0 )
	{
		printf("Function pn_ChangeCurrentDirectory. GetCurrentDirectory failed (%d)\n", GetLastError());
		return;
	}
	
	// cfg_Maindata.result_logfile	
	flag = FALSE;
	for( i=0; i < sizeof(cfg_Maindata.result_logfile); i++ )
	{
		if( cfg_Maindata.log_filename[i] == 0x5C )
		{
			break;
		}
		new_directory[i] = cfg_Maindata.result_logfile[i];
	}
	
	if( i == sizeof(cfg_Maindata.result_logfile) )
	{
		return;
	}
	else
	{
		if( SetCurrentDirectory( new_directory ) )
		{
			dir_level++;
			return;
		}
		else
		{
			if( CreateDirectory( new_directory, NULL ) )
			{
				SetCurrentDirectory( new_directory );
				dir_level++;
				return;
			}
			else
			{
				printf( "Function pn_ChangeCurrentDirectory. CreateDirectory failed (%d)\n", GetLastError() );
				return;
			}
		}
			
	}
	
	return;
}


// --------------------------------------------------------------------------------------------------



int pn_OnePing( unsigned char *ipaddress, unsigned long tmout, int printflag )
{

	// function make one ping to ipaddress.tmout - ping timeout. 
	// printflag == 0 - no debug print, no log file printf, printflag == 1 - only debug print, printflag == 2 - only log print, 3 - both log
	// function return pEchoReply->Status value if success, or -1, -2, -3 if error 
	
unsigned long ipaddr = INADDR_NONE;
int ReplySize, dwRetVal, dwError, res1, i, j, k;
unsigned char *ReplyBuffer;
unsigned char SendData[] = "Hello Ros Network";
PICMP_ECHO_REPLY pEchoReply;
struct in_addr ReplyAddr;
HANDLE hIcmpFile;
char print_message[90];
char log_message[120];
char current_directory[1024];
char new_directory[1024];
char file_name[1024];
BOOL res2;
SYSTEMTIME sm;
unsigned char *temp = NULL;
	
	// check input parameters
	if( ipaddress == NULL )
	{
		if( (printflag & 1) > 0 )
		{
			printf( "\t pn_OpenPing. Error in input param: ipaddress.\n" );
		}
		return -1;
	}	
	if( ( tmout < 10 ) || ( tmout > 200000 ) )
	{
		if( (printflag & 1) > 0 )
		{
			printf( "\t pn_OpenPing. Error in input param: tmout.\n" );
		}
		return -1;
	}
	
	// main work
	hIcmpFile = IcmpCreateFile();
    if( hIcmpFile == INVALID_HANDLE_VALUE ) 
	{
      if( (printflag & 1) > 0 )
	  {
			printf("\tpn_OpenPing. Unable to open handle.\n");
			printf("\tpn_OpenPing. IcmpCreatefile returned error: %ld\n", GetLastError() );
	  }
	  return -1;
    }
	if((printflag & 1) > 0)
	{
		printf( "\tpn_OpenPing. ICMP handle created \n" );
	}

	ipaddr = inet_addr( ipaddress );
	if( ipaddr == INADDR_NONE ) 
	{
        if((printflag & 1) > 0)
		{
			printf( "\tpn_OpenPing. Error in IP address format\n" );
		}
        return -2;
    }
		
	ReplySize = sizeof( ICMP_ECHO_REPLY ) + sizeof( SendData ) + 16;
    if( (ReplyBuffer = (VOID *) malloc(ReplySize)) == NULL )
	{
		if((printflag & 1) > 0)
		{
			printf( "\tpn_OpenPing. Error: no memory to malloc \n" );
		}
		return -3;
	}
	
	dwRetVal = IcmpSendEcho2( hIcmpFile, NULL, NULL, NULL, ipaddr, 
							  SendData, sizeof (SendData), NULL, ReplyBuffer, ReplySize, tmout );


	// Need to close the handle when done using it
	IcmpCloseHandle(hIcmpFile);

	if( dwRetVal != 0 )
	{
		pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
        ReplyAddr.S_un.S_addr = pEchoReply->Address;
        
		if((printflag & 1) > 0)
		{
			printf("\tpn_OpenPing. Sent icmp message to %s\n", ipaddress );
			printf("\tpn_OpenPing. Received %ld icmp message response\n", dwRetVal);
		}
		if(dwRetVal > 1) 
		{
			if((printflag & 1) > 0)
			{
				printf("\tpn_OpenPing. Information from the first response:\n");
			}
        } 
		else
		{
            if((printflag & 1) > 0)
			{
				printf("\tpn_OpenPing. Information from this response:\n");
			}
		}
		
		if((printflag & 1) > 0)
		{
			printf("\tpn_OpenPing. Received from IP: %s\n", inet_ntoa(ReplyAddr) );
			printf("\tpn_OpenPing. Status = %ld  \n", pEchoReply->Status );
		}
		
        switch( pEchoReply->Status ) 
		{
			case IP_SUCCESS:
				sprintf( print_message, "\tpn_OpenPing. Roundtrip time = %ld milliseconds\n\tpn_OpenPing. Ping success\n", pEchoReply->RoundTripTime );
				sprintf( log_message, "Sent icmp message to %s \tRoundtrip time = %ld milliseconds. \tPing success.", ipaddress, pEchoReply->RoundTripTime );
				break;
				
			case IP_DEST_HOST_UNREACHABLE:
				sprintf( print_message, "\tpn_OpenPing. Destination host was unreachable.\n" );
				sprintf( log_message, "Sent icmp message to %s \tDestination host was unreachable.", ipaddress );
				break;
				
			case IP_DEST_NET_UNREACHABLE:
				sprintf( print_message, "\tpn_OpenPing. Destination network was unreachable.\n" );
				sprintf( log_message, "Sent icmp message to %s \tDestination network was unreachable.", ipaddress );
				break;
				
			case IP_REQ_TIMED_OUT:
				sprintf( print_message, "\tpn_OpenPing. Request timed out.\n" );
				sprintf( log_message, "Sent icmp message to %s \tRequest timed out.", ipaddress );
				break;
        
			default:
				sprintf( print_message, "\tpn_OpenPing. Unknown status.\n" );
				sprintf( log_message, "Sent icmp message to %s \tUnknown status.", ipaddress );
				break;		
		}
		if((printflag & 1) > 0)
		{
			printf( "%s", print_message );
		}
		if((printflag & 2) > 0)
		{
			logMessage( log_message, sizeof( log_message ) );
		}

		
		// ------------------------------- print in special log file ------------------------------------------------------------
		res2 = FALSE;
		for( i = 0, j = 0; i < sizeof(cfg_Maindata.log_filename); i++ )
		{
			if( cfg_Maindata.log_filename[i] == 0x5C )
			{
				res2 = TRUE;
				i++;
			}
			if( res2 )
			{
				file_name[j] =  cfg_Maindata.log_filename[i];
				j++;
			}
		}
		pingFile = fopen( file_name, "a" );
		if( pingFile != NULL )
		{
			if( (temp = (unsigned char *)malloc( sizeof( log_message ) + 14 ) ) != NULL )
			{
				GetLocalTime(&sm);	
				sprintf( temp, "%.2d:%.2d:%.2d.%.3d %s\n", sm.wHour, sm.wMinute, sm.wSecond, sm.wMilliseconds, log_message );
				fputs( temp, pingFile );
				//printf( "write success\n" );
			free( temp );
			}
		}
		else
		{
			if( (printflag & 1) > 0 )
			{
				printf( "\tpn_OnePing. Error in open ping log file: %s  Function stop.\n", cfg_Maindata.log_filename );
			}
			if(  (printflag & 2) > 0  )
			{
				sprintf( log_message, "pn_pn_OnePing. Error in open ping log file: %s  Function stop.", cfg_Maindata.log_filename );
				logMessage( log_message, strlength( log_message )+1 );
			} 
		} 
		fclose( pingFile ); 
		SetCurrentDirectory( ".." );


		return pEchoReply->Status;
	}	
	else
	{
        dwError = GetLastError();
        switch (dwError) 
		{
			case IP_BUF_TOO_SMALL:
				sprintf( print_message, "\tpn_OpenPing. Call to IcmpSendEcho2 failed.\n\tReplyBufferSize to small\n" );
				sprintf( log_message, "Sent icmp message to %s \tCall to IcmpSendEcho2 failed.\tReplyBufferSize to small.", ipaddress );
				break;
				
			case IP_REQ_TIMED_OUT:
				sprintf( print_message, "\tpn_OpenPing. Call to IcmpSendEcho2 failed.\n\tRequest timed out\n" );
				sprintf( log_message, "Sent icmp message to %s \tCall to IcmpSendEcho2 failed.\tRequest timed out.", ipaddress );
				break;
				
			default:
				sprintf( print_message, "\tpn_OpenPing. Call to IcmpSendEcho2 failed.\n\tExtended error returned: %ld\n", dwError );
				sprintf( log_message, "Sent icmp message to %s \tCall to IcmpSendEcho2 failed.\tExtended error returned: %ld.", ipaddress, dwError );
				break;
        }

		if((printflag & 1) > 0)
		{
			printf( "%s", print_message );
		}
		if((printflag & 2) > 0)
		{
			logMessage( log_message, sizeof( log_message ) );
		}
		
		// print in special log file
		pingFile = fopen( cfg_Maindata.log_filename, "a" );
		if( pingFile != NULL )
		{
			if( (temp = (unsigned char *)malloc( sizeof( log_message ) + 14 ) ) != NULL )
			{
				GetLocalTime(&sm);	
				sprintf( temp, "%.2d:%.2d:%.2d.%.3d %s\n", sm.wHour, sm.wMinute, sm.wSecond, sm.wMilliseconds, log_message );
				fputs( temp, pingFile );
				// printf( "write success\n" );
				free( temp );
			}
		}
		else
		{
			if( (printflag & 1) > 0 )
			{
				printf( "\tpn_OnePing. Error in open ping log file: %s  Function stop.\n", cfg_Maindata.log_filename );
			}
			if(  (printflag & 2) > 0  )
			{
				sprintf( log_message, "pn_pn_OnePing. Error in open ping log file: %s  Function stop.", cfg_Maindata.log_filename );
				logMessage( log_message, strlength( log_message )+1 );
			}
		}
		fclose( pingFile );

        return -1;		
	}

	return -1;
}

// ---------------------------------------------------------------------------------- 



int pn_MainPingfunction( int printflag )
{
unsigned long colping;
int res[CFG_MAX_IP_TARGET], sost[CFG_MAX_IP_TARGET], i[CFG_MAX_IP_TARGET];
unsigned char logmess[2*CFG_MAX_LEN_CONFIG_STRING];
SYSTEMTIME sm;
int k;
int m, x;
char new_file_name[1024];
char tmp[1024];
BOOL res2;

	
	// check input params 
	if( (printflag < 0) || (printflag > 3) )
	{
		return ERR_NULLSTR;
	}

	for( k = 0; k < CFG_MAX_IP_TARGET; k++ )
	{
		sost[k] = 1;
		i[k] = 0;
	}
	
	pn_ChangeCurrentDirectory();
	memset( new_file_name, 0, sizeof(new_file_name) );
	res2 = FALSE;
	for( m = 0, x = 0; m < strlength(cfg_Maindata.result_logfile); m++ )
	{
		if( cfg_Maindata.result_logfile[m] == 0x5C )
		{
			res2 = TRUE;
			m++;
		}
		else {
			new_file_name[x] = cfg_Maindata.result_logfile[m];
			x++;
		}
	}
	GetCurrentDirectory( 1024, tmp );
	printf("!!!: %s %s\n", tmp, new_file_name );
	
	for( colping = 0; ; colping++ )
	{
	
		for( k = 0; k < CFG_MAX_IP_TARGET; k++ )
		{
			if( cfg_Maindata.ip_target_on[k] == 1 )
			{
				res[k] = pn_OnePing( cfg_Maindata.ip_target[k], cfg_Maindata.timout, 0 );
				if( (printflag & 1) > 0 )
				{	
					printf( "\tpn_MainPingfunction. %d. ping addr %s. res: %d\n", colping, cfg_Maindata.ip_target[k], res[k] );
				}
			
				if( res[k] == sost[k] )
				{
					i[k] = 0;
				}
				else
				{
					i[k]++;
					if( i[k] == cfg_Maindata.test_count )
					{
						sost[k] = res[k];

						GetLocalTime( &sm );
				
						if( res[k] == ERR_SUCCESS )
						{
							printf( "%.2d.%.2d.%.4d  %.2d:%.2d:%.2d Cool!!! Connection with ip host %s state\n", 
									sm.wDay, sm.wMonth, sm.wYear, sm.wHour, sm.wMinute, sm.wSecond, cfg_Maindata.ip_target[k] );
							// open result file 
							printf( "pn_MainPingfunction new_file_name: %s\n", new_file_name );
							// resultFile = fopen( cfg_Maindata.result_logfile, "a" );
							resultFile = fopen( new_file_name, "a" );
							if( resultFile == NULL )
							{
								if( (printflag & 1) > 0 )
								{
									printf( "\tpn_MainPingfunction. Error in open result log file: %s  Function stop.\n", cfg_Maindata.result_logfile, GetLastError() );
								}
								if(  (printflag & 2) > 0  )
								{
									sprintf( logmess, "pn_MainPingfunction. Error in open result log file: %s  Function stop.", cfg_Maindata.result_logfile );
									logMessage( logmess, strlength( logmess )+1 );
								}	
								return ERR_CREATELOGFILE;
							}
							else
							{
								// write message in result file
								sprintf( logmess, "%.2d.%.2d.%.4d  %.2d:%.2d:%.2d Cool!!! Connection with ip host %s state\n", 
										 sm.wDay, sm.wMonth, sm.wYear, sm.wHour, sm.wMinute, sm.wSecond, cfg_Maindata.ip_target[k] );
								fputs( logmess, resultFile );
								fclose( resultFile );
							}
						}
						else
						{
							printf( "%.2d.%.2d.%.4d  %.2d:%.2d:%.2d Bad!!! Connection with ip host %s LOST\n", 
									sm.wDay, sm.wMonth, sm.wYear, sm.wHour, sm.wMinute, sm.wSecond, cfg_Maindata.ip_target[k] );
							// open result file 
							
							resultFile = fopen( new_file_name, "a" );
							if( resultFile == NULL )
							{
								if( (printflag & 1) > 0 )
								{
									printf( "\tpn_MainPingfunction. Error in open result log file: %s  Function stop.\n", cfg_Maindata.result_logfile );
								}
								if(  (printflag & 2) > 0  )
								{
									sprintf( logmess, "pn_MainPingfunction. Error in open result log file: %s  Function stop.", cfg_Maindata.result_logfile );
									logMessage( logmess, strlength( logmess )+1 );
								}	
								return ERR_CREATELOGFILE;
							}
							else
							{
								// write message in result file
								sprintf( logmess, "%.2d.%.2d.%.4d  %.2d:%.2d:%.2d Bad!!! Connection with ip host %s LOST\n",
										 sm.wDay, sm.wMonth, sm.wYear, sm.wHour, sm.wMinute, sm.wSecond, cfg_Maindata.ip_target[k] );
								fputs( logmess, resultFile );
								fclose( resultFile );
							}
						}
					
					} 
				} 
			} 
		}
		Sleep( cfg_Maindata.ping_period*1000 );
	}
	
	if( dir_level > 0 )
	{
		SetCurrentDirectory(current_dir);
	}

	return ERR_SUCCESS;
}


// ---------------------------------- end of file pingfunc.c ---------------
