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

	url := `http://www.cnblogs.com/StudyLife/archive/2012/03/07/2384125.html`
	// filepath := GetCurrPath()

	filename := `./origin.html`

	if ok := GetPageToFile(url, filename); !ok {
		return
	}

}

func GetPageToFile(url string, filename string) (ok bool) {
	var f *os.File
	var err error
	ok = false
	s, statusCode := Get(url)

	if statusCode != 200 {
		fmt.Println(statusCode, "Url error")
		return
	}

	// if checkFileIsExist(filename) {

	// f, err = os.OpenFile(filename, os.O_APPEND, 0666) //打开文件
	// 	fmt.Println("File exited,the original file will be overwritten")
	// 	if err != nil {
	// 		fmt.Println("Failed to open file. 0_0", filename)
	// 		return
	// 	}
	// 	fmt.Println("File has been opened...")
	// } else {
	// 	f, err = os.Create(filename) //创建文件
	// 	fmt.Println("File not exit, now creating it...")
	// 	if err != nil {
	// 		fmt.Println("Failed to create file. 0_0", filename)
	// 		return
	// 	}
	// 	fmt.Println("File created, prepare writing...")
	// }

	if f, err = os.OpenFile(filename, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0666); err != nil {
		fmt.Println("Error:", err)
		return
	}

	defer f.Close()

	_, err = io.WriteString(f, s) //写入文件(字符串)

	if err != nil {
		fmt.Println("Error:", err)
		return
	}
	fmt.Println("Success! ^_^")
	ok = true
	return
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

// func checkFileIsExist(filename string) bool {
// 	var exist = true
// 	if _, err := os.Stat(filename); os.IsNotExist(err) {
// 		exist = false
// 	}
// 	return exist
// }

// func GetCurrPath() string {
// 	file, _ := exec.LookPath(os.Args[0])
// 	path, _ := filepath.Abs(file)
// 	splitstring := strings.Split(path, "\\")
// 	size := len(splitstring)
// 	splitstring = strings.Split(path, splitstring[size-1])
// 	ret := strings.Replace(splitstring[0], "\\", "/", size-1)
// 	return ret
// }
