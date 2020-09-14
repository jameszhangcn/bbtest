package cucpsvc
//#cgo CFLAGS: -I../../../E1Codec/asn1c
//#cgo LDFLAGS: -L../../../E1Codec -lasn1
/*
#include "../../../E1Codec/simucucp_message.h"

uint32_t size = 12;

*/
import "C"
import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"reflect"
	"bbtest/impl/simucucp/srvc/common"
	"bbtest/impl/simucucp/srvc/sctp"
	"unsafe"
)

func Call(m map[string]interface{}, name string, params ...interface{}) (result []reflect.Value, err error) {
	f := reflect.ValueOf(m[name])
	if len(params) != f.Type().NumIn() {
		err = errors.New("The number of params is not adapted.")
		return
	}

	in := make([]reflect.Value, len(params))
	for k, param := range params {
		in[k] = reflect.ValueOf(param)
	}
	result = f.Call(in)
	return
}

//E1MsgHandler : handle E1 Msg from CP
func E1MsgHandler(buf []byte, len int) {
	fmt.Println("CUCP receive message ", buf)

	funcs := map[string]interface{}{
		"GNB-CU-UP-E1-SETUP-REQUEST":                 handleUpE1SetupReq,
		"GNB-CU-CP-E1-SETUP-RESPONSE":                handleCpE1SetupRsp,
		"GNB-CU-UP-CONFIGURATION-UPDATE":             handleUpConfigUpdate,
		"GNB-CU-CP-CONFIGURATION-UPDATE-ACKNOWLEDGE": handleCpConfigUptAck,
		"E1-RELEASE-REQUEST":                         handleUpE1RelReq,
		"E1-RELEASE-RESPONSE":                        handleCpE1RelRsp,
		"GNB-CU-UP-STATUS-INDICATION":                handleUpStatusInd,
	}
	var req C.UPE1SetupReq_st
	var out_len int
	msgbuf := unsafe.Pointer(&buf[0])
	c_len := C.int(len)
	var type_len int
	c_type_len := C.int(type_len)
	c_out_len := C.int(out_len)
	var msgType string
	msgType = "TheMaxLenForThemessageDoubleTesing"
	c_msgType := C.CString(msgType)
	//ms := (*MyString)(unsafe.Pointer(&s))
	//may be need C.free
	C.decode_UPE1Msg(msgbuf, c_len, &req, &c_out_len, &c_msgType, &c_type_len)

	fmt.Println("decoded msgType", msgType, C.GoStringN(c_msgType, C.int(c_type_len)), C.int(c_type_len))

	if C.int(c_type_len) == 0 {
		fmt.Println("unknown received msg")
	} else {
		goMsgType := C.GoStringN(c_msgType, C.int(c_type_len))
		if result, err := Call(funcs, goMsgType, goMsgType, &req); err == nil {
			fmt.Println("result", result, "err", err)
		}
	}
}

type E1SetupReq struct {
	transID           uint32
	gnbCuUpID         uint64
	namePre           uint8
	cuUpName          string
	numSupportedPLMNs uint8
}

type E1SetupRsp struct {
	TransID  uint32
	CuUpName string
}

type E1SetupREP struct {
	CuUpName string `json:"cuupname"`
}

// UPE1SetupReqCtoGo convert the C struct to GO struct
func UPE1SetupReqCtoGo(msg *C.UPE1SetupReq_st) (ret *E1SetupReq) {
	var E1SetupMsg = &E1SetupReq{}

	E1SetupMsg.transID = uint32(msg.transID)
	E1SetupMsg.gnbCuUpID = uint64(msg.gnbCuUpID.size)
	E1SetupMsg.namePre = uint8(msg.namePre)
	E1SetupMsg.cuUpName = C.GoStringN(&msg.cuUpName[0], 12)
	E1SetupMsg.numSupportedPLMNs = uint8(msg.numSupportedPLMNs)
	fmt.Println(E1SetupMsg.gnbCuUpID)
	fmt.Println(E1SetupMsg.transID)
	fmt.Println(E1SetupMsg.namePre)
	fmt.Println(E1SetupMsg.cuUpName)
	fmt.Println(E1SetupMsg.numSupportedPLMNs)

	return E1SetupMsg
}

func SendE1SetupRsp(rsp E1SetupRsp) {
	//get Rsp config, may be some error value setted
	var e1setuprsp C.UpE1SetupRsp_t

	char := C.CString(rsp.CuUpName)
	defer C.free(unsafe.Pointer(char))

	e1setuprsp.transactionID = C.long(rsp.TransID)
	e1setuprsp.GNB_CU_CP_Name = (*C.char)(unsafe.Pointer(char))
	buf := make([]uint8, 1024)
	encodeBuf := unsafe.Pointer(&buf[0])
	msgLen := C.encode_UPE1SetupRsp(encodeBuf, &e1setuprsp)
	fmt.Println("msgLen:  ", msgLen)

	cuUpE1SetupRsp := make([]uint8, msgLen)
	var asnMsg []string
	for i := 0; i < int(msgLen); i++ {
		tmp := unsafe.Pointer(uintptr(encodeBuf) + uintptr(i)*unsafe.Sizeof(cuUpE1SetupRsp[0]))

		cuUpE1SetupRsp[i] = *(*uint8)(tmp)
		fmt.Printf("0x%x ", cuUpE1SetupRsp[i])
		asnMsg = append(asnMsg, fmt.Sprintf("0x%x ", cuUpE1SetupRsp[i]))
	}
	sctp.SendSctp(cuUpE1SetupRsp, int(msgLen))

}

func handleUpE1SetupReq(msgType string, data *C.UPE1SetupReq_st) {
	fmt.Println("simu cp respToUpE1SetupReq")

	//get the proc profile
	proc := common.GetProcCfg(msgType)
	var rspCfg E1SetupRsp
	if proc == nil {
		fmt.Println("Proc is not ready")
		return
	}
	if proc.RespPara != "" {
		//read the json file from DB
		key := common.GetProcDBKey(proc.RespPara)
		cfgData := common.GetFromDB(context.Background(), key)
		err := json.Unmarshal(cfgData, &rspCfg)
		if err != nil {
			fmt.Println(err)
		}
	}
	fmt.Println("resp to up E1 setup req procedure", *proc)
	switch proc.Behaviour {

	case "RESPONSE":
		{
			fmt.Println("RESPONSE behaviour")
			//send the resp
			SendE1SetupRsp(rspCfg)
		}
	case "FAILURE":
		{
			fmt.Println("FAILURE behaviour")
			//send the failure resp
		}
	case "MUTE":
		{
			fmt.Println("MUTE behaviour")
			//don't resp
		}
	default:
		{
			fmt.Println("Unknown behaviour")
		}

	}
	//send NATS event to notify simuctl
	//at the save time, save the result and resp content to DB
	if proc.NeedCheck == "TRUE" {

		cuUpName := "TEST-CUUP" //C.GoString((*_Ctype_char)(data.cuUpName))
		rep := &E1SetupREP{
			CuUpName: cuUpName,
		}
		repJson, err := json.Marshal(rep)
		if err != nil {
			fmt.Println(err)
		} else {

			natReport := common.NatsReport{
				MsgName: "GNB-CU-UP-E1-SETUP-REQUEST",
				Data:    repJson,
			}
			fmt.Println("Before marshal ", natReport)
			value, err := json.Marshal(natReport)
			if err != nil {
				fmt.Println(err)
			} else {
				fmt.Println("Nats report: ", value)
				common.PubEvent(subjectPub, value)
			}
			var simunatReport common.NatsReport
			err = json.Unmarshal(value, &simunatReport)
			if err != nil {
				fmt.Println(err)
			}
			fmt.Println("The simu reported: ", simunatReport)
		}
	}
}

func handleCpE1SetupRsp() {

}
func handleUpConfigUpdate() {

}
func handleCpConfigUptAck() {

}
func handleUpE1RelReq() {

}
func handleCpE1RelRsp() {

}
func handleUpStatusInd() {

}
