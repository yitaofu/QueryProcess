#ifndef _QUERYANALYSE__h
#define _QUERYANALYSE__h

#include <iostream>
#include <cstring>
#include <algorithm>
#include <ctype.h>
#include "ic_url.h"
#include "cJSON.h"
#include "ts_chinese_translate.h"

#define MAX_STRING 4096

using namespace std;
#pragma pack(1)
struct QUERYLIST{
	char query[1024];
	char filter_query_list[1024];
	int displaystyle;
};

class queryAnalyse{
private:
	//string query;
	//string filter_query_list;
	int displaystyle;
public:
	queryAnalyse();
	~queryAnalyse();
	string getQuery(char* queryListi, string buyeridInterface);  // 对query处理,获得query
	bool isNum(string str);  // 判断是否为纯数字
	string singleParticiple(string str, string nameStr);  // 单字分词
	string getWholeNumOutput(string encode, string keyword);  // 纯数字的name结果输出
	string getOtherOutput(string keyword);  // 其他的name结果输出
	string getFilter_Query_List();  // 获得filter_query_list
	int getDisplaystyle();  // 获得displaystyle
	string getBuyerID(string buyerid, string buyeridInterface);  // 获得买家ID

	QUERYLIST getQueryList(char* queryList); // 获得querylist最终结果
};

#pragma pack()
#endif
