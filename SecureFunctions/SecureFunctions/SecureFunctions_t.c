#include "SecureFunctions_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */

#include <errno.h>
#include <string.h> /* for memcpy etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)


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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4200)
#endif

static sgx_status_t SGX_CDECL sgx_enclaveStrcpy(void* pms)
{
	ms_enclaveStrcpy_t* ms = SGX_CAST(ms_enclaveStrcpy_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_src = ms->ms_src;
	size_t _len_src = _tmp_src ? strlen(_tmp_src) + 1 : 0;
	char* _in_src = NULL;
	char* _tmp_dest = ms->ms_dest;
	size_t _len_dest = _tmp_dest ? strlen(_tmp_dest) + 1 : 0;
	char* _in_dest = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_enclaveStrcpy_t));
	CHECK_UNIQUE_POINTER(_tmp_src, _len_src);
	CHECK_UNIQUE_POINTER(_tmp_dest, _len_dest);

	if (_tmp_src != NULL) {
		_in_src = (char*)malloc(_len_src);
		if (_in_src == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_src, _tmp_src, _len_src);
		_in_src[_len_src - 1] = '\0';
	}
	if (_tmp_dest != NULL) {
		_in_dest = (char*)malloc(_len_dest);
		if (_in_dest == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_dest, _tmp_dest, _len_dest);
		_in_dest[_len_dest - 1] = '\0';
	}
	enclaveStrcpy(_in_src, _in_dest, ms->ms_lenDest);
err:
	if (_in_src) free(_in_src);
	if (_in_dest) {
		memcpy(_tmp_dest, _in_dest, _len_dest);
		free(_in_dest);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_enclaveDivideByZero(void* pms)
{
	ms_enclaveDivideByZero_t* ms = SGX_CAST(ms_enclaveDivideByZero_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int* _tmp_num1 = ms->ms_num1;
	size_t _len_num1 = sizeof(*_tmp_num1);
	int* _in_num1 = NULL;
	int* _tmp_num2 = ms->ms_num2;
	size_t _len_num2 = sizeof(*_tmp_num2);
	int* _in_num2 = NULL;
	int* _tmp_result = ms->ms_result;
	size_t _len_result = sizeof(*_tmp_result);
	int* _in_result = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_enclaveDivideByZero_t));
	CHECK_UNIQUE_POINTER(_tmp_num1, _len_num1);
	CHECK_UNIQUE_POINTER(_tmp_num2, _len_num2);
	CHECK_UNIQUE_POINTER(_tmp_result, _len_result);

	if (_tmp_num1 != NULL) {
		_in_num1 = (int*)malloc(_len_num1);
		if (_in_num1 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_num1, _tmp_num1, _len_num1);
	}
	if (_tmp_num2 != NULL) {
		_in_num2 = (int*)malloc(_len_num2);
		if (_in_num2 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_num2, _tmp_num2, _len_num2);
	}
	if (_tmp_result != NULL) {
		if ((_in_result = (int*)malloc(_len_result)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_result, 0, _len_result);
	}
	enclaveDivideByZero(_in_num1, _in_num2, _in_result);
err:
	if (_in_num1) free(_in_num1);
	if (_in_num2) free(_in_num2);
	if (_in_result) {
		memcpy(_tmp_result, _in_result, _len_result);
		free(_in_result);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_enclaveGetArrayIndex(void* pms)
{
	ms_enclaveGetArrayIndex_t* ms = SGX_CAST(ms_enclaveGetArrayIndex_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int* _tmp_array = ms->ms_array;
	size_t _len_array = sizeof(*_tmp_array);
	int* _in_array = NULL;
	int* _tmp_value = ms->ms_value;
	size_t _len_value = sizeof(*_tmp_value);
	int* _in_value = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_enclaveGetArrayIndex_t));
	CHECK_UNIQUE_POINTER(_tmp_array, _len_array);
	CHECK_UNIQUE_POINTER(_tmp_value, _len_value);

	if (_tmp_array != NULL) {
		_in_array = (int*)malloc(_len_array);
		if (_in_array == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_array, _tmp_array, _len_array);
	}
	if (_tmp_value != NULL) {
		if ((_in_value = (int*)malloc(_len_value)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_value, 0, _len_value);
	}
	enclaveGetArrayIndex(_in_array, ms->ms_index, _in_value);
err:
	if (_in_array) free(_in_array);
	if (_in_value) {
		memcpy(_tmp_value, _in_value, _len_value);
		free(_in_value);
	}

	return status;
}

static sgx_status_t SGX_CDECL sgx_enclaveSetArrayIndex(void* pms)
{
	ms_enclaveSetArrayIndex_t* ms = SGX_CAST(ms_enclaveSetArrayIndex_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int* _tmp_array = ms->ms_array;
	size_t _len_array = sizeof(*_tmp_array);
	int* _in_array = NULL;
	int* _tmp_value = ms->ms_value;
	size_t _len_value = sizeof(*_tmp_value);
	int* _in_value = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_enclaveSetArrayIndex_t));
	CHECK_UNIQUE_POINTER(_tmp_array, _len_array);
	CHECK_UNIQUE_POINTER(_tmp_value, _len_value);

	if (_tmp_array != NULL) {
		_in_array = (int*)malloc(_len_array);
		if (_in_array == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_array, _tmp_array, _len_array);
	}
	if (_tmp_value != NULL) {
		_in_value = (int*)malloc(_len_value);
		if (_in_value == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_value, _tmp_value, _len_value);
	}
	enclaveSetArrayIndex(_in_array, ms->ms_index, _in_value);
err:
	if (_in_array) {
		memcpy(_tmp_array, _in_array, _len_array);
		free(_in_array);
	}
	if (_in_value) free(_in_value);

	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv;} ecall_table[4];
} g_ecall_table = {
	4,
	{
		{(void*)(uintptr_t)sgx_enclaveStrcpy, 0},
		{(void*)(uintptr_t)sgx_enclaveDivideByZero, 0},
		{(void*)(uintptr_t)sgx_enclaveGetArrayIndex, 0},
		{(void*)(uintptr_t)sgx_enclaveSetArrayIndex, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[6][4];
} g_dyn_entry_table = {
	6,
	{
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
		{0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;

	ocalloc_size += (str != NULL && sgx_is_within_enclave(str, _len_str)) ? _len_str : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));

	if (str != NULL && sgx_is_within_enclave(str, _len_str)) {
		ms->ms_str = (char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_str);
		memcpy((void*)ms->ms_str, str, _len_str);
	} else if (str == NULL) {
		ms->ms_str = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	status = sgx_ocall(0, ms);


	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(*cpuinfo);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	ocalloc_size += (cpuinfo != NULL && sgx_is_within_enclave(cpuinfo, _len_cpuinfo)) ? _len_cpuinfo : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));

	if (cpuinfo != NULL && sgx_is_within_enclave(cpuinfo, _len_cpuinfo)) {
		ms->ms_cpuinfo = (int*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		memcpy(ms->ms_cpuinfo, cpuinfo, _len_cpuinfo);
	} else if (cpuinfo == NULL) {
		ms->ms_cpuinfo = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_leaf = leaf;
	ms->ms_subleaf = subleaf;
	status = sgx_ocall(1, ms);

	if (cpuinfo) memcpy((void*)cpuinfo, ms->ms_cpuinfo, _len_cpuinfo);

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));

	ms->ms_self = SGX_CAST(void*, self);
	status = sgx_ocall(2, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));

	ms->ms_waiter = SGX_CAST(void*, waiter);
	status = sgx_ocall(3, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));

	ms->ms_waiter = SGX_CAST(void*, waiter);
	ms->ms_self = SGX_CAST(void*, self);
	status = sgx_ocall(4, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(*waiters);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;

	ocalloc_size += (waiters != NULL && sgx_is_within_enclave(waiters, _len_waiters)) ? _len_waiters : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));

	if (waiters != NULL && sgx_is_within_enclave(waiters, _len_waiters)) {
		ms->ms_waiters = (void**)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		memcpy((void*)ms->ms_waiters, waiters, _len_waiters);
	} else if (waiters == NULL) {
		ms->ms_waiters = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_total = total;
	status = sgx_ocall(5, ms);

	if (retval) *retval = ms->ms_retval;

	sgx_ocfree();
	return status;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
