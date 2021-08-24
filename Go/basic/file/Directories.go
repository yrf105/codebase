package myfile

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
)

func Directories() {
	err := os.Mkdir("subdir", 0755) // 在当前文件加下创建一个子目录
	check(err)

	defer os.RemoveAll("subdir") // 类似 rm -rf subdir

	createEmptyFile := func(name string) {
		d := []byte("")
		check(ioutil.WriteFile(name, d, 0644))
	}

	createEmptyFile("subdir/file1")

	err = os.MkdirAll("subdir/parent/child", 0755)
	check(err)

	createEmptyFile("subdir/parent/file2")
	createEmptyFile("subdir/parent/file3")
	createEmptyFile("subdir/parent/child/file4")

	c, err := os.ReadDir("subdir/parent")
	check(err)
	// child true
	// file2 false
	// file3 false
	for _, entry := range c {
		fmt.Println(entry.Name(), entry.IsDir())
	}

	err = os.Chdir("subdir/parent/child")
	check(err)

	// [0xc0000b0780] <nil>
	fmt.Println(os.ReadDir("."))

	err = os.Chdir("../../..")
	check(err)

	// 对目录下每一个文件执行相同函数
	// subdir true
	// subdir/file1 false
	// subdir/parent true
	// subdir/parent/child true
	// subdir/parent/child/file4 false
	// subdir/parent/file2 false
	// subdir/parent/file3 false
	err = filepath.Walk("subdir", visit)
	check(err)
}

func visit(p string, info os.FileInfo, err error) error {
	if err != nil {
		return nil
	}

	fmt.Println(p, info.IsDir())
	return nil
}
