package main

import (
	"fmt"
	"reflect"
)

func main8() {
	x := 10
	t := reflect.TypeOf(x)
	v := reflect.ValueOf(x)
	fmt.Println("type:", t, "value:", v)
}
