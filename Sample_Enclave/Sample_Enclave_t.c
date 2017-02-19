#include "Sample_Enclave_t.h"

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


typedef struct ms_enclaveStrcatBuf_t {
	char* ms_src;
	size_t ms_lenSrc;
	char* ms_dest;
	size_t ms_lenDest;
} ms_enclaveStrcatBuf_t;

typedef struct ms_ocall_print_string_t {
	char* ms_str;
} ms_ocall_print_string_t;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4200)
#endif

static sgx_status_t SGX_CDECL sgx_enclaveStrcatBuf(void* pms)
{
	ms_enclaveStrcatBuf_t* ms = SGX_CAST(ms_enclaveStrcatBuf_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_src = ms->ms_src;
	size_t _len_src = ms->ms_lenSrc;
	char* _in_src = NULL;
	char* _tmp_dest = ms->ms_dest;
	size_t _len_dest = ms->ms_lenDest;
	char* _in_dest = NULL;

	CHECK_REF_POINTER(pms, sizeof(ms_enclaveStrcatBuf_t));
	CHECK_UNIQUE_POINTER(_tmp_src, _len_src);
	CHECK_UNIQUE_POINTER(_tmp_dest, _len_dest);

	if (_tmp_src != NULL) {
		_in_src = (char*)malloc(_len_src);
		if (_in_src == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_src, _tmp_src, strlen(_tmp_src) + 1);
	}
	if (_tmp_dest != NULL) {
		_in_dest = (char*)malloc(_len_dest);
		if (_in_dest == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memcpy(_in_dest, _tmp_dest, strlen(_tmp_dest) + 1);
	}
	enclaveStrcatBuf(_in_src, _len_src, _in_dest, _len_dest);
err:
	if (_in_src) {
		memcpy(_tmp_src, _in_src, strlen(_in_src) + 1);
		free(_in_src);
	}
	if (_in_dest) {
		memcpy(_tmp_dest, _in_dest, strlen(_in_dest) + 1);
		free(_in_dest);
	}

	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv;} ecall_table[1];
} g_ecall_table = {
	1,
	{
		{(void*)(uintptr_t)sgx_enclaveStrcatBuf, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[1][1];
} g_dyn_entry_table = {
	1,
	{
		{0, },
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif
