#pragma once

#include <iostream>
#include <string.h>
#include "Sample_Enclave_u.h"

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#define SIZE_OF_BUFFER	10240

using namespace std;

typedef enum _STATE_E {
	SAFE,
	SUSPICIOUS,
	DANGEROUS,
}STATE_E;

typedef enum _EVENT_E {
	INIT_EVT,
	SGX_STRCAT_WITH_SGX,
	SGX_STRCAT_WITHOUT_SGX,
	SGX_CODE_DONE_ERROR,
	SGX_CODE_DONE_SUCCESS,
}EVENT_E;

class EventData
{
public:
	EventData();
	~EventData();
};

class StrcatEvent : EventData
{
public:
	char* buffer;
	char* bufferInput;
	StrcatEvent(char* buf, char* bufInput);
	~StrcatEvent();
};

class StateMachine
{
public:
	EVENT_E event;
	STATE_E state;
	StateMachine();
	~StateMachine();
	bool eventHandle(sgx_enclave_id_t eid, EventData* eventData);
};



#endif