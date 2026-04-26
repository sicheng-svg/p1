package main

import "fmt"

func f1() int {
	x := 5
	defer func() {
		x++
	}()
	return x
}

func ff2() (x int) {
	defer func() {
		x++
	}()
	return 5
}

func f3() (y int) {
	x := 5
	defer func() {
		x++
	}()
	return x
}
func f4() (x int) {
	defer func(x int) {
		x++
	}(x)
	return 5
}
func main3() {
	fmt.Println(f1())  // 6
	fmt.Println(ff2()) // 5
	fmt.Println(f3())  // 报错？
	fmt.Println(f4())  // 8
}
