package main

import (
	"context"
	"fmt"
	"log"
	"bbtest/impl/simuctl/srvc/common"
	"bbtest/impl/simuctl/srvc/mail"
	"bbtest/impl/simuctl/srvc/config"
)

//add the test scope and scenario here
func init() {
    log.Println("simuctl main... ")
}

func prepareTest() {
	//init the event queue for nats
	common.GlobalDataQueue = common.NewDataQueue(1024)
	fmt.Println("Prepare testing")
}

func main() {
	parent := context.Background()

	defer func() {
		fmt.Println("Main stopped!!")
	}()
	mail.InitMail()
	common.WaitDBReady()
	common.WaitNatsReady()

	prepareTest()
	//for testing
	//common.AddJobToDB(parent)
	//get job from etcd
	//common.GetJobFromDB(parent)
	//save job to local
	config.ReadJobAssigned()
	//loop all registed test case, if match jobs in etcd, run the case
	common.RunJob(parent)

	//send the result email
	common.PublishEmail()
}
