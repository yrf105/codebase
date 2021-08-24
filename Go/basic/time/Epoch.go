package mytime

import (
	"fmt"
	"time"
)

func Epoch() {
	p := fmt.Println

	now := time.Now()
	p(now)

	// 从 Unix 纪元开始的秒数和纳秒数
	secs := now.Unix()
	nsecs := now.UnixNano()
	p(secs)
	p(nsecs)
	
	msec := nsecs / 1000000
	p(msec)

	p(time.Unix(secs, 0))
	p(time.Unix(0, nsecs))
}