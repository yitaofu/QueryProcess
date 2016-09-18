本程序是对query进行分析。

1.输入参数说明：
  (1)卖家搜索shopid=xxx&encode=xxxx&keyword=xxxx
  (2)买家搜索buyerid=xxx&encode=xxxx&keyword=xxxx
  (3)keyword是用户输入明文字符串，encode是明文加密后字符串。
2.encode与keyword处理如下：
  (1)明文为纯数字输出: name=keyword(单字分词) OR id=keyword(不分词) OR tel=encode(不分词)
  (2)明文为其他（除了数字的）输出: name=keyword(单字分词) OR buyername=keyword(不分词)
3.买家搜索需对buyerid做处理：
  (1)由于一个买家可能存在多个buyerid,  根据输入的buyerid返回关联的所有buyerid
  (2)查询buyerid接口 http://idc01-ms-web-vip09/udc/user/getRelatedIdByUserId?param={"userId":"604745131"}
  (3)如果buyerid存在多个值，输出多个值buyerid=xxx OR buyerid=xxx

备注：单字分词时，字之间用AND连接；
      encode与keyword处理后与shopid/buyerid之间用AND连接。

举两个例子：
1. 
输入：shopid=1234&encode=6789&keyword=连衣裙
输出：((name:连 AND name:衣 AND name:裙) OR (buyername:连衣裙)) AND (shopid:1234)
2.
输入：buyerid=604745133&encode=34567&keyword=1234
输出：((name:1 AND name:2 AND name:3 AND name:4) OR (id:1234) OR (tel:34567)) AND ((buyerid:34150756) OR (buyerid:604745133))

程序使用:
1. ic_url: 获取buyerid内容，格式为JSON
2. cJSON: 对获取JSON格式进行解析
3. queryAnalyse: 对输入的参数进行分析，生成输出结果。其中会调用ic_url和cJSON两个函数

  测试中直接引用queryAnalyse.h这个头文件，用queryAnalyse这个类实例化一个对象qa，用qa调用函数getQueryList(str)（str为输入的参数）返回一个QUERYLIST的结构体，该结构体包含三个值分别是query（上面的输出结果）、filter_query_list（暂时默认为空）和displaystyle（暂时默认为3）

例子：
struct QUERYLIST{
	char query[1024];
	char filter_query_list[1024];
	int displaystyle;
} 

	queryAnalyse qa;
	char str[1024] = "shopid=1234&encode=6789&keyword=连衣裙";
	QUERYLIST ql = qa.getQueryList(str);
	cout << ql.query << endl;
	cout << ql.filter_query_list << endl;
	cout << ql.displaystyle << endl;

结果：
((name:连 AND name:衣 AND name:裙) OR (buyername:连衣裙)) AND (shopid:1234)

3

备注：使用g++编译的时候需要将 -lcurl 编译进去
