package multithread

import (
	"log"
	"math/rand"
	"sync/atomic"
	"time"
)

type readOp struct {
	key int
	resp chan int
}

type writeOp struct {
	key int
	val int
	resp chan bool
}

func StatefulGoroutines() {
	state := make(map[int]int)

	reads := make(chan readOp)
	writes := make(chan writeOp)

	var readOps int64
	var writeOps int64

	go func() {
		for {
			select {
			case read := <- reads:
				read.resp <- state[read.key]
			case write := <- writes:
				state[write.key] = write.val
				write.resp <- true
			}
		}
	}()

	for i := 0; i < 100; i++ {
		go func() {
			total := 0
			re := make(chan int)
			for {
				read := readOp{
					key:  rand.Intn(5),
					resp: re,
				}
				reads <- read
				total += <-re
				atomic.AddInt64(&readOps, 1)
				time.Sleep(time.Millisecond)
			}
		}()
	}

	for i := 0; i < 10; i++ {
		go func() {
			re := make(chan bool)
			for {
				write := writeOp{
					key:  rand.Intn(5),
					val:  rand.Intn(100),
					resp: re,
				}
				writes <- write
				<-re
				atomic.AddInt64(&writeOps, 1)
				time.Sleep(time.Millisecond)
			}
		}()
	}

	time.Sleep(time.Second)

	readFinalOps := atomic.LoadInt64(&readOps)
	log.Println("readFinalOps", readFinalOps)
	writeFinalOps := atomic.LoadInt64(&writeOps)
	log.Println("writeFinalOps", writeFinalOps)

	log.Println("state", state)
}