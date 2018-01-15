#!/usr/bin/python
#coding:utf-8

#直接从动态src里手动取url发get请求到服务器，解析返回的json并存入硬盘

import os
import sys
import urllib2
import requests
import re
import json
import datetime
from lxml import etree
from bs4 import BeautifulSoup


payloads = {}
payloads['国内新闻-点击量排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'news_china_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['国内新闻-评论数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'gnxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['国内新闻-分享数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfgnxw', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfgnxw_1_data'}]

payloads['国际新闻-点击量排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'news_world_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['国际新闻-评论数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'gjxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}] 
payloads['国际新闻-分享数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfgwxw', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfgwxw_1_data'}] 

payloads['社会新闻-点击量排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'news_society_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}] 
payloads['社会新闻-评论数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'shxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['社会新闻-分享数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfshxw', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfshxw_1_data'}]

#payloads['体育新闻-点击量排行'] = ["http://top.sports.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'sports_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
#payloads['体育新闻-评论数排行'] = ["http://top.sports.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'tyxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
#payloads['体育新闻-分享数排行'] = ["http://top.sports.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfty', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfty_1_data'}]

payloads['财经新闻-点击量排行'] = ["http://top.finance.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'finance_0_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['财经新闻-评论数排行'] = ["http://top.finance.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'cjxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['财经新闻-分享数排行'] = ["http://top.finance.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfcj', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfcj_1_data'}]

#payloads['娱乐新闻-点击量排行'] = ["http://top.ent.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'ent_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
#payloads['娱乐新闻-评论数排行'] = ["http://top.ent.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'ylxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
#payloads['娱乐新闻-分享数排行'] = ["http://top.ent.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfyl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfyl_1_data'}]

payloads['科技新闻-点击量排行'] = ["http://top.tech.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'tech_news_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['科技新闻-评论数排行'] = ["http://top.tech.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'kjxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['科技新闻-分享数排行'] = ["http://top.tech.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfkj', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfkj_1_data'}]

payloads['军事新闻-点击量排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'news_mil_suda', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['军事新闻-评论数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'jsxwpl', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'news_'}]
payloads['军事新闻-分享数排行'] = ["http://top.news.sina.com.cn/ws/GetTopDataList.php", {'top_type': 'day', 'top_cat': 'wbrmzfjsxw', 'top_time':datetime.datetime.now().strftime('%Y%m%d'), 'top_show_num': '20', 'top_order':'DESC', 'js_var':'wbrmzfjsxw_1_data'}]

def saveTitle(save_path, filename, title):
    if not os.path.exists(save_path):
        os.makedirs(save_path)
    path = save_path+"/"+filename+".log"
    with open(path, "a+") as fp:
        fp.write("%s\n" % title)

def saveUrl(save_path, filename, slist):
    if not os.path.exists(save_path):
        os.makedirs(save_path)
    path = save_path+"/"+filename+".log"
    with open(path, "a+") as fp:
        for s in slist:
            #fp.write("%s\t\t%s\n" % (s['title'].encode("utf8"), s['url'].encode("utf8")))
            fp.write("%s %s %s %s\n" % (s['create_date'].encode('utf8'), s['create_time'].encode('utf8'), s['title'].encode("utf8"), s['url'].encode("utf8")))

def Spider():
    for k in sorted(payloads.keys()):
        print k, "downloading ", payloads[k][0]
        saveTitle("tmp", 'sina', k)
        myPage = requests.get(payloads[k][0], params=payloads[k][1])
        decode = json.loads((myPage.content.split("=",1)[1])[1:-2])
        saveUrl("tmp", 'sina', decode['data'])

if __name__ == '__main__':
    print "start"
    Spider()
    print "end"

