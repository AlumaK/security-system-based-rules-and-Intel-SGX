<<<<<<< HEAD
// SecurityApplication.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "Dictionary.h"
#include "ParseFile.h"

int main()
{
	defineDictionary();
	findWordFromDictionary(g_dictionary, "..\\..\\BufferOverflow\\StackOverrun.c");
	getchar();
    return 0;
}

=======
// SecurityApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Dictionary.h"

int main()
{
	defineDictionary();

    return 0;
}

>>>>>>> 02a2bd6865d691f248192f21a43041cc72f71b62
