介绍：该程序是对Query进行分析, 加入了繁转简功能
例子：
1. 输入：shopid=1234&encode=6789&keyword=连衣裙 
输出：((name:连 AND name:衣 AND name:裙) OR (buyername:连衣裙)) AND (shopid:1234) 
2. 输入：buyerid=604745133&encode=34567&keyword=1234 
输出：((name:1 AND name:2 AND name:3 AND name:4) OR (id:1234) OR (tel:34567)) AND ((buyerid:34150756) OR (buyerid:604745133)) 
