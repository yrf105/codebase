package mycodec

import (
	"fmt"
	"strconv"
)

func NumberParsing() {
	// 字符串转数字在 strconv 包中

	p := fmt.Println

	// 64 表示期望解析为 float64
	f1, _ := strconv.ParseFloat("3.14", 64)
	p(f1)

	i1, _ := strconv.ParseInt("234", 0, 64)
	p(i1)

	i2, _ := strconv.ParseInt("234", 8, 64)
	p(i2)


	i3, _ := strconv.ParseInt("0x111", 0, 64)
	p(i3)

	p(strconv.Atoi("123"))
	p(strconv.Atoi("3.14"))
}