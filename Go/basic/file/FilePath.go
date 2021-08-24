package myfile

import (
	"fmt"
	"path/filepath"
)

func FilePath() {
	path := filepath.Join("..", "tmp", "tmp") // ../tmp/tmp
	fmt.Println(path)
	// 应该总是使用 Join 构造文件路径
	// 除了带来可移植性外，Join 还会纠正并优化路径表示
	path = filepath.Join("..", "tmp//", "tmp")
	fmt.Println(path) // ../tmp/tmp
	path = filepath.Join("../tmp/../file", "tmp")
	fmt.Println(path) // ../file/tmp

	path = "../tmp/file1.txt"
	path2 := ".."
	fmt.Println(filepath.Dir(path)) // 目录名
	fmt.Println(filepath.Base(path)) // 文件名
	fmt.Println(filepath.Ext(filepath.Base(path))) // 文件扩展名
	fmt.Print(filepath.Rel(path, path2)) // 文件路径之间的关系
}
