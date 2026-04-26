package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
)

func openFile() {
	// 借助os.Open打开一个文件，返回一个*File和err。os.Close方法可以关闭文件。通常将关闭语句注册为defer
	file, err := os.Open("xx.txt")
	if err != nil {
		fmt.Println("open file error :", err)
		return
	}
	defer file.Close()
}

func ReadFile() {
	// func (f *File) Read(b []byte) (n int, err error)
	// 它接收一个字节切片，返回读取的字节数和可能的具体错误，读到文件末尾时会返回0和io.EOF
	file, err := os.Open("xx.txt")
	if err != nil {
		fmt.Println("open file error :", err)
		return
	}
	defer file.Close()

	// 单次读取128字节数据
	var tmp = make([]byte, 128)
	n, err := file.Read(tmp)
	if err == io.EOF {
		fmt.Println("文件读完了")
		return
	}
	if err != nil {
		fmt.Println("read file failed, err:", err)
		return
	}
	fmt.Printf("读取了%d字节数据\n", n)
	fmt.Println(string(tmp[:n]))
}

func ReadFile2() {
	// 通过循环读取文件内容，直到读到文件末尾
	file, err := os.Open("xx.txt")
	if err != nil {
		fmt.Println("open file error :", err)
		return
	}
	defer file.Close()

	var content []byte
	var tmp = make([]byte, 128)
	for {
		n, err := file.Read(tmp)
		if err == io.EOF {
			fmt.Println("文件读完了")
			break
		}
		if err != nil {
			fmt.Println("read file failed, err:", err)
			return
		}
		content = append(content, tmp[:n]...) // ...表示插入的是一个切片
	}
	fmt.Println(string(content))
}

func readWholeFile() {
	content, err := os.ReadFile("xx.txt")
	if err != nil {
		fmt.Println("read file failed, err:", err)
		return
	}
	fmt.Println(string(content))
}

func test_bufio() {
	file, err := os.Open("xx.txt")
	if err != nil {
		fmt.Println("open file failed, err:", err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file) // MaxScanTokenSize = 64 * 1024
	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		fmt.Println("read file failed, err:", err)
	}
}

func test_OpenFile() {
	// os.OpenFile函数可以指定文件的打开模式和权限
	// func OpenFile(name string, flag int, perm FileMode) (*File, error)
	file, err := os.OpenFile("xx.txt", os.O_APPEND|os.O_WRONLY, 0666)
	if err != nil {
		fmt.Println("open file failed, err:", err)
		return
	}
	defer file.Close()

	str := "hello, world\n"
	file.Write([]byte(str))
	file.WriteString(str)
}

func main5() {
	// openFile()
	// ReadFile()
	// ReadFile2()
	// test_bufio()
	// readWholeFile()
	test_OpenFile()
}
