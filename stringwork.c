/* --------------------------------- 
Проект:		KernelModeEthernetBridge
Файл:		stringwork.c
Дата:		20.08.2010
Автор:		T.O.
Версия:		1.0.0

Описание:	Файл содержит функции для преобразования строк.
------------------------------------*/

#include "common_project.h"

// сообщения об ошибках
unsigned char *errmessages[] = 
{
	"all OK",
	"miss parameters <filename>",
	"could not open input configure file",
	"could not read input configure file",
	"bad input parameters",
	"error in string in config.file",
	"configure file have not all required parameters",
	"bad XML configuration file",
	"XML file have no only one rules",
	"bad argument in the command line",
	"could not open kmethbridge driver",
	"could not connect to kmethbridge driver",
	"could not create directory to logs:",
	"too many parameters in request_parameter tag in access matrix",
	"",
};



/*
// Возвращает текстовую строку с сообщением по числовому идентификатору ощибки.
*/
unsigned char *errstring( unsigned int err )
{

#ifdef OPTION_TEST
	printf( "errstring\n" );
#endif

	if( err < MAXERRORS )
	{
#ifdef OPTION_TEST
	printf( "End errstring\n" );
#endif
		return errmessages[err];
	}
	else
	{
#ifdef OPTION_TEST
	printf( "End errstring\n" );
#endif
		return "unknown error";
	}
}



/*
// Возвращает размер строки.
*/
unsigned int strlength( unsigned char *str )
{
	unsigned int i;

	#ifdef OPTION_TEST
		printf( "strlength\n" );
	#endif

	if( str == NULL )
	{
		#ifdef OPTION_TEST
			printf( "End strlength\n" );
		#endif
		return 0;
	}
	for( i = 0; str[i]!=0; i++ );

	#ifdef OPTION_TEST
		printf( "End strlength\n" );
	#endif
	return i;
}

// ------------------------------------------------------------------------------ //

/*
// Возвращает количество печатных символов в строке.
*/
unsigned int strprintlength( unsigned char *str )
{
	unsigned int i, k;

	#ifdef OPTION_TEST
		printf( "strlength\n" );
	#endif

	if( str == NULL )
	{
		#ifdef OPTION_TEST
			printf( "End strlength\n" );
		#endif
		return 0;
	}
	for( i = 0, k = 0; str[i]!=0; i++ )
	{
		if( isspace(str[i]) == 0 )
		{
			k++;
		}
	}

	#ifdef OPTION_TEST
		printf( "End strlength\n" );
	#endif
	return k;
}


// --------------------------------------------------------------------------------- //


/*
// Удаляет из строки непечатные символы.
*/
void rmspacestr( unsigned char *str, unsigned int *strlen)
{
	unsigned int i, k, s;

	#ifdef OPTION_TEST
		printf( "rmspacestr\n" );
	#endif

	if( strlen == 0 || str == NULL )
	{
		#ifdef OPTION_TEST
			printf( "End rmspacestr\n" );
		#endif
		return;
	}
	s = *strlen;
	for( i = 0; str[i] != 0 && i < s; i++ )
	{
		if( isspace( str[i] ) != 0 )
		{
			for( k = i+1; k < s; k++ )
			{
				str[k-1] = str[k];
			}
			s--;
			str[s] = 0;
			i--;
		}
	}
	*strlen = s;
	#ifdef OPTION_TEST
		printf( "End rmspacestr\n" );
	#endif
	return;
}

// ---------------------------------------------------------- xxx -------------------------------------------------- //



/*
// Удаляет из начала строки непечатные символы.
*/
void rmspacebeginstr( unsigned char *str, unsigned int *strlen)
{
	unsigned int i, k, s;

	#ifdef OPTION_TEST
		printf( "rmspacebeginstr\n" );
	#endif

	if( strlen == 0 || str == NULL )
	{
		#ifdef OPTION_TEST
			printf( "End rmspacebeginstr\n" );
		#endif
		return;
	}
	
//	printf( "w: .%s.", str );
	s = *strlen;
	for( i = 0; str[i] != 0 && i < s; i++ )
	{
		if( isspace( str[i] ) != 0 )
		{
			for( k = i+1; k < s; k++ )
			{
				str[k-1] = str[k];
			}
			s--;
			str[s] = 0;
			i--;
		}
		else
		{
			break;
		}
	}
	*strlen = s;
//	printf( "    b: .%s.\n", str );

	#ifdef OPTION_TEST
		printf( "End rmspacebeginstr\n" );
	#endif
	return;
}

// --------------------------------------------- zzz -------------------------------------------------- //

/* 
// Возвращает ERR_SUCCESS, если строка состоит только из десятичных цифр, иначе 0xFFFF.
*/
unsigned int isdigitstring( unsigned char *str, unsigned int len )
{

#ifdef OPTION_TEST
	printf( "isdigitstring\n" );
#endif

	if( len == 0 || str == NULL )
	{
#ifdef OPTION_TEST
	printf( "End isdigitstring\n" );
#endif
		return 0xFFFF;
	}

	if( str[len-1] == 0 )
	{
		len -= 1;
	} 

	for( len-=1; len != 0; len-- )
	{
		if( isdigit( str[len] ) == 0 )
		{
#ifdef OPTION_TEST
	printf( "End isdigitstring\n" );
#endif
			return 0xFFFF;
		}
	}
	if( isdigit( str[0] ) == 0 )
	{
#ifdef OPTION_TEST
	printf( "End isdigitstring\n" );
#endif
		return 0xFFFF;
	}

#ifdef OPTION_TEST
	printf( "End isdigitstring\n" );
#endif
	return ERR_SUCCESS;
}

// --------------------------------------------------------------------------------------------//


/*
// Конвертирует строку содержщую число в числовую переменную.
// Возвращает ERR_SUCCESS при успешном конвертировании, иначе 0.
*/
unsigned int mystrtoint( unsigned char *str, unsigned int len, unsigned long *res )
{
	
	unsigned int i, t=0;
	unsigned long a, b;

#ifdef OPTION_TEST
	printf( "mystrtoint\n" );
#endif


	// проверка входных параметров.
	if( len == 0 || str == NULL )
	{
#ifdef OPTION_TEST
	printf( "End mystrtoint\n" );
#endif
		return 0xffff;
	}
	if( isdigitstring( str, len ) != 0 )
	{
#ifdef OPTION_TEST
	printf( "End mystrtoint\n" );
#endif
		return 0xffff;
	}
	if( len > 10 )
	{
#ifdef OPTION_TEST
	printf( "End mystrtoint\n" );
#endif
		return 0xffff;
	}

	if( str[len-1] == 0 )
	{
		len -= 1;
	}
	len -= 1;
	a = (str[len]-48);
	b = 10;
	
	for( ; len!=0; len--, b=b*10 )
	{
		a += ((str[len-1]-48)*b);
	}

	if( a > 0x7FFFFFFF )
	{
#ifdef OPTION_TEST
	printf( "End mystrtoint\n" );
#endif
		return 0xffff;
	}

	*res = (unsigned long)a;
#ifdef OPTION_TEST
	printf( "End mystrtoint\n" );
#endif
	return ERR_SUCCESS;
}

//-----------------------------------------------------------------------------//


/*
// Конвертирует числовую переменную в строку.
// Возвращает ERR_SUCCESS при успешном конвертировании, иначе 0.
*/
unsigned int myinttostr( unsigned char *str, unsigned int len, unsigned int num )
{

	unsigned int i, a;
	unsigned char t[5];

#ifdef OPTION_TEST
	printf( "myinttostr\n" );
#endif

	if( str == NULL || len < 5 || num > 0xFFFF )
	{
#ifdef OPTION_TEST
	printf( "End myinttostr\n" );
#endif
		return ERR_NULLSTR;
	}

	memset( str, 0, len );
	for( i = 0;  (num / 10) != 0; i++, num /= 10 )
	{
		t[i] = (num % 10) + 0x30;
	}
	t[i] = (num % 10) + 0x30;
	for( a = 0; (i-a)!= 0; a++ )
	{
		str[a] = t[i-a];
	}
	str[a] = t[i-a];
#ifdef OPTION_TEST
	printf( "End myinttostr\n" );
#endif
	return ERR_SUCCESS;
}
// ---------------------------------------------------------------------------------------------//


/*
// Сравнивает 2 строки.
// Возвращает ERR_SUCCESS, если 2 строки равны, иначе 0xFFFF.
*/
unsigned int mystrncmp( unsigned char *s1, unsigned int l1, unsigned char *s2, unsigned int l2 )
{

	unsigned int i;

#ifdef OPTION_TEST
	printf( "mystrncmp\n" );
#endif

	// check input param
	if( l1 == 0 || l2 == 0 || s1 == NULL || s2 == NULL )
	{
#ifdef OPTION_TEST
	printf( "End mystrncmp\n" );
#endif
		return ERR_NULLSTR;
	}

	if( l1 != l2 )
	{
#ifdef OPTION_TEST
	printf( "End mystrncmp\n" );
#endif
		return 0xFFFF;
	}

	for( i = 0; i <= l1; i++ )
	{
		if( s1[i] != s2[i] )
		{
#ifdef OPTION_TEST
	printf( "End mystrncmp\n" );
#endif
			return 0xffff;
		}
	}

#ifdef OPTION_TEST
	printf( "End mystrncmp\n" );
#endif
	return ERR_SUCCESS;
}


//-----------------------------------------------------------------------------------//


/*
// Проверяет строку, содержащею IP адрес на корректность IP адреса.
// Возвращает ERR_SUCCESS при корректном IP адресе, иначе 0xFFFF.
*/
unsigned int verifyip( unsigned char *str, unsigned int len, unsigned char *ip )
{
	
	unsigned char separated[] = ".";
	unsigned char *tok;
	unsigned char temp[PN_IP_ADDRESS_LEN];
	unsigned int l, r, cn = 0;
	unsigned char mass[4];

#ifdef OPTION_TEST
	printf( "verifyip\n" );
#endif

	if( len < 7 || len > PN_IP_ADDRESS_LEN || str == NULL )
	{
#ifdef OPTION_TEST
	printf( "End verifyip\n" );
#endif
		return 0xffff;
	}
	memset( temp, 0, PN_IP_ADDRESS_LEN );
	memset( mass, 0, 4 );
	memcpy( temp, str, len );
	tok = (unsigned char *)strtok( temp, separated );
	while( tok != NULL )
	{
		l = strlength( tok );
		if( isdigitstring( tok, l ) != 0 )
		{
#ifdef OPTION_TEST
	printf( "End verifyip\n" );
#endif
			return 0xFFFF;
		}
		if( mystrtoint( tok, l, &r ) != 0 )
		{ 
#ifdef OPTION_TEST
	printf( "End verifyip\n" );
#endif
			return 0xFFFF;
		}
		if( r > 255 )
		{
#ifdef OPTION_TEST
	printf( "End verifyip\n" );
#endif
			return 0xFFFF;
		}
		mass[cn] = r;
		cn++;
		tok = (UCHAR *)strtok( NULL, separated );
	}

	if( cn != 4 )
	{
#ifdef OPTION_TEST
	printf( "End verifyip\n" );
#endif
		return 0xFFFF;
	}
	if( ip != NULL )
	{
		memcpy( ip, mass, 4 );
	}

#ifdef OPTION_TEST
	printf( "End verifyip\n" );
#endif
	return ERR_SUCCESS;
}

//-------------------------------------------------------------------------//

/*
// Проверяет строку, содержащею MAC адрес на корректность MAC адреса.
// Возвращает ERR_SUCCESS при корректном MAC адресе, иначе 0xFFFF.
*/
unsigned int verifymac( unsigned char *str, unsigned int len, unsigned char *mac )
{

	unsigned char separated[] = ":-\"\t";
	unsigned char *tok;
	unsigned char temp[PN_MAC_ADDRESS_LEN];
	unsigned char mass[PN_ETHER_ADDRESS_LEN];
	unsigned int l, cn = 0;

#ifdef OPTION_TEST
	printf( "verifymac\n" );
#endif

	if( (len+1) != PN_MAC_ADDRESS_LEN || str == NULL )
	{
#ifdef OPTION_TEST
	printf( "End verifymac\n" );
#endif
		return 0xffff;
	}
	memset( temp, 0, PN_MAC_ADDRESS_LEN );
	memset( mass, 0, PN_ETHER_ADDRESS_LEN );
	memcpy( temp, str, len );
	tok = (UCHAR *)strtok( temp, separated );
	while( tok != NULL )
	{
		l = strlength( tok );
		if( l != 2 )
		{
#ifdef OPTION_TEST
	printf( "End verifymac\n" );
#endif
			return 0xffff;
		}
		if( (isxdigit( tok[0] ) == 0) || (isxdigit( tok[1] ) == 0) )
		{
#ifdef OPTION_TEST
	printf( "End verifymac\n" );
#endif
			return 0xffff;
		}
		if( mystrtohex( tok, l, &mass[cn] ) != ERR_SUCCESS )
		{
#ifdef OPTION_TEST
	printf( "End verifymac\n" );
#endif
			return 0xffff;
		}
		cn++;
		tok = (UCHAR *)strtok( NULL, separated );
	}
	if( cn != 6 )
	{
#ifdef OPTION_TEST
	printf( "End verifymac\n" );
#endif
		return 0xffff;
	}
	if( mac != NULL )
	{
		memcpy( mac, mass, PN_ETHER_ADDRESS_LEN );
	} 

#ifdef OPTION_TEST
	printf( "End verifymac\n" );
#endif
	return ERR_SUCCESS;
}

// ----------------------------------------------------------------------------------------//


/*
// Переводит строку, содержащую число в шестнадцатиричном виде в числовую переменную.
// Возвращает ERR_SUCCESS при успешной конвертации, иначе 0xFFFF.
*/
int mystrtohex( unsigned char *str, unsigned int len, unsigned char *res )
{

	unsigned int a, b;
	
#ifdef OPTION_TEST
	printf( "mystrtohex\n" );
#endif

	if( str == NULL || len != 2 )
	{
#ifdef OPTION_TEST
	printf( "End mystrtohex\n" );
#endif
		return 0xFFFF;
	}
	if( isxdigit( str[0] ) == 0 || isxdigit( str[1] ) == 0 )
	{
#ifdef OPTION_TEST
	printf( "End mystrtohex\n" );
#endif
		return 0xFFFF;
	}
	
	if( 48 <= str[0] && str[0] <= 57 )
	{
		a = str[0] - 0x30;
	}
	if( 65 <= str[0] && str[0] <= 70 )
	{
		a = str[0] - 55;
	}
	if( 97 <= str[0] && str[0] <= 102 )
	{
		a = str[0] - 87;
	}

	if( 48 <= str[1] && str[1] <= 57 )
	{
		b = str[1] - 0x30;
	}
	if( 65 <= str[1] && str[1] <= 70 )
	{
		b = str[1] - 55;
	}
	if( 97 <= str[1] && str[1] <= 102 )
	{
		b = str[1] - 87;
	}
	*res = (a<<4)+b;
#ifdef OPTION_TEST
	printf( "End mystrtohex\n" );
#endif
	return ERR_SUCCESS;
}

//------------------------------------------------------------------------------------


/*
// Переводит строку из кодировки MultiByte в кодировку WideChar
*/
int  cp1251_to_uft16( unsigned char * inbuf, size_t inlen, wchar_t * outbuf, size_t outlen )
{
    unsigned int res_code = 0xffff;
    int res;

#ifdef OPTION_TEST
	printf( "cp1251_to_uft16\n" );
#endif

	if( (inbuf == NULL) || (inlen == 0) || (outbuf == NULL) || (outlen == 0) )
	{
		printf( "Error: critical internal error in function cp1251_to_uft16.\n" );
#ifdef OPTION_TEST
	printf( "End cp1251_to_uft16\n" );
#endif
		return res_code;
	}

    memset( outbuf, 0, outlen );
    res = MultiByteToWideChar(CP_ACP, 0, inbuf, inlen, outbuf, outlen );
	if( res == 0 )
	{
		printf( "Error: critical internal error in system function MultiByteToWideChar.\n" );
		res_code = 0xffff;
	}
    else 
	{
		res_code = ERR_SUCCESS;
	}
#ifdef OPTION_TEST
	printf( "End cp1251_to_uft16\n" );
#endif
    return (res_code);
}

// --------------------//-------------------------------------//------------------------------//


/*
// Безопасно копирует данные из одной строки в другую.
*/
unsigned char *  safe_strncpy ( char *dest, const char *src, size_t n )
{
    char          * s;                      /* Temporary  */

#ifdef OPTION_TEST
	printf( "safe_strncpy\n" );
#endif

	for( s = dest; 0 < (n-1) && *src!= '\0'; --n )
	{
         *s++ = *src++;                     /* Copy at most n-1 chars */
	}

    for( ; 0 < n; --n )
	{
         *s++ = '\0';                       /* Padding */
	}

#ifdef OPTION_TEST
	printf( "End safe_strncpy\n" );
#endif
    return( dest );
}

//============================================================================================//



/*
// Переводит время в виде количества секунд в строку.
*/
void  time2str( time_t this_time, unsigned char * tmpbuf, unsigned int bn )
{
  char * p;

#ifdef OPTION_TEST
	printf( "time2str\n" );
#endif

	_snprintf( tmpbuf, bn, "%s", asctime(gmtime(&this_time)) );
  p = strchr( tmpbuf, '\n');
  if( p != NULL )
  {
	  *p = '\0';
  }

#ifdef OPTION_TEST
	printf( "End time2str\n" );
#endif
}

// ---------------------------------------------------------------------------------------//



//// конец файла.