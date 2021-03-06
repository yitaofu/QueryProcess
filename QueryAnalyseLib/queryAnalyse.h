#ifndef _QUERYANALYSE__h
#define _QUERYANALYSE__h

#include <iostream>
#include <cstring>
#include <algorithm>
#include "ic_url.h"
#include "cJSON.h"

using namespace std;

struct QUERYLIST{
	char query[1024];
	char filter_query_list[1024];
	int displaystyle;
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
#endif
