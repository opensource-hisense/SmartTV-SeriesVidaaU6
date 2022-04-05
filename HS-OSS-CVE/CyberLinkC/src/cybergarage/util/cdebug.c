/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: cdebug.c
*
*	Revision:
*
*	06/06/05
*		- first revision
*
******************************************************************/

#include <cybergarage/util/cdebug.h>

static BOOL debugFlag = TRUE;

/****************************************
* cg_debug_ison
****************************************/

BOOL cg_debug_ison()
{
	return debugFlag;
}

/****************************************
* cg_debug_on
****************************************/

void cg_debug_on()
{
	debugFlag = TRUE;
}

/****************************************
* cg_debug_off
****************************************/

void cg_debug_off()
{
	debugFlag = FALSE;
}
