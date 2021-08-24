package mycodec

import (
	"encoding/json"
	"fmt"
	"os"
)

// 只要以大写字母开头的成员才会被编解码
type response1 struct {
	Page int
	Fruits []string
	stock int
}

type response2 struct {
	Page   int      `json:"page,omitempty"`
	Fruits []string `json:"fruits,omitempty"`
	stock  int      
}

func Json() {

	// 编码

	bolB, _ := json.Marshal(true)
	fmt.Println(string(bolB))

	intB, _ := json.Marshal(55)
	fmt.Println(string(intB))

	floB, _ := json.Marshal(3.14)
	fmt.Println(string(floB))

	slcD := []string{"yue", "rui", "feng"}
	slcB, _ := json.Marshal(slcD)
	fmt.Println(string(slcB))

	mapD := map[string]int{"first": 1, "second": 2, "third": 3}
	mapB, _ := json.Marshal(mapD)
	fmt.Println(string(mapB))

	resD1 := response1{
		Page:   1,
		Fruits: []string{"apple", "orange", "banana"},
		stock:  33,
	}
	resB1, _ := json.Marshal(resD1)
	fmt.Println(string(resB1))

	resD2 := response2{
		Page:   1,
		Fruits: []string{"apple", "orange", "banana"},
		stock:  33,
	}
	resB2, _ := json.Marshal(resD2)
	fmt.Println(string(resB2))

	// 解码

	var intD int
	if err := json.Unmarshal(intB, &intD); err != nil {
		panic(err)
	}
	fmt.Println(intD)

	var floD float64
	if err := json.Unmarshal(floB, &floD); err != nil {
		panic(err)
	}
	fmt.Println(floD)

	var slcDa []string
	if err := json.Unmarshal(slcB, &slcDa); err != nil {
		panic(err)
	}
	fmt.Println(slcDa)

	var resDa2 response2
	if err := json.Unmarshal(resB2, &resDa2); err != nil {
		panic(err)
	}
	fmt.Println(resDa2)

	// 直接编码到 os.Writers
	enc := json.NewEncoder(os.Stdout)
	enc.Encode(resD2)
}