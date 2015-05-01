package main

import (
	"crypto/md5"
	"encoding/hex"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"regexp"
)

func main() {
	var rooturl string
	var depth int
	flag.StringVar(&rooturl, "u", "http://www.hzic.edu.cn/", "the url of firstpage")
	flag.IntVar(&depth, "d", 3, "the depth of spider")
	flag.Parse()
	GetPageToFile(rooturl, depth)
}

func GetHash(c string) (h string) {
	H := md5.New()
	io.WriteString(H, c)
	h = hex.EncodeToString(H.Sum(nil))
	return h
}

func GetPageToFile(url string, dep int) (ok bool) {
	fileStorePath := `./`
	logdb := `./visitedurl`
	if dep < 0 {
		ok = true
		return
	}
	contentbyte, statusCode := Get(url)
	contentstr := string(contentbyte)
	if statusCode != 200 {
		fmt.Println(statusCode, url)
		return
	}
	fmt.Println("depth", dep, "Hit url:", url)
	ok = str2file(contentstr, fileStorePath, logdb, url, dep)
	if !ok {
		return ok
	}
	ParseUrls(contentbyte, dep)
	ok = true
	return ok
}

func str2file(c, fp, logfile, url string, d int) (ok bool) {
	var f, flog *os.File
	var err error
	rx, err1 := regexp.Compile("(?:\\.)[[:alpha:]]+$")
	if err1 != nil {
		return ok
	}
	mx := rx.FindString(url)
	fmt.Println("url:", url, "extenion:", mx)
	name := GetHash(c)
	filename := fp + name + mx
	if checkFileIsExist(filename) {
		ok = true
		return ok
	}
	if f, err = os.OpenFile(filename, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0666); err != nil {
		fmt.Println("Error:", err)
		return ok
	}
	defer f.Close()
	_, err = io.WriteString(f, c)
	if err != nil {
		fmt.Println("Error:", err)
		return ok
	}
	if flog, err = os.OpenFile(logfile, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0666); err != nil {
		fmt.Println(err)
		return ok
	}
	defer flog.Close()
	_, err = io.WriteString(flog, "depth "+string(d)+url+"\t"+filename+"\n")

	if err != nil {
		fmt.Println("Error:", err)
		return ok
	}
	ok = true
	fmt.Println("Stored url:", url)
	return ok
}

func checkFileIsExist(filename string) bool {
	var exist = true
	if _, err := os.Stat(filename); os.IsNotExist(err) {
		exist = false
	}
	return exist
}

func Get(url string) (data []byte, statusCode int) {
	var errs error
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
	return data, statusCode
}

func ParseUrls(bytes []byte, depth int) {
	urlRx, err := regexp.Compile("https?://[[:alpha:]]+([.]?[[:alpha:]]+)*?(/+[[:alpha:]]+)+(/|(\\.[[:alpha:]]+))?")
	if err != nil {
		return
	}
	urls := urlRx.FindAll(bytes, -1)
	urlCount := len(urls)

	urlChan := make(chan []byte, 10000)
	for idx := 0; idx < urlCount; idx++ {
		urlChan <- urls[idx]
	}
	close(urlChan)
	urlpoor := getURLs(urlChan)

	for m := range urlpoor {
		GetPageToFile(m, depth-1)
	}
}

func getURLs(urls <-chan []byte) <-chan string {
	out := make(chan string)

	go func() {
		for url := range urls {
			out <- string(url[:])
		}
		close(out)
	}()
	return out
}
