/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: callowedvalue_list.c
*
*	Revision:
*
*	02/23/05
*		- first revision
*
******************************************************************/

#include <cybergarage/upnp/callowedvalue.h>

/****************************************
* cg_upnp_allowedvalue_new
****************************************/

CgUpnpAllowedValueList *cg_upnp_allowedvaluelist_new()
{
	CgUpnpAllowedValue *allowedvalueList = (CgUpnpAllowedValue *)malloc(sizeof(CgUpnpAllowedValue));
	cg_list_header_init((CgList *)allowedvalueList);
	return allowedvalueList;
}

/****************************************
* cg_upnp_allowedvalue_delete
****************************************/

void cg_upnp_allowedvaluelist_delete(CgUpnpAllowedValueList *allowedvalueList)
{
	cg_list_remove((CgList *)allowedvalueList);
	free(allowedvalueList);
}
