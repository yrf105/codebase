package miscs

import (
	"log"
)

func CollectionFunc() {
	collection := []string{"yue", "rui", "feng", "rui", "feng", "yue"}
	log.Println(Index(collection, "yue"))

}

// Index 返回 t 在 vs 中第一次出现的位置下标
// 若 t 未在 vs 中出现则返回 -1
func Index(vs []string, t string) int {
	for i, v := range vs {
		if v == t {
			return i
		}
	}
	return -1
}

// 若 vs 中存在 t，则 Include 返回 true，
// 否则 Include 返回 false
func Include(vs []string, t string) bool {
	return Index(vs, t) != -1
}

// 若 vs 中存在满足 fun 的元素则 Any 返回 true，
// 否则 Any 返回 false
func Any(vs []string, fun func(string) bool) bool {
	for _, s := range vs {
		if fun(s) {
			return true
		}
	}
	return false
}

// 若 vs 中的所有元素都满足 fun 则 All 返回 true，
// 否则 All 返回 false
func All(vs []string, fun func(string) bool) bool {
	for _, v := range vs {
		if !fun(v) {
			return false
		}
	}
	return true
}

// Filter 返回一个新的链表，可能为空链表，若不空则其中所有元素都满足 f
func Filter(vs []string, f func(string) bool) []string {
	vsf := make([]string, 0)
	for _, v := range vs {
		if f(v) {
			vsf = append(vsf, v)
		}
	}
	return vsf
}

// Map 返回一个新链表，新链表中的元素都是旧链表中对应位置元素经过 f 之后返回的新值
func Map(vs []string, f func(string) string) []string {
	vsm := make([]string, len(vs))
	for i, v := range vs {
		vsm[i] = f(v)
	}
	return vsm
}
