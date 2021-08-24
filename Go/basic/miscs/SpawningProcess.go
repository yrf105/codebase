package miscs

import (
	"fmt"
	"io/ioutil"
	"os/exec"
)

// 如何在 go 中执行非 go 程序呢
func SpawnProcess() {
	dateCmd := exec.Command("date")
	dateOutput, err := dateCmd.Output()
	if err != nil {
		panic(err)
	}
	fmt.Println("> date")
	fmt.Println(string(dateOutput))

	grepCmd := exec.Command("grep", "hello")
	grepIn, _ := grepCmd.StdinPipe()
	grepOut, _ := grepCmd.StdoutPipe()
	grepCmd.Start()
	grepIn.Write([]byte("hello grep\nbye grep"))
	grepIn.Close()
	outBytes, _ := ioutil.ReadAll(grepOut)
	grepCmd.Wait() // 等待进程退出
	fmt.Println("> grep hello")
	fmt.Println(string(outBytes))

	lsCmd := exec.Command("bash", "-c", "ls -a -l -h")
	outBytes, _ = lsCmd.Output()
	fmt.Println("> ls -a -l -h")
	fmt.Println(string(outBytes))
}