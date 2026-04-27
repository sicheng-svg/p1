package main

import (
	"fmt"
	"sync"
)

var (
	x  int64
	wg sync.WaitGroup // 等待组
	m  sync.Mutex
)

// add 对全局变量x执行5000次加1操作
func sum() {
	defer wg.Done()
	for i := 0; i < 5000; i++ {
		x = x + 1
	}
}

func main() {
	wg.Add(2)

	go sum()
	go sum()

	wg.Wait()
	fmt.Println(x)
}
