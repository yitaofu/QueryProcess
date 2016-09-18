#include "ts_chinese_translate.h"

TSWords *tsList[TSLEN];
TSWords *stList[TSLEN];

//字符串hash转化成整数
unsigned int SDBMHash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
    return (hash & 0x7FFFFFFF);
}
/******************/
void char_type(char *s, int len, char *char_len, char *type) {
    int unicode = 0;
    char c = *s;
    if (len < 1) {
        *char_len = 0;
        *type = CHAR_TYPE_ERR;
        return;
    }
    if ((c&0x80) == 0x00) { /* ascii char */
        *char_len = 1;
        if (((c >= 'a')&&(c <= 'z')) || ((c >= 'A') && (c <= 'Z'))) *type = CHAR_TYPE_APB;
        else if (((c >= '0') && (c <= '9')) || (c == '.') || (c == '%') || (c == '-') || (c == '~')) *type = CHAR_TYPE_NUM;
        else if (c <= '\032' || c == '\127') *type = CHAR_TYPE_ERR;  //contral char
        else *type = CHAR_TYPE_PCT;
    } else if ((c&0xf0) == 0xe0) { /* maybe cn char, len 3 */
        if (len < 3) { /* incomplete code */
            *char_len = 1;
            *type = CHAR_TYPE_ERR;
        } else {
            if (((s[1]&0xc0) != 0x80) || ((s[2]&0xc0) != 0x80)) { /* damaged code */
                *char_len = 1;
                *type = CHAR_TYPE_ERR;
            } else {
                /* utf-8 to unicode */
                unicode = 4096* (int)(s[0]&0x0f);
                unicode += 64 * (int)(s[1]&0x3f);
                unicode += (int)(s[2]&0x3f);
                /*
                 3400: 13312
                 4db5: 19893
                 4dff: 19967
                 
                 4e00: 19968
                 9fcb: 40907
                 9fff: 40959
                 */
                if (((unicode >= 13312) && (unicode <= 19893)) || ((unicode >= 19968) && (unicode <= 40907))) { /* ch char, len 3 */
                    *char_len = 3;
                    *type = CHAR_TYPE_CNC;
                } else { /* other language such as Japanese or Korean */
                    *char_len = 3;
                    *type = CHAR_TYPE_ERR;
                }
            }
        }
    } else if ((c&0xf8) == 0xf0) { /* maybe cn char, len 4 */
        if(len < 4){ /* incomplete code */
            *char_len = 1;
            *type = CHAR_TYPE_ERR;
        } else {
            if(((s[1]&0xc0) != 0x80) || ((s[2]&0xc0) != 0x80) || ((s[3]&0xc0) != 0x80)) { /* damaged code */
                *char_len = 1;
                *type = CHAR_TYPE_ERR;
            } else {
                /* utf-8 to unicode */
                unicode = 32768 * (int)(s[0]&0x07);
                unicode += 4096 * (int)(s[1]&0x3f);
                unicode += 64 * (int)(s[2]&0x3f);
                unicode += (int)(s[3]&0x3f);
                /*
                 20000: 131072
                 2a6d6: 173782
                 2a6df: 173791
                 */
                if ((unicode >= 131072) && (unicode <= 173782)) { /* ch char, len 4 */
                    *char_len = 4;
                    *type = CHAR_TYPE_CNC;
                } else { /* other language such as Japanese or Korean */
                    *char_len = 4;
                    *type = CHAR_TYPE_ERR;
                }
            }
        }
    } else { /* error code or chars I do not care, but char_len should be comfirmed */
        *type = CHAR_TYPE_ERR;
        if ((c&0xc0) == 0x80) { /* illegal char */
            *char_len = 1;
        } else if ((c&0xe0) == 0xc0) {
            if (len < 2) *char_len=1;  /* incomplete code */
            else if((s[1]&0xc0) == 0x80) *char_len=2;
            else *char_len = 1;
        }else if ((c&0xfc) == 0xf8) {
            if (len < 5) *char_len = 1; /* incomplete code */
            else if (((s[1]&0xc0) == 0x80) && ((s[2]&0xc0) == 0x80) && ((s[3]&0xc0) == 0x80) && ((s[4]&0xc0) == 0x80)) *char_len = 5;
            else *char_len = 1; /* damaged code */
        } else if ((c&0xfe) == 0xfc) {
            if (len < 6) *char_len = 1; /* incomplete code */
            else if (((s[1]&0xc0) == 0x80) && ((s[2]&0xc0) == 0x80) && ((s[3]&0xc0) == 0x80) && ((s[4]&0xc0) == 0x80) && ((s[5]&0xc0) == 0x80)) *char_len = 6;
            else *char_len = 1; /* damaged code */
        } else {
            *char_len = 1;
        }
    }
}
/******************/
void TSinit(char *dic_file) {
    int i;
    for (i = 0; i <  TSLEN; i++) {
        tsList[i] = (TSWords*)malloc(sizeof(TSWords));
        tsList[i]->tagNum = 0;
        stList[i] = (TSWords*)malloc(sizeof(TSWords));
        stList[i]->tagNum = 0;
    }
    readTSHash(dic_file);
}

void createTSHash(char *key, char *value) {
    unsigned int wordHash = SDBMHash(key);
    int tag = wordHash%TSLEN;
    TSWords *findNULL = tsList[tag];
    while (findNULL->tagNum != 0) {
        findNULL = findNULL->next;
    }
    findNULL->Tword = (char*)malloc((strlen(key) + 3) * sizeof(char));
    strcpy(findNULL->Tword, key);
    findNULL->tagNum = 1;
    findNULL->Sword = (char*)malloc((strlen(value) + 3) * sizeof(char));
    strcpy(findNULL->Sword, value);
    
    findNULL->next = (TSWords*)malloc(sizeof(TSWords));
    findNULL->next->tagNum = 0;
}

void createSTHash(char *key, char *value) {
    unsigned int wordHash = SDBMHash(key);
    int tag = wordHash % TSLEN;
    TSWords *findNULL = stList[tag];
    while (findNULL->tagNum != 0) {
        findNULL = findNULL->next;
    }
    findNULL->Tword = (char*)malloc((strlen(key) + 3) * sizeof(char));
    strcpy(findNULL->Tword, key);
    findNULL->tagNum = 1;
    findNULL->Sword = (char*)malloc((strlen(value)+3) * sizeof(char));
    strcpy(findNULL->Sword, value);
    
    findNULL->next = (TSWords*)malloc(sizeof(TSWords));
    findNULL->next->tagNum = 0;
}

void readTSHash(char *dic_file) {
    FILE *fp;
    char *tmp = (char*)malloc(30 * sizeof(char));
    if ((fp = fopen(dic_file, "rb+")) == NULL) {
        printf("文件打开错误！");
        exit(0);
    }
    char *c = "\t";
    
    while (!feof(fp)) {
        fgets(tmp, 30, fp);
        if (tmp[strlen(tmp)-1] == '\n') tmp[strlen(tmp)-1] = '\0';
        char *a = strtok(tmp, c);
        a[strlen(a)] = '\0';
        char *b = strtok(NULL, c);
        createTSHash(a, b);
        createSTHash(b, a);
    }
    free(tmp);
    fclose(fp);
}

void freeTS() {
    int i;
    TSWords *tmp, *p, *tmp1;
    for (i = 0; i < TSLEN; i++) {
        tmp = tsList[i];
        tmp1 = stList[i];
        while (tmp->tagNum != 0) {
            p = tmp->next;
            free(tmp->Tword);
            free(tmp->Sword);
            free(tmp);
            tmp = p;
        }
        free(tmp);
        while (tmp1->tagNum != 0) {
            p = tmp1->next;
            free(tmp1->Tword);
            free(tmp1->Sword);
            free(tmp1);
            tmp1 = p;
        }
        free(tmp1);
    }
}

char *checkTS(char *words, int ts_flag) {
    unsigned int wordHash = SDBMHash(words);
    int tag = wordHash % TSLEN;
    TSWords *findNULL;
    if (ts_flag == 0) findNULL = tsList[tag];
    else findNULL = stList[tag];
    
    while (findNULL->tagNum != 0) {
        if (strcmp(findNULL->Tword, words) == 0)  return findNULL->Sword;
        findNULL = findNULL->next;
    }
    return words;
}
/******************/
//-----繁简转换－－－－
char* substring(char *ch, int pos, int length) {
    char* pch = ch;
    //定义一个字符指针，指向传递进来的ch地址。
    char* subch = (char*)calloc(sizeof(char), (length + 3));
    //通过calloc来分配一个length长度的字符数组，返回的是字符指针。
    int i;
    //只有在C99下for循环中才可以声明变量，这里写在外面，提高兼容性。
    pch = pch + pos;
    //是pch指针指向pos位置。
    for (i = 0; i < length; i++) subch[i] = *(pch++); //循环遍历赋值数组。
    subch[length] = '\0';  //加上字符串结束符。
    return subch;          //返回分配的字符数组地址。
}

int translate(char *wordLIST[], char *minWs, int ts_flag) {
    int n = 0, len = 0;
    int n2 = strlen(minWs);
    char char_len, this_type;
    char *trans_result;
    
    for (n = 0; n < n2;) {
        char_type(minWs + n, n2 - n, &char_len, &this_type);
        if (this_type == CHAR_TYPE_CNC) { //中文
            char *empt = substring(minWs, n, char_len);
            trans_result = checkTS(empt, ts_flag);
            wordLIST[len] = (char*)malloc(sizeof(char) * (strlen(trans_result) + 2));
            strcpy(wordLIST[len], trans_result);
            free(empt);
	    len += 1;
        } else {
            wordLIST[len] = (char*)malloc(sizeof(char) * (char_len + 2));
            char *empt = substring(minWs, n, char_len);
            strcpy(wordLIST[len], empt);
            free(empt);
            len += 1;
        }
        n += char_len;
    }
    return len;
}
