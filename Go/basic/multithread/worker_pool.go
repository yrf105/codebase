package multithread

import (
	"log"
	"time"
)

func worker(id int, jobs <-chan int, results chan<- int) {
	for job := range jobs {
		log.Printf("worker_%v 开始工作", id)
		time.Sleep(time.Second)
		results <- (job * job)
		log.Printf("worker_%v 完成工作", id)
	}
}

func WorkerPool() {
	const count = 5
	jobs := make(chan int, count)
	results := make(chan int, count)

	for i := 0; i < 3; i++ {
		go worker(i + 1, jobs, results)
	}

	for i := 0; i < count; i++ {
		jobs <- i
	}
	log.Println("任务分发完毕")
	close(jobs)

	for i := 0; i < count; i++ {
		log.Println("result: ", <-results)
	}
}