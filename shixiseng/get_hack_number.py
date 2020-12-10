# -*- coding: utf-8 -*-

import requests
from bs4 import BeautifulSoup
headers = {"User-Agent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36"}

url = "https://www.shixiseng.com/intern/inn_7n2ebnftzk14?pcm=pc_SearchList"
req = requests.get(url,headers=headers)
html = req.text
soup = BeautifulSoup(html,"lxml")
job = soup.title.text.split("招聘")[0]
company_name = soup.select('.com_intro .com-name')[0].text.strip()
salary_text = soup.select(".job_money.cutom_font")[0].text
print("工作职位的名称为：{}\n薪金为：{}\n招聘公司的名称为：{}\n".format(job,salary_text,company_name))
print("该页面的薪金对应为:\n\n***-***/天 - 自己看网页")    # !!!!注意：这里的真实薪资要根据前面的url去网页上看
print(f"{salary_text}".encode("utf-8"))

# 各加密字符参考连接
# 0,1,2,5: https://www.shixiseng.com/intern/inn_ffkznvgyqjdu?pcm=pc_SearchList
# 3,4:     https://www.shixiseng.com/intern/inn_eutrcyb5hesz
# 6,8:     https://www.shixiseng.com/intern/inn_ekyvjpaonjys?pcm=pc_SearchList
# 7,8:     https://www.shixiseng.com/intern/inn_tej1xdw8pf5o?pcm=pc_SearchList
# 9:       https://www.shixiseng.com/intern/inn_7n2ebnftzk14?pcm=pc_SearchList


# 2020/12/07
def hack_number_20201207(text):
# 实习僧的薪资数据经过反扒处理（是其自定义的字体），下面是自定义的字体与数字的一一对应
	text = text.encode("utf-8")
	text = text.replace(b'\xee\xa1\x8c',b"0")  # updated 2020/12/07
	text = text.replace(b'\xef\x83\xbc',b"1")  # updated 2020/12/07
	text = text.replace(b'\xef\x8f\x87',b"2")  # updated 2020/12/07
	text = text.replace(b'\xef\x92\x84',b"3")  # updated 2020/12/07
	text = text.replace(b'\xef\x9b\xaf',b"4")  # updated 2020/12/07
	text = text.replace(b'\xef\x99\x92',b"5")  # updated 2020/12/07
	text = text.replace(b'\xee\x9d\xb8',b"6")  # updated 2020/12/07
	text = text.replace(b'\xee\xab\x82',b"7")  # updated 2020/12/07
	text = text.replace(b'\xef\xa2\xad',b"8")  # updated 2020/12/07
	text = text.replace(b'\xef\x89\xbc',b"9")  # updated 2020/12/07
	text = text.decode()
	
	return text


# 2020/12/06
def hack_number_20201206(text):
# 实习僧的薪资数据经过反扒处理（是其自定义的字体），下面是自定义的字体与数字的一一对应
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