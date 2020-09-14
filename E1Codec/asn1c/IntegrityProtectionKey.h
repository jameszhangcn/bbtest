/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "E1AP-IEs"
 * 	found in "E1AP-IEs.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps`
 */

#ifndef	_IntegrityProtectionKey_H_
#define	_IntegrityProtectionKey_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IntegrityProtectionKey */
typedef OCTET_STRING_t	 IntegrityProtectionKey_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IntegrityProtectionKey;
asn_struct_free_f IntegrityProtectionKey_free;
asn_struct_print_f IntegrityProtectionKey_print;
asn_constr_check_f IntegrityProtectionKey_constraint;
ber_type_decoder_f IntegrityProtectionKey_decode_ber;
der_type_encoder_f IntegrityProtectionKey_encode_der;
xer_type_decoder_f IntegrityProtectionKey_decode_xer;
xer_type_encoder_f IntegrityProtectionKey_encode_xer;
oer_type_decoder_f IntegrityProtectionKey_decode_oer;
oer_type_encoder_f IntegrityProtectionKey_encode_oer;
per_type_decoder_f IntegrityProtectionKey_decode_uper;
per_type_encoder_f IntegrityProtectionKey_encode_uper;
per_type_decoder_f IntegrityProtectionKey_decode_aper;
per_type_encoder_f IntegrityProtectionKey_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _IntegrityProtectionKey_H_ */
#include <asn_internal.h>
