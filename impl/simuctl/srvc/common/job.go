package common

import (
	"bbtest/impl/simuctl/srvc/mail"
	"bbtest/impl/simuctl/srvc/types"
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"plugin"
	"reflect"
	"time"
)

var pathEtcdWaitingJob string

func init() {
	pathEtcdWaitingJob = "waitingJob"
}

func AddJobToDB(ctx context.Context) {
	var scope *types.Scope
	var scenario *types.Scenario

	job := new(types.Job)
	job.JobID = 10001
	job.Scopes = make([]types.Scope, 0)

	scope = new(types.Scope)
	scope.Name = "E1INTERFACE"
	scope.Scenarios = make([]types.Scenario, 0)

	scenario = new(types.Scenario)
	scenario.Name = "E1_SETUP_NORMAL"
	scope.Scenarios = append(scope.Scenarios, *scenario)

	scenario = new(types.Scenario)
	scenario.Name = "E1_SETUP_FAILURE"
	scope.Scenarios = append(scope.Scenarios, *scenario)

	scenario = new(types.Scenario)
	scenario.Name = "E1_SETUP_RESET"
	scope.Scenarios = append(scope.Scenarios, *scenario)

	job.Scopes = append(job.Scopes, *scope)

	scope = new(types.Scope)
	scope.Name = "BEARERCONTEXT"
	scope.Scenarios = make([]types.Scenario, 0)

	scenario = new(types.Scenario)
	scenario.Name = "BEARER_SETUP_NORMAL"
	scope.Scenarios = append(scope.Scenarios, *scenario)

	scenario = new(types.Scenario)
	scenario.Name = "BEARER_SETUP_FAILURE"
	scope.Scenarios = append(scope.Scenarios, *scenario)

	scenario = new(types.Scenario)
	scenario.Name = "BEARER_RELEASE"
	scope.Scenarios = append(scope.Scenarios, *scenario)

	job.Scopes = append(job.Scopes, *scope)

	value, err := json.Marshal(job)
	if err != nil {
		fmt.Println(err)
	}
	SendToDB(ctx, pathEtcdWaitingJob, value)
}

func GetJobFromDB(ctx context.Context) {
	fmt.Println("getJobFromDB")

	types.JobInstance = new(types.Job)

	data := getFromDB(ctx, pathEtcdWaitingJob)
	fmt.Println("We get ", data)
	fmt.Println(data)
	err := json.Unmarshal(data, types.JobInstance)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(types.JobInstance)
}

func Call(m map[string]interface{}, name string, params ...interface{}) (result []reflect.Value, err error) {

	f := reflect.ValueOf(m[name])
	if len(params) != f.Type().NumIn() {
		err = errors.New("The number of params is not adapted.")
		return
	}
	fmt.Println("In Call: ", name)
	in := make([]reflect.Value, len(params))
	for k, param := range params {
		in[k] = reflect.ValueOf(param)
	}
	result = f.Call(in)
	return
}

func runPlugin(scope, scenario string, testcases []types.TestCase) {
	soName := scope + "-" + scenario + ".so"
	p, err := plugin.Open(soName)
	if err != nil {
		log.Println("run Plugin open so failed : ", soName, err)
		panic(err)
	}
	for _, testcase := range testcases {
		if testcase.IsRun == true {
			caseName := scope + "_" + scenario + "_" + testcase.Name
			//find the function
			f, err := p.Lookup(caseName)
			if err != nil {
				log.Println("run Plugin lookup func failed : ", caseName, err)
				panic(err)
			}
			//call the func
			f.(func())()
		}
	}
}

func RunJob(ctx context.Context) {

	if types.JobInstance == nil {
		fmt.Println("job instance not inited")
		return
	}
	for _, scope := range types.JobInstance.Scopes {
		for _, scenario := range scope.Scenarios {
			name := "TC_" + scope.Name + "_" + scenario.Name
			fmt.Println("runJob testcase: ", name)
			scenario.State = "INIT"
			runPlugin(scope.Name, scenario.Name, scenario.TestCases)
			//if _, ok := TC_TABLE[name]; ok != true {
			//fmt.Println("TC not found: ", name)
			//continue
			//}
			//if result, err := Call(TC_TABLE, name, scope.Name, scenario.Name); err == nil {
			//fmt.Println("result", result, "err", err)
			//}
		}
	}
	ShowJobResult()
}

func ShowJobResult() {
	for _, scope := range types.JobInstance.Scopes {
		fmt.Println("Scope: ", scope.Name)
		for _, scenario := range scope.Scenarios {
			if scenario.State != "INIT" {
				fmt.Printf("| %-20s| %-20s|\n", scenario.Name, scenario.State)
			}
		}
	}
}

func PublishEmail() {
	result := make([]string, 0)
	for _, scope := range types.JobInstance.Scopes {
		fmt.Println("Scope: ", scope.Name)
		for _, scenario := range scope.Scenarios {
			if scenario.State != "INIT" {
				fmt.Printf("| %-30s| %-20s|\n", scenario.Name, scenario.State)
				ret := fmt.Sprintf("| %-30s| %-20s|\n", scenario.Name, scenario.State)
				result = append(result, ret)
			}
		}
	}

	subject := "BlackBoxTesting: " + time.Now().String()
	mail.SendMultiLineMail(subject, result)

}
