#include "stdafx.h"
#include "Dictionary.h"

map<string, string> g_dictionary;

void defineDictionary()
{
	g_dictionary["strcpy"] = "strcpy";
	g_dictionary["strcat"] = "strcat";
	g_dictionary["/"] = "division";
}