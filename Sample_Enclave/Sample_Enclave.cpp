#include "Sample_Enclave_t.h"
#include "sgx_trts.h"
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
		if (lenDest > 31)
		{
			strncpy(dest, "from enclave - can't append src to dest - DANGEROUS STATE\n", 59);
		}
		ocall_print_string("from enclave - can't append src to dest\n");
	}
}