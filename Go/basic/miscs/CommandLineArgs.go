package miscs

import (
	"fmt"
	"os"
)

func CommandLineArguments() {
	argWithProg := os.Args
	argWithoutProg := os.Args[1:]

	fmt.Println(argWithProg)
	fmt.Println(argWithoutProg)
}