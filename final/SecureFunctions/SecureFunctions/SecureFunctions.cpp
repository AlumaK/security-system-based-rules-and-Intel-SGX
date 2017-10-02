#include "SecureFunctions_t.h"
#include "sgx_trts_exception.h"
#include "sgx_trts.h"
#include <string.h>
#include <wchar.h>
#include <limits>

void enclaveStrcpy(char* dest, const char* src)
{
	int len;
	if (strlen(src) > sizeof(dest) - 1)
	{
		len = sizeof(dest) - 1;
	}
	else
	{
		len = strlen(src);
	}
	strncpy(dest, src, len + 1);
}

void enclaveDivideByZero(int* num1, int* num2, int* result)
{
	if (*num2 == 0)
	{
		*result = std::numeric_limits<int>::max();
		ocall_print_string(" [Enclave]: tried to devide by 0. cuntinue running the program.");
	}
	else
	{
		*result = *num1 / *num2;
	}
}

int handler_DivideByZero(sgx_exception_info_t* info) {
	//return EXCEPTION_CONTINUE_EXECUTION;
	ocall_print_string("[handler_DivideByZero]");
	return EXCEPTION_CONTINUE_EXECUTION;
}


// Address range: 0x804847d - 0x80484c3
int32_t fibonacci(uint32_t a1) {
	// 0x804847d
	int32_t result; // 0x80484c3_2
	if (a1 >= 2) {
		int32_t v1 = fibonacci(a1 - 1); // 0x80484a7
		result = fibonacci(a1 - 2) + v1;
		// branch -> 0x80484be
	}
	else {
		result = a1;
	}
	// 0x80484be
	return result;
}

int32_t enclaveFibonacci(uint32_t a1)
{
	a1 = (a1 > 30) ? 30 : a1;
	return fibonacci(a1);
}
