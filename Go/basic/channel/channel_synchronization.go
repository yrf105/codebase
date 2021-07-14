package channel

import (
	"fmt"
	"time"
)

func worker(done chan bool) {
	fmt.Println("开始工作...")
	time.Sleep(time.Second)
	fmt.Println("结束工作")

	done<-true
}

func Coordinater() {
	done := make(chan bool)

	go worker(done)

	<-done
}

// 结束工作
// 开始工作