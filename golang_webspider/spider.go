package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
)

func main() {
	var f *os.File
	var err error
	url := `http://www.oschina.net/p/go_spider`
	// filepath := GetCurrPath()

	s, statusCode := Get(url)

	if statusCode != 200 {
		fmt.Println(statusCode, "Link error")
		return
	}

	filename := `./origin.html`

	if checkFileIsExist(filename) {
		f, err = os.OpenFile(filename, os.O_APPEND, 0666) //打开文件
		fmt.Println("File exited")
		if err != nil {
			fmt.Println("Failed to open file==", filename)
			return
		}
		fmt.Println("File opened...")
	} else {
		f, err = os.Create(filename) //创建文件
		fmt.Println("File not exit")
		if err != nil {
			fmt.Println("Failed to created file==", filename)
			return
		}
		fmt.Println("File prepared...")
	}
	defer f.Close()

	_, err = io.WriteString(f, s) //写入文件(字符串)

	if err != nil {
		fmt.Println("Failed writing file")
		return
	}
	fmt.Println("Success")

}

func Get(url string) (content string, statusCode int) {
	resp, errf := http.Get(url)
	if errf != nil {
		statusCode = -100
		fmt.Println(errf)
		return
	}

	defer resp.Body.Close()
	data, errs := ioutil.ReadAll(resp.Body)
	if errs != nil {
		statusCode = -200
		fmt.Println(errf)
		return
	}
	statusCode = resp.StatusCode
	content = string(data)
	return
}

func checkFileIsExist(filename string) bool {
	var exist = true
	if _, err := os.Stat(filename); os.IsNotExist(err) {
		exist = false
	}
	return exist
}

func GetCurrPath() string {
	file, _ := exec.LookPath(os.Args[0])
	path, _ := filepath.Abs(file)
	splitstring := strings.Split(path, "\\")
	size := len(splitstring)
	splitstring = strings.Split(path, splitstring[size-1])
	ret := strings.Replace(splitstring[0], "\\", "/", size-1)
	return ret
}

// var wireteString = "测试n"
// var filename = "./output1.txt";
// var f    *os.File
// var err1   error;
// /***************************** 第一种方式: 使用 io.WriteString 写入文件 ***********************************************/
// if checkFileIsExist(filename) {  //如果文件存在
//  f, err1 = os.OpenFile(filename, os.O_APPEND, 0666)  //打开文件
//  fmt.Println("文件存在");
// }else {
//  f, err1 = os.Create(filename)  //创建文件
//  fmt.Println("文件不存在");
// }
// check(err1)
// n, err1 := io.WriteString(f, wireteString) //写入文件(字符串)
// check(err1)
// fmt.Printf("写入 %d 个字节n", n);
