// SecureApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SecureFunctions_u.h"


/* OCall functions */
void ocall_print_string(const char *str)
{
	/* Proxy/Bridge will check the length and null-terminate
	* the input string to prevent buffer overflow.
	*/
	printf("%s", str);
}

int main()
{
    return 0;
}

