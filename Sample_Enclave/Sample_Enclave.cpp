#include "Sample_Enclave_t.h"
#include "sgx_trts.h"
//#include <windows.h>
#include <string.h>

void enclaveStrcatBuf(char* src, size_t lenSrc, char* dest, size_t lenDest)
{
	if (lenDest > (strlen(src) + strlen(dest)))
	{
		strncat(dest, src, strlen(src) + 1);
		if (lenDest > (strlen(dest) + 13))
		{
			strncat(dest, "enclave func", 13);
		}
	}
	else
	{
		strncat(dest, src, lenDest - strlen(dest) - 31);
		strncat(dest, " can't append all src to dest\n", 31);
		ocall_print_string("can't append all src to dest\n");
	}
}