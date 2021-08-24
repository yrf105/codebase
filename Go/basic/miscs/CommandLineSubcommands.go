package miscs

import (
	"flag"
	"fmt"
	"os"
)

// 需要先 go build

func SubCommands() {
	fooCMD := flag.NewFlagSet("foo", flag.ExitOnError)
	intFoo := fooCMD.Int("int", 19, "a foo int")
	boolFoo := fooCMD.Bool("bool", false, "a foo bool");

	barCMD := flag.NewFlagSet("bar", flag.ExitOnError)
	initBar := barCMD.Int("int", 18, "a bar int")
	strBar := barCMD.String("String", "cc", "a bar string")

	if len(os.Args) < 2 {
		fmt.Println("请输入子命令：foo 或者 bar")
		os.Exit(1)
	}

	switch os.Args[1] {
	case "foo":
		fmt.Println(*intFoo)
		fmt.Println(*boolFoo)
	case "bar":
		fmt.Println(*initBar)
		fmt.Println(*strBar)
	default:
		fmt.Println("请输入子命令：foo 或者 bar")
		os.Exit(1)
	}
}