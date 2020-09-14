package main

import (
	"fmt"
	"bbtest/impl/simubcc/srvc/common"
	bccsvc "bbtest/impl/simubcc/srvc"
	"time"
)

func init() {

}

func main() {
	fmt.Println("BCC simu started!!")
	common.WaitDBReady()
	common.WaitNatsReady()
	bccsvc.NatsSub()
	common.StartGrpc()

	for {
		time.Sleep(5 * time.Second)
		fmt.Println("loop BCC main ... ")
	}
}
