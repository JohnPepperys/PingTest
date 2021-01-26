/* --------------------------------- 
#//Project:		PinTest - Ping Testing
#//File:		start.c
#//Date:		01.10.2015
#//Author:		T.O.
#//Version:		1.1

#//
#// version 1.1 - add 4 address ping, add data in print log, add different log files for different ip targets
#------------------------------------*/


#include "common_project.h"
#include <signal.h>

void funcSignal_Break( int signal )
{
	if( signal == SIGINT )
	{
		logFinish_Programm();
		logEnds();
		printf( "Press: Control+C, programm abort.\n" );
		exit( 0 );
	}
}

void main(void)
{

int res;

	// ----------- start log --------
	logInicializ();
	logStart_Programm();

	// --------------- loadiong config param from file -------------------
	cfg_InitConfigSubsystem();
	res = cfg_ReadConfig( 3 );
	printf( "Loading config: %d\n", res );
	if( res != ERR_SUCCESS )
	{
		// ----------- end logs ---------
		logFinish_Programm();
		logEnds();
		exit( 0 );
	}

	// set interapt on signal Control+C
	signal( SIGINT, funcSignal_Break );
	
	printf( "Result pn_MainPingfunction: %d\n", pn_MainPingfunction( 2+cfg_Maindata.verbose ) );
	
	signal( SIGINT, funcSignal_Break );
	// ----------- end logs ---------
	logFinish_Programm();
	logEnds();
}


// --------------------------------------------------- 




// ---------------------------------- end of file start.c ---------------
