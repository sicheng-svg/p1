package main

import (
	"flag"
	"fmt"
	"os"
	"unicode/utf8"
)

func print() {
	// fmt.Print() 直接输出内容
	// fmt.Printf() 格式化输出
	// fmt.Println() 输出内容并打印换行
	fmt.Print("Hello Golang")
	fmt.Printf("Hello %s\n", "Golang")
	fmt.Println("Hello Golang")

	//Fprint 系列函数可以讲内容输出到指定io流中。stdout、stderr，文件等
	fmt.Fprintln(os.Stdout, "向标准输出写入内容")
	fileObj, err := os.OpenFile("./xx.txt", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0644)
	if err != nil {
		fmt.Println("打开文件出错，err:", err)
		return
	}
	name := "沙河小王子"
	// 向打开的文件句柄中写入内容
	fmt.Fprintf(fileObj, "往文件中写如信息：%s", name)

	// Sprint系列函数内容进行格式化，返回一个string
	s1 := fmt.Sprint("Hello Golang")
	title := "沙河小王子"
	age := 21
	s2 := fmt.Sprintf("title:%s, age:%d", title, age)
	s3 := fmt.Sprintln("Hello Golang")
	fmt.Println(s1, s2, s3)
}

func scan() {
	// fmt.Scan从标准输入获取输入，以空白格作为结束标志，换行视为空白格
	var (
		name  string
		age   int
		adult bool
	)
	fmt.Println("请输入姓名、年龄、是否成年（true/false）:")
	// fmt.Scan(&name, &age, &adult)
	fmt.Println(name, age, adult)

	// 使用fmt.Scanf时，输入时要和格式化字符串的格式一致，否则会扫描失败
	fmt.Scanf("1:%s 2:%d 3:%t", &name, &age, &adult)
	fmt.Printf("扫描结果 name:%s age:%d married:%t \n", name, age, adult)

	// fmt.Scanln从标准输入获取输入，以换行符作为结束标志
}

func arr() {
	// 定义数组 var 数组名 [长度]类型
	var arr1 [5]int   // 不指定初始化值时，使用该类型的默认值
	fmt.Println(arr1) // [0 0 0 0 0]

	// 使用指定值进行初始化
	var arr2 = [3]int{1, 2}
	fmt.Println(arr2)

	var arr3 = [5]string{"Go", "Python", "Java", "C++"}
	fmt.Println(arr3)

	// 使用[...]让编译器根据初始化值的数量自动推断数组长度
	var arr4 = [...]float64{3.14, 2.718, 1.618}
	fmt.Println(arr4)

	fmt.Printf("arr1 的类型是: %T\n", arr1)
	fmt.Printf("arr2 的类型是: %T\n", arr2)
	fmt.Printf("arr3 的类型是: %T\n", arr3)
	fmt.Printf("arr4 的类型是: %T\n", arr4)

	// 指定索引初始化
	var arr5 = [5]int{0: 10, 2: 20, 4: 30}
	fmt.Println(arr5) // [10 0 20 0 30]

	// 数组的遍历
	fmt.Println("\n--- 数组遍历 ---")
	for i := 0; i < len(arr2); i++ {
		fmt.Printf("arr2[%d] = %d\n", i, arr2[i])
	}

	fmt.Println("\n--- 使用 range 遍历 ---")
	for i, v := range arr3 {
		fmt.Printf("arr3[%d] = %s\n", i, v)
	}

	// 二维数组
	a := [3][2]string{
		{"北京", "上海"},
		{"广州", "深圳"},
		{"成都", "重庆"},
	}
	fmt.Println(a)       //[[北京 上海] [广州 深圳] [成都 重庆]]
	fmt.Println(a[2][1]) //支持索引取值:重庆

	// 二维数组的遍历
	for _, v1 := range a {
		for _, v2 := range v1 {
			fmt.Printf("%s ", v2)
		}
	}

	// 数组是值类型，赋值和传参会复制整个数组。因此改变副本的值，不会改变本身的值。
}

func test_string() {
	s := "Go 语言 🚀"

	fmt.Println("原字符串:", s)
	fmt.Println("字节长度 len(s):", len(s))
	fmt.Println("字符数 RuneCount:", utf8.RuneCountInString(s))

	fmt.Println("\n--- byte 遍历（按字节）---")
	for i := 0; i < len(s); i++ {
		fmt.Printf("  [%2d] %3d  %x\n", i, s[i], s[i])
	}

	fmt.Println("\n--- rune 遍历（按字符）---")
	for i, r := range s {
		fmt.Printf("  字节位置 %d, 字符 '%c', Unicode %U\n", i, r, r)
	}

	fmt.Println("\n--- 切片对比 ---")
	fmt.Println("byte 切片 s[3:6]:", s[3:6], "（这是 '语' 的 3 个字节）")
	rs := []rune(s)
	fmt.Println("rune 切片 rs[3:5]:", string(rs[3:5]), "（这是 '语言' 2 个字符）")
}

func slice() {
	// 切片（Slice）是一个拥有相同类型元素的可变长度的序列。它是基于数组类型做的一层封装。它非常灵活，支持自动扩容。
	// var a []string              // 声明一个字符串切片
	// var b = []int{}             // 声明并初始化一个整形切片
	// var c = []bool{true, false} // 声明并初始化一个布尔型切片
	// var d = []bool{false, true} // 声明并初始化一个布尔型切片
	// fmt.Println(a)
	// fmt.Println(b)
	// fmt.Println(c)
	// fmt.Println(d)

	// fmt.Println(a == nil)
	// fmt.Println(b == nil)

	// // 借助len() 和 cap() 函数可以获取切片的长度和容量
	// fmt.Println(len(a))
	// fmt.Println(cap(a))

	a := []int{1, 2, 3, 4, 5}
	// 简单切片
	s := a[1:3] // 左闭右开
	fmt.Println(s)
	fmt.Println(len(s), cap(s))

	// 完整切片
	// 完整切片容量等于 max - low
	s2 := a[1:3:4] // 左闭右开，第三个参数指定切片的容量上限
	fmt.Println(s2)
	fmt.Println(len(s2), cap(s2))

	// 使用make构建切片
	// make([]T, len, cap) 构建一个元素类型为T，长度为len，容量为cap的切片
	s3 := make([]int, 5, 10)
	fmt.Println(s3)
	fmt.Println(len(s3), cap(s3))

	// 判断一个切片是否为空，要用len() == 0来判断。不能为nil判断，因为长度和容量为0不能推出切片一定是空的
	s4 := []int{} // len == 0 cap == 0 但不为nil
	fmt.Println(len(s4) == 0)
	fmt.Println(s4 == nil)

	// 切片的拷贝赋值，两者会共享底层的数组
	s5 := []int{1, 2, 3}
	s6 := s5
	s6[0] = 100
	fmt.Println(s5) // [100 2 3]
	fmt.Println(s6) // [100 2 3]

	// 切片的遍历
	// 索引遍历
	for i := 0; i < len(s5); i++ {
		fmt.Printf("s5[%d] = %d\n", i, s5[i])
	}

	for idx, val := range s5 {
		fmt.Printf("s5[%d] = %d\n", idx, val)
	}

	// 切片扩容
	var numSlice []int
	for i := 0; i < 10; i++ {
		numSlice = append(numSlice, i)
		fmt.Printf("%v  len:%d  cap:%d  ptr:%p\n", numSlice, len(numSlice), cap(numSlice), numSlice)
	}

	var a1 = make([]string, 5, 10)
	for i := 0; i < 10; i++ {
		// a1 = append(a, fmt.Sprintf("%v", i))
	}
	fmt.Println(a1)
}

func test_flag() {
	// 使用flag包解析的命令行参数都是指针
	name := flag.String("name", "张三", "姓名")
	age := flag.Int("age", 18, "年龄")
	married := flag.Bool("married", false, "婚否")
	delay := flag.Duration("delay", 0, "时间间隔")

	flag.Parse()
	fmt.Println("命令行参数解析前：", *name, *age, *married, *delay)
}

func test_map() {
	// 使用make初始化map时，还有一个可选的参数cap
	person := make(map[string]int)
	fmt.Println(person)

	person["soren"] = 21
	person["alice"] = 22
	fmt.Println(person)

	// map也支持在声明时填充元素
	userInfo := map[string]string{
		"name":     "Soren",
		"password": "123",
	}
	fmt.Println(userInfo)

	// 判断某个键是否存在
	if value, ok := userInfo["name"]; ok {
		fmt.Println("name存在，值为:", value)
	} else {
		fmt.Println("name不存在")
	}

	// map的遍历
	for key, value := range person {
		fmt.Printf("key:%s value:%d\n", key, value)
	}

	// 使用delete 删除键值对
	delete(person, "soren")
	fmt.Println(person)

}

// 可变参数
// 可变参数通常需要作为函数的最后一个参数
func intsum(x ...int) int {
	fmt.Println(x)
	sum := 0
	for _, v := range x {
		sum += v
	}
	return sum
}

// return 多个值
// 可以给返回值命名，命名后，直接return就可以。会自动将对应名字的变量返回
func returnMore(x int, y ...string) (int, string) {
	return x, y[0]
}

// 定义函数类型
// 定义了一个calculation类型，它是一个函数类型，表示接受两个int参数并返回一个int结果的函数。
// 满足calculation的参数和返回值的函数，都是该类型，可以赋值给该类型的对象
type calculation func(int, int) int

func add(x, y int) int {
	return x + y
}

func test_type() {
	var calc calculation
	calc = add
	result := calc(10, 20)
	fmt.Println(result)
}

func main() {
	// test_string()
	// arr()
	// print()
	// scan()
	// slice()
	// test_flag()
	// test_map()
	// fmt.Println(intsum(1, 2, 3, 4, 5))
	// fmt.Println(intsum(10, 20))
	test_type()
}
