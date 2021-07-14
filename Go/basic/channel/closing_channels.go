package channel

import "log"

func Closing() {
	jobs := make(chan int, 5)
	done := make(chan bool)

	go func() {
		for {
			job, more := <-jobs
			if more {
				log.Println("收到 job ", job)
			} else {
				log.Println("收到结束工作信号")
				done<-true
				return
			}
		}
	}()

	for i := 0; i < 3; i++ {
		jobs<-i
	}

	close(jobs)
	log.Println("全部工作结束")

	<-done
}