package miscs

import (
	"fmt"
	"math/rand"
	"time"
)

func Random() {
	p := fmt.Println

	// [0, 10)
	p(rand.Intn(10))
	// [0.0, 1.0)
	p(rand.Float64())
	// [5.0, 10.0)
	p(rand.Float64() * 5 + 5);

	// 根据当前时间生成随机数种子
	s1 := rand.NewSource(time.Now().UnixNano())
	// 根据随机数种子构造随机数生成器
	r1 := rand.New(s1)

	// 使用方法同上
	// [0, 10)
	p(r1.Intn(10))
	// [0.0, 1.0)
	p(r1.Float64())
	// [5.0, 10.0)
	p(r1.Float64() * 5 + 5);
}