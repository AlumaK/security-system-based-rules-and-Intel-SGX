
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "sgx_urts.h"
#include "Sample_Enclave_u.h"
#define ENCLAVE_FILE _T("Sample_Enclave.signed.dll")

int main()
{
	//STATE_E state = SAFE;
	//EVENT_E event = INIT_EVT;
	sgx_enclave_id_t eid;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
	char src[30] = "Hello World!", dest[30] = "";

	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);

	if (ret != SGX_SUCCESS)
	{
		printf("App: error %#x, failed to create enclave.\n", ret);
		return -1;
	}
	//case 1 - try to strcat to dest when dest is empty
	printf("dest len is: %d dest len is: %d\n", strlen(dest), sizeof(dest));
	if (((strlen(dest) * 100) / sizeof(dest) * 100) < 80)
	{
		strncat(dest, src, strlen(src));
	}
	else
	{
		//call to enclave function
		enclaveStrcatBuf(eid, src, 30, dest, 30);
	}
	printf("Final destination string : |%s|\n", dest);

	//case 2 - try to strcat to dest when dest is almost full
	strcpy(dest, "abcdefghijklmnopqrstuvwxyz");
	if (((strlen(dest) * 100) / sizeof(dest) ) < 80)
	{
		strncat(dest, src, strlen(src));
	}
	else
	{
		//call to enclave function
		enclaveStrcatBuf(eid, src, 30, dest, 30);
	}
	printf("Final destination string : |%s|\n", dest);
	

	// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid))
		return -1;

	return 0;
}
