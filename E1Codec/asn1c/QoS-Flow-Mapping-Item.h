/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#ifndef	_QoS_Flow_Mapping_Item_H_
#define	_QoS_Flow_Mapping_Item_H_


#include <asn_application.h>

/* Including external dependencies */
#include "QoS-Flow-Identifier.h"
#include "QoS-Flow-Mapping-Indication.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ProtocolExtensionContainer;

/* QoS-Flow-Mapping-Item */
typedef struct QoS_Flow_Mapping_Item {
	QoS_Flow_Identifier_t	 qoS_Flow_Identifier;
	QoS_Flow_Mapping_Indication_t	*qoSFlowMappingIndication;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} QoS_Flow_Mapping_Item_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_QoS_Flow_Mapping_Item;
extern asn_SEQUENCE_specifics_t asn_SPC_QoS_Flow_Mapping_Item_specs_1;
extern asn_TYPE_member_t asn_MBR_QoS_Flow_Mapping_Item_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _QoS_Flow_Mapping_Item_H_ */
#include <asn_internal.h>
