#ifndef SECUREFUNCTIONS_U_H__
#define SECUREFUNCTIONS_U_H__

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
void SGX_UBRIDGE(SGX_CDECL, sgx_oc_cpuidex, (int cpuinfo[4], int leaf, int subleaf));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_setwait_untrusted_events_ocall, (const void* waiter, const void* self));
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_multiple_untrusted_events_ocall, (const void** waiters, size_t total));

sgx_status_t enclaveStrcpy(sgx_enclave_id_t eid, char* src, char* dest, size_t lenDest);
sgx_status_t enclaveDivideByZero(sgx_enclave_id_t eid, int* num1, int* num2, int* result);
sgx_status_t enclaveGetArrayIndex(sgx_enclave_id_t eid, int* array, int index, int* value);
sgx_status_t enclaveSetArrayIndex(sgx_enclave_id_t eid, int* array, int index, int* value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
