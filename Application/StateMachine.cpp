#include "StateMachine.h"

StateMachine::StateMachine()
{
	event = INIT_EVT;
	state = SAFE;
}

StateMachine::~StateMachine()
{
}

bool StateMachine::eventHandle(sgx_enclave_id_t eid, EventData* eventData)
{
	if (event == SGX_STRCAT_WITH_SGX ||
		event == SGX_STRCAT_WITHOUT_SGX)
	{
		StrcatEvent* strcatEvt = (StrcatEvent*)eventData;
		char* buffer = strcatEvt->buffer;
		char* bufferInput = strcatEvt->bufferInput;

		if (event == SGX_STRCAT_WITH_SGX)
		{
			printf("enclave\n");
			enclaveStrcatBuf(eid, bufferInput, SIZE_OF_BUFFER, buffer, SIZE_OF_BUFFER);
		}
		else //event == SGX_STRCAT_WITHOUT_SGX
		{
			printf("not enclave\n");
			strcat(buffer, bufferInput);
			strcat(buffer, "from App");
		}
	}
	event = SGX_CODE_DONE_SUCCESS;
	return true;
}

EventData::EventData()
{
}

EventData::~EventData()
{
}

StrcatEvent::StrcatEvent(char* buf, char* bufInput)
{
	buffer = buf;
	bufferInput = bufInput;
}

StrcatEvent::~StrcatEvent()
{
}
