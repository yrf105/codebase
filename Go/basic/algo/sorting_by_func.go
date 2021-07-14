package algo

import (
	"log"
	"sort"
)

type Strs []string

func (s Strs) Len() int {
	return len(s)
}

func (s Strs) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}

func (s Strs) Less(i, j int) bool {
	return s[i] < s[j]
}

func SortByFunc() {
	strs := Strs{"yue", "rui", "feng"}
	sort.Sort(strs)
	log.Println(strs)
}

var dire = [5]int{1, 2, 3, 4, 5}
