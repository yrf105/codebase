package multithread

import (
	"fmt"
	"log"
	"time"
)

func RateLimiting() {
	log.SetFlags(log.Ldate | log.Ltime | log.Lmicroseconds)

	requests := make(chan time.Time, 5)
	for i := 0; i < 5; i++ {
		requests <- time.Now()
	}
	close(requests)

	limiter := time.NewTicker(200 * time.Millisecond)
	for req := range requests {
		log.Println(req)
		<-limiter.C
	}

	burstyLimiter := make(chan time.Time, 3)

	for i := 0; i < 3; i++ {
		burstyLimiter <- time.Now()
	}

	go func() {
		for t := range time.Tick(200 * time.Millisecond) {
			burstyLimiter <- t
		}
	}()

	requests = make(chan time.Time, 5)
	for i := 0; i < 5; i++ {
		requests <- time.Now()
	}
	close(requests)

	fmt.Println()
	for req := range requests {
		log.Println(req)
		<-burstyLimiter
	}

}
