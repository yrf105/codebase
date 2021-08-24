package my_net

import (
	"bufio"
	"fmt"
	"net/http"
)

func HttpClient() {
	resp, err := http.Get("https://www.qq.com/")
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()

	fmt.Println("response status: ", resp.StatusCode)

	bodyScanner := bufio.NewScanner(resp.Body)
	for bodyScanner.Scan() {
		fmt.Println(bodyScanner.Text())
	}

	if err = bodyScanner.Err(); err != nil {
		panic(err)
	}
}