package myfile

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
)

func TempDirFile() {
	f, err := ioutil.TempFile("", "sample")
	check(err)
	fmt.Println(f.Name()) // /tmp/sample938602552

	defer os.Remove(f.Name())

	dName, err := ioutil.TempDir("", "sample")
	check(err)
	defer os.RemoveAll(dName)

	fmt.Println(dName) // /tmp/sample382114871
	fileName := filepath.Join(dName, "file")
	d := []byte{0,1,11}
	ioutil.WriteFile(fileName, d, 0666)
}
