#pragma once
#include <stdio.h>

typedef enum _STATE_E {
	SAFE,
	SUSPICIOUS,
	DANGEROUS,
}STATE_E;

typedef enum _EVENT_E {
	INIT_EVT,
	SGX_STRCAT_NEEDED,
	SGX_CODE_DONE_ERROR,
	SGX_CODE_DONE_SUCCESS,
}EVENT_E;