package main

import "fmt"

type Singer interface {
	sing()
}

type bird struct {
	name string
}

func (b bird) sing() {
	fmt.Println(b.name, "在唱歌")
}

// 定义一个函数，参数是接口类型
// 所有实现了该接口的类型的变量都可以作为参数传入该函数
func happy(s Singer) {
	s.sing()
}

func main7() {
	b := bird{name: "小鸟"}
	b.sing()
	happy(b)
}
