#include "E1AP_message.h"

#if 0
typedef struct UpE1SetupReq
{
    TransactionID_t transactionID;
    GNB_CU_UP_ID_t gnb_cu_up_id;
    GNB_CU_UP_Name_t gnb_cu_up_name;
    CNSupport_t   cn_support;
    SupportedPLMNs_List_t supported_plmns;
    GNB_CU_UP_Capacity_t gnb_cu_up_capacity;
    TransportLayerAddress_t transport_layer_address;
}UpE1SetupReq_t;
#endif

#define MAX_CU_UP_NAME_LEN 12

typedef struct {
	uint32_t  transID;
	GNB_CU_UP_ID_t  gnbCuUpID;
	uint8_t  namePre;
	char  cuUpName[MAX_CU_UP_NAME_LEN];
	uint8_t numSupportedPLMNs; 
    SupportedPLMN_t plmn;   
}UPE1SetupReq_st;

typedef struct {
    char tnla[8];
}TNLAI_st;

typedef struct {
	uint32_t  transID;
	char  cuCpName[MAX_CU_UP_NAME_LEN];
	TNLAI_st tnla;
}UPE1SetupRsp_st;


//decode functions
void decode_UPE1SetupReq(void *decodeBuf, int msg_len, UPE1SetupReq_st *req);
void  decode_UPE1Msg(void *buf, int msg_len, UPE1SetupReq_st *out_buf, int *out_len, char **msgType, int *type_len);

//encode functions
int encode_UPE1SetupRsp(void *encodeBuf, UpE1SetupRsp_t *rsp);
int encode_UPE1SetupFailure(void *encodeBuf, UpE1SetupFailure_t *fail);