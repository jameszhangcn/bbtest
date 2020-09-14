#ifndef _E1AP_MESSAGE_H_
#define _E1AP_MESSAGE_H

#include "E1AP-PDU.h"
#include "InitiatingMessage.h"
#include "SuccessfulOutcome.h"
#include "UnsuccessfulOutcome.h"
#include "ProtocolIE-Field.h"
#include "PDU-Session-Resource-To-Setup-Item.h"
#include "PDU-Session-Resource-To-Modify-Item.h"
#include "DRB-To-Setup-Item-NG-RAN.h"
#include "DRB-To-Modify-Item-NG-RAN.h"
#include "DRB-To-Modify-List-NG-RAN.h"
#include "Cell-Group-Information-Item.h"
#include "QoS-Flow-QoS-Parameter-Item.h"
#include "Non-Dynamic5QIDescriptor.h"
#include "GTPTunnel.h"
#include "UP-Parameters.h"
#include "UP-Parameters-Item.h"
#include "SupportedPLMNs-Item.h"



#ifdef __cplusplus
extern "C" {
#endif

typedef struct Plmn_Identity
{
   uint8_t mcc[3];
   uint8_t len_Mnc;
   uint8_t mnc[3]; 
}Plmn_Identity_t;

typedef struct SupportedPLMN
{
    Plmn_Identity_t	 plmn_Identity;
	//struct Slice_Support_List	*slice_Support_List;	/* OPTIONAL */
	//struct NR_CGI_Support_List	*nR_CGI_Support_List;	/* OPTIONAL */
	//struct QoS_Parameters_Support_List	*qoS_Parameters_Support_List;	/* OPTIONAL */
}SupportedPLMN_t;

typedef struct CuUpConfig
{
    TransactionID_t    transactionID;  
    uint64_t     GNB_CU_UP_ID;  
    uint8_t   GNB_CU_UP_Name_Pre;
    char*  GNB_CU_UP_Name;
    CNSupport_t  CNSupport;
    uint8_t Num_SupportedPLMNs;
    SupportedPLMN_t SupportedPLMNs_List[12];
    uint8_t   GNB_CU_UP_Capacity_Pre;
    GNB_CU_UP_Capacity_t	 GNB_CU_UP_Capacity;       
}CuUpConfig_t;

typedef struct UpE1SetupRsp
{
    TransactionID_t transactionID;
    uint8_t   GNB_CU_CP_Name_Pre;
    char*  GNB_CU_CP_Name;
}UpE1SetupRsp_t;


typedef struct UpE1SetupFailure
{
    TransactionID_t transactionID;
    Cause_t  cause;
    uint8_t  timeToWaitPre;
    long  timeToWait;
    uint8_t  criticalityDiagnosticsPre;
    CriticalityDiagnostics_t criticalityDiagnostics;
}UpE1SetupFailure_t;


int encode_UPE1SetupReq(void *encodeBuf, CuUpConfig_t *CuUpConfig);
void decode_UpE1SetupRsp(void *buf, int msg_len, UpE1SetupRsp_t *rsp);
void decode_UpE1SetupFailure(void *buf, int msg_len, UpE1SetupFailure_t *rsp);

int encode_UP_E1APSetupFailure(void *encodeBuf, uint8_t timeToWaitPre);

#ifdef __cplusplus
}
#endif

#endif
