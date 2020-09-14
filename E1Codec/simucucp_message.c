#include "simucucp_message.h"
#include "string.h"


void UpE1SetupReqHandler(void *buf, UPE1SetupReq_st *req, int *out_len){
    GNB_CU_UP_E1SetupRequest_t *container;
    GNB_CU_UP_E1SetupRequestIEs_t **ie;
    int IEcount;
    asn_dec_rval_t dec_ret;
    int CU_CP_Name_Len;
    int i,j;

    container = (GNB_CU_UP_E1SetupRequest_t *)buf;
    //may send response or failure
    printf("decoding UpE1SetupReq \r\n");
    IEcount = container->protocolIEs.list.count;
    ie = container->protocolIEs.list.array;
    for (i = 0; i < IEcount; i++)
    {
        switch((*ie)->id)
        {
              case ProtocolIE_ID_id_TransactionID:
                req->transID = (*ie)->value.choice.TransactionID;
                printf("In C req transID %d \r\n", req->transID);
                break;
              case ProtocolIE_ID_id_gNB_CU_UP_ID:
                req->gnbCuUpID = ((*ie)->value.choice.GNB_CU_UP_ID);
                printf("In C req gnbCuUpID size %d \r\n", (int)req->gnbCuUpID.size);
                break;

              case ProtocolIE_ID_id_gNB_CU_UP_Name:
                if((*ie)->value.present == GNB_CU_UP_E1SetupRequestIEs__value_PR_GNB_CU_UP_Name)
                {
                    CU_CP_Name_Len = (*ie)->value.choice.GNB_CU_UP_Name.size;
                    printf("In C req CU_CP_Name_Len %d \r\n", CU_CP_Name_Len);
                    for(j = 0; j < CU_CP_Name_Len; j++)
                    {
                        req->cuUpName[j] = (*ie)->value.choice.GNB_CU_UP_Name.buf[j];
                    }
                }else {
                    req->namePre = 0;
                }
                break;
              case ProtocolIE_ID_id_SupportedPLMNs:
                req->numSupportedPLMNs = (*ie)->value.choice.SupportedPLMNs_List.list.count;
                printf("In C req plmns %d \r\n", req->numSupportedPLMNs);
                for (j = 0; j < req->numSupportedPLMNs; j++) {
                    unsigned char mccmnc[3];
                    SupportedPLMNs_Item_t* item = (*ie)->value.choice.SupportedPLMNs_List.list.array[j];
                    memcpy(&mccmnc[0],&item->pLMN_Identity.buf[0],sizeof(uint8_t));
                    memcpy(&mccmnc[1],&item->pLMN_Identity.buf[1],sizeof(uint8_t));
                    memcpy(&mccmnc[2],&item->pLMN_Identity.buf[2],sizeof(uint8_t));
                    req->plmn.plmn_Identity.mcc[0] = (mccmnc[0]&0xF0)>>4;
                    req->plmn.plmn_Identity.mcc[1] = (mccmnc[0]&0xF);
                    req->plmn.plmn_Identity.mcc[2] = (mccmnc[1]&0xF0)>>4;
                    if(0xF == mccmnc[1]&0xF) {
                        req->plmn.plmn_Identity.len_Mnc = 2;
                        req->plmn.plmn_Identity.mnc[0] = (mccmnc[1]&0xF);
                        req->plmn.plmn_Identity.mnc[1] = (mccmnc[2]&0xF0)>>4;

                    } else {
                        req->plmn.plmn_Identity.mnc[0] = (mccmnc[1]&0xF);
                        req->plmn.plmn_Identity.mnc[1] = (mccmnc[2]&0xF0)>>4;
                        req->plmn.plmn_Identity.mnc[2] = (mccmnc[2]&0xF);
                    }
                }
                break;
              default:
                    printf("Invalid IE\n");
        }
        ie++;
    }

    return;
}


//decode functions
void  decode_UPE1Msg(void *buf, int msg_len, UPE1SetupReq_st *out_buf, int* out_len, char** msgType, int *type_len)
{
    int IEcount;
    asn_dec_rval_t dec_ret;
    E1AP_PDU_t *pdu = NULL;
    int i,j;
    GNB_CU_UP_E1SetupRequestIEs_t  **ptr;
    GNB_CU_UP_E1SetupRequest_t *setupReq;
    int CU_CP_Name_Len;

    char *tmpMsgType = malloc(48*sizeof(char));

    dec_ret = aper_decode(NULL,
                        &asn_DEF_E1AP_PDU,
                        (void **)&pdu,
                        buf,
                        msg_len,
                        0,
                        0);

    if (dec_ret.code != RC_OK)
    {
        printf("decode failed\n");
        *type_len = 0;
    }
    else
    {
        switch(pdu->present)
        {
            case E1AP_PDU_PR_successfulOutcome:
                switch(pdu->choice.successfulOutcome->procedureCode)
                {
                    case ProcedureCode_id_gNB_CU_UP_E1Setup:
                    {
                        //call the function to decode the response message
                        //decodeXXX_Rsp()
                        break;
                    }
                    default:
                        printf("The E1AP_PDU_PR_successfulOutcome unknown procedureCode \n");
                        break;
                }
                break;
            case E1AP_PDU_PR_initiatingMessage:
                switch(pdu->choice.initiatingMessage->procedureCode)
                {
                    case ProcedureCode_id_gNB_CU_UP_E1Setup:
                    switch(pdu->choice.initiatingMessage->value.present)
                    {
                        case InitiatingMessage__value_PR_GNB_CU_UP_E1SetupRequest:
                        {
                            //decodeXXX_Req(
			                char* test = "GNB-CU-UP-E1-SETUP-REQUEST";
                            *type_len = strlen(test);
                            memcpy(tmpMsgType,test,*type_len);
                            printf("decode C: %s",test);
                            printf("decode C: %s", tmpMsgType);
                            *msgType = tmpMsgType;
                            UpE1SetupReqHandler(&pdu->choice.initiatingMessage->value.choice.GNB_CU_CP_E1SetupRequest, out_buf, out_len);
                            break;
                        }
                        default:
                        {
                            printf("Unknown Initiating Message value present \r\n");
                        }
                    }
                }
                break;
            default:
                printf("Unknown pdu present!! \n");
        }
    }

}

//encode functions
int encode_UPE1SetupRsp(void *encodeBuf, UpE1SetupRsp_t *rsp)
{
    //should return the encoded length
    GNB_CU_UP_E1SetupResponse_t *container;
    GNB_CU_UP_E1SetupResponseIEs_t *ie;

    E1AP_PDU_t *pdu = (E1AP_PDU_t *)calloc(1,sizeof(E1AP_PDU_t));
    pdu->present = E1AP_PDU_PR_successfulOutcome;
    pdu->choice.successfulOutcome = (SuccessfulOutcome_t*)calloc(1, sizeof(SuccessfulOutcome_t));
    pdu->choice.successfulOutcome->procedureCode = ProcedureCode_id_gNB_CU_UP_E1Setup;
    pdu->choice.successfulOutcome->criticality = Criticality_reject;
    pdu->choice.successfulOutcome->value.present = SuccessfulOutcome__value_PR_GNB_CU_UP_E1SetupResponse;
    container = &pdu->choice.successfulOutcome->value.choice.GNB_CU_UP_E1SetupResponse;

    ie = (GNB_CU_UP_E1SetupResponseIEs_t*)calloc(1, sizeof(GNB_CU_UP_E1SetupResponseIEs_t));
    ie->id = ProtocolIE_ID_id_TransactionID;
    ie->criticality = Criticality_reject;
    ie->value.present = GNB_CU_UP_E1SetupResponseIEs__value_PR_TransactionID;
    ie->value.choice.TransactionID = rsp->transactionID;
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);

    ie = (GNB_CU_UP_E1SetupResponseIEs_t*)calloc(1, sizeof(GNB_CU_UP_E1SetupResponseIEs_t));
    ie->id = ProtocolIE_ID_id_gNB_CU_CP_Name;
    ie->criticality = Criticality_ignore;
    ie->value.present = GNB_CU_UP_E1SetupResponseIEs__value_PR_GNB_CU_CP_Name;
    OCTET_STRING_fromBuf(&ie->value.choice.GNB_CU_CP_Name, (const char*)rsp->GNB_CU_CP_Name, strlen(rsp->GNB_CU_CP_Name));
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);
    //container->protocolIEs.list.count = 2;

    uint8_t *buf;
    int encoded_len = aper_encode_to_new_buffer(&asn_DEF_E1AP_PDU, NULL, pdu, (void**)&buf);
    printf("aper_encode_to_new_buffer len %d", encoded_len);
    if(encoded_len != -1){
        memcpy(encodeBuf, buf, encoded_len);

    }
    printf("encodeUEE1SetupRsp len %d", encoded_len);
    uint8_t *temp = buf;
    for (int i = 0; i < encoded_len; i++) {
        printf("%X ", *temp);
        temp++;
    }
    return encoded_len;
}
int encode_UPE1SetupFailure(void *encodeBuf, UpE1SetupFailure_t *fail)
{
    //should return the encoded length
    return 0;
}
