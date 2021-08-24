package miscs

import (
	"flag"
	"fmt"
)

// 使用 flag 需要先运行 go build 编译

func CMFlags() {
	wordArg := flag.String("word", "foo", "a string") // 注意：返回的是指针
	intArg := flag.Int("int", 12, "a int")
	boolArg := flag.Bool("boo", false, "a bool")

	var wordVar string
	flag.StringVar(&wordVar, "stringVar", "bar", "a string var")

	fmt.Println(*wordArg)
	fmt.Println(*intArg)
	fmt.Println(*boolArg)
}