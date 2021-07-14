package miscs

import (
	"fmt"
	"log"
	"os"
)

func Defer() {
	file := createFile("/tmp/defer.txt")
	defer closeFile(file)
	writeFile(file)
}

func createFile(filename string) *os.File {
	log.Println("创建文件")
	f, err := os.Create(filename)
	if err != nil {
		panic(err)
	}
	return f
}

func writeFile(file *os.File) {
	log.Println("写入数据")
	fmt.Fprintln(file, "数据")
}

func closeFile(file *os.File) {
	log.Println("关闭文件")

	err := file.Close()
	if err != nil {
		panic(err)
	}
}