package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func hello() {
	fmt.Println("Hello World")
	wg.Done()
}

func main10() {
	// wg.Add(1)
	// go hello()
	// fmt.Println("你好")
	// wg.Wait()

	// for i := 0; i < 5; i++ {
	// 	wg.Add(1)
	// 	go func() {
	// 		defer wg.Done()
	// 		fmt.Println(i)
	// 	}()
	// }
	// wg.Wait()

	// 创建一个无缓冲的channel
	// ch := make(chan int)
	ch := make(chan int, 1) // 有缓冲的channel，第二个参数表示缓冲区大小
	// go func() {
	// 	// 从channel中接收数据
	// 	num := <-ch
	// 	fmt.Println("接收到数据：", num)
	// }()
	ch <- 1
	fmt.Println("发送成功")
}

func f2(ch chan int) {
	for {
		v, ok := <-ch
		if !ok {
			fmt.Println("通道已关闭")
			break
		}
		fmt.Printf("v:%#v ok:%#v\n", v, ok)
	}
}

func main() {
	ch := make(chan int, 2)
	ch <- 1
	ch <- 2
	close(ch)
	f2(ch)
}
