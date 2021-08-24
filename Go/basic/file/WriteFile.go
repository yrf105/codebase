package myfile

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
)

func checkErr(err error) {
	if err != nil {
		panic(err)
	}
}

func WriteFile() {
	file := "../tmp/file2.txt"

	s := []byte("123456789hellogo")
	err := ioutil.WriteFile(file, s, 0644)
	checkErr(err)

	file1 := "../tmp/file3.txt"
	f, err := os.Create(file1)
	checkErr(err)
	defer f.Close()

	n, err := f.Write(s)
	checkErr(err)
	fmt.Println("写入", n, "字节")

	n, err = f.WriteString("ccddee")
	checkErr(err)
	fmt.Println("写入", n, "字节")

	f.Sync() // 落盘

	b := bufio.NewWriter(f)
	n, err = b.WriteString("sssssss")
	checkErr(err)
	fmt.Println("写入", n, "字节")

	n, err = b.Write(s)
	checkErr(err)
	fmt.Println("写入", n, "字节")

	b.Flush()
}