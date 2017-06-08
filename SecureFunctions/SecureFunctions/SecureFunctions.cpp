#include "SecureFunctions_t.h"
#include "sgx_trts.h"
#include <string.h>
#include <wchar.h>

void enclaveStrcpy(char* src, char* dest, size_t lenDest)
{
	strncpy(src, dest, lenDest);
}

void enclaveDivideByZero(int* num1, int* num2, int* result)
{
	*result = *num1 / *num2;
}

void enclaveGetArrayIndex(int* array, int index, int* value)
{
	*value = array[index];
}

void enclaveSetArrayIndex(int* array, int index, int* value)
{
	array[index] = *value;
}