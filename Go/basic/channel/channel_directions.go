package channel

import "fmt"

func ping(pingCh chan<- string, msg string) {
	pingCh <- msg
}

func pong(pingCh <-chan string, pongCh chan<- string) {
	pongCh <- <-pingCh
}

func PingPong() {
	pingCh := make(chan string, 1)
	pongCh := make(chan string, 1)

	ping(pingCh, "pingpong")
	pong(pingCh, pongCh)
	fmt.Println(<-pongCh) // pingpong
}