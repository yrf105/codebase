package miscs

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
)

func HandleSig() {
	// 如何在 Go 中使用 channel 处理信号?
	sigs := make(chan os.Signal, 1)
	done := make(chan bool, 1)

	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	go func(){
		sig := <- sigs
		fmt.Println()
		fmt.Println(sig, "信号")
		done <- true
	}()

	<-done
	fmt.Println("结束")
}