package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
)

func main() {

	rootUrl := `http://www.bnuoj.com`
	fileStorePath := `./`
	logdb := `./visitedurl`
	depth := 3

	if ok := GetPageToFile(depth, rootUrl, fileStorePath, logdb); !ok {
		return
	}

}

func GetHash(c string) (h string) {
	H := md5.New()
	io.WriteString(H, c)
	h = hex.EncodeToString(H.Sum(nil))
	return h
}

func GetPageToFile(dep int, url, FilePath, Logfile string) (ok bool) {

	if dep < 0 {
		// only crawl until depth has reached the bottom or zero
		ok = true
		return
	}

	var f, flog *os.File
	var err error
	ok = false

	content, statusCode := Get(url)
	contentstr := string(content)

	if statusCode != 200 {
		fmt.Println(statusCode, "Url error")
		return
	}
	// fmt.Println(statusCode)

	name := GetHash(contentstr)
	filename := FilePath + name + ".html"

	if f, err = os.OpenFile(filename, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0666); err != nil {
		fmt.Println("Error:", err)
		return
	}

	defer f.Close()

	_, err = io.WriteString(f, contentstr) //写入文件(字符串)

	if err != nil {
		fmt.Println("Error:", err)
		return
	}
	fmt.Println("Success! ^_^")
	ok = true

	if flog, err = os.OpenFile(Logfile, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0666); err != nil {
		fmt.Println(err)
		ok = false
		return
	}

	defer flog.Close()

	_, err = io.WriteString(flog, url+"\t"+name+"\n")

	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	return
}

func Get(url string) (data []byte, statusCode int) {
	var errs, error
	resp, errf := http.Get(url)
	if errf != nil {
		statusCode = -100
		fmt.Println(errf)
		return
	}

	defer resp.Body.Close()
	data, errs = ioutil.ReadAll(resp.Body)
	if errs != nil {
		statusCode = -200
		fmt.Println(errf)
		return
	}
	statusCode = resp.StatusCode
	// content = string(data)
	return
}

func ParseLinks() {

}
