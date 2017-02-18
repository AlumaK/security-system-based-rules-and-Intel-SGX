#include "Sample_Enclave_u.h"
#include <errno.h>

typedef struct ms_enclaveStrcatBuf_t {
	char* ms_src;
	size_t ms_lenSrc;
	char* ms_dest;
	size_t ms_lenDest;
} ms_enclaveStrcatBuf_t;

typedef struct ms_ocall_print_string_t {
	char* ms_str;
} ms_ocall_print_string_t;

static sgx_status_t SGX_CDECL Sample_Enclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string((const char*)ms->ms_str);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[1];
} ocall_table_Sample_Enclave = {
	1,
	{
		(void*)(uintptr_t)Sample_Enclave_ocall_print_string,
	}
};

sgx_status_t enclaveStrcatBuf(sgx_enclave_id_t eid, char* src, size_t lenSrc, char* dest, size_t lenDest)
{
	sgx_status_t status;
	ms_enclaveStrcatBuf_t ms;
	ms.ms_src = src;
	ms.ms_lenSrc = lenSrc;
	ms.ms_dest = dest;
	ms.ms_lenDest = lenDest;
	status = sgx_ecall(eid, 0, &ocall_table_Sample_Enclave, &ms);
	return status;
}

