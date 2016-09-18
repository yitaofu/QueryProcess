#include "queryAnalyse.h"
#include <pthread.h>
#include <sstream>

pthread_mutex_t work_mutex;

void* getQueryList(void* dat){
	char* queryList = (char*)dat;
	queryAnalyse qa;
	QUERYLIST ql = qa.getQueryList(queryList);
	pthread_mutex_lock(&work_mutex);
	cout << "query: " << ql.query << " | list: " << ql.filter_query_list << " | display: " << ql.displaystyle << endl;
	pthread_mutex_unlock(&work_mutex);
}

int main(int argc, char* argv[]){
	pthread_t *pid_list = 0;
	queryAnalyse qa;

	int thread_num = atoi(argv[1]);
	if ( (pid_list = (pthread_t *)malloc(sizeof(pthread_t)*thread_num)) == NULL ){
		cout << "error" << endl;
		exit(1);
	}

	cout << "创建线程！" << endl;
	string strId = "shopid=";
	string strEncode = "encode=";
	string strKeyword = "keyword=";
	string result;
	string num;
	int i;
	char queryChar[1024][1024];
	for (i = 0; i < thread_num; i++){
		stringstream ss;
		ss << i;
		num = ss.str() + ss.str() + ss.str();
		if (i < thread_num/3){
			result = strId + num + "&" + strEncode + num + "&" + strKeyword + num;
		}else if (i >= thread_num/3 && i < thread_num*2/3){
			result = strId + num + "&" + strEncode + num + "&" + strKeyword + num + "fuyitao";
		}else{
			result = strId + num + "&" + strEncode + num + "&" + strKeyword + num + "付一韬";
		}
			
			
		memset(queryChar[i], 0, 1024);
		strncpy(queryChar[i], result.c_str(), result.length());
	}
		
	pthread_mutex_init(&work_mutex, NULL);
	for (i = 0; i < thread_num; i++){
//		char query[1024] = "keyword=连衣裙&shopid=34324&encode=123";
		pthread_create(&(pid_list[i]), NULL, getQueryList, queryChar[i]);
	}
	for (i = 0; i < thread_num; i++){
		pthread_join(pid_list[i], NULL);
	}
	pthread_mutex_destroy(&work_mutex);
	
	return 0;
}
