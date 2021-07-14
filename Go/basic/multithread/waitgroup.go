package multithread

import (
	"log"
	"sync"
	"time"
)

func worker2(id int, wg *sync.WaitGroup) {
	defer wg.Done()

	log.Printf("worker_%v 开始运行", id)
	time.Sleep((time.Second))
	log.Printf("worker_%v 结束运行", id)
}

func Waitgroup() {
	var wg sync.WaitGroup

	for i := 0; i < 5; i++ {
		wg.Add(1)
		go worker2(i, &wg)
	}

	wg.Wait()
}