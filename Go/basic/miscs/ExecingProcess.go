package miscs

import (
	"os"
	"os/exec"
	"syscall"
)

func ExecProcess() {
	// 使用其他进程完全替换当前进程
	lsPath, _ := exec.LookPath("ls")
	args := []string{"ls", "-l", "-a", "-h"}

	env := os.Environ()

	syscall.Exec(lsPath, args, env)
}
