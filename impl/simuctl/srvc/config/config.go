package config

import (
	"bbtest/impl/simuctl/srvc/common"
	"bbtest/impl/simuctl/srvc/types"
	"context"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

var vsConfigJson = "./config/DAY1_vs.json"
var vsConfigJsonUpdate = "./config/DAY1_vs_update.json"
var threegppConfigJson = "./config/DAY1_3GPP.json"
var jobAssignedFilePath = "/opt/conf/jobs.json"
var dayOneConfigPath string

var (
	Namespace        = "default"
	MicroserviceName = "CUUP"
)

func init() {
	log.SetPrefix("SIMUCTL-CONFIG: ")
	log.SetFlags(log.Ldate | log.Lmicroseconds | log.Llongfile)
	types.JobInstance = new(types.Job)
}

func LoadDay1Config() {
	dayOneConfigPath = "config/" + Namespace + "/" + MicroserviceName

	//send the day1 config
	vsConfig, err := ioutil.ReadFile(vsConfigJson)
	if err != nil {
		fmt.Println("readfile", err)
		return
	}

	//push to ETCD
	value, err := json.Marshal(vsConfig)
	if err != nil {
		fmt.Println(err)
	}
	common.SendToDB(context.Background(), dayOneConfigPath, value)

	threeGppConfig, err := ioutil.ReadFile(threegppConfigJson)
	if err != nil {
		fmt.Println("readfile", err)
		return
	}

	value, err = json.Marshal(threeGppConfig)
	if err != nil {
		fmt.Println(err)
	}
	common.SendToDB(context.Background(), dayOneConfigPath, value)
	//push to ETCD
}

func SendConfigPatch() {
	fmt.Println("SendConfigPatch")
}

func ReadJobAssigned() {
	var content []byte
	_, err := os.Stat(jobAssignedFilePath)
	log.Println("Job assigned info : ", err)
	if os.IsNotExist(err) {
		log.Println("No job to run....")
		return
	} else {
		content, err := ioutil.ReadFile(jobAssignedFilePath)
		if err != nil {
			log.Println("Job assigend read file error")
			return
		}
		data := types.JobInstance
		err = json.Unmarshal(content, &data)
		if err != nil {
			log.Println("Job assigend unmarshal error: ", content)
			return
		}
		//types.jobInstance = data
		log.Println("Unmarshell sucess: ", types.JobInstance)
	}
	log.Println("ReadJobAssigned : ", string(content[:]))
}
