
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

	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);

	if (ret != SGX_SUCCESS)
	{
		printf("App: error %#x, failed to create enclave.\n", ret);
		return -1;
	}

	// ------------- example 1 -------------
	ofstream outFile1;
	outFile1.open("output1.txt");
	streambuf* sbuf1 = cout.rdbuf();
	cout.rdbuf(outFile1.rdbuf());

	string filename = "text2.5KB.txt";
	ifstream file1(filename.c_str());
	stringstream bufferStringstream1;

	bufferStringstream1 << file1.rdbuf();
	string str = bufferStringstream1.str();
	strcpy(buffer, str.c_str());

	filename = "text5KB.txt";
	ifstream file2(filename.c_str());
	stringstream bufferStringstream2;

	bufferStringstream2 << file2.rdbuf();
	str = bufferStringstream2.str();
	strcpy(bufferInput, str.c_str());
	StrcatEvent* strcatEvt = new StrcatEvent(buffer, bufferInput);

	printf("strlen(buffer) = %d\nsizeof(buffer) = %d\n", strlen(buffer), sizeof(*buffer));
	event = (((strlen(buffer) * 100) / SIZE_OF_BUFFER) < 80) ? SGX_STRCAT_WITHOUT_SGX : SGX_STRCAT_WITH_SGX;
	sm->event = event;

	sm->state = SUSPICIOUS;
	sm->eventHandle(eid, (EventData*)strcatEvt);

	cout << buffer << endl;
	
	// ------------- example 2 -------------
	ofstream outFile2;
	outFile2.open("output2.txt");
	streambuf* sbuf2 = cout.rdbuf();
	cout.rdbuf(outFile2.rdbuf());

	filename = "text8.5KB.txt";
	ifstream file3(filename.c_str());
	stringstream bufferStringstream3;

	bufferStringstream3 << file3.rdbuf();
	str = bufferStringstream3.str();
	strcpy(buffer, str.c_str());

	strcatEvt->bufferInput = bufferInput;

	event = (((strlen(buffer) * 100) / SIZE_OF_BUFFER) < 80) ? SGX_STRCAT_WITHOUT_SGX : SGX_STRCAT_WITH_SGX;
	sm->event = event;

	printf("%p\n", buffer);
	sm->state = SUSPICIOUS;
	sm->eventHandle(eid, (EventData*)strcatEvt);

	cout << buffer << endl;

	// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid))
		return -1;

	free(sm);
	free(buffer);
	free(bufferInput);

	getchar();
	
	return 0;
}
