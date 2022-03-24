package multithread

import (
	"context"
	"fmt"
	"net/http"
	"time"
)

func ContextTimoutsExample() {
	// 新建一个 5 秒之后取消的上下文
	ctx := context.Background()
	ctx, _ = context.WithTimeout(ctx, 5 * time.Millisecond)

	// 新建一个向百度主页发起的请求
	req, _ := http.NewRequest(http.MethodGet, "http://baidu.com", nil)
	// 将请求与 ctx 关联起来
	req = req.WithContext(ctx)

	// 新建一个 http 客户端发起请求
	client := &http.Client{}
	res, err := client.Do(req)

	if err != nil {
		fmt.Println("请求失败：", err)
		return
	}

	fmt.Println("响应码：", res.StatusCode);
}