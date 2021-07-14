package channel

import (
	"log"
	"time"
)

func Select() {
	c1 := make(chan string)
	c2 := make(chan string)

	go func() {
		time.Sleep(time.Second)
		c1 <- "hello"
	}()

	go func() {
		time.Sleep(2 * time.Second)
		c2 <- "world"
	}()

	for i := 0; i < 2; i++ {	
		select {
		case msg := <- c1:
			log.Println(msg)
		case msg := <- c2:
			log.Println(msg)
		}
	}
}
