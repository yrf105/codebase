package mytime

import (
	"log"
	"time"
)

func Ticker() {
	// 创建一个 500ms 超时时间的循环定时器 
	ticker1 := time.NewTicker(500 * time.Millisecond)
	done := make(chan bool)

	go func() {
		for {
			select {
			case <-done:
				return
			case t := <-ticker1.C:
				log.Println("ticker1 tick at ", t)
			}
		}
	}()

	time.Sleep(2 * time.Second)
	done<-true
}