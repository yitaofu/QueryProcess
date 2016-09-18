//test
#include "test.h"

// 编码，解码------------------------------------------
static char hex2dec(char ch){
	if (ch >= 'A' && ch <= 'Z'){
		ch = (ch - 'A') + 'a';
	}
	return (ch >= '0' && ch <= '9') ? (ch - '0') : (ch -'a' + 10);
}

static char dec2hex(char ch){
	static char hex[] = "0123456789ABCDEF";
	return hex[ch & 15];
}

// urlencode 编码
char *urlencode(char *src){
	char *psrc = src;
	char *buf;
	char *pbuf;
	int len = strlen(src) * 3 + 1;
	buf = (char *)malloc(len);
	// errdo(buf == NULL, src);
	memset(buf, 0, len);

	pbuf = buf;
        while (*psrc){
                if ((*psrc >= '0' && *psrc <= '9') || (*psrc >= 'a' && *psrc <= 'z') || (*psrc >= 'A' && *psrc <= 'Z')){
                        *pbuf = *psrc;
                }else if (*psrc == '-' || *psrc == '_' || *psrc == '.' || *psrc == '~'){
                        *pbuf = *psrc;
                }else if (*psrc == ' '){
                        *pbuf = '+';
                }else{
                        pbuf[0] = '%';
                        pbuf[1] = dec2hex(*psrc >> 4);
                        pbuf[2] = dec2hex(*psrc & 15);
                        pbuf += 2;
                }

                psrc++;
                pbuf++;
        }

        *pbuf = '\0';
        return buf;
}

// urldecode 解码
char *urldecode(char *src){
	char *psrc = src;
        char *buf;
        char *pbuf;
        int len = strlen(src) + 1;
        buf = (char *)malloc(len);
	// errdo(buf == NULL, src);
	memset(buf, 0, len);

	pbuf = buf;
        while (*psrc){
                if (*psrc == '%'){
                        if (psrc[1] && psrc[2]){
                                *pbuf = hex2dec(psrc[1]) << 4 | hex2dec(psrc[2]);
                                psrc += 2;
                        }
                }else if (*psrc == '+'){
                        *pbuf = ' ';
                }else{
                        *pbuf = *psrc;
                }

                psrc++;
                pbuf++;
        }

        *pbuf = '\0';
        return buf;
}
// ----------------------------------------------------------
// 类的实现
queryAnalyse::queryAnalyse(){
        query = "\0";
        filter_query_list = "\0";
        displaystyle = 3;
}

string queryAnalyse::getFilter_Query_List(){
        return filter_query_list;
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

string queryAnalyse::singleParticiple(string str){
        if (str.length() == 0){
                return "name:";
        }

        string partStr = "";
        int i = 0;
        if ( (int)str[i] >= 0 ){
                partStr = "name:" + str.substr(i, 1);
                i++;
        }else{
                if ( (int)str.length() >= 3 ){
                        partStr = "name:" + str.substr(i, 3);
                        i += 3;
                }else{
                        partStr = "name:" + str;
                        i += (int)str.length();
                }
        }

        for (; i < (int)str.length(); i++){
                if ( (int)str[i] >= 0 ){
                        partStr = partStr + " AND name:" + str[i];
                }else{
                        if ( i+3 < (int)str.length() ){
                                partStr = partStr + " AND name:" + str.substr(i, 3);
                                i += 2;
                        }else{
                                partStr = partStr + " AND name:" + str.substr(i);
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
        string name = singleParticiple(keyword);

        result = "((" + name + ") OR (id:" + id + ") OR (tel:" + tel + "))";

        return result;
}// 纯数字name输出

string queryAnalyse::getOtherOutput(string keyword){
        string result = "";
        string name = singleParticiple(keyword);
        string buyername = keyword;

        result = "((" + name + ") OR (buyername:" + buyername + "))";

        return result;
}// 其他name输出

string queryAnalyse::getBuyerID(string buyerid, string buyeridInterface){
        if (buyerid == ""){
                return "buyerid:";
        }
//	ofstream fpout("diaryfile", ios::app);  // 写日志
//	fpout << "buyerid:" << endl;  // 写日志

        string text = buyerid;
        string url;
        url = buyeridInterface + text + "\"}";

        char url_tmp[4096];
        memset(url_tmp, 0, 4096);
        if ( (int)url.length() > 4095){
                strncpy(url_tmp, url.c_str(), 4095);
        }else{
                strncpy(url_tmp, url.c_str(), url.length());
        }

        char * contChar = get_url_contents(url_tmp);
        string cont = contChar;
        free(contChar);
//	fpout << "url_contents: " << cont << endl;  // 写日志

        // root
     	cJSON *root = cJSON_Parse(cont.c_str());
        if ( !root ){
        	cJSON_Delete(root);
        		return "buyerid:";
        }
       // status_code
	char *statusStr = cJSON_Print(root->child);
//	fpout << "status: " << endl;  // 写日志
        if ( !statusStr ){
                cJSON_Delete(root);
                free(statusStr);
                return "buyerid:";
        }else{
                cJSON *statusResult = cJSON_Parse(statusStr);
                if ( !statusResult ){
                        free(statusStr);
                        cJSON_Delete(root);
                        cJSON_Delete(statusResult);
                        return "buyerid:";
                }else{
                        cJSON *status_code_json = cJSON_GetObjectItem(statusResult, "status_code");
                        if ( !status_code_json ){
                                free(statusStr);
                                cJSON_Delete(root);
                                cJSON_Delete(statusResult);
                                return "buyerid:";
                        }else if ( status_code_json->valueint ){  // status_code不为0返回空
                                free(statusStr);
                                cJSON_Delete(root);
                                cJSON_Delete(statusResult);
                                return "buyerid:";
                        }
                        cJSON_Delete(statusResult);
                }
        }
        free(statusStr);

	// id
	char *resultStr = cJSON_Print(root->child->next);
//	fpout << "result: " << resultStr << endl;  // 写日志
        if ( !resultStr ){
                free(resultStr);
                cJSON_Delete(root);
                return "buyerid:";
        }
        cJSON *resultResult = cJSON_Parse(resultStr);
        if ( !resultResult ){
                free(resultStr);
                cJSON_Delete(resultResult);
                cJSON_Delete(root);
                return "buyerid:";
        }

        cJSON *buyer_id_json = cJSON_GetObjectItem(resultResult, "buyer_id");
//	fpout << "完成buyer_id赋值" << endl;  // 写日志
        cJSON *wx_id_json = cJSON_GetObjectItem(resultResult, "wx_id");
//	fpout << "完成wx_id赋值" << endl;  // 写日志
        cJSON *qq_id_json = cJSON_GetObjectItem(resultResult, "qq_id");
//	fpout << "完成qq_id赋值" << endl;  // 写日志

        string buyer_id = "";
        string wx_id = "";
        string qq_id = "";

        if (buyer_id_json){
                buyer_id = buyer_id_json->valuestring;
        }
        if (wx_id_json){
                wx_id = wx_id_json->valuestring;
        }
        if (qq_id_json){
                qq_id = qq_id_json->valuestring;
        }
//	fpout << "buyer_id: " << buyer_id << endl;  // 写日志
//	fpout << "wx_id: " << wx_id << endl;  // 写日志
//	fpout << "qq_id: " << qq_id << endl;  // 写日志


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

	// free
	free(resultStr);
        cJSON_Delete(root);
        cJSON_Delete(resultResult);

//	fpout << "buyeridResult: " << buyeridResult << endl;  // 写日志 
//	fpout << "----------------------------------------------" << endl;  // 写日志
//	fpout.close();  // 写日志

        return buyeridResult;
}

string queryAnalyse::getQuery(char* queryList, string buyeridInterface){

        string queryStr = queryList;

        string shopid = "";
        string buyerid = "";
        string encode = "";
        string keyword = "";
        bool isBuyer = false;
        int local = 0;
        string tempStr;

        while (1){
                if (queryStr.find("&") == string::npos){
                        tempStr = queryStr;
                }else{
                        if (queryStr.find("keyword=") != 0){
                                local = queryStr.find("&");
                                tempStr = queryStr.substr(0, local);
                        }else{
                                tempStr = queryStr;
                        }
                }

                if (tempStr.find("shopid=") != string::npos && (int)tempStr.find("shopid=")+7 < (int)tempStr.length() ){
                        shopid = tempStr.substr(tempStr.find("shopid=")+7);
                }else if (tempStr.find("buyerid=") != string::npos){
                        isBuyer = true;
                        if ( (int)tempStr.find("buyerid=")+8 < (int)tempStr.length() ){
                                buyerid = tempStr.substr(tempStr.find("buyerid=")+8);
                        }
                }else if (tempStr.find("encode=") != string::npos && (int)tempStr.find("encode=")+7 < (int)tempStr.length()){
                        encode = tempStr.substr(tempStr.find("encode=")+7);
                }else if (tempStr.find("keyword=") != string::npos && (int)tempStr.find("keyword=")+8 < (int)tempStr.length()){
                        keyword = tempStr.substr(tempStr.find("keyword=")+8);
                        break;
                }

                if (queryStr.find("&") == string::npos){
                        break;
                }
                queryStr = queryStr.substr(local+1);
        }// 截取shopid/buyerid, encode, keyword

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
                string shopidResult = "shopid:" + shopid;
                queryResult = name + " AND " + "(" + shopidResult + ")";
        }
//	ofstream fpout("diaryfile", ios::app);  // 写日志
//	fpout << "queryResult: " << queryResult << endl;  // 写日志
//	fpout.close();  // 写日志

        return queryResult;
}// query分析

QUERYLIST queryAnalyse::getQueryList(char* queryList, string buyeridInterface){
        string temp_query = getQuery(queryList, buyeridInterface);
        string temp_filter_query_list = getFilter_Query_List();
        int temp_displaystyle = getDisplaystyle();

        QUERYLIST ql;

        memset(ql.query, 0, 1024);
        if ( (int)temp_query.length() > 1023 ){
                strncpy(ql.query, temp_query.c_str(), 1023);
        }else{
                strncpy(ql.query, temp_query.c_str(), temp_query.length());
        }


        memset(ql.filter_query_list, 0, 1024);
        if ( (int)temp_filter_query_list.length() > 1023 ){
                strncpy(ql.filter_query_list, temp_filter_query_list.c_str(), 1023);
        }else{
                strncpy(ql.filter_query_list, temp_filter_query_list.c_str(), temp_filter_query_list.length());
        }

        ql.displaystyle = temp_displaystyle;

        return ql;
}// 获得querylist最终结果

// ----------------------------------------------------------
//初始化
struct test_data * test_init(char * conf_file) {
	struct test_data * test_data = 0;
//
	if((test_data = (struct test_data *)malloc(sizeof(struct test_data))) == NULL || (test_data->data = (char *)malloc(strlen(conf_file) + 1)) == NULL) {
		fprintf(stderr, "%s: %d: %s: ERROR: malloc error\n", __FILE__, __LINE__, __FUNCTION__);
		exit(0);
	}
//
	strcpy(test_data->data, conf_file);
//
	return test_data;
}

//工作函数
char * test_work(struct test_data * test_data, char * args) {
	char * res = 0;
	//
	if((res = (char *)malloc(strlen(test_data->data) + strlen(args) + 64)) == NULL) {
		fprintf(stderr, "%s: %d: %s: ERROR: malloc error\n", __FILE__, __LINE__, __FUNCTION__);
		exit(0);
	}
	//
	//////////////////////////////////////////////////////////////
	// 获得特征
	char *argsStr = urldecode(args);
	queryAnalyse qa;
	QUERYLIST ql;
	ql = qa.getQueryList(argsStr);
//	ofstream fpout("diaryfile", ios::app);  // 写日志
//	fpout << "query: " << ql.query << endl;  // 写日志
	
	sprintf(res, "%s", ql.query);
	//
//	fpout << "成功输出结果！" << endl;  // 写日志
//	fpout << "-------------------------------------------" << endl;  // 写日志
//	fpout.close();  // 写日志
	return res;
}

