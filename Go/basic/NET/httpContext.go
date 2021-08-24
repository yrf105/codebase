package my_net

import (
	"fmt"
	"net/http"
	"time"
)

func HttpContext() {
	http.HandleFunc("/hello", func(rw http.ResponseWriter, r *http.Request) {
		// A Context carries deadlines, cancellation signals,
		// and other request-scoped values across API boundaries and goroutines.
		ctx := r.Context()
		fmt.Println("开始处理 /hello")
		defer fmt.Println("结束处理 /hello")

		select {
		case <-time.After(10 * time.Second):
			fmt.Fprintln(rw, "hello")
		case <-ctx.Done():
			err := ctx.Err()
			fmt.Println("server: ", err)
			httpServerErr := http.StatusInternalServerError
			http.Error(rw, err.Error(), httpServerErr)
		}
	})

	http.ListenAndServe(":12345", nil)
}
