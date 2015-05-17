## Notes

- 需要golang的编译器的支持，下载请[戳这里](http://golangtc.com/download)
- 运行`go build spider.go`生成可执行文件`spider.exe`(Win),或者`spider`(Linux),运行即可
- 程序支持命令行参数，`-u url`表示爬取网址为url的页面，默认为`http://www.hzic.edu.cn/`，`-d depth`，表示爬取深度为depth，默认为`3`
- 未完待续



## To do

1. 解决重定向
2. 程序繁冗，需优化
3. 爬取百度首页