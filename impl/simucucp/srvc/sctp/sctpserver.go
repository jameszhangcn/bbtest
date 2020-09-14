package sctp

import (
	"fmt"
	"net/http"
	"time"
	"unsafe"
)

//#cgo LDFLAGS: ${SRCDIR}/sctpserver.a -lsctp
//#include <stdio.h>
//#include <stdlib.h>
//#include "cwrap.h"
import "C"

func StartSctpServer() {
	fmt.Println("Start Sctp server")
	C.startSctp()
	go RecvSctp()
	//go testSend()
}

func testAllocMem() {
	for {
		test := make([]int, 1024000)
		test[0] = 0
		time.Sleep(time.Second)
	}
}

//add regiter func for callback
type Listener func(buf []byte, len int)

var listenners Listener

func RegisterListener(l Listener) {
	fmt.Println("register listener : ", l)
	listenners = l
}

func RecvSctp() {
	fmt.Println("Start Sctp server recv!")
	//remote interface for get pprof data
	//go testAllocMem()
	go func() {
		fmt.Println("Start the pprof http server")
		http.ListenAndServe("localhost:7777", nil)
	}()

	for {
		buf := make([]uint8, 2048)
		decodeBuf := unsafe.Pointer(&buf[0])

		len := C.recvSctp(decodeBuf)
		fmt.Println("SctpServer recved len ", len)
		if len < 0 {
			continue
		}

		for i := 0; i < int(len); i++ {
			tmp := unsafe.Pointer(uintptr(decodeBuf) + uintptr(i)*unsafe.Sizeof(buf[0]))
			buf[i] = *(*uint8)(tmp)
			buf = append(buf, byte(buf[i]))
		}
		listenners(buf, int(len))
		fmt.Println("SctpServer recved len 2 ", len, buf)
		//C.free(unsafe.Pointer(c_char))
		//C.free(unsafe.Pointer(res))

	}

}

func SendSctp(buf []byte, len int) {
	fmt.Println("send sctp data len: ", len)
	c_buf := unsafe.Pointer(&buf[0])
	c_len := C.int(len)
	C.sendSctp(c_buf, c_len)

}

func testSend() {
	for {
		SendSctp([]byte("send sctp testing"), 10)
		fmt.Println("Send sctp testing")
		time.Sleep(time.Second)
	}
}
