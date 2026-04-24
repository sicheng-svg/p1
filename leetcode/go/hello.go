package main

import "fmt"

func main1() {
	fmt.Println("hello world")

	var name string
	var age int = 21
	name = "Soren"
	fmt.Println(name)
	fmt.Println(age)

	// 函数外的变量必须使用var显示声明，不能使用短变量声明。
	m := 100
	fmt.Println(m)

	// 定义常量时，需要将var 替换为 const
	const pi = 3.14
	fmt.Println(pi)

	// 定义枚举时，可以使用 iota 关键字来自动生成枚举值
	type Weekday int
	const (
		Sunday Weekday = iota // 0
		Monday
		Tuesday
		Wednesday
		Thursday
		Friday
		Saturday
	)
	fmt.Println(Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday)
}
