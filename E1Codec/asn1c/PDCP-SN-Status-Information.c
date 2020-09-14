/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#include "PDCP-SN-Status-Information.h"

#include "ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_PDCP_SN_Status_Information_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PDCP_SN_Status_Information, pdcpStatusTransfer_UL),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DRBBStatusTransfer,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdcpStatusTransfer-UL"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PDCP_SN_Status_Information, pdcpStatusTransfer_DL),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PDCP_Count,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"pdcpStatusTransfer-DL"
		},
	{ ATF_POINTER, 1, offsetof(struct PDCP_SN_Status_Information, iE_Extension),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_128P66,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extension"
		},
};
static const int asn_MAP_PDCP_SN_Status_Information_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_PDCP_SN_Status_Information_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_PDCP_SN_Status_Information_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pdcpStatusTransfer-UL */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* pdcpStatusTransfer-DL */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extension */
};
asn_SEQUENCE_specifics_t asn_SPC_PDCP_SN_Status_Information_specs_1 = {
	sizeof(struct PDCP_SN_Status_Information),
	offsetof(struct PDCP_SN_Status_Information, _asn_ctx),
	asn_MAP_PDCP_SN_Status_Information_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_PDCP_SN_Status_Information_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_PDCP_SN_Status_Information = {
	"PDCP-SN-Status-Information",
	"PDCP-SN-Status-Information",
	&asn_OP_SEQUENCE,
	asn_DEF_PDCP_SN_Status_Information_tags_1,
	sizeof(asn_DEF_PDCP_SN_Status_Information_tags_1)
		/sizeof(asn_DEF_PDCP_SN_Status_Information_tags_1[0]), /* 1 */
	asn_DEF_PDCP_SN_Status_Information_tags_1,	/* Same as above */
	sizeof(asn_DEF_PDCP_SN_Status_Information_tags_1)
		/sizeof(asn_DEF_PDCP_SN_Status_Information_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_PDCP_SN_Status_Information_1,
	3,	/* Elements count */
	&asn_SPC_PDCP_SN_Status_Information_specs_1	/* Additional specs */
};

