#ifndef _TS_CHINESE_TRANSLATE_H_
#define _TS_CHINESE_TRANSLATE_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

unsigned int SDBMHash(char *str);
/******************/
/* type of utf8 char
 * APB : english alphabet
 * NUM : arabic numberals
 * PCT : english punctuation
 * CNC : chinese char,
 *       include simplified and traditional chinese
 * ERR : error codes or chinese punctuation or bad codes
 */
#define CHAR_TYPE_APB   0x01
#define CHAR_TYPE_NUM   0x02
#define CHAR_TYPE_PCT   0x04
#define CHAR_TYPE_CNC   0x08
#define CHAR_TYPE_ERR   0x10

/* judge utf-8 char type by transcode utf-8 to unicode
 * so far, type include: alphabet, number, punctuation(标点), chinese, error char.
 *
 * s        : start position of target string
 * len      : length of total target string
 * char_len : will store bytes the head char includes
 * type     : type of the head char 
 */
void char_type(char * s, int len, char * char_len, char * type);
/******************/
#define TSLEN 10000

//字典
typedef struct TSWords {
    char *Tword;
    char *Sword;
    int tagNum;
    struct TSWords *next;
} TSWords;

void TSinit(char *dic_file);
void createTSHash(char *key, char *value);
void createSTHash(char *key, char *value);
void readTSHash(char *dic_file);
void freeTS();
char *checkTS(char *words, int ts_flag);
/******************/
int translate(char *wordLIST[], char *minWs, int ts_flag);

#ifdef __cplusplus
}
#endif

#endif
