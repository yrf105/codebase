package channel

import (
	"fmt"
)

func Channels() {
	messages := make(chan string)

	go func() {
		messages <- "hello"
		fmt.Println("world")
	}()

	fmt.Println(<-messages)
}

// world
// hello