/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#include "CauseRadioNetwork.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
static asn_oer_constraints_t asn_OER_type_CauseRadioNetwork_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
asn_per_constraints_t asn_PER_type_CauseRadioNetwork_constr_1 CC_NOTUSED = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  5,  5,  0,  24 }	/* (0..24,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static const asn_INTEGER_enum_map_t asn_MAP_CauseRadioNetwork_value2enum_1[] = {
	{ 0,	11,	"unspecified" },
	{ 1,	49,	"unknown-or-already-allocated-gnb-cu-cp-ue-e1ap-id" },
	{ 2,	49,	"unknown-or-already-allocated-gnb-cu-up-ue-e1ap-id" },
	{ 3,	42,	"unknown-or-inconsistent-pair-of-ue-e1ap-id" },
	{ 4,	32,	"interaction-with-other-procedure" },
	{ 5,	23,	"pPDCP-Count-wrap-around" },
	{ 6,	23,	"not-supported-QCI-value" },
	{ 7,	23,	"not-supported-5QI-value" },
	{ 8,	35,	"encryption-algorithms-not-supported" },
	{ 9,	45,	"integrity-protection-algorithms-not-supported" },
	{ 10,	36,	"uP-integrity-protection-not-possible" },
	{ 11,	42,	"uP-confidentiality-protection-not-possible" },
	{ 12,	33,	"multiple-PDU-Session-ID-Instances" },
	{ 13,	22,	"unknown-PDU-Session-ID" },
	{ 14,	30,	"multiple-QoS-Flow-ID-Instances" },
	{ 15,	19,	"unknown-QoS-Flow-ID" },
	{ 16,	25,	"multiple-DRB-ID-Instances" },
	{ 17,	14,	"unknown-DRB-ID" },
	{ 18,	23,	"invalid-QoS-combination" },
	{ 19,	19,	"procedure-cancelled" },
	{ 20,	14,	"normal-release" },
	{ 21,	28,	"no-radio-resources-available" },
	{ 22,	34,	"action-desirable-for-radio-reasons" },
	{ 23,	37,	"resources-not-available-for-the-slice" },
	{ 24,	32,	"pDCP-configuration-not-supported" },
	{ 25,	29,	"ue-dl-max-IP-data-rate-reason" },
	{ 26,	31,	"uP-integrity-protection-failure" },
	{ 27,	26,	"release-due-to-pre-emption" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_CauseRadioNetwork_enum2value_1[] = {
	22,	/* action-desirable-for-radio-reasons(22) */
	8,	/* encryption-algorithms-not-supported(8) */
	9,	/* integrity-protection-algorithms-not-supported(9) */
	4,	/* interaction-with-other-procedure(4) */
	18,	/* invalid-QoS-combination(18) */
	16,	/* multiple-DRB-ID-Instances(16) */
	12,	/* multiple-PDU-Session-ID-Instances(12) */
	14,	/* multiple-QoS-Flow-ID-Instances(14) */
	21,	/* no-radio-resources-available(21) */
	20,	/* normal-release(20) */
	7,	/* not-supported-5QI-value(7) */
	6,	/* not-supported-QCI-value(6) */
	24,	/* pDCP-configuration-not-supported(24) */
	5,	/* pPDCP-Count-wrap-around(5) */
	19,	/* procedure-cancelled(19) */
	27,	/* release-due-to-pre-emption(27) */
	23,	/* resources-not-available-for-the-slice(23) */
	11,	/* uP-confidentiality-protection-not-possible(11) */
	26,	/* uP-integrity-protection-failure(26) */
	10,	/* uP-integrity-protection-not-possible(10) */
	25,	/* ue-dl-max-IP-data-rate-reason(25) */
	17,	/* unknown-DRB-ID(17) */
	13,	/* unknown-PDU-Session-ID(13) */
	15,	/* unknown-QoS-Flow-ID(15) */
	1,	/* unknown-or-already-allocated-gnb-cu-cp-ue-e1ap-id(1) */
	2,	/* unknown-or-already-allocated-gnb-cu-up-ue-e1ap-id(2) */
	3,	/* unknown-or-inconsistent-pair-of-ue-e1ap-id(3) */
	0	/* unspecified(0) */
	/* This list is extensible */
};
const asn_INTEGER_specifics_t asn_SPC_CauseRadioNetwork_specs_1 = {
	asn_MAP_CauseRadioNetwork_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_CauseRadioNetwork_enum2value_1,	/* N => "tag"; sorted by N */
	28,	/* Number of elements in the maps */
	26,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_CauseRadioNetwork_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_CauseRadioNetwork = {
	"CauseRadioNetwork",
	"CauseRadioNetwork",
	&asn_OP_NativeEnumerated,
	asn_DEF_CauseRadioNetwork_tags_1,
	sizeof(asn_DEF_CauseRadioNetwork_tags_1)
		/sizeof(asn_DEF_CauseRadioNetwork_tags_1[0]), /* 1 */
	asn_DEF_CauseRadioNetwork_tags_1,	/* Same as above */
	sizeof(asn_DEF_CauseRadioNetwork_tags_1)
		/sizeof(asn_DEF_CauseRadioNetwork_tags_1[0]), /* 1 */
	{ &asn_OER_type_CauseRadioNetwork_constr_1, &asn_PER_type_CauseRadioNetwork_constr_1, NativeEnumerated_constraint },
	0, 0,	/* Defined elsewhere */
	&asn_SPC_CauseRadioNetwork_specs_1	/* Additional specs */
};

