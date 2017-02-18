
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "sgx_urts.h"
#include "Sample_Enclave_u.h"
#include "StateMachine.h"
#include <fstream>
#include <sstream>
#define ENCLAVE_FILE _T("Sample_Enclave.signed.dll")

using namespace std;


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
	sgx_enclave_id_t eid;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
	char* buffer = new char[SIZE_OF_BUFFER];	//size 10KB
	StateMachine* sm = new StateMachine();
	char* bufferInput = new char[SIZE_OF_BUFFER];
	string bufferInputStr;	//input from files
	EVENT_E event;
	
	ofstream outFile;
	outFile.open("output.txt");
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(outFile.rdbuf());

	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);

	if (ret != SGX_SUCCESS)
	{
		printf("App: error %#x, failed to create enclave.\n", ret);
		return -1;
	}

	string filename = "text2.5KB.txt";
	ifstream file1(filename.c_str());
	stringstream bufferStringstream1;

	bufferStringstream1 << file1.rdbuf();
	string str = bufferStringstream1.str();
	cout << str.length() << endl;
	strcpy(buffer, str.c_str());

	cout << strlen(buffer) << endl;
	filename = "text5KB.txt";
	ifstream file2(filename.c_str());
	stringstream bufferStringstream2;

	bufferStringstream2 << file2.rdbuf();
	str = bufferStringstream2.str();
	cout << str.length() << endl;
	strcpy(bufferInput, str.c_str());
	cout << strlen(bufferInput) << endl;
	StrcatEvent* strcatEvt = new StrcatEvent(buffer, bufferInput);

	printf("strlen(buffer) = %d\nsizeof(buffer) = %d\n", strlen(buffer), sizeof(*buffer));
	event = (((strlen(buffer) * 100) / SIZE_OF_BUFFER) < 80) ? SGX_STRCAT_WITHOUT_SGX : SGX_STRCAT_WITH_SGX;
	sm->event = event;

	sm->state = SUSPICIOUS;
	sm->eventHandle(eid, (EventData*)strcatEvt);

	cout << "strcat #1" << endl;
	cout << buffer << endl;
	
	filename = "text8.5KB.txt";
	ifstream file3(filename.c_str());
	stringstream bufferStringstream3;

	bufferStringstream3 << file3.rdbuf();
	str = bufferStringstream3.str();
	cout << str.length() << endl;
	strcpy(buffer, str.c_str());
	cout << strlen(buffer) << endl;

	strcatEvt->bufferInput = bufferInput;

	event = (((strlen(buffer) * 100) / SIZE_OF_BUFFER) < 80) ? SGX_STRCAT_WITHOUT_SGX : SGX_STRCAT_WITH_SGX;
	sm->event = event;

	printf("%p\n", buffer);
	sm->state = SUSPICIOUS;
	sm->eventHandle(eid, (EventData*)strcatEvt);

	cout << "strcat #2" << endl;
	cout << buffer << endl;

	// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid))
		return -1;
	free(sm);
	free(buffer);
	free(bufferInput);


	getchar();
	/*
	
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
		*/
	return 0;
}
