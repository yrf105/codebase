package tmp

import "math"

func titleToNumber(columnTitle string) int {
	n := len(columnTitle)
	ans := 0.0
	for i := n - 1; i >= 0; i-- {
		ans += float64(columnTitle[i]-'A') * math.Pow(float64(26), float64(n-1-i))
	}

	return int(ans)
}
