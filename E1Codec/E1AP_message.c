#define ASN_EMIT_DEBUG 1
#include "E1AP_message.h"
#include <stdio.h>


int encode_UPE1SetupReq(void *encodeBuf, CuUpConfig_t *CuUpConfig)
{
    GNB_CU_UP_E1SetupRequest_t    *container;
    GNB_CU_UP_E1SetupRequestIEs_t *ie;

    E1AP_PDU_t *pdu = (E1AP_PDU_t *)calloc(1, sizeof(E1AP_PDU_t));

    pdu->present = E1AP_PDU_PR_initiatingMessage;
    pdu->choice.initiatingMessage = (InitiatingMessage_t *)calloc(1, sizeof(InitiatingMessage_t));
    pdu->choice.initiatingMessage->procedureCode = ProcedureCode_id_gNB_CU_UP_E1Setup;
    pdu->choice.initiatingMessage->criticality   = Criticality_reject;
    pdu->choice.initiatingMessage->value.present = InitiatingMessage__value_PR_GNB_CU_UP_E1SetupRequest;
    container = &pdu->choice.initiatingMessage->value.choice.GNB_CU_UP_E1SetupRequest;
  

    /* c1. Transaction ID (integer value) */
    ie = (GNB_CU_UP_E1SetupRequestIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupRequestIEs_t));
    ie->id                        = ProtocolIE_ID_id_TransactionID;
    ie->criticality               = Criticality_reject;
    ie->value.present             = GNB_CU_UP_E1SetupRequestIEs__value_PR_TransactionID;
    ie->value.choice.TransactionID = CuUpConfig->transactionID;
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);

    ie = (GNB_CU_UP_E1SetupRequestIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupRequestIEs_t));
    ie->id                        = ProtocolIE_ID_id_gNB_CU_UP_ID;
    ie->criticality               = Criticality_reject;
    ie->value.present             = GNB_CU_UP_E1SetupRequestIEs__value_PR_GNB_CU_UP_ID;
    asn_uint642INTEGER(&ie->value.choice.GNB_CU_UP_ID, CuUpConfig->GNB_CU_UP_ID);
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);
    
    if (CuUpConfig->GNB_CU_UP_Name_Pre)
    {
        ie = (GNB_CU_UP_E1SetupRequestIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupRequestIEs_t));
        ie->id                        = ProtocolIE_ID_id_gNB_CU_UP_Name;
        ie->criticality               = Criticality_ignore;
        ie->value.present             = GNB_CU_UP_E1SetupRequestIEs__value_PR_GNB_CU_UP_Name;
        OCTET_STRING_fromBuf(&ie->value.choice.GNB_CU_UP_Name, (const char*)CuUpConfig->GNB_CU_UP_Name, strlen(CuUpConfig->GNB_CU_UP_Name));
        ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);
    }

    ie = (GNB_CU_UP_E1SetupRequestIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupRequestIEs_t));
    ie->id                        = ProtocolIE_ID_id_CNSupport;
    ie->criticality               = Criticality_reject;
    ie->value.present             = GNB_CU_UP_E1SetupRequestIEs__value_PR_CNSupport;
    ie->value.choice.CNSupport    = CuUpConfig->CNSupport;
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);


    ie = (GNB_CU_UP_E1SetupRequestIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupRequestIEs_t));
    ie->id                        = ProtocolIE_ID_id_SupportedPLMNs;
    ie->criticality               = Criticality_reject;
    ie->value.present             = GNB_CU_UP_E1SetupRequestIEs__value_PR_SupportedPLMNs_List;
    SupportedPLMNs_Item_t *ieSupportedPLMNs_Item;
    ieSupportedPLMNs_Item = (SupportedPLMNs_Item_t *)calloc(1, sizeof(SupportedPLMNs_Item_t));  
    int plmnCount = CuUpConfig->Num_SupportedPLMNs;
    for (int i = 0;  i < plmnCount; i++)
    {
        ieSupportedPLMNs_Item->pLMN_Identity.size = 3;
        ieSupportedPLMNs_Item->pLMN_Identity.buf = calloc(3, sizeof(uint8_t));  
        ieSupportedPLMNs_Item->pLMN_Identity.buf[0] = (CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mcc[1] << 4) | CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mcc[0];
        if (CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.len_Mnc == 2)
        {
            ieSupportedPLMNs_Item->pLMN_Identity.buf[1] = (0xF << 4) | CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mcc[2];     
            ieSupportedPLMNs_Item->pLMN_Identity.buf[2] = (CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mnc[1] << 4) | CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mnc[0];
        }

        if (CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.len_Mnc == 3)
        {
            ieSupportedPLMNs_Item->pLMN_Identity.buf[1] = (CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mnc[0] << 4) | CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mcc[2];     
            ieSupportedPLMNs_Item->pLMN_Identity.buf[2] = (CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mnc[2] << 4) | CuUpConfig->SupportedPLMNs_List[i].plmn_Identity.mnc[1];
        }
        
        ASN_SEQUENCE_ADD(&ie->value.choice.SupportedPLMNs_List.list, ieSupportedPLMNs_Item);
    }
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);

    if (CuUpConfig->GNB_CU_UP_Capacity_Pre)
    {
        ie = (GNB_CU_UP_E1SetupRequestIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupRequestIEs_t));
        ie->id                        = ProtocolIE_ID_id_gNB_CU_UP_Capacity;
        ie->criticality               = Criticality_ignore;
        ie->value.present             = GNB_CU_UP_E1SetupRequestIEs__value_PR_GNB_CU_UP_Capacity;
        ie->value.choice.GNB_CU_UP_Capacity    = CuUpConfig->GNB_CU_UP_Capacity;
        ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);
    }


    uint8_t *buf;
    int encoded_len = aper_encode_to_new_buffer(&asn_DEF_E1AP_PDU, NULL, pdu, (void **)&buf);
    if(encoded_len != -1)
    {
        memcpy(encodeBuf, buf, encoded_len);
    }

    for (int i = 0; i < encoded_len; i++)
    {
        if ((0 != i) && (0 == i%8))
        {
                printf("\n");
        }
        if (i == encoded_len-1)
        {
                printf("0x%x ", buf[i]);
        }
        else
        {
            printf("0x%x, ", buf[i]);
        }
                                        
    }
        printf("\n\n");

    return encoded_len;
}

void decode_UpE1SetupRsp(void *buf, int msg_len, UpE1SetupRsp_t *rsp)
{
    int IEcount;
    asn_dec_rval_t dec_ret;
    E1AP_PDU_t *pdu = NULL;
    int i,j;
    GNB_CU_UP_E1SetupResponseIEs_t  **ptr;
    GNB_CU_UP_E1SetupResponse_t *setupRsp;
    int CU_CP_Name_Len;

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
    }
    else
    {
        switch(pdu->present)
        {
            case E1AP_PDU_PR_successfulOutcome:
                switch(pdu->choice.successfulOutcome->procedureCode)
                {
                    case ProcedureCode_id_gNB_CU_UP_E1Setup:
                        setupRsp = &pdu->choice.successfulOutcome->value.choice.GNB_CU_UP_E1SetupResponse;
                        IEcount = setupRsp->protocolIEs.list.count;     
                        ptr = setupRsp->protocolIEs.list.array;
                        for(i = 0; i < IEcount; i++)
                        {
                            switch((*ptr)->id)
                            {
                                case ProtocolIE_ID_id_TransactionID:
                                    rsp->transactionID = (*ptr)->value.choice.TransactionID; 
                                    break;
                                case ProtocolIE_ID_id_gNB_CU_CP_Name:
                                    if ((*ptr)->value.present == GNB_CU_CP_E1SetupRequestIEs__value_PR_GNB_CU_CP_Name)
                                    {
                                        rsp->GNB_CU_CP_Name_Pre = 1;
                                        CU_CP_Name_Len = (*ptr)->value.choice.GNB_CU_CP_Name.size;
                                        for (j = 0; j < CU_CP_Name_Len; j++)
                                        {
                                            rsp->GNB_CU_CP_Name[j] = (*ptr)->value.choice.GNB_CU_CP_Name.buf[j];
                                        }
                                    }
                                    else
                                    {
                                        rsp->GNB_CU_CP_Name_Pre = 0;
                                    }
                                    break;
                                default:
                                        printf("Invalid IE\n");
                            }
                            ptr++;
                        }
                        break;

                    default:
                        printf("The ProcedureCode is not gNB_CU_UP_E1Setup\n");
                }
                break;

            default:
                printf("The message is not successfulOutcome\n");
        }
    }
}

void decode_UpE1SetupFailure(void *buf, int msg_len, UpE1SetupFailure_t *setupFailure)
{
    int IEcount;
    asn_dec_rval_t dec_ret;
    E1AP_PDU_t *pdu = NULL;
    int i;
    GNB_CU_UP_E1SetupFailureIEs_t  **ptr;
    GNB_CU_UP_E1SetupFailure_t *failure;

    dec_ret = aper_decode(NULL,
                        &asn_DEF_E1AP_PDU,
                        (void **)&pdu,
                        buf,
                        msg_len,
                        0,
                        0);

    if (dec_ret.code != RC_OK)
    {
        printf("decode UpE1SetupFailure failed\n");
    }
    else
    {
        switch(pdu->present)
        {
            case E1AP_PDU_PR_unsuccessfulOutcome:
                switch(pdu->choice.unsuccessfulOutcome->procedureCode)
                {
                    case ProcedureCode_id_gNB_CU_UP_E1Setup:
                        failure = &pdu->choice.unsuccessfulOutcome->value.choice.GNB_CU_UP_E1SetupFailure;
                        IEcount = failure->protocolIEs.list.count;     
                        ptr = failure->protocolIEs.list.array;
                        for(i = 0; i < IEcount; i++)
                        {
                            switch((*ptr)->id)
                            {
                                case ProtocolIE_ID_id_TransactionID:
                                    setupFailure->transactionID = (*ptr)->value.choice.TransactionID; 
                                    break;
                                case ProtocolIE_ID_id_Cause:
                                    setupFailure->cause = (*ptr)->value.choice.Cause;
                                    break;
                                case ProtocolIE_ID_id_TimeToWait:
                                    if ((*ptr)->value.present == GNB_CU_UP_E1SetupFailureIEs__value_PR_TimeToWait)
                                    {
                                        setupFailure->timeToWaitPre = 1;
                                        setupFailure->timeToWait = (*ptr)->value.choice.TimeToWait;
                                    }
                                    else
                                    {
                                        setupFailure->timeToWaitPre = 0;
                                    }
                                    break;
                                case ProtocolIE_ID_id_CriticalityDiagnostics:
                                    if ((*ptr)->value.present == GNB_CU_UP_E1SetupFailureIEs__value_PR_CriticalityDiagnostics)
                                    {
                                        setupFailure->criticalityDiagnosticsPre = 1;
                                        setupFailure->criticalityDiagnostics = (*ptr)->value.choice.CriticalityDiagnostics;
                                    }
                                    else
                                    {
                                        setupFailure->criticalityDiagnosticsPre = 0;
                                    }
                                    break;
                                default:
                                    printf("Invalid IE\n");
                            }
                            ptr++;
                        }
                        break;

                    default:
                        printf("The ProcedureCode is not gNB_CU_UP_E1Setup\n");
                }
                break;

            default:
                printf("The message is not unsuccessfulOutcome\n");
        }
    }
}

int encode_UP_E1APSetupFailure(void *encodeBuf, uint8_t timeToWaitPre)
{
    GNB_CU_UP_E1SetupFailure_t    *container;
    GNB_CU_UP_E1SetupFailureIEs_t *ie;

    E1AP_PDU_t *pdu = (E1AP_PDU_t *)calloc(1, sizeof(E1AP_PDU_t));

    pdu->present = E1AP_PDU_PR_unsuccessfulOutcome;
    pdu->choice.unsuccessfulOutcome = (UnsuccessfulOutcome_t *)calloc(1, sizeof(UnsuccessfulOutcome_t));
    pdu->choice.unsuccessfulOutcome->procedureCode = ProcedureCode_id_gNB_CU_UP_E1Setup;
    pdu->choice.unsuccessfulOutcome->criticality   = Criticality_reject;
    pdu->choice.unsuccessfulOutcome->value.present = UnsuccessfulOutcome__value_PR_GNB_CU_UP_E1SetupFailure;
    container = &pdu->choice.unsuccessfulOutcome->value.choice.GNB_CU_UP_E1SetupFailure;
   

    /* c1. Transaction ID (integer value) */
    ie = (GNB_CU_UP_E1SetupFailureIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupFailureIEs_t));
    ie->id                        = ProtocolIE_ID_id_TransactionID;
    ie->criticality               = Criticality_reject;
    ie->value.present             = GNB_CU_UP_E1SetupFailureIEs__value_PR_TransactionID;
    ie->value.choice.TransactionID = 0;
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);

    ie = (GNB_CU_UP_E1SetupFailureIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupFailureIEs_t));
    ie->id                        = ProtocolIE_ID_id_Cause;
    ie->criticality               = Criticality_ignore;
    ie->value.present             = GNB_CU_UP_E1SetupFailureIEs__value_PR_Cause;
    ie->value.choice.Cause.present = Cause_PR_protocol;
    ie->value.choice.Cause.choice.protocol = CauseProtocol_abstract_syntax_error_reject;
    ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);

    
    if (timeToWaitPre)
    {
        ie = (GNB_CU_UP_E1SetupFailureIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupFailureIEs_t));
        ie->id                        = ProtocolIE_ID_id_TimeToWait;
        ie->criticality               = Criticality_ignore;
        ie->value.present             = GNB_CU_UP_E1SetupFailureIEs__value_PR_TimeToWait;
        ie->value.choice.TimeToWait   = TimeToWait_v20s;
        ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);
    }

    uint8_t CriticalityDiagnosticsPre = 0;
    if (CriticalityDiagnosticsPre)
    {
        ie = (GNB_CU_UP_E1SetupFailureIEs_t *)calloc(1, sizeof(GNB_CU_UP_E1SetupFailureIEs_t));
        ie->id                        = ProtocolIE_ID_id_CriticalityDiagnostics;
        ie->criticality               = Criticality_ignore;
        ie->value.present             = GNB_CU_UP_E1SetupFailureIEs__value_PR_CriticalityDiagnostics;
        //ie->value.choice.CriticalityDiagnostics    = NULL;
        ASN_SEQUENCE_ADD(&container->protocolIEs.list, ie);
    }


    uint8_t *buf;
    int encoded_len = aper_encode_to_new_buffer(&asn_DEF_E1AP_PDU, NULL, pdu, (void **)&buf);
    if(encoded_len != -1)
    {
        memcpy(encodeBuf, buf, encoded_len);
    }

    printf("%s,%d, encoded_len = %d\n", __FILE__, __LINE__, encoded_len);
    return encoded_len;
}
