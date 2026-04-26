package main

import "fmt"

func funcA() {
	fmt.Println("func A")
}

func funcB() {
	defer func() {
		err := recover()
		if err != nil {
			fmt.Println("recover in B, err :", err)
		}
	}()

	panic("panic in B")
}

func funcC() {
	fmt.Println("func C")
}
func main4() {
	funcA()
	funcB()
	funcC()
}
