/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-PDU-Contents"
 * 	found in "E1AP-PDU-Contents.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#ifndef	_UE_associatedLogicalE1_ConnectionListResAck_H_
#define	_UE_associatedLogicalE1_ConnectionListResAck_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolIE_SingleContainer;

/* UE-associatedLogicalE1-ConnectionListResAck */
typedef struct UE_associatedLogicalE1_ConnectionListResAck {
	A_SEQUENCE_OF(struct ProtocolIE_SingleContainer) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UE_associatedLogicalE1_ConnectionListResAck_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UE_associatedLogicalE1_ConnectionListResAck;
extern asn_SET_OF_specifics_t asn_SPC_UE_associatedLogicalE1_ConnectionListResAck_specs_1;
extern asn_TYPE_member_t asn_MBR_UE_associatedLogicalE1_ConnectionListResAck_1[1];
extern asn_per_constraints_t asn_PER_type_UE_associatedLogicalE1_ConnectionListResAck_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _UE_associatedLogicalE1_ConnectionListResAck_H_ */
#include <asn_internal.h>
