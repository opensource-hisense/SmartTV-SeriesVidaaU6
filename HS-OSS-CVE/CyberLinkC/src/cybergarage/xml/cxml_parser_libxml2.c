/******************************************************************
*
*	CyberXML for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File:	cxml_parser_libxml2.c
*
*	Revision;
*
*	08/19/05
*		- first version
*		- The functions are created using a contribution from
*		  Smolander Visa <visa.smolander@nokia.com> to CyberLink for C++.
*
******************************************************************/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

/****************************************
* Compiler Switch (BEGIN)
****************************************/

#if defined(CG_XMLPARSER_LIBXML2)

/****************************************
* Header Files
****************************************/

#include <cybergarage/xml/cxml.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>

/****************************************
* cg_libxml2_parse
****************************************/

BOOL cg_libxml2_parse(CgXmlNodeList *nodeList, xmlDocPtr doc, xmlNodePtr cur, int depth)
{
	CgXmlNode *node;
	xmlChar *key;
	xmlAttrPtr prop;
	xmlChar *attrValue;
	xmlNodePtr child;

	/**** This should never happen... ****/
	if ( cur == NULL )
		return FALSE;

	/**** Recursion depth > 10. Are you sure this is OK!?" ****/
	if ( depth > 12 )
		return FALSE;

	/**** We are only interested in XML_ELEMENT_NODEs.
	       Note, that the resulting Node tree will only contain them... ****/

	if ( cur->type == XML_ELEMENT_NODE ) {

		node = cg_xml_node_new();
		cg_xml_nodelist_add(nodeList, node);

		/**** Set name and value for the new node. ****/
		cg_xml_node_setname(node, (char *)cur->name);
		xmlChar *key = NULL;
		if ( cur->xmlChildrenNode )
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		else
			key = xmlNodeListGetString(doc, cur, 1);
		cg_xml_node_setvalue(node, key);
		xmlFree( key );

		/**** Get attributes (if any) and copy to newNode ****/
		prop = cur->properties;
		while (prop) {
			attrValue = xmlNodeListGetString(doc, prop->xmlChildrenNode, 1);
			cg_xml_node_setattribute(node, (char *)prop->name, attrValue);
			xmlFree(attrValue);
			prop = prop->next;
		}

		/**** Then convert (recursively) all the children of the current node ****/
		xmlNodePtr child = cur->xmlChildrenNode;
		while (child != NULL) {
			if (cg_libxml2_parse(cg_xml_node_getchildnodelist(node), doc, child, depth + 1) == FALSE)
				return FALSE;
			child = child->next;
		}

	}

	return TRUE;
}

/****************************************
* cg_xml_parse
****************************************/

BOOL cg_xml_parse(CgXmlParser *parser, CgXmlNodeList *nodeList, char *data, int len)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	BOOL parseSuccess;

	// First, parse the XML memory buffer ito a DOM object
	doc = xmlParseMemory( data, len );
	if ( doc == NULL )
		return FALSE;

	// Then get a pointer to the root node
	cur = xmlDocGetRootElement( doc );
	if (cur == NULL) {
		xmlFreeDoc(doc);
		return FALSE;
	}

	// Then convert the libxml2 type node tree into CyberLink XML node tree
	parseSuccess = cg_libxml2_parse(nodeList, doc, cur, 0);

	// Now all data is copied to CyberLink, release the original DOM object tree
	xmlFreeDoc(doc);

	return parseSuccess;
}

/****************************************
* Compiler Switch (END)
****************************************/

#endif
