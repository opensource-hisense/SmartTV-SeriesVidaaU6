/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: callowedvalue.c
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

CgUpnpAllowedValue *cg_upnp_allowedvalue_new()
{
	CgUpnpAllowedValue *allowedVal = (CgUpnpAllowedValue *)malloc(sizeof(CgUpnpAllowedValue));
	cg_list_node_init((CgList *)allowedVal);
	
	allowedVal->allowedValueNode = NULL;
	
	return allowedVal;
}

/****************************************
* cg_upnp_allowedvalue_delete
****************************************/

void cg_upnp_allowedvalue_delete(CgUpnpAllowedValue *allowedVal)
{
	cg_list_remove((CgList *)allowedVal);
	free(allowedVal);
}
