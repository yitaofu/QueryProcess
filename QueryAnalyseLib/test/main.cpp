#include "queryAnalyse.h"

int main(){
	queryAnalyse qa;
//	string result;

	char str[1024] = "shopid=1234&encode=123456&keyword=连";
	cout << "输入：" << str << endl; 
	QUERYLIST ql1 = qa.getQueryList(str);
	cout << "输出：" << endl;
	cout << "query: " << ql1.query << endl;
	cout << "filter_query_list: " << ql1.filter_query_list << endl;
	cout << "displaystyle: " << ql1.displaystyle << endl;
//	result = qa.getQuery(str);
//	cout << "result: " << result << endl;
	cout << endl;

	char str1[1024] = "buyerid=165872275&encode=456789&keyword=1234";
	cout << "输入：" << str1 << endl; 
	QUERYLIST ql2 = qa.getQueryList(str1);
	cout << "输出：" << endl;
	cout << "query: " << ql2.query << endl;
	cout << "filter_query_list: " << ql2.filter_query_list << endl;
	cout << "displaystyle: " << ql2.displaystyle << endl;
//	result = qa.getQuery(str1);
//	cout << "result: " << result << endl;
	cout << endl;

	char str2[1024] = "buyerid=604745133&encode=ksd3sdf24&keyword=3";
	cout << "输入：" << str2 << endl;
	QUERYLIST ql3 = qa.getQueryList(str2);
	cout << "query: " << ql3.query << endl;
	cout << "filter_query_list: " << ql3.filter_query_list << endl;
	cout << "displaystyle: " << ql3.displaystyle << endl;
//	result = qa.getQuery(str2);
//	cout << "result: " << result << endl;
	cout << endl;
	
	char str3[1024] = "status=";
	cout << "输入：" << str3 << endl;
	QUERYLIST ql4 = qa.getQueryList(str3);
	cout << "query: " << ql4.query << endl;
	cout << "filter_query_list: " << ql4.filter_query_list << endl;
	cout << "displaystyle: " << ql4.displaystyle << endl;
	cout << endl;

	return 0;
}
