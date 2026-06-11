#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// ==================== 图书信息常量与结构 ====================

#define ID_MAX 20
#define NAME_MAX 50
#define AUTHOR_MAX 30
#define PRESS_MAX 50

typedef struct BookInfo
{
	char id[ID_MAX];
	char name[NAME_MAX];
	char author[AUTHOR_MAX];
	char press[PRESS_MAX];
	int total;		// 总库存量
	int current;	// 现存量
} BookInfo;

// ==================== 顺序表（底层存储） ====================

typedef BookInfo SLDataType;

typedef struct SeqList
{
	SLDataType* arr;
	size_t size;
	size_t capacity;
} SL;

// 顺序表操作
void SLInit(SL* ps);
void SLDestory(SL* ps);
void SLPushFront(SL* ps, SLDataType x);
void SLPushBack(SL* ps, SLDataType x);
void SLPrint(SL ps);

// ==================== 图书馆（顺序表别名） ====================

typedef struct SeqList Library;

// 图书馆操作
void LibraryInit(Library* pl);
void LibraryDesTory(Library* pl);
void LibraryAdd(Library* pl);
void LibraryModify(Library* pl);
void LibraryDel(Library* pl);
void LibraryFind(Library* pl);
void LibraryShow(Library* pl);
void LibraryBorrow(Library* pl);
void LibraryReturn(Library* pl);
