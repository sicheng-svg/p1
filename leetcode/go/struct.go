package main

import "fmt"

func test_type() {
	// type newType oldType 类型定义
	// type newType = oldType 类型别名
	type myInt int
	type yourInt = int

	var a myInt = 10
	var b yourInt = 20

	// a type:main.myInt, value:10
	// b type:int, value:20
	fmt.Printf("a type:%T, value:%v\n", a, a)
	fmt.Printf("b type:%T, value:%v\n", b, b)
}

// 定义结构体
type person struct {
	name string
	age  int8
}

func test_struct() {
	// 结构体的声明和初始化
	var p1 person
	p1.name = "Soren"
	p1.age = 21
	fmt.Printf("name:%v, age:%v\n", p1.name, p1.age)

	// 匿名结构体
	var p2 struct {
		name string
		age  int8
	}
	p2.name = "Alice"
	p2.age = 22
	fmt.Printf("name:%v, age:%v\n", p2.name, p2.age)
}

type student struct {
	name string
	age  int
}

func newStudent(name string, age int) *student {
	return &student{name: name, age: age}
}

func test_struct2() {
	m := make(map[string]*student)
	stus := []student{
		{name: "小王子", age: 18},
		{name: "娜扎", age: 23},
		{name: "大王八", age: 9000},
	}

	for _, stu := range stus {
		m[stu.name] = &stu
	}
	for k, v := range m {
		fmt.Println(k, "=>", v.name)
	}
}

type teacher struct {
	name string
	age  int
}

func newTeacher(name string, age int) *teacher {
	return &teacher{name: name, age: age}
}

// 值接收，用于读，修改变量不会影响外部变量
func (t teacher) getAge() {
	fmt.Println("teacher age:", t.age)
}

// 指针接收，用于写/拷贝代价大，修改变量会影响外部变量
func (t *teacher) setAge(age int) {
	t.age = age
}

func main6() {
	// test_type1()
	// test_struct()
	// test_struct2()
	// p := newStudent("Soren", 21)
	// fmt.Printf("name:%v, age:%v\n", p.name, p.age)

	t := newTeacher("Soren", 21)
	t.getAge()
	t.setAge(32)
	t.getAge()
	fmt.Println(t.age)
}
