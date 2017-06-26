// SecurityApplication.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "Dictionary.h"
#include "parsefile.h"

int main()
{
	defineDictionary();
	findWordFromDictionary(g_dictionary, "..\\..\\BufferOverflow\\decompile\\StackOverrun.c");
	getchar();
	return 0;
}