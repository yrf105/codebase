package main

import (
	"fmt"
	"time"
)

// import "github.com/yrf/codebase/Go/basic/multithread"
// import "github.com/yrf/codebase/Go/basic/file"
// import "github.com/yrf/codebase/Go/basic/miscs"
// import "github.com/yrf/codebase/Go/basic/NET"

func main() {
	// multithread.AtomicCounter()
	// myfile.LineFilter()
	// miscs.CommandLineArguments()
	// miscs.CMFlags()
	// my_net.HttpServer()
	// my_net.HttpContext()
	// miscs.HandleSig()

	foo()
	fmt.Println("hello")
}

func foo() {
	defer time.Sleep(5 * time.Second)
}
