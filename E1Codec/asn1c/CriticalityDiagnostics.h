/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#ifndef	_CriticalityDiagnostics_H_
#define	_CriticalityDiagnostics_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProcedureCode.h"
#include "TriggeringMessage.h"
#include "Criticality.h"
#include "TransactionID.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CriticalityDiagnostics_IE_List;
struct ProtocolExtensionContainer;

/* CriticalityDiagnostics */
typedef struct CriticalityDiagnostics {
	ProcedureCode_t	*procedureCode;	/* OPTIONAL */
	TriggeringMessage_t	*triggeringMessage;	/* OPTIONAL */
	Criticality_t	*procedureCriticality;	/* OPTIONAL */
	TransactionID_t	*transactionID;	/* OPTIONAL */
	struct CriticalityDiagnostics_IE_List	*iEsCriticalityDiagnostics;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CriticalityDiagnostics_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CriticalityDiagnostics;
extern asn_SEQUENCE_specifics_t asn_SPC_CriticalityDiagnostics_specs_1;
extern asn_TYPE_member_t asn_MBR_CriticalityDiagnostics_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _CriticalityDiagnostics_H_ */
#include <asn_internal.h>
