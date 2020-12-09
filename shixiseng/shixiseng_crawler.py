# -*- coding: utf-8 -*-
"""
Created on Tue Dec  8 13:59:21 2020

@author: Zhou Jiahao
"""

# 这段代码参考了CSDN用户Be_melting的文章，在此之上我做了以下修改，算是练习
# 1.原文的方法只爬取了工作名称、薪资、公司这三个信息，我尝试使用另一种方法爬取更多信息
# 2.修复requests库请求url时可能出现的服务器负载过重，无法建立更多的链接的问题('Max retries exceeded with url')
# 3.将爬取的数据保存到MySQL数据库，同时保存一份csv文件，以便做一些分析
# 4.将函数封装，以便进行后续的最新实习岗位自动推送项目
# 5.改进一些原本会报错使得程序终止的地方
# 原文地址：https://blog.csdn.net/lys_828/article/details/104162681


#安装第三方库
#pip install requests
#pip install bs4  # beautifulsoup在bs4安装包中

#Import
import requests
import random
import pymysql
import csv
from bs4 import BeautifulSoup

class shixiseng_crawler():
    def __init__(self, city, keyword):
        db = pymysql.connect(host="你的主机",database="你新创建的模式名",user="你的用户名",password="你的密码",port=3306,charset='utf8')  #需要你先创建一个新的模式(SCHEMA)
        self.city = city
        self.keyword = keyword
        self.cur = db.cursor()
        self.db = db
        self.count = 0  # 所获岗位计数
        requests.adapters.DEFAULT_RETRIES = 5  # 增加重试连接次数

    def run(self):
        #发送请求头信息
        headers = self.get_headers()
        #每个循环即为爬取一页的数据
        #对于不同关键词，网页提供的最大页数是不同的
        #下面输入的需爬页数⬇超过最大页数的部分不会写入任何信息进csv，但也不会报错
        for i in range(1,2):
            #获取职位列表URL中的网页数据
            req = requests.get(f'https://www.shixiseng.com/interns?keyword={self.keyword}&page={i}&city={self.city}&type=intern',
    			headers = headers)
            html = req.text
            #用beautifulSoup处理相关的网页数据
            soup = BeautifulSoup(html,'lxml')
            #定位网页中职位一栏对应的标签信息
            offers = soup.select('.intern-wrap.intern-item')
            
            for offer in offers:
                #URL是在'.intern-wrap.intern-item'标签的孙子class标签下面，因此可以使用“ .f-l.intern-detail__job a”方式访问
                url = offer.select(" .f-l.intern-detail__job a")[0]['href']
                #职位详情URL中的有用数据提取出来并保存
                self.save_job_details(url,i)
            print("-"*25, "第%d页写入完毕" %(i))
            
        # 关闭数据库
        self.db.close()

    def save_job_details(self, url, i):
        #发送请求头信息
        headers = self.get_headers()
        #获取一个职位详情URL中的网页数据
        html = requests.get(url,headers=headers).text
        #用beautifulSoup处理相关的网页数据
        soup = BeautifulSoup(html, 'lxml')
    	
        try:
            #职位名称
            positionName = soup.select('.new_job_name')[0].text.strip()
            #薪资
            salary = self.hack_number(soup.select('.job_money')[0].text.strip())
            #公司名称
            company = soup.select('.com_intro .com-name')[0].text.strip()
            #地点
            area = soup.select('.job_position')[0].text.strip()
            #学历要求
            education = soup.select('.job_academic')[0].text.strip()
            #岗位职责与要求
            jobDescription = soup.select('.job_detail')[0].text.strip()
            #每周实习天数
            dayPerWeek = self.hack_number(soup.select('.job_week')[0].text.strip())
            #可实习时长 - 如果爬到没有给出时长要求的岗位会报错，比如"校园大使"岗位
            duration = self.hack_number(soup.select('.job_time')[0].text.strip())
            #公司的信息
            companyDetails = self.hack_number(soup.select(".com-detail")[0].text.strip().split('\n\n')[2])
            
            try:
                #保存到csv文件 - 把 URL也附上方便之后查看详情
                #self.write_csv(["第%d页" %(i),positionName,salary,company,area,education,dayPerWeek,duration,jobDescription,companyDetails,url])
                
                #保存到MySQL数据库
                sqlcmd = "insert into shixiseng \
                          (positionName,salary,company,area,education,dayPerWeek,duration,jobDescription,companyDetails,url) \
                          values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')" \
                          % (positionName,salary,company,area,education,dayPerWeek,duration,jobDescription,companyDetails,url)
                self.cur.execute(sqlcmd)
                self.db.commit()
                
                self.count += 1
                print('第%d个岗位信息写入成功' % self.count)
            
            except Exception as e:
                print('文件写入出错，错误信息为：%s' % e)
            
        except Exception as e:
                print('*'*25, "有一个岗位获取内容时出错，错误信息为：%s" % e)
                print('*'*25, '出错链接为：%s' % url)     

    # 实习僧的薪资数据经过反扒处理（是其自定义的字体），下面是自定义的字体与数字的一一对应
    # 加密字符一般一两天更新一次，本代码没有自动更新加密字符功能，需要用get_hack_number.py文件手动更新
    def hack_number(self, text):
    	text = text.encode("utf-8")
    	text = text.replace(b'\xef\x87\xba',b"0")  # updated 2020/12/06
    	text = text.replace(b'\xee\x87\xa9',b"1")  # updated 2020/12/06
    	text = text.replace(b'\xef\x93\xb5',b"2")  # updated 2020/12/06
    	text = text.replace(b'\xef\x92\xb9',b"3")  # updated 2020/12/06
    	text = text.replace(b'\xee\xb7\xa2',b"4")  # updated 2020/12/06
    	text = text.replace(b'\xee\xb9\xa5',b"5")  # updated 2020/12/06
    	text = text.replace(b'\xee\x9d\x9d',b"6")  # updated 2020/12/06
    	text = text.replace(b'\xee\xa9\xbc',b"7")  # updated 2020/12/06
    	text = text.replace(b'\xef\x9d\x84',b"8")  # updated 2020/12/06
    	text = text.replace(b'\xee\x88\xb6',b"9")  # updated 2020/12/06
    	text = text.decode()
    	
    	return text

    # 随机获得一个headers
    def get_headers(self):
            ua_list = [
                'Mozilla/5.0(Macintosh;U;IntelMacOSX10_6_8;en-us)AppleWebKit/534.50(KHTML,likeGecko)Version/5.1Safari/534.50',
                'Mozilla/5.0(Windows;U;WindowsNT6.1;en-us)AppleWebKit/534.50(KHTML,likeGecko)Version/5.1Safari/534.50',
                'Mozilla/5.0(compatible;MSIE9.0;WindowsNT6.1;Trident/5.0',
                'Mozilla/4.0(compatible;MSIE8.0;WindowsNT6.0;Trident/4.0)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT6.0)',
                'Mozilla/4.0(compatible;MSIE6.0;WindowsNT5.1)',
                'Mozilla/5.0(Macintosh;IntelMacOSX10.6;rv:2.0.1)Gecko/20100101Firefox/4.0.1',
                'Mozilla/5.0(WindowsNT6.1;rv:2.0.1)Gecko/20100101Firefox/4.0.1',
                'Opera/9.80(Macintosh;IntelMacOSX10.6.8;U;en)Presto/2.8.131Version/11.11',
                'Opera/9.80(WindowsNT6.1;U;en)Presto/2.8.131Version/11.11',
                'Mozilla/5.0(Macintosh;IntelMacOSX10_7_0)AppleWebKit/535.11(KHTML,likeGecko)Chrome/17.0.963.56Safari/535.11',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1;Maxthon2.0)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1;TencentTraveler4.0)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1;TheWorld)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1;Trident/4.0;SE2.XMetaSr1.0;SE2.XMetaSr1.0;.NETCLR2.0.50727;SE2.XMetaSr1.0)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1;360SE)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1;AvantBrowser)',
                'Mozilla/4.0(compatible;MSIE7.0;WindowsNT5.1)'
            ]
            ua = random.choice(ua_list)
            headers = {"User-Agent": ua}
    
            return headers

    def write_csv(self, data):
        with open('./shixiseng_pisitions.csv', 'a', encoding='utf-8', newline='') as f:
            #引入csv
            writer = csv.writer(f)
            #写入csv
            writer.writerow(data)
            #print("写入成功")


if __name__ == '__main__':

    crawler = shixiseng_crawler(city='上海', keyword='')
    crawler.run()
    print('数据采集完成')