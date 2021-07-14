package mytime

import (
	"log"
	"time"
)

func Timeouts() {
	c1 := make(chan string, 1)
	go func() {
		time.Sleep(2 * time.Second)
		c1 <- "hello"
	}()

	select {
	case msg := <- c1:
		log.Println(msg)
	case <-time.After(time.Second * 1):
		log.Println("timeout")
	}

	c2 := make(chan string, 1)
	go func() {
		time.Sleep(time.Second * 2)
		c2 <- "world"
	}()

	select {
	case msg:=<-c2:
		log.Println(msg)
	case <-time.After(time.Second * 3):
		log.Println("timeout")
	}
}