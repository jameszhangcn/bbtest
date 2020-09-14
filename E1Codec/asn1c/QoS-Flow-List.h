/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#ifndef	_QoS_Flow_List_H_
#define	_QoS_Flow_List_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct QoS_Flow_Item;

/* QoS-Flow-List */
typedef struct QoS_Flow_List {
	A_SEQUENCE_OF(struct QoS_Flow_Item) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} QoS_Flow_List_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_QoS_Flow_List;
extern asn_SET_OF_specifics_t asn_SPC_QoS_Flow_List_specs_1;
extern asn_TYPE_member_t asn_MBR_QoS_Flow_List_1[1];
extern asn_per_constraints_t asn_PER_type_QoS_Flow_List_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _QoS_Flow_List_H_ */
#include <asn_internal.h>
