package main

import (
	"bbtest/impl/simuctl/srvc/common"
	"bbtest/impl/simuctl/srvc/config"
	"context"
	"encoding/json"
	"fmt"
)

type E1SetupRsp struct {
	TransID  uint32 `json:"transid"`
	CuUpName string `json:"cuupname"`
}

type E1SetupREP struct {
	CuUpName string `json:"cuupname"`
}

func constructE1SetupRsp(rspPara string) {
	e1setuprsp := &E1SetupRsp{
		TransID:  5,
		CuUpName: "SIMU-CU-CP",
	}
	value, err := json.Marshal(e1setuprsp)
	if err != nil {
		fmt.Println(err)
	}
	dbkey := common.GetProcDBKey(rspPara)
	common.SendToDB(context.Background(), dbkey, value)
}

func checkProcE1Setup() {
	fmt.Println("In checkProcE1Setup ")
	//read from the event queue
	//check the msg and parameters
	//set the PROC status
	var natsData []byte
	var report common.NatsReport
	var item interface{}
	var rep E1SetupREP

	item = common.GlobalDataQueue.Pop(1)
	if item != nil {
		switch item.(type) {
		case []byte:
			fmt.Println("Type is byte")
			natsData = item.([]byte)
			fmt.Printf("item msg : %v \n", natsData)

			err := json.Unmarshal(natsData, &report)
			if err != nil {
				fmt.Println(err)
			}
			fmt.Println("The reported: ", report)
		case string:
			fmt.Println("Type is string")
		default:
			fmt.Println("Unkown type")
		}

		err := json.Unmarshal([]byte(report.Data), &rep)
		if err != nil {
			fmt.Println(err)
		}

		fmt.Println("The reported: ", rep)

		//check the para
		//set the Proc state
		if rep.CuUpName == "TEST-CUUP" {
			common.SetProcState(report.MsgName, "SUCCESS")
		}
	} else {
		//no event in the queu
		fmt.Println("no Event in nats report queue")
		return
	}
}
func TC_E1_SETUP_NORMAL(sco, sce string) {
	fmt.Println("testing TC_E1_E1SETUP_NORMAL")
	common.GetScenarioMgt(sco, sce)
	common.SetScenarioTO(10)
	common.SetWillRun("RUN")

	common.SetUp()
	config.LoadDay1Config()
	//add the resp para

	para := "cuupe1setuprsp.json"
	constructE1SetupRsp(para)
	common.AddProc("GNB-CU-UP-E1-SETUP-REQUEST", "RESPONSE", para, "TRUE", checkProcE1Setup)

	//publish the Test scenario to DB
	common.PubScenario()

	common.WaitAllFinish()
	common.ShowScenResult()
	common.SaveScenResult()

	common.CleanUp()
}

func TC_E1_SETUP_FAILURE(sco, sce string) {
	fmt.Println("testing TC_E1_E1SETUP_FAILURE")
	common.GetScenarioMgt(sco, sce)
	common.SetScenarioTO(3)
	common.SetWillRun("SKIP")

	common.SetUp()

	common.AddProc("UP-E1-RELEASE-REQ", "RESPONSE", "", "FALSE", nil)

	//publish the Test scenario to DB
	common.PubScenario()

	common.WaitAllFinish()

	common.ShowScenResult()
	common.SaveScenResult()

	common.CleanUp()
}

func TC_E1_SETUP_RESET(sco, sce string) {
	fmt.Println("testing TC_E1_E1SETUP_RESET")
	common.GetScenarioMgt(sco, sce)

	common.SetScenarioTO(5)
	common.SetUp()
	common.AddProc("UP-E1-RESET-REQ", "RESPONSE", "", "FALSE", nil)
	//publish the Test scenario to DB
	common.PubScenario()

	common.WaitAllFinish()
	common.ShowScenResult()
	common.SaveScenResult()

	common.CleanUp()
}
