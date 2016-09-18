//test
#ifndef _TEST_H
#define _TEST_H
//
#include <iostream>
#include <cstring>
#include <fstream>
#include "ic_url.h"
#include "cJSON.h"

using namespace std;

#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////
struct QUERYLIST{
	char query[1024];
	char filter_query_list[1024];
	int displaystyle;
};

struct test_data{
	char* data;
};

class queryAnalyse{
private:
        string query;
        string filter_query_list;
        int displaystyle;
public:
        queryAnalyse();
        string getQuery(char* queryList, string buyeridInterface);  // 对query处理,获得query
        bool isNum(string str);  // 判断是否为纯数字
        string singleParticiple(string str);  // 单字分词
        string getWholeNumOutput(string encode, string keyword);  // 纯数字的name结果输出
        string getOtherOutput(string keyword);  // 其他的name结果输出
        string getFilter_Query_List();  // 获得filter_query_list
        int getDisplaystyle();  // 获得displaystyle
        string getBuyerID(string buyerid, string buyeridInterface);  // 获得买家ID

        QUERYLIST getQueryList(char* queryList, string buyeridInterface = "http://idc01-ms-web-vip09/udc/user/getRelatedIdByUserId?param={\"userId\":\""); // 获得querylist最终结果
};

/////////////
struct test_data * test_init(char * conf_file);
char *urldecode(char *src);
char * test_work(struct test_data * test_data, char * args);

#endif
