package main

import (
	"crypto/md5"
	"encoding/hex"
	// "flag"
	"fmt"
	"io"
	// "os"
)

func main() {
	h := md5.New()
	// io.WriteString(h, "hfjsahfdjsf")
	io.WriteString(h, "The fog is getting thicker!")
	io.WriteString(h, "And Leon's getting laaarger!")
	fmt.Println(hex.EncodeToString(h.Sum(nil)))
}
