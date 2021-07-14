package algo

import (
	"log"
	"sort"
)

func MySort() {
	ints := []int{1, 5, 3, 7, 6, 6, 6}
	strs := []string{"yue", "rui", "feng"}

	sort.Strings(strs)
	log.Println(strs)

	sort.Ints(ints)
	log.Println(ints)

	ints = []int{1, 5, 3, 7, 6, 6, 6}
	strs = []string{"yue", "rui", "feng"}
	ints1 := sort.IntsAreSorted(ints)
	log.Println(ints, ints1)
	strs1 := sort.StringsAreSorted(strs)
	log.Println(strs, strs1)
}
