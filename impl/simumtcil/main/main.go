package main

import (
	"fmt"
	"bbtest/impl/simucim/srvc/common"
	mtcilsvc "bbtest/impl/simumtcil/srvc"
	"sync"
	"time"
)

func init() {

}

func main() {
	fmt.Println("MTCIL simu started!!")
	wg := sync.WaitGroup{}
	common.WaitDBReady()
	common.WaitNatsReady()
	mtcilsvc.NatsSub()
	wg.Add(1)
	mtcilsvc.StartMtcil(&wg)

	//wg.Wait()
	for {
		time.Sleep(time.Second)
		fmt.Println("In mtcil main loop ... ")
	}
}
