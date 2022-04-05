/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: callowedvaluerange.c
*
*	Revision:
*
*	02/23/05
*		- first revision
*
******************************************************************/

#include <cybergarage/upnp/callowedvalue.h>

/****************************************
* cg_upnp_allowedvaluerange_new
****************************************/

CgUpnpAllowedValueRange *cg_upnp_allowedvaluerange_new()
{
	CgUpnpAllowedValueRange *allowedVal = (CgUpnpAllowedValueRange *)malloc(sizeof(CgUpnpAllowedValueRange));
	cg_list_node_init((CgList *)allowedVal);
	
	allowedVal->allowedValueRangeNode = NULL;
	
	return allowedVal;
}

/****************************************
* cg_upnp_allowedvaluerange_delete
****************************************/

void cg_upnp_allowedvaluerange_delete(CgUpnpAllowedValueRange *allowedVal)
{
	cg_list_remove((CgList *)allowedVal);
	free(allowedVal);
}
