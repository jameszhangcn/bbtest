/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-PDU-Contents"
 * 	found in "E1AP-PDU-Contents.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#include "System-GNB-CU-UP-CounterCheckRequest.h"

#include "ProtocolIE-Container.h"
#include "ProtocolIE-SingleContainer.h"
static asn_oer_constraints_t asn_OER_type_System_GNB_CU_UP_CounterCheckRequest_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_System_GNB_CU_UP_CounterCheckRequest_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  2 }	/* (0..2) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_System_GNB_CU_UP_CounterCheckRequest_1[] = {
	{ ATF_POINTER, 0, offsetof(struct System_GNB_CU_UP_CounterCheckRequest, choice.e_UTRAN_GNB_CU_UP_CounterCheckRequest),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_Container_99P45,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"e-UTRAN-GNB-CU-UP-CounterCheckRequest"
		},
	{ ATF_POINTER, 0, offsetof(struct System_GNB_CU_UP_CounterCheckRequest, choice.nG_RAN_GNB_CU_UP_CounterCheckRequest),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_Container_99P46,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"nG-RAN-GNB-CU-UP-CounterCheckRequest"
		},
	{ ATF_POINTER, 0, offsetof(struct System_GNB_CU_UP_CounterCheckRequest, choice.choice_extension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_SingleContainer_102P15,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"choice-extension"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_System_GNB_CU_UP_CounterCheckRequest_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* e-UTRAN-GNB-CU-UP-CounterCheckRequest */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* nG-RAN-GNB-CU-UP-CounterCheckRequest */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* choice-extension */
};
asn_CHOICE_specifics_t asn_SPC_System_GNB_CU_UP_CounterCheckRequest_specs_1 = {
	sizeof(struct System_GNB_CU_UP_CounterCheckRequest),
	offsetof(struct System_GNB_CU_UP_CounterCheckRequest, _asn_ctx),
	offsetof(struct System_GNB_CU_UP_CounterCheckRequest, present),
	sizeof(((struct System_GNB_CU_UP_CounterCheckRequest *)0)->present),
	asn_MAP_System_GNB_CU_UP_CounterCheckRequest_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_System_GNB_CU_UP_CounterCheckRequest = {
	"System-GNB-CU-UP-CounterCheckRequest",
	"System-GNB-CU-UP-CounterCheckRequest",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{ &asn_OER_type_System_GNB_CU_UP_CounterCheckRequest_constr_1, &asn_PER_type_System_GNB_CU_UP_CounterCheckRequest_constr_1, CHOICE_constraint },
	asn_MBR_System_GNB_CU_UP_CounterCheckRequest_1,
	3,	/* Elements count */
	&asn_SPC_System_GNB_CU_UP_CounterCheckRequest_specs_1	/* Additional specs */
};

