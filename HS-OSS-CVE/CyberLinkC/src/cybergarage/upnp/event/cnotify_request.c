/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: cnotify_request.c
*
*	Revision:
*
*	07/07/05
*		- first revision
*
******************************************************************/

#include <cybergarage/upnp/event/cnotify.h>
#include <cybergarage/upnp/event/cevent.h>
#include <cybergarage/upnp/event/csubscriber.h>
#include <cybergarage/upnp/event/cproperty.h>
#include <cybergarage/upnp/control/ccontrol.h>
#include <cybergarage/upnp/cstatevariable.h>

/****************************************
* CG_UPNP_NOUSE_SUBSCRIPTION (Begin)
****************************************/

#if !defined(CG_UPNP_NOUSE_SUBSCRIPTION)

/****************************************
* Static Function Prototype
****************************************/

static CgXmlNode *cg_upnp_event_notify_request_createpropertysetnode(CgUpnpStateVariable *statVar);

#define cg_upnp_event_notify_request_getpropertylistonly(notifyReq) ((CgUpnpPropertyList *)cg_soap_request_getuserdata(notifyReq))

/****************************************
* cg_upnp_event_notify_request_new
****************************************/

CgUpnpNotifyRequest *cg_upnp_event_notify_request_new()
{
	CgUpnpNotifyRequest *notifyReq;
	CgUpnpPropertyList *propList;
	
	notifyReq = cg_soap_request_new();

	propList = cg_upnp_propertylist_new();
	cg_upnp_event_notify_request_setpropertylist(notifyReq, propList);

	return notifyReq;
}

/****************************************
* cg_upnp_event_notify_request_delete
****************************************/

void cg_upnp_event_notify_request_delete(CgUpnpNotifyRequest *notifyReq)
{
	CgUpnpPropertyList *propList;

	propList = cg_upnp_event_notify_request_getpropertylistonly(notifyReq);
	cg_upnp_propertylist_delete(propList);
	
	cg_soap_request_delete(notifyReq);
}

/****************************************
* cg_upnp_event_notify_request_clear
****************************************/

void cg_upnp_event_notify_request_clear(CgUpnpNotifyRequest *notifyReq)
{
	CgUpnpPropertyList *propList;

	propList = cg_upnp_event_notify_request_getpropertylistonly(notifyReq);
	cg_upnp_propertylist_clear(propList);
	
	cg_soap_request_clear(notifyReq);
}

/****************************************
* cg_upnp_event_notify_request_setsid
****************************************/

void cg_upnp_event_notify_request_setsid(CgUpnpNotifyRequest *soapReq, char *sid)
{
	char buf[CG_UPNP_SUBSCRIPTION_SID_HEADER_SIZE];
	cg_http_packet_setheadervalue((CgHttpPacket*)(soapReq->httpReq), CG_HTTP_SID, cg_upnp_event_subscription_tosidheaderstring(sid, buf, sizeof(buf)));
}

/****************************************
* cg_upnp_event_notify_request_setpropertysetnode
****************************************/

BOOL cg_upnp_event_notify_request_setpropertysetnode(CgUpnpNotifyRequest *notifyReq, CgUpnpSubscriber *sub, CgUpnpStateVariable *statVar)
{
	CgHttpRequest *httpReq;	
	CgXmlNode *propSetNode;

	httpReq = cg_soap_request_gethttprequest(notifyReq);

	cg_http_request_setmethod(httpReq, CG_HTTP_NOTIFY);
	cg_http_request_seturi(httpReq, cg_upnp_subscriber_getdeliverypath(sub));
	cg_http_request_sethost(httpReq, cg_upnp_subscriber_getdeliveryhost(sub), cg_upnp_subscriber_getdeliveryport(sub));
	cg_upnp_event_notify_request_setnt(notifyReq, CG_UPNP_NT_EVENT);
	cg_upnp_event_notify_request_setnts(notifyReq, CG_UPNP_NTS_PROPCHANGE);
	cg_upnp_event_notify_request_setsid(notifyReq, cg_upnp_subscriber_getsid(sub));
	cg_upnp_event_notify_request_setseq(notifyReq, cg_upnp_subscriber_getnotifycount(sub));

	propSetNode = cg_upnp_event_notify_request_createpropertysetnode(statVar);
	cg_soap_request_setcontent(notifyReq, propSetNode);
	cg_xml_node_delete(propSetNode);

	return TRUE;
}

/****************************************
* cg_upnp_event_notify_request_createpropertysetnode
****************************************/

static CgXmlNode *cg_upnp_event_notify_request_createpropertysetnode(CgUpnpStateVariable *statVar)
{
	CgXmlNode *propSetNode;
	CgXmlNode *propNode;
	CgXmlNode *varNode;
		
	propSetNode = cg_xml_node_new();
	cg_xml_node_setname(propSetNode, CG_UPNP_NOTIFY_XMLNS CG_SOAP_DELIM CG_UPNP_NOTIFY_PROPERTYSET);
	cg_xml_node_setnamespace(propSetNode, CG_UPNP_NOTIFY_XMLNS, CG_UPNP_SUBSCRIPTION_XMLNS);
	
	propNode = cg_xml_node_new();
	cg_xml_node_setname(propNode, CG_UPNP_NOTIFY_XMLNS CG_SOAP_DELIM CG_UPNP_NOTIFY_PROPERTY);
	cg_xml_node_addchildnode(propSetNode, propNode);
	
	varNode = cg_xml_node_new();
	cg_xml_node_setname(varNode, cg_upnp_statevariable_getname(statVar));
	cg_xml_node_setvalue(varNode, cg_upnp_statevariable_getvalue(statVar));
	cg_xml_node_addchildnode(propNode, varNode);
	
	return propSetNode;
}

/****************************************
* cg_upnp_event_notify_request_getvariablenode
****************************************/

CgXmlNode *cg_upnp_event_notify_request_getvariablenode(CgUpnpNotifyRequest *nofityReq)
{
	CgXmlNode *propSetNode;
	CgXmlNode *propNode;

	propSetNode = cg_upnp_event_notify_request_getpropertysetnode(nofityReq);
	if (propSetNode == NULL)
		return NULL;
	if (cg_xml_node_haschildnodes(propSetNode) == FALSE)
		return NULL;

	propNode = cg_xml_node_getchildnodes(propSetNode);
	if (propNode == NULL)
		return NULL;
	if (cg_xml_node_haschildnodes(propNode) == FALSE)
		return NULL;

	return cg_xml_node_getchildnodes(propNode);		
}

/****************************************
* cg_upnp_property_createfromnode
****************************************/

static CgUpnpProperty *cg_upnp_property_createfromnode(CgXmlNode *varNode) 
{
	CgUpnpProperty *prop;
	char *varName;
	char *varValue;
	int colonIdx;
	
	prop = cg_upnp_property_new();
	if (varNode == NULL)
		return prop;
		
	// remove the event namespace
	varName = cg_xml_node_getname(varNode);
	colonIdx = cg_strstr(varName, ":");
	if (0 <= colonIdx)
		varName = varName + colonIdx + 1;
	varValue = cg_xml_node_getvalue(varNode);
	cg_upnp_property_setname(prop, varName);
	cg_upnp_property_setvalue(prop, varValue);
	
	return prop;
}

/****************************************
* cg_upnp_event_notify_request_getpropertylist
****************************************/

CgUpnpPropertyList *cg_upnp_event_notify_request_getpropertylist(CgUpnpNotifyRequest *notifyReq)
{
	CgUpnpPropertyList *propList;
	CgXmlNode *propSetNode;
	CgXmlNode *propNode;
	CgXmlNode *varNode;
	CgUpnpProperty *prop;
	char *sid;
	int seq;
	
	sid = cg_upnp_event_notify_request_getsid(notifyReq);
	seq = cg_upnp_event_notify_request_getseq(notifyReq);
		
	propList = cg_upnp_event_notify_request_getpropertylistonly(notifyReq);
	cg_upnp_propertylist_clear(propList);
	
	propSetNode = cg_soap_request_getrootnoode(notifyReq);
	for (propNode = cg_xml_node_getchildnodes(propSetNode); propNode != NULL; propNode = cg_xml_node_next(propNode)) {
		varNode = cg_xml_node_getchildnodes(propNode);
		prop = cg_upnp_property_createfromnode(varNode);
		cg_upnp_property_setsid(prop, sid);
		cg_upnp_property_setseq(prop, seq);
		cg_upnp_propertylist_add(propList, prop);
	}
	
	return propList;
}

/****************************************
* CG_UPNP_NOUSE_SUBSCRIPTION (End)
****************************************/

#endif
