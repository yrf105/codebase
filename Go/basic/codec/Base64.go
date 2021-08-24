package mycodec

import (
	"encoding/base64"
	"fmt"
)


// 采用 Base64 编码不仅比较简短，同时具有不可读性，被编码的数据不会被人用肉眼所直接看到。 
// 但是，标准的 Base64 并不适合直接放在 URL 里使用，
// 因为Base64中含有'+' 和 '/'， URL 编码器会 '+' 和 '/' 变为形如 "%XX” 的形式。

func Base64() {
	s := "yue rui feng abc123!?$*&()'-=@~"

	sEnc := base64.StdEncoding.EncodeToString([]byte(s))
	fmt.Println(sEnc)

	sDec, _ := base64.StdEncoding.DecodeString(sEnc)
	fmt.Println(string(sDec))

	uEnc := base64.URLEncoding.EncodeToString([]byte(s))
	fmt.Println(uEnc)
	uDec, _ := base64.URLEncoding.DecodeString(uEnc)
	fmt.Println(string(uDec))
}