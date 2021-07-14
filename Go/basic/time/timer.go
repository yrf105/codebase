package mytime

import (
	"log"
	"time"
)

func Timer() {
	timer1 := time.NewTimer(2 * time.Second) // 一旦创建就开始计时
	time.Sleep(time.Second)
	log.Println("time1 start")
	<-timer1.C
	log.Println("timer1 is fired")

	timer2 := time.NewTimer(time.Second)
	go func() {
		<-timer2.C
		log.Println("timer2 is fired")
	}()
	timer2Stop := timer2.Stop()
	if timer2Stop {
		log.Println("timer2 stopped")
	}

	time.Sleep(2 * time.Second)
}

// === RUN   TestTimer
// === RUN   TestTimer/norm
// 2021/07/03 07:38:45 time1 start
// 2021/07/03 07:38:46 timer1 is fired
// 2021/07/03 07:38:46 timer2 stopped  
// --- PASS: TestTimer (4.01s)
//     --- PASS: TestTimer/norm (4.01s)
// PASS
// ok  	github.com/yrf105/basic/time	4.009