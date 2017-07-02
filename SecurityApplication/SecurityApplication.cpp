// SecurityApplication.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include "stdafx.h"
#include "Dictionary.h"
#include "parsefile.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	defineDictionary();
	parseFile(g_dictionary,"..\\..\\..\\..\\ExecutionFiles\\StackOverrun.c", "..\\..\\..\\SecureApplication.cpp");

	return 0;
}