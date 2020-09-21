package main

import (
	"bbtest/impl/simuctl/srvc/common"
	"bbtest/impl/simuctl/srvc/config"
	"context"
	"fmt"
	"os"
        "time"
	log "github.com/jeanphorn/log4go"
)

//add the test scope and scenario here
func init() {
	path := "/tmp/logs"
	if !pathExists(path) {
		log.Warn("dir: /tmp/logs not found.")
		err := os.MkdirAll(path, 0711)
		if err != nil {
			log.Error(err.Error())
			panic(err)
		}
	}
	file,err := os.OpenFile("/tmp/logs/test.log", os.O_RDWR| os.O_CREATE, 0766);
	fmt.Println(file)
	fmt.Println(err)
	file.Close()

	log.LoadConfiguration("/opt/conf/log.json")
}

func pathExists(path string) bool {
    _, err := os.Stat(path)
    if err == nil {
	    return true
    }
    if os.IsNotExist(err) {
	    return false
    }
    return false
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
	defer log.Close()
	log.LOGGER("Test").Info("BBTest simuctl started!")

	//mail.InitMail()
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
	//common.PublishEmail()
	//save the result to NFS
	common.SaveResultToNFS()
	log.LOGGER("Test").Info("BBTest simuctl ended!")
	counter := 1
	for {
		time.Sleep(time.Second)
		log.LOGGER("Test").Info("BBTest simuctl ended ! count: ", counter)
		counter++
	}
}
