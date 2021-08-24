package my_net

import (
	"fmt"
	"net/http"
)

func HttpServer() {
	http.HandleFunc("/hello", func(rw http.ResponseWriter, r *http.Request) {
		fmt.Fprintln(rw, "hello")
	})
	http.HandleFunc("/head", func(rw http.ResponseWriter, r *http.Request) {
		for name, value := range r.Header {
			fmt.Fprintln(rw, name, ": ", value)
		}
	})

	http.ListenAndServe(":12345", nil)
}