#ifndef SAMPLE_ENCLAVE_U_H__
#define SAMPLE_ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));

sgx_status_t enclaveStrcatBuf(sgx_enclave_id_t eid, char* src, size_t lenSrc, char* dest, size_t lenDest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
