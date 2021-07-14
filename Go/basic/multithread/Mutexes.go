package multithread

import (
	"log"
	"math/rand"
	"sync"
	"sync/atomic"
	"time"
)

func Mutexes() {
	state := make(map[int]int)
	mu := &sync.Mutex{}
	
	var readOps int64 = 0
	var writeOps int64 = 0

	for i := 0; i < 100; i++ {
		go func() {
			total := 0
			for {
				key := rand.Intn(5)
				mu.Lock()
				total += state[key]
				mu.Unlock()
				atomic.AddInt64(&readOps, 1)
				time.Sleep(time.Millisecond)
			}
		}()
	}

	for i := 0; i < 10; i++ {
		go func() {
			for {
				key := rand.Intn(5)
				val := rand.Intn(100)
				mu.Lock()
				state[key] = val
				mu.Unlock()
				atomic.AddInt64(&writeOps, 1)
				time.Sleep(time.Millisecond)
			}
		}()
	}

	time.Sleep(time.Second)

	writeFinalOps := atomic.LoadInt64(&writeOps)
	log.Println("writeFinalOps:", writeFinalOps)
	readFinalOps := atomic.LoadInt64(&readOps)
	log.Println("readFinalOps", readFinalOps)

	mu.Lock()
	log.Println("state", state)
	mu.Unlock()
}
