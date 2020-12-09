## 实习僧爬虫与自动推送项目 

### shixiseng_crawler

这段代码参考了CSDN用户Be_melting的文章，在此之上我做了大量修改，算是练习

1.原文的方法只爬取了工作名称、薪资、公司这三个信息，我尝试使用另一种方法爬取更多信息

2.修复requests库请求url时可能出现的服务器负载过重，无法建立更多的链接的问题('Max retries exceeded with url')

3.将爬取的数据保存到MySQL数据库，同时保存一份csv文件，以便做一些分析

4.将函数封装，以便进行后续的最新实习岗位自动推送项目

5.改进一些原本会报错使得程序终止的地方

原文地址：https://blog.csdn.net/lys_828/article/details/104162681
