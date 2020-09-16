package common

import (
	"context"
	"encoding/json"
	"fmt"
	"strconv"
	"time"
	"bbtest/impl/simuctl/srvc/types"
)

//ExptProc : the expect msg behaviour
type ExptProc struct {
	MsgType   string `json:"msgType"`
	Behaviour string `json:"behaviour"`
	RespPara  string `json:"respPara"` //this is the filename of parameter json
	NeedCheck string `json:"needCheck"`
	State     string `json:"state"`
	execFunc  func()
}

type ScenarioMgt struct {
	JobID        int        `json:"jobID"`
	ScopeName    string     `json:"scope"`
	ScenarioName string     `json:"scenario`
	TimeOut      int        //seconds
	State        string     //INIT, SUCCESS, TIMEOUT
	WillRun      string     //RUN, SKIP
	Procs        []ExptProc `json:"procs`
}

type NatsReport struct {
	MsgName string `json:"msgname"`
	Data    []byte `json:"data"`
}

var GlobalScenarioMgmt *ScenarioMgt

var TC_TABLE map[string]interface{}

var pathJobScenario string

func AddProc(msg, behr, rsp, is_check string, execFunc func()) {

	if !IsWillRun() {
		//the case will not be added
		return
	}
	exptProc := &ExptProc{
		MsgType:   msg,
		Behaviour: behr,
		RespPara:  rsp,
		NeedCheck: is_check,
		State:     "INIT",
		execFunc:  execFunc,
	}

	GlobalScenarioMgmt.Procs = append(GlobalScenarioMgmt.Procs, *exptProc)

	if rsp != "" {
		//push the response para to DB
		fmt.Println("Proc resp json: ", rsp)
	}
}

//AddTrigger : this function add the spontaneout msg and the trigger condition
//eg. some msg received, or tigger a failure after some time
func AddTrigger(service, msgType, condition string) {
	fmt.Println("AddTrigger")
}

func GetScenarioMgt(sco, sce string) {
	GlobalScenarioMgmt = new(ScenarioMgt)
	GlobalScenarioMgmt.ScopeName = sco
	GlobalScenarioMgmt.ScenarioName = sce
	GlobalScenarioMgmt.Procs = make([]ExptProc, 0)
	GlobalScenarioMgmt.TimeOut = 60 //second
	GlobalScenarioMgmt.JobID = types.JobInstance.JobID
	GlobalScenarioMgmt.WillRun = "RUN"
	GlobalScenarioMgmt.State = "INIT"
	pathJobScenario = "JobScenario" + string(types.JobInstance.JobID)

	InitScenResult()
}

func isAllProcFinished() bool {
	for i := 0; i < len(GlobalScenarioMgmt.Procs); i++ {
		proc := GlobalScenarioMgmt.Procs[i]
		fmt.Println("The expt proc:", i, proc)
		if proc.State == "INIT" {
			return false
		}
	}
	return true
}

func SetProcState(msg, state string) bool {
	for i := 0; i < len(GlobalScenarioMgmt.Procs); i++ {
		proc := &GlobalScenarioMgmt.Procs[i]
		if proc.MsgType == msg {
			proc.State = state
			return true
		}
	}
	return false
}

func SetUp() {
	time.Sleep(3 * time.Second)
}

func CleanUp() {
	GlobalDataQueue.Empty(5)
	time.Sleep(3 * time.Second)
}

func ShowScenResult() {
	fmt.Println("The result for ", GlobalScenarioMgmt.ScenarioName, GlobalScenarioMgmt.State)
	for i := 0; i < len(GlobalScenarioMgmt.Procs); i++ {
		proc := GlobalScenarioMgmt.Procs[i]
		fmt.Println("The proc:", proc, proc.State)
	}
}

func SaveScenResult() {
	for i := 0; i < len(types.JobInstance.Scopes); i++ {
		scope := &types.JobInstance.Scopes[i]
		for j := 0; j < len(scope.Scenarios); j++ {
			scenario := &scope.Scenarios[j]
			//fmt.Println("Save Scen result: ", GlobalScenarioMgmt.ScopeName, GlobalScenarioMgmt.ScenarioName)
			//fmt.Println("Save Scen result: ", scope.Name, scenario.Name)
			if scope.Name == GlobalScenarioMgmt.ScopeName && scenario.Name == GlobalScenarioMgmt.ScenarioName {
				fmt.Println("Results saved: ", scenario.Name, GlobalScenarioMgmt.State)
				scenario.State = GlobalScenarioMgmt.State
				return
			}
		}
	}
}

func InitScenResult() {
	for i := 0; i < len(types.JobInstance.Scopes); i++ {
		scope := &types.JobInstance.Scopes[i]
		for j := 0; j < len(scope.Scenarios); j++ {
			scenario := &scope.Scenarios[j]
			//fmt.Println("Save Scen result: ", GlobalScenarioMgmt.ScopeName, GlobalScenarioMgmt.ScenarioName)
			//fmt.Println("Save Scen result: ", scope.Name, scenario.Name)
			if scope.Name == GlobalScenarioMgmt.ScopeName && scenario.Name == GlobalScenarioMgmt.ScenarioName {
				fmt.Println("Results Init: ", scenario.Name, GlobalScenarioMgmt.State)
				scenario.State = "INIT"
				return
			}
		}
	}
}

func GetProcDBKey(para string) string {
	return (strconv.Itoa(GlobalScenarioMgmt.JobID) + "/" + GlobalScenarioMgmt.ScopeName + "/" + GlobalScenarioMgmt.ScenarioName + "/" + para)
}

func SetScenarioTO(to int) {
	GlobalScenarioMgmt.TimeOut = to
}

func SetScenarioState(state string) {
	GlobalScenarioMgmt.State = state
}

func SetWillRun(run string) {
	GlobalScenarioMgmt.WillRun = run
}

func IsWillRun() bool {
	if GlobalScenarioMgmt.WillRun == "RUN" {
		return true
	}
	return false
}

func PubScenario() {
	fmt.Println("PubScenario")
	value, err := json.Marshal(GlobalScenarioMgmt)
	if err != nil {
		fmt.Println(err)
	}
	SendToDB(context.Background(), pathJobScenario, value)
	go PubScenarioStart()
}

func CallbackAllProc() {
	for i := 0; i < len(GlobalScenarioMgmt.Procs); i++ {
		proc := GlobalScenarioMgmt.Procs[i]
		fmt.Println("The proc:", proc)
		if proc.State == "INIT" {
			if proc.execFunc != nil {
				proc.execFunc()
			}
		}
	}
}

func WaitAllFinish() {
	ch := make(chan string)
	//start timer
	duration := GlobalScenarioMgmt.TimeOut
	go func() {
		time.Sleep(time.Duration(duration) * time.Second)
		fmt.Println("In WaitAllFinish ", duration)
		ch <- "timeout"
	}()

	tickTimer := time.NewTicker(1 * time.Second)

WAIT:
	for {
		select {
		case <-ch:
			fmt.Println("TestCase ", GlobalScenarioMgmt.ScenarioName, " wait Timeout")
			GlobalScenarioMgmt.State = "TIMEOUT"
			break WAIT
		case <-tickTimer.C:
			{
				//call the proc func
				CallbackAllProc()

				if isAllProcFinished() {
					fmt.Println("all proc done for scenario ", GlobalScenarioMgmt.ScenarioName, "Finished")
					break WAIT
				}
				fmt.Println("Waiting all proc done for scenario ", GlobalScenarioMgmt.ScenarioName)
			}
		}
	}
}

func PubScenarioStart() {
	time.Sleep(time.Second)
	value, err := json.Marshal(GlobalScenarioMgmt)
	if err != nil {
		fmt.Println(err)
	}
	PubEvent("SIMUCUCP-S", value)
	PubEvent("SIMUCIM-S", value)
	PubEvent("SIMUBCC-S", value)
}
