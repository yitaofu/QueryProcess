#include "queryAnalyse.h"

queryAnalyse::queryAnalyse(){
	//query = "\0";
	//filter_query_list = "\0";
	displaystyle = 3;
	char *dic_file = (char *)malloc(sizeof(char) * MAX_STRING);
        strcpy(dic_file, "data/ts_convert.txt");
	TSinit(dic_file);
	free(dic_file);
}

queryAnalyse::~queryAnalyse(){
	freeTS();
}

string queryAnalyse::getFilter_Query_List(){
	return "";
}

int queryAnalyse::getDisplaystyle(){
	return displaystyle;
}

bool queryAnalyse::isNum(string str){
	if (str == ""){
		return false;
	}
	for (int i = 0; i < (int)str.length(); i++){
		if ( (int)str[i] < 48 || (int)str[i] > 57 ){
			return false;
		}
	}

	return true;
}// 判断是否纯数字

string queryAnalyse::singleParticiple(string str, string nameStr){
	string Name = "";
	if (nameStr == "name"){
		Name = "name:";
	}else if (nameStr == "buyername"){
		Name = "buyername:";
	}

	if (str.length() == 0){
		return Name;
	}

	string partStr = "";
	int i = 0;
	if ( (int)str[i] >= 0 ){
		partStr = Name + str.substr(i, 1);
		i++;
	}else{
		if ( (int)str.length() >= 3 ){
			partStr = Name + str.substr(i, 3);
			i += 3;
		}else{
			partStr = Name + str;
			i += (int)str.length();
		}
	}

	for (; i < (int)str.length(); i++){
		if ( (int)str[i] >= 0 ){
			partStr = partStr + " AND " + Name + str[i];
		}else{
			if ( i+3 < (int)str.length() ){
				partStr = partStr + " AND " + Name + str.substr(i, 3);
				i += 2;
			}else{
				partStr = partStr + " AND " + Name + str.substr(i);
				break;
			}
		}
	}

	return partStr;
}// 单字分词

string queryAnalyse::getWholeNumOutput(string encode, string keyword){
	string result = "";
	string tel = encode;
	string id = keyword;
	string name = singleParticiple(keyword, "name");
	string buyername = singleParticiple(keyword, "buyername");

	result = "((" + name + ") OR (" + buyername + ") OR (id:" + id + ") OR (tel:" + tel + "))";

	return result;
}// 纯数字name输出

string queryAnalyse::getOtherOutput(string keyword){
	string result = "";
	string name = singleParticiple(keyword, "name");
	string buyername = singleParticiple(keyword, "buyername");

	result = "((" + name + ") OR (" + buyername + "))";

	return result;
}// 其他name输出

string queryAnalyse::getBuyerID(string buyerid, string buyeridInterface){
	if (buyerid == ""){
		return "buyerid:";
	}
	string text = buyerid;
	string url;
	if (buyeridInterface == "http://idc01-ms-web-vip09/udc/user/getRelatedIdByUserId?param={\"userId\":\""){
		url = buyeridInterface + text + "\"}";
	}else{
		url = buyeridInterface + text;
	}
	char url_tmp[4096];
	strcpy(url_tmp, url.c_str());
	char * contChar = get_url_contents(url_tmp);
	string cont = contChar;
	free(contChar);
	
	
	cJSON *root = cJSON_Parse(cont.c_str());
	char *tempStr = cJSON_Print(root->child->next);
	cJSON *result = cJSON_Parse(tempStr);

	cJSON *buyer_id_json = cJSON_GetObjectItem(result, "buyer_id");
        cJSON *wx_id_json = cJSON_GetObjectItem(result, "wx_id");
        cJSON *qq_id_json = cJSON_GetObjectItem(result, "qq_id");

        string buyer_id = buyer_id_json->valuestring;
        string wx_id = wx_id_json->valuestring;
        string qq_id = qq_id_json->valuestring;

	// free
	free(tempStr);
	cJSON_Delete(root);
	cJSON_Delete(result);

	string buyeridResult = "";
	int count = 0;  // 对不为空的id进行计数
	if (buyer_id != ""){
		count += 1;
	}
	if (wx_id != ""){
		count += 1;
	}
	if (qq_id != ""){
		count += 1;
	}
	
	//
	if (count == 0){  // 没有id
		buyeridResult = "buyerid:";
		return buyeridResult;
	}else if (count == 1){  // 只有一个id
		if (buyer_id != ""){
			buyeridResult = "buyerid:" + buyer_id;
		}else if (wx_id != ""){
			buyeridResult = "buyerid:" + wx_id;
		}else if (qq_id != ""){
			buyeridResult = "buyerid:" + qq_id;
		}
	}else{  // 2个或3个id
		if (buyer_id != ""){
			buyeridResult = "(buyerid:" + buyer_id + ")";
		}
		if (wx_id != ""){
			if (buyeridResult != ""){
				buyeridResult = buyeridResult + " OR (buyerid:" + wx_id + ")";
			}else{
				buyeridResult = "(buyerid:" + wx_id + ")";
			}
		}
		if (qq_id != ""){
			if (buyeridResult != ""){
				buyeridResult = buyeridResult + " OR (buyerid:" + qq_id + ")";
			}else{
				buyeridResult = "(buyerid:" + qq_id + ")";
			}
		}
	}
	return buyeridResult;
}

string queryAnalyse::getQuery(char* queryList, string buyeridInterface = "http://idc01-ms-web-vip09/udc/user/getRelatedIdByUserId?param={\"userId\":\""){
	string queryStr = queryList;

	string shopid = "";
	string buyerid = "";
	string encode = "";
	string keyword = "";
	bool isBuyer = false;
	int local;
	string tempStr;

	while (1){
		local = queryStr.find("&");
		tempStr = queryStr.substr(0, local);	
	
		if (tempStr.find("shopid") == 0){
			if (tempStr.rfind("=") != string::npos){
				shopid = tempStr.substr(tempStr.find("=")+1);
			}
		}else if (tempStr.find("buyerid") == 0){
			if (tempStr.rfind("=") != string::npos){
				buyerid = tempStr.substr(tempStr.find("=")+1);
				isBuyer = true;
			}
		}else if (tempStr.find("encode") == 0){
			if (tempStr.rfind("=") != string::npos){
				encode = tempStr.substr(tempStr.find("=")+1);
			}
		}else if (tempStr.find("keyword") == 0){
			if (tempStr.rfind("=") != string::npos){
				keyword = tempStr.substr(tempStr.find("=")+1);
			}
		}

		if (queryStr.find("&") == string::npos){
			break;
		}
		queryStr = queryStr.substr(local+1);
	}// 截取shopid/buyerid, encode, keyword

	transform(keyword.begin(), keyword.end(), keyword.begin(), (int (*)(int))tolower);
		
	if (keyword != ""){
		int i;
		char *wordlist[MAX_STRING];
		char *minWs = (char *)malloc(sizeof(char) * MAX_STRING);
		strncpy(minWs, keyword.c_str(), MAX_STRING);
		int back = translate(wordlist, minWs, 0);  // 0表示繁转简；1表示简转繁
		keyword = "";
		for (i = 0; i < back; i++){
			keyword = keyword + wordlist[i];
		}
		for (i = 0; i < back; i++){
			free(wordlist[i]);
		}
		free(minWs);
	}// 繁转简
	
	string name = "";
	if ( isNum(keyword) ){
		name = getWholeNumOutput(encode, keyword);
	}else{
		name = getOtherOutput(keyword);
	}

	string queryResult = "";
	if (isBuyer){
		string buyeridResult = getBuyerID(buyerid, buyeridInterface);
		queryResult = name + " AND " + "(" + buyeridResult + ")";
	}else{
		queryResult = name + " AND " + "(shopid:" + shopid + ")";
	}

	return queryResult;
}// query分析

QUERYLIST queryAnalyse::getQueryList(char* queryList){
	string temp_query = getQuery(queryList);
	string temp_filter_query_list = getFilter_Query_List();
	int temp_displaystyle = getDisplaystyle();

	QUERYLIST ql;
	memset(&ql,0,sizeof(ql));
	strcpy(ql.query, temp_query.c_str());
	strcpy(ql.filter_query_list, temp_filter_query_list.c_str());
	ql.displaystyle = temp_displaystyle;

	return ql;
}// 获得querylist最终结果

