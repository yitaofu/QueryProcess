#include "queryAnalyse.h"

int main(){
	queryAnalyse qa;

	char str[1024] = "shopid=1234&keyword=JDSK连衣裙A芋圓dE&encode=1234";
	cout << "输入：" << str << endl; 
	QUERYLIST ql1 = qa.getQueryList(str);
	cout << "输出：" << endl;
	cout << "query: " << ql1.query << endl;
	cout << "filter_query_list: " << ql1.filter_query_list << endl;
	cout << "displaystyle: " << ql1.displaystyle << endl;
	cout << endl;

	char str1[1024] = "keyword=456789就开始了对方&encode=1234&buyerid=165872276";
	cout << "输入：" << str1 << endl; 
	QUERYLIST ql2 = qa.getQueryList(str1);
	cout << "输出：" << endl;
	cout << "query: " << ql2.query << endl;
	cout << "filter_query_list: " << ql2.filter_query_list << endl;
	cout << "displaystyle: " << ql2.displaystyle << endl;
	cout << endl;

	char str2[1024] = "keyword=韩版傳奇&encode=1231233&buyerid=165872390";
	cout << "输入：" << str2 << endl;
	QUERYLIST ql3 = qa.getQueryList(str2);
	cout << "输出：" << endl;
	cout << "query: " << ql3.query << endl;
	cout << "filter_query_list: " << ql3.filter_query_list << endl;
	cout << "displaystyle: " << ql3.displaystyle << endl;
	cout << endl;
	
	char str3[1024] = "shopid=337410640&encode=137477&keyword=366";
	cout << "输入：" << str3 << endl;
	QUERYLIST ql4 = qa.getQueryList(str3);
	cout << "query: " << ql4.query << endl;
	cout << "filter_query_list: " << ql4.filter_query_list << endl;
	cout << "displaystyle: " << ql4.displaystyle << endl;
	cout << endl;


	return 0;
}
