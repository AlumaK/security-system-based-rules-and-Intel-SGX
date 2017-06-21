<<<<<<< HEAD
#include "stdafx.h"
#include "Dictionary.h"

map<string, string> g_dictionary;

void defineDictionary()
{
	g_dictionary["strcpy"] = "strcpy";
	g_dictionary["strcat"] = "strcat";
	g_dictionary["/"] = "division";
=======
#include "Dictionary.h"

void defineDictionary()
{
	m["strcpy"] = "strcpy";
	m["strcat"] = "strcat";
	m["/"] = "division";
>>>>>>> 02a2bd6865d691f248192f21a43041cc72f71b62
}