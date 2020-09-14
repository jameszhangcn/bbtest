package common

import "strconv"

//ExptProc : the expect msg behaviour
type ExptProc struct {
	MsgType   string `json:"msgType"`
	Behaviour string `json:"behaviour"`
	RespPara  string `json:"respPara"` //this is the filename of parameter json
	NeedCheck string `json:"needCheck"`
	State     string `json:"state"`
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

var Scenario ScenarioMgt

func GetProcCfg(msgType string) *ExptProc {
	for i := 0; i < len(Scenario.Procs); i++ {
		proc := Scenario.Procs[i]
		if proc.MsgType == msgType {
			return &proc
		}
	}
	return nil
}

func GetProcDBKey(para string) string {
	return (strconv.Itoa(Scenario.JobID) + "/" + Scenario.ScopeName + "/" + Scenario.ScenarioName + "/" + para)
}
