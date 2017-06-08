#include "SecureFunctions_u.h"
#include <errno.h>

typedef struct ms_enclaveStrcpy_t {
	char* ms_src;
	char* ms_dest;
	size_t ms_lenDest;
} ms_enclaveStrcpy_t;

typedef struct ms_enclaveDivideByZero_t {
	int* ms_num1;
	int* ms_num2;
	int* ms_result;
} ms_enclaveDivideByZero_t;

typedef struct ms_enclaveGetArrayIndex_t {
	int* ms_array;
	int ms_index;
	int* ms_value;
} ms_enclaveGetArrayIndex_t;

typedef struct ms_enclaveSetArrayIndex_t {
	int* ms_array;
	int ms_index;
	int* ms_value;
} ms_enclaveSetArrayIndex_t;

typedef struct ms_ocall_print_string_t {
	char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	void* ms_waiter;
	void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL SecureFunctions_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string((const char*)ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureFunctions_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureFunctions_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall((const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureFunctions_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall((const void*)ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureFunctions_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall((const void*)ms->ms_waiter, (const void*)ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL SecureFunctions_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall((const void**)ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[6];
} ocall_table_SecureFunctions = {
	6,
	{
		(void*)(uintptr_t)SecureFunctions_ocall_print_string,
		(void*)(uintptr_t)SecureFunctions_sgx_oc_cpuidex,
		(void*)(uintptr_t)SecureFunctions_sgx_thread_wait_untrusted_event_ocall,
		(void*)(uintptr_t)SecureFunctions_sgx_thread_set_untrusted_event_ocall,
		(void*)(uintptr_t)SecureFunctions_sgx_thread_setwait_untrusted_events_ocall,
		(void*)(uintptr_t)SecureFunctions_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};

sgx_status_t enclaveStrcpy(sgx_enclave_id_t eid, char* src, char* dest, size_t lenDest)
{
	sgx_status_t status;
	ms_enclaveStrcpy_t ms;
	ms.ms_src = src;
	ms.ms_dest = dest;
	ms.ms_lenDest = lenDest;
	status = sgx_ecall(eid, 0, &ocall_table_SecureFunctions, &ms);
	return status;
}

sgx_status_t enclaveDivideByZero(sgx_enclave_id_t eid, int* num1, int* num2, int* result)
{
	sgx_status_t status;
	ms_enclaveDivideByZero_t ms;
	ms.ms_num1 = num1;
	ms.ms_num2 = num2;
	ms.ms_result = result;
	status = sgx_ecall(eid, 1, &ocall_table_SecureFunctions, &ms);
	return status;
}

sgx_status_t enclaveGetArrayIndex(sgx_enclave_id_t eid, int* array, int index, int* value)
{
	sgx_status_t status;
	ms_enclaveGetArrayIndex_t ms;
	ms.ms_array = array;
	ms.ms_index = index;
	ms.ms_value = value;
	status = sgx_ecall(eid, 2, &ocall_table_SecureFunctions, &ms);
	return status;
}

sgx_status_t enclaveSetArrayIndex(sgx_enclave_id_t eid, int* array, int index, int* value)
{
	sgx_status_t status;
	ms_enclaveSetArrayIndex_t ms;
	ms.ms_array = array;
	ms.ms_index = index;
	ms.ms_value = value;
	status = sgx_ecall(eid, 3, &ocall_table_SecureFunctions, &ms);
	return status;
}

