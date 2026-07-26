#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

// ==================== 图书管理系统====================

#define ID_MAX 20
#define NAME_MAX 50
#define AUTHOR_MAX 30
#define PRESS_MAX 50
#define TIME_MAX 20
#define READER_ID_MAX 20
#define READER_NAME_MAX 30

// ==================== 图书信息 ====================

typedef struct BookInfo
{
	char id[ID_MAX];                   // 书号
	char name[NAME_MAX];               // 书名
	char author[AUTHOR_MAX];           // 作者
	char press[PRESS_MAX];             // 出版社
	int total;                         // 总库存
	int current;                       // 现存量
	char entry_time[TIME_MAX];         // 入库时间

	// 借阅者信息（当前借出状态）
	int is_borrowed;                   // 当前借出数量
	char borrower_id[READER_ID_MAX];   // 借阅者证件号
	char borrower_name[READER_NAME_MAX];// 借阅者姓名
} BookInfo;

// ==================== 顺序表 ====================

typedef BookInfo SLDataType;

typedef struct SeqList
{
	SLDataType* arr;
	size_t size;
	size_t capacity;
} SL;

// 顺序表基础操作
void SLInit(SL* ps);
void SLDestory(SL* ps);
void SLPushFront(SL* ps, SLDataType x);
void SLPushBack(SL* ps, SLDataType x);
void SLPrint(SL ps);

// ==================== 图书馆类型 ====================

typedef struct SeqList Library;

// ==================== 原有功能 ====================

void LibraryInit(Library* pl);
void LibraryDesTory(Library* pl);
void LibraryAdd(Library* pl);
void LibraryModify(Library* pl);
void LibraryDel(Library* pl);
void LibraryFind(Library* pl);
void LibraryShow(Library* pl);
void LibraryBorrow(Library* pl);
void LibraryReturn(Library* pl);

// ==================== PDF要求新增功能 ====================

// 按作者查找全部著作
void LibraryFindByAuthor(Library* pl);

// ==================== 辅助函数 ====================

void GetCurrentTime(char* buf, size_t bufSize);
void AddDaysToTime(const char* src, int days, char* dst, size_t dstSize);
