# -*- coding: utf-8 -*-

import smtplib
from email.mime.text import MIMEText
import datetime
import re

def sendEmail(data):

    # 第三方 SMTP 服务
    mail_host = "smtp.126.com"      # SMTP服务器
    mail_user = "********@126.com"  # 用户名
    mail_pass = "****************"  # 授权密码，注意不是你的邮箱登录密码

    sender = '********@126.com'        # 发件人邮箱
    receivers = ['********@sina.com']  # 接收邮箱

    # 邮件主体:岗位信息表 (DataFrame --> HTML)
    content = data.to_html(escape=False,index=False)

    # URL太长了 改成一个超链接
    url_list = re.findall('<td>(http.*?)</td>',content)
    
    for url in url_list:
        rep_old = "<td>{}</td>".format(url)
        rep_new = """<td><a href="{}" width=25> __link__ </td>""".format(url)
        content = content.replace(rep_old, rep_new)
 
    # 邮件细节
    title = str(datetime.date.today()) + ' 新增职位 ' + str(data.shape[0]) + ' 个' # 邮件主题
    message = MIMEText(content, 'html', 'utf-8')  # 内容, 格式, 编码
    message['From'] = "{}".format(sender)
    message['To'] = ",".join(receivers)
    message['Subject'] = title

    # 发送部分容易出错
    try:
        smtpObj = smtplib.SMTP_SSL(mail_host, 465)  # 启用SSL发信, 端口一般是465
        smtpObj.login(mail_user, mail_pass)  # 登录验证
        smtpObj.sendmail(sender, receivers, message.as_string())  # 发送
        print("成功发送邮件")
    except smtplib.SMTPException as e:
        print(e)