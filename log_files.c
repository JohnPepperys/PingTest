/* --------------------------------- 
#//Project:		Ping Testing
#//File:		log_files.c
#//Date:		01.10.2015
#//Author:		T.O.
#//Version:		1.1

#//
#//
#------------------------------------*/

#include "common_project.h"

FILE *logFile = NULL;

// -------------------------- beginer function ----------------------------- 

int logInicializ( void )
{
		// open or create file
		logFile = fopen( LOG_FILE_NAME, "a" );
		if( logFile == NULL )
		{
			printf( "Can`t start loggin subsystem. Error in open log file.\n" );
			exit( 0 );
		}
		/*if( fseek( logFile, 0, SEEK_END ) != 0 )
		{
			printf( "Can`t start loggin subsystem. Error in open position log file.\n" );
			exit( 0 );
		} */
		return 0;
}

void logEnds( void )
{
		// close file for log
		if( logFile != NULL )
		{
			fclose( logFile );
		}
}

int logMessage( char *mass, int masslen )
{
char *temp;
SYSTEMTIME sm;
	
	// if success - return positive value, else return -1
	// check input parameters -------------
	if( mass == NULL )
	{
		return -1;
	}
	if( ( masslen < 2 ) && ( masslen > LOG_MAX_MESSAGE_LEN ) )
	{
		return -1;
	}
	if( logMessage == NULL )
	{
		return -1;
	}
	
	if( (temp = malloc( masslen+14 ) ) == NULL )
	{
		return -1;
	}
	
	// ------------- time include in message ------------- 
	GetLocalTime(&sm);	
	// printf( "%.2d:%.2d:%.2d.%.3d ", sm.wHour, sm.wMinute, sm.wSecond, sm.wMilliseconds );
	sprintf( temp, "%.2d:%.2d:%.2d.%.3d %s\n", sm.wHour, sm.wMinute, sm.wSecond, sm.wMilliseconds, mass );
	fputs( temp, logFile );
	return 1; 
}

// --------------------------------------------- common function ------------------------


void logStart_Programm()
{
		// write in file messages about start programm
char messag[] = "Programm started!"; 
		 logMessage( messag, sizeof( messag ) );
}



void logFinish_Programm()
{
		// write in file message about finish programm
char messag[] = "Finish programm!\n"; 
		logMessage( messag, sizeof( messag ) );
}


// ---------------------------------------- end of file log_files.c -------------------

