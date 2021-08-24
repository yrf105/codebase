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

	// 限流器，速度限制在 个/200ms
	limiter := time.NewTicker(200 * time.Millisecond)
	for req := range requests {
		log.Println(req)
		<-limiter.C
	}

	// 突发限流器
	burstyLimiter := make(chan time.Time, 3)
	// 突发限流器的前 3 个可以瞬间接收
	for i := 0; i < 3; i++ {
		burstyLimiter <- time.Now()
	}
	// 突发限流器后面的则限制在 个/200ms
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
