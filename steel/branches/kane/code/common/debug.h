/***************************************************************************
 *            debug.h
 *
 *  Fri Mar 17 09:47:08 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG
#include <string>
#include <iostream>

#define _ \
std::cout << "DEBUG @ " << __FILE__ << " " << __FUNCTION__ << ":" << __LINE__ << std::endl;

#define pdebug( data, ... ) \
_pdebug(__FILE__,__FUNCTION__,__LINE__,data,##__VA_ARGS__)

void _pdebug( char *filen, char *func, int line, char *fmt, ... );
void _pdebug( std::string filen, std::string func, int line, std::string str );

#else	// DEBUG

#define _ ;
#define pdebug ;

#endif	// DEBUG

#endif	// __DEBUG_H__
