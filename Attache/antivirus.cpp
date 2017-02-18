#include <stdio.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include "sgx_urts.h"
#include "Sample_Enclave_u.h"
#include "Security.h"
#define ENCLAVE_FILE _T("Sample_Enclave.signed.dll")
#define MAX_BUF_LEN 100

using namespace std;

int main()
{
	sgx_enclave_id_t eid;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
	char buffer[MAX_BUF_LEN] = "Hello World!";

	if (!init())
	{
		printf("init failed\n");
		getchar();
		return -1;
	}

	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);

	if (ret != SGX_SUCCESS)
	{
		printf("App: error %#x, failed to create enclave.\n", ret);
		return -1;
	}
	ofstream myfile;
	myfile.open("appexample.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
	while (1)
	{
		if (!isAntiVirusRunning())
		{
			printf("at least one of services is stop\n");
			// A bunch of Enclave calls (ECALL) will happen here.
			checkAntiVirusProcess(eid);
			getchar();
		}
	}

	// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid))
	{
		return -1;
	}
	getchar();
	return 0;
}