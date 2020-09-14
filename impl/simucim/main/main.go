package main

import (
	"fmt"
	"bbtest/impl/simucim/srvc/common"
	cimsvc "bbtest/impl/simucim/srvc"
	"sync"
	"time"
)

func init() {

}

func main() {
	fmt.Println("CIM simu started!!")
	wg := sync.WaitGroup{}
	common.WaitDBReady()
	common.WaitNatsReady()
	cimsvc.NatsSub()
	wg.Add(1)
	cimsvc.StartCim(&wg)

	//wg.Wait()
	for {
		time.Sleep(time.Second)
		fmt.Println("In Cim main loop ... ")
	}
}
