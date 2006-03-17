/***************************************************************************
 *            debug.cpp
 *
 *  Fri Mar 17 09:52:24 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

void _pdebug( char *filen, char *func, int line, char *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	printf("[%s# %s:%d]@ ",filen,func,line);
	vprintf(fmt,ap);
	printf("\n");
	va_end(ap);
}

void _pdebug( std::string filen, std::string func, int line, std::string str )
{
	std::cout << "[" << filen << "# " << func << ":" << line << "]@ " << str << std::endl;
}
