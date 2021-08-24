package mytime

import (
	"fmt"
	"time"
)

func Time() {
	p := fmt.Println

	// 这个是 time_point 的概念
	now := time.Now()
	p(now)

	then := time.Date(1997, 12, 3, 12, 12, 12, 12, time.UTC)
	p(then)

	p(then.Year())
	p(then.Month())
	p(then.Day())
	p(then.Hour())
	p(then.Minute())
	p(then.Second())
	p(then.Location())

	p(then.Weekday())

	p(then.Before(now))
	p(then.After(now))
	p(then.Equal(now))

	// 这个是 duration 的概念
	diff := then.Sub(now)
	p(diff)

	p(diff.Hours())
	p(diff.Minutes())
	p(diff.Seconds())
	p(diff.Nanoseconds())

}