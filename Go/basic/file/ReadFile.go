package myfile

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"os"
)

func check(err error) {
	if err != nil {
		panic(err)
	}
}

func ReadFile() {
	file := "../tmp/file1.txt"

	fmt.Println("一下子将文件的所有内容读入内存")
	data, err := ioutil.ReadFile(file)
	check(err)
	print(string(data)) // 该方式不需要关闭文件

	fmt.Println("先打开文件，再一点一点读取")
	f, err := os.Open(file)
	check(err)
	defer f.Close() // 该方式需要注意关闭文件

	b := make([]byte, 5)
	n, err := f.Read(b) // 最多读取 5 字节，可能小于 5 字节
	check(err)
	fmt.Println(string(b[0:n]))

	o1, err := f.Seek(6, 0) // 从文件开始偏移 6 个字节开始读取
	check(err)
	b2 := make([]byte, 2)
	n, err = f.Read(b2)
	check(err)
	fmt.Println(o1, n, string(b2[0:n]))

	o2, err := f.Seek(6, 0) // 从文件开始偏移 6 个字节开始读取
	check(err)
	b3 := make([]byte, 2)
	n, err = io.ReadAtLeast(f, b3, 2) // 更安全的读取 2 个字节
	check(err)
	fmt.Println(o2, n, string(b3[0:n]))

	o3, err := f.Seek(6, 1) // 从当前偏移再偏移 6 个字节开始读取
	check(err)
	b4 := make([]byte, 2)
	n, err = io.ReadAtLeast(f, b4, 2) // 更安全的读取 2 个字节
	check(err)
	fmt.Println(o3, n, string(b4[0:n]))

	_, err = f.Seek(0, 0) 
	check(err)

	r4 := bufio.NewReader(f)
	b5, err := r4.Peek(5)
	check(err)
	fmt.Println(string(b5))
}
