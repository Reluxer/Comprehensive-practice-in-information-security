package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"regexp"
)

type URLMeta struct {
	extension string
	URL       string
}

func main() {

	rootUrl := `https://www.baidu.com/`
	depth := 2

	GetPageToFile(rootUrl, depth)
}

func GetHash(c string) (h string) {
	H := md5.New()
	io.WriteString(H, c)
	h = hex.EncodeToString(H.Sum(nil))
	return h
}

func GetPageToFile(url string, dep int) (ok bool) {

	if dep < 0 {
		// only crawl until depth has reached the bottom or zero
		ok = true
		return
	}

	contentbyte, statusCode := Get(url)
	contentstr := string(contentbyte)

	if statusCode != 200 {
		fmt.Println(statusCode, url)
		return
	}
	// fmt.Println(statusCode)

	fmt.Println("depth", dep, "Hit url:", url)

	fileStorePath := `./`
	logdb := `./visitedurl`

	ok = str2file(contentstr, fileStorePath, logdb, url)

	if !ok {
		return ok
	}

	ParseUrls(contentbyte, dep)

	ok = true
	return ok
}

func str2file(c string, fp, logfile, url string) (ok bool) {
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

	_, err = io.WriteString(f, c) //写入文件(字符串)

	if err != nil {
		fmt.Println("Error:", err)
		return ok
	}

	// fmt.Println("Success! ^_^")

	if flog, err = os.OpenFile(logfile, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0666); err != nil {
		fmt.Println(err)
		return ok
	}

	defer flog.Close()

	_, err = io.WriteString(flog, url+"\t"+filename+"\n")

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
	// content = string(data)
	return data, statusCode
}

func ParseUrls(bytes []byte, depth int) {
	urlRx, err := regexp.Compile("https?://[[:alpha:]]+([.]?[[:alpha:]]+)*?(/+[[:alpha:]]+)+(/|(\\.[[:alpha:]]+))?")
	if err != nil {
		return
	}
	urls := urlRx.FindAll(bytes, -1)
	urlCount := len(urls)

	// create a channel to send all of the urls we encounter
	urlChan := make(chan []byte, 10000)
	for idx := 0; idx < urlCount; idx++ {
		urlChan <- urls[idx]
	}
	close(urlChan)
	urlpoor := getURLs(urlChan)

	for m := range urlpoor {
		GetPageToFile(m, depth-1)
	}
	// URLMetaPrinter(urlpoor, depth)
}

func getURLs(urls <-chan []byte) <-chan string {
	out := make(chan string)
	// rx, err := regexp.Compile("(?:\\.)[[:alpha:]]+$")
	// if err != nil {
	// 	return out
	// }
	go func() {
		for url := range urls {
			// for each url try to grab the extension
			// match := rx.Find(url)
			// mt := URLMeta{URL: string(url[:]), extension: string(match[:])}
			out <- string(url[:])
		}
		close(out)
	}()
	return out
}

// func URLMetaPrinter(um <-chan string, depth int) {
// 	for m := range um {
// 		// if m.extension != "" {
// 		// 	fmt.Println("depth:", depth, "URL:", m.URL, "extension:", m.extension)
// 		// } else {
// 		// 	fmt.Println("depth:", depth, "URL:", m.URL)
// 		// }
// 		// recursively crawl found urls
// 		GetPageToFile(m.URL, depth-1)
// 	}
// }
