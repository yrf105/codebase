package miscs

import "os"

func Panic() {
	panic("a problem")

	_, err := os.Create("/tmp/file.txt")
	if err != nil {
		panic(err)
	}
}