package multithread

import (
	"context"
	"errors"
	"fmt"
	"net/http"
	"os"
	"time"
)

func ProcessReq() {
	http.ListenAndServe(":12345", http.HandlerFunc(func(rw http.ResponseWriter, r *http.Request) {
		context := r.Context()
		fmt.Fprintln(os.Stdout, "正在处理请求")
		select {
		case <-time.After(2 * time.Second):
			// 模拟 2 秒之后连接处理完毕
			rw.Write([]byte("请求已处理"))
		case <-context.Done():
			// 客户端在请求处理完毕之前关闭连接
			fmt.Fprintln(os.Stderr, "请求被取消")
		}
	}))
}

func op1(ctx context.Context) error {
	time.After(200 * time.Millisecond)
	return errors.New("op1 error")
}

// func op1(ctx context.Context) {
// 	time.Sleep(200 * time.Millisecond)
// 	_, cancel := context.WithCancel(ctx)
// 	cancel()
// }

func op2(ctx context.Context) {
	select {
	case <-time.After(500 * time.Millisecond):
		fmt.Println("op2 完成")
	case <-ctx.Done():
		fmt.Println("op2 被终止")
	}
}

func ContextCancellationExample() {
	ctx := context.Background()
	ctx, cancel := context.WithCancel(ctx)

	go func() {
		error := op1(ctx)
		if error != nil {
			cancel()
			cancel()
		}
	}()
	// go op1(ctx)

	op2(ctx)
}
