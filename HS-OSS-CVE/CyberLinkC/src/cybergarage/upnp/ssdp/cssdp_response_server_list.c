/******************************************************************
*
*	CyberNet for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: cssdp_notify_server_list.c
*
*	Revision:
*
*	05/31/05
*		- first revision
*
******************************************************************/

#include <cybergarage/upnp/ssdp/cssdp_server.h>
#include <cybergarage/net/cinterface.h>

/****************************************
* CG_UPNP_NOUSE_CONTROLPOINT (Begin)
****************************************/

#if !defined(CG_UPNP_NOUSE_CONTROLPOINT)

/****************************************
* cg_upnp_ssdpresponse_serverlist_new
****************************************/

CgUpnpSSDPResponseServerList *cg_upnp_ssdpresponse_serverlist_new()
{
	CgUpnpSSDPResponseServerList *ssdpServerList = (CgUpnpSSDPResponseServerList *)malloc(sizeof(CgUpnpSSDPResponseServerList));
	
	cg_list_header_init((CgList *)ssdpServerList);
	
	ssdpServerList->httpuSock = NULL;
	ssdpServerList->recvThread = NULL;
	ssdpServerList->listener = NULL;
	
	return ssdpServerList;
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_delete
****************************************/

void cg_upnp_ssdpresponse_serverlist_delete(CgUpnpSSDPResponseServerList *ssdpServerList)
{
	cg_upnp_ssdpresponse_serverlist_clear(ssdpServerList);
	
	free(ssdpServerList);
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_open
****************************************/

BOOL cg_upnp_ssdpresponse_serverlist_open(CgUpnpSSDPResponseServerList *ssdpServerList, int bindPort)
{
	CgNetworkInterfaceList *netIfList;
	CgNetworkInterface *netIf;
	CgUpnpSSDPResponseServer *ssdpServer;
	char *bindAddr;
	
	netIfList = cg_net_interfacelist_new();
	cg_net_gethostinterfaces(netIfList);
	for (netIf = cg_net_interfacelist_gets(netIfList); netIf; netIf = cg_net_interface_next(netIf)) {
		bindAddr = cg_net_interface_getaddress(netIf);
		if (cg_strlen(bindAddr) <= 0)
			continue;
		ssdpServer = cg_upnp_ssdpresponse_server_new();
		if (cg_upnp_ssdpresponse_server_open(ssdpServer, bindPort, bindAddr) == FALSE) {
			cg_upnp_ssdpresponse_server_delete(ssdpServer);
			continue;
		}
		cg_upnp_ssdpresponse_serverlist_add(ssdpServerList, ssdpServer);
	}

	cg_net_interfacelist_delete(netIfList);

	return TRUE;	
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_close
****************************************/

BOOL cg_upnp_ssdpresponse_serverlist_close(CgUpnpSSDPResponseServerList *ssdpServerList)
{
	CgUpnpSSDPResponseServer *ssdpServer;
	
	for (ssdpServer = cg_upnp_ssdpresponse_serverlist_gets(ssdpServerList); ssdpServer != NULL; ssdpServer = cg_upnp_ssdpresponse_server_next(ssdpServer))
		cg_upnp_ssdpresponse_server_close(ssdpServer);

	return TRUE;
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_start
****************************************/

BOOL cg_upnp_ssdpresponse_serverlist_start(CgUpnpSSDPResponseServerList *ssdpServerList)
{
	CgUpnpSSDPResponseServer *ssdpServer;
	
	for (ssdpServer = cg_upnp_ssdpresponse_serverlist_gets(ssdpServerList); ssdpServer != NULL; ssdpServer = cg_upnp_ssdpresponse_server_next(ssdpServer))
		cg_upnp_ssdpresponse_server_start(ssdpServer);
	
	return TRUE;
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_stop
****************************************/

BOOL cg_upnp_ssdpresponse_serverlist_stop(CgUpnpSSDPResponseServerList *ssdpServerList)
{
	CgUpnpSSDPResponseServer *ssdpServer;
	
	for (ssdpServer = cg_upnp_ssdpresponse_serverlist_gets(ssdpServerList); ssdpServer != NULL; ssdpServer = cg_upnp_ssdpresponse_server_next(ssdpServer))
		cg_upnp_ssdpresponse_server_stop(ssdpServer);
		
	return TRUE;
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_setlistener
****************************************/

void cg_upnp_ssdpresponse_serverlist_setlistener(CgUpnpSSDPResponseServerList *ssdpServerList, CG_UPNP_SSDP_LISTNER listener)
{
	CgUpnpSSDPResponseServer *ssdpServer;
	
	for (ssdpServer = cg_upnp_ssdpresponse_serverlist_gets(ssdpServerList); ssdpServer != NULL; ssdpServer = cg_upnp_ssdpresponse_server_next(ssdpServer))
		cg_upnp_ssdpresponse_server_setlistener(ssdpServer, listener);
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_setuserdata
****************************************/

void cg_upnp_ssdpresponse_serverlist_setuserdata(CgUpnpSSDPResponseServerList *ssdpServerList, void *data)
{
	CgUpnpSSDPResponseServer *ssdpServer;
	
	for (ssdpServer = cg_upnp_ssdpresponse_serverlist_gets(ssdpServerList); ssdpServer != NULL; ssdpServer = cg_upnp_ssdpresponse_server_next(ssdpServer))
		cg_upnp_ssdpresponse_server_setuserdata(ssdpServer, data);
}

/****************************************
* cg_upnp_ssdpresponse_serverlist_post
****************************************/

BOOL cg_upnp_ssdpresponse_serverlist_post(CgUpnpSSDPResponseServerList *ssdpServerList, CgUpnpSSDPRequest *ssdpReq)
{
	CgUpnpSSDPResponseServer *ssdpServer;
	
	for (ssdpServer = cg_upnp_ssdpresponse_serverlist_gets(ssdpServerList); ssdpServer != NULL; ssdpServer = cg_upnp_ssdpresponse_server_next(ssdpServer))
		cg_upnp_ssdpresponse_server_post(ssdpServer, ssdpReq);
		
	return TRUE;
}

/****************************************
* CG_UPNP_NOUSE_CONTROLPOINT (End)
****************************************/

#endif
