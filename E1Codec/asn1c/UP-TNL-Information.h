/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#ifndef	_UP_TNL_Information_H_
#define	_UP_TNL_Information_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UP_TNL_Information_PR {
	UP_TNL_Information_PR_NOTHING,	/* No components present */
	UP_TNL_Information_PR_gTPTunnel,
	UP_TNL_Information_PR_choice_extension
} UP_TNL_Information_PR;

/* Forward declarations */
struct GTPTunnel;
struct ProtocolIE_SingleContainer;

/* UP-TNL-Information */
typedef struct UP_TNL_Information {
	UP_TNL_Information_PR present;
	union UP_TNL_Information_u {
		struct GTPTunnel	*gTPTunnel;
		struct ProtocolIE_SingleContainer	*choice_extension;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UP_TNL_Information_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UP_TNL_Information;
extern asn_CHOICE_specifics_t asn_SPC_UP_TNL_Information_specs_1;
extern asn_TYPE_member_t asn_MBR_UP_TNL_Information_1[2];
extern asn_per_constraints_t asn_PER_type_UP_TNL_Information_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _UP_TNL_Information_H_ */
#include <asn_internal.h>
