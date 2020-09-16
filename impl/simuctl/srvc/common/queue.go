package common

import (
	"fmt"
	"time"
)

type DataQueue struct {
	Queue chan interface{}
}

var GlobalDataQueue *DataQueue

func NewDataQueue(max_len int) (db *DataQueue) {
	dq := &DataQueue{}
	dq.Queue = make(chan interface{}, max_len)
	return dq
}

func (dq *DataQueue) Push(data interface{}, waittime time.Duration) bool {
	to := time.After(waittime)
	select {
	case dq.Queue <- data:
		return true
	case <-to:
		return false
	}
}

func (db *DataQueue) Pop(waittime time.Duration) (data interface{}) {
	click := time.After(waittime)
	select {
	case data = <-db.Queue:
		return data
	case <-click:
		return nil
	}
}

func (db *DataQueue) Empty(waittime time.Duration) {
	click := time.After(waittime)
	select {
	case data := <-db.Queue:
		fmt.Println("remove data: ", data)
	case <-click:
		fmt.Println("Empty queue timeout")
		return
	}
}
