package main

import (
	"bbtest/impl/simuctl/srvc/common"
	"fmt"
)

func TC_BEARER_SETUP_NORMAL(sco, sce string) {
	fmt.Println("testing TC_BEARER_BEARER-SETUP-NORMAL")
	common.GetScenarioMgt(sco, sce)
	common.SetScenarioTO(5)
	common.SetUp()
	//common.AddProc("UP-E1-RESET-REQ", "RESPONSE", "", "FALSE", nil)
	//publish the Test scenario to DB
	common.PubScenario()

	common.WaitAllFinish()
	common.ShowScenResult()
	common.SaveScenResult()

	common.CleanUp()
}

func TC_BEARER_SETUP_FAILURE(sco, sce string) {
	fmt.Println("testing TC_BEARER_BEARER-SETUP-FAILURE")
	common.GetScenarioMgt(sco, sce)
	common.SetScenarioTO(5)
	common.SetUp()
	//common.AddProc("UP-E1-RESET-REQ", "RESPONSE", "", "FALSE", nil)
	//publish the Test scenario to DB
	common.PubScenario()

	common.WaitAllFinish()
	common.ShowScenResult()
	common.SaveScenResult()

	common.CleanUp()
}

func TC_BEARER_RELEASE(sco, sce string) {
	fmt.Println("testing TC_BEARER_BEARER-RELEASE")
	common.GetScenarioMgt(sco, sce)
	common.SetScenarioTO(5)
	common.SetUp()
	//common.AddProc("UP-E1-RESET-REQ", "RESPONSE", "", "FALSE", nil)
	//publish the Test scenario to DB
	common.PubScenario()

	common.WaitAllFinish()
	common.ShowScenResult()
	common.SaveScenResult()

	common.CleanUp()
}
