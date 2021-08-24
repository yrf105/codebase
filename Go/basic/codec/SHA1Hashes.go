package mycodec

import (
	"crypto/sha1"
	"fmt"
)

func SHA1Hashes() {
	s := "yue rui feng"
	h := sha1.New()
	h.Write([]byte(s))
	bs := h.Sum(nil)

	fmt.Printf("%s 的 sha1 是 %x\n", s, bs)
}