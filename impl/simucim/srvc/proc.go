package cimsvc

import (
	"container/list"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"bbtest/impl/simucim/srvc/common"
	"sync"

	//flatbuffers "github.com/google/flatbuffers/go"
	"github.com/gorilla/mux"
	"github.com/nats-io/nats.go"
	"go.etcd.io/etcd/clientv3"
)

const (
	subjectSub            = "SIMUCIM-S"
	subjectPub            = "SIMUCIM-P"
	EventFMaasNatsSubject = "EVENT"
)

// KeyVal ...
type KeyVal struct {
	Key string
	Val string
}

// Alarm ...
type Alarm struct {
	EventName             string
	EventTime             int64
	ContainerID           string
	SourceID              string
	SourceName            string
	ManagedObject         []*KeyVal
	AdditionalInfo        []*KeyVal
	ThresholdInfo         []*KeyVal
	StateChangeDefinition []*KeyVal
	MonitoredAttributes   []*KeyVal
}

//type Event struct {
//	_tab flatbuffers.Table
//}

// Marshal coding the AlarmReq entity to flatbuffer
//func Marshal(req *Alarm) *[]byte {
//	builder := flatbuffers.NewBuilder(1024)
	//eventName := builder.CreateString(req.EventName)
	//containerID := builder.CreateString(req.ContainerID)
	//eventTime := req.EventTime
	//sourceID := builder.CreateString(req.SourceID)
	//sourceName := builder.CreateString(req.SourceName)

	/*
		managedObjects := flatKeyValueVec(builder, req.ManagedObject, EventInterface.EventStartManagedObjectVector)
		additionalInfos := flatKeyValueVec(builder, req.AdditionalInfo, EventInterface.EventStartAdditionalInfoVector)
		thresholdInfos := flatKeyValueVec(builder, req.ThresholdInfo, EventInterface.EventStartThresholdInfoVector)
		stateChangeDefinitions := flatKeyValueVec(builder, req.StateChangeDefinition, EventInterface.EventStartStateChangeDefinitionVector)
		monitoredAttributes := flatKeyValueVec(builder, req.MonitoredAttributes, EventInterface.EventStartMonitoredAttributesVector)


			EventInterface.EventStart(builder)
			EventInterface.EventAddEventName(builder, eventName)
			EventInterface.EventAddEventTime(builder, eventTime)
			EventInterface.EventAddContainerId(builder, containerID)
			EventInterface.EventAddManagedObject(builder, managedObjects)
			EventInterface.EventAddAdditionalInfo(builder, additionalInfos)
			EventInterface.EventAddThresholdInfo(builder, thresholdInfos)
			EventInterface.EventAddStateChangeDefinition(builder, stateChangeDefinitions)
			EventInterface.EventAddMonitoredAttributes(builder, monitoredAttributes)
			EventInterface.EventAddSourceId(builder, sourceID)
			EventInterface.EventAddSourceName(builder, sourceName)
			event := EventInterface.EventEnd(builder)
			builder.Finish(event)
	*/
//	flatbuf := builder.FinishedBytes()
//	return &flatbuf
//}

//func flatKeyValueVec(builder *flatbuffers.Builder, keyValueVec []*KeyVal,
//	onStartVec func(*flatbuffers.Builder, int) flatbuffers.UOffsetT) flatbuffers.UOffsetT {
//	offsets := make([]flatbuffers.UOffsetT, 0)
	//for _, v := range keyValueVec {
	//key := builder.CreateString(v.Key)
	//val := builder.CreateString(v.Val)
	/*
		EventInterface.KeyValueStart(builder)
		EventInterface.KeyValueAddKey(builder, key)
		EventInterface.KeyValueAddValue(builder, val)
		offset := EventInterface.KeyValueEnd(builder)
		offsets = append(offsets, offset)
	*/
	//}

//	offsetsLen := len(offsets)
//	onStartVec(builder, offsetsLen)
//	for i := offsetsLen; i > 0; i-- {
//		builder.PrependUOffsetT(offsets[i-1])
//	}
//	return builder.EndVector(offsetsLen)
//}

func simuctlHandler(msg *nats.Msg) {
	fmt.Println("nats simuctlHandler received: ", msg)
	err := json.Unmarshal(msg.Data, &common.Scenario)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(&common.Scenario)
}

func eventHandler(msg *nats.Msg) {
	fmt.Println("nats eventHandler received: ", msg)
	fmt.Println(msg)
}

func NatsSub() {
	common.NatsMsgSub(subjectSub, simuctlHandler)
	common.NatsMsgSub(EventFMaasNatsSubject, simuctlHandler)
}

const (
	cimMtcilRegisterHTTPPath = "/api/v1/_operations/mtcil/register"
	cimMetricsPath           = "/metrics"
	cimMtcilRegisterPort     = ":6060"
	cimMtcilAppPort          = ":9999"
)

type Container struct {
	name string
	id   string
}

var (
	r             *mux.Router
	containerList *list.List
	etcdClient    *clientv3.Client
)

var etcdHostAddr string
var etcdWatchKey string

//Register registration struct
type Register struct {
	ContainerName string `json:"container_name,omitempty"`
	ContainerID   string `json:"container_id"`
}

//RegisterResp registration Response struct
type RegisterResp struct {
	Status             string `json:"status"`
	TMaasFqdn          string `json:"tmaas_fqdn"`
	ErrMsg             string `json:"err_msg,omitempty"`
	ApigwRestFqdn      string `json:"apigw_rest_fqdn"`
	ApigwRestURIPrefix string `json:"apigw_rest_uri_prefix"`
	ApigwAuthType      string `json:"apigw_auth_type"`
	ApigwUsername      string `json:"apigw_username"`
	ApigwPassword      string `json:"apigw_password"`
}

func init() {
	containerList = list.New()
}

func httpHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Sim CIM receive http post in mtcil register")
	var registerDetails Register
	var registerRespDetails RegisterResp
	reqBody, err := ioutil.ReadAll(r.Body)
	if err != nil {
		fmt.Println("error while reading the request body", err)
		fmt.Println("MICROSERVICE REGISTRATION FAILURE")
		w.WriteHeader(http.StatusBadRequest)
		registerRespDetails.Status = "failure"
		registerRespDetails.ErrMsg = "Unable to process the request body."
		json.NewEncoder(w).Encode(registerRespDetails)
		return
	}

	err = json.Unmarshal(reqBody, &registerDetails)
	if err != nil {
		fmt.Println("error while unmarshalling the request", err)
		fmt.Println("MICROSERVICE REGISTRATION FAILURE")
		w.WriteHeader(http.StatusBadRequest)
		registerRespDetails.Status = "failure"
		registerRespDetails.ErrMsg = "Invalid request body."
		json.NewEncoder(w).Encode(registerRespDetails)
		return
	}

	container := &Container{
		name: registerDetails.ContainerName,
		id:   registerDetails.ContainerID,
	}
	containerList.PushBack(container)
	//TODO Container id validation need to be implemented.
	//logs.LogConf.Info("Registration request recieved Container Id :", types.ContainerID)

	//start watching container state
	//This watch should be invoked only once as app container can restart anytime.

	//reset the notConfigured flag in case of app container restart
	//agents.InitialNotConfiguredEvent = false

	fmt.Println(container.name, " MICROSERVICE REGISTRATION SUCCCESSFUL")
	w.WriteHeader(http.StatusOK)
	registerRespDetails.Status = "success"
	//if types.TMaasFqdn == "" {
	//types.TMaasFqdn = common_infra.GetTMaaSFqdn()
	//}
	//if types.ApigwRestFqdn == "" {
	//types.ApigwRestFqdn = common_infra.GetApigwRestFqdn()
	//}
	//types.ApigwRestURIPrefix = "/restconf/tailf/query"

	registerRespDetails.TMaasFqdn = "127.0.0.1 gwsvc intfmgrsvc iwfsvc-0.iwfsvc dataplane-0.bccsvc dprmsvc"
	registerRespDetails.ApigwRestFqdn = ""
	registerRespDetails.ApigwRestURIPrefix = "/restconf/tailf/query"
	registerRespDetails.ApigwAuthType = "Basic"
	registerRespDetails.ApigwUsername = "admin"
	registerRespDetails.ApigwPassword = "admin"

	json.NewEncoder(w).Encode(registerRespDetails)

}
func metricsHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Sim CIM receive metrics post in mtcil register")
}
func testHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("Sim CIM receive http post in test")
	fmt.Println(r)
}

//StartCim : start the cim simu
func StartCim(wg *sync.WaitGroup) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Println("goroutine exited reason", r)
		}
	}()
	fmt.Println("Start CIM simu")

	WatchETCD()
	r = mux.NewRouter()
	r.HandleFunc(cimMtcilRegisterHTTPPath, httpHandler)
	r.HandleFunc(cimMetricsPath, metricsHandler)
	r.HandleFunc("/testcim", testHandler)
	http.ListenAndServe(cimMtcilRegisterPort, r)
	//maybe need context to terminate the test
	//wg.Done()
}
