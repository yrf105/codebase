package channel

import (
	"fmt"
)

func Nonblockingchanneloperations() {
	message := make(chan string)
	signal := make(chan string)

	select {
	case msg := <-message:
		fmt.Println("收到消息", msg)
	default:
		fmt.Println("没有收到消息")
	}

	select {
	case message <- "消息":
		fmt.Println("发送消息成功")
	default:
		fmt.Println("发送消息失败")
	}

	select {
	case msg := <-message:
		fmt.Println("收到消息", msg)
	case sig := <-signal:
		fmt.Println("收到信号", sig)
	default:
		fmt.Println("没有收到任何东西")
	}
}

// 没有收到消息
// 发送消息失败
// 没有收到任何东西
