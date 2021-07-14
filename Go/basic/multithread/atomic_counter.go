package multithread

import (
	"log"
	"sync"
	"sync/atomic"
)

func AtomicCounter() {
	var ops uint64
	var wg sync.WaitGroup

	for i := 0; i < 50; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for j := 0; j < 1000; j++ {
				atomic.AddUint64(&ops, 1)
				// ops++
			}
		}()
	}

	wg.Wait()
	log.Println(ops)
}
