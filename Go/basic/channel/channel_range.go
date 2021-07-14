package channel

import "log"

func Range() {
	messages := make(chan string, 2)

	messages <- "hello"
	messages <- "world"
	close(messages) // 不 close 的话，下面的 range 会阻塞

	for msg := range messages {
		log.Println(msg)
	}
}