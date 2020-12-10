# -*- coding: utf-8 -*-

import pandas as pd
from shixiseng_crawler import shixiseng_crawler
from email_sender import sendEmail


if __name__ == '__main__':
    
    crawler = shixiseng_crawler(city='上海', keyword='数据', publishTime='day', pages=1)  # pages - 要爬取的页数; publishTime='day' - 最近一天发布的岗位
    crawler.run()
    print('数据采集完成')

    # 编写MySQL语句 用LIKE通配符匹配合适的岗位
    sqlcmd = " SELECT positionName, company, area, education, companyDetails, url FROM shixiseng  \
                WHERE date(uploadTime) = curdate( ) AND jobDescription LIKE '%excel%'"  # 这里选择比较常用的Excel进行匹配
    # 执行语句
    crawler.cur.execute(sqlcmd)
    # fetchall返回一个tuple
    data = crawler.cur.fetchall()
    
    if len(data) != 0:
        data = pd.DataFrame(list(data))
        data.columns = ['职位名称', '公司名称', '工作地点', '学历要求', '公司规模', '连接']
        # 发送到个人邮箱
        sendEmail(data)
        
    else:
        print('今天没有合适的岗位推荐')
    
    # 关闭数据库
    crawler.db.close()