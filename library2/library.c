#include "library.h"

// ==================== 工具函数 ====================

void GetCurrentTime(char* buf, size_t bufSize)
{
	time_t t = time(NULL);
	struct tm* tm_info = localtime(&t);
	strftime(buf, bufSize, "%Y-%m-%d", tm_info);
}

// 日期加天数
void AddDaysToTime(const char* src, int days, char* dst, size_t dstSize)
{
	struct tm tm_info = { 0 };
	int y, m, d;
	sscanf(src, "%d-%d-%d", &y, &m, &d);
	tm_info.tm_year = y - 1900;
	tm_info.tm_mon = m - 1;
	tm_info.tm_mday = d;
	tm_info.tm_hour = 12;

	time_t t = mktime(&tm_info);
	t += (time_t)days * 86400;
	struct tm* new_tm = localtime(&t);
	strftime(dst, dstSize, "%Y-%m-%d", new_tm);
}

// ==================== 顺序表实现====================

void SLInit(SL* ps)
{
	assert(ps);
	ps->arr = NULL;
	ps->size = 0;
	ps->capacity = 0;
}

void SLDestory(SL* ps)
{
	if (ps->arr)
	{
		free(ps->arr);
	}
	ps->arr = NULL;
	ps->size = 0;
	ps->capacity = 0;
}

static void CheckCapacity(SL* ps)
{
	if (ps->capacity == ps->size)
	{
		size_t newcapacity = ps->capacity == 0 ? 4 : ps->capacity * 2;
		SLDataType* tmp = (SLDataType*)realloc(ps->arr, sizeof(SLDataType) * newcapacity);
		if (tmp == NULL)
		{
			perror("realloc fail !");
			return;
		}
		ps->arr = tmp;
		ps->capacity = newcapacity;
	}
}

void SLPushBack(SL* ps, SLDataType x)
{
	assert(ps);
	CheckCapacity(ps);
	ps->arr[ps->size] = x;
	ps->size++;
}

void SLPushFront(SL* ps, SLDataType x)
{
	assert(ps);
	CheckCapacity(ps);
	for (int i = (int)ps->size - 1; i >= 0; i--)
	{
		ps->arr[i + 1] = ps->arr[i];
	}
	ps->arr[0] = x;
	ps->size++;
}

void SLPrint(SL ps)
{
	for (size_t i = 0; i < ps.size; i++)
	{
		printf("%s ", ps.arr[i].id);
	}
	printf("\n");
}

// ==================== 图书馆功能实现 ====================

void LibraryInit(Library* pl)
{
	SLInit(pl);
}

void LibraryDesTory(Library* pl)
{
	SLDestory(pl);
}

// 按书号查找，返回下标，未找到返回-1
static int FindById(Library* pl, char id[])
{
	for (size_t i = 0; i < pl->size; i++)
	{
		if (0 == strcmp(pl->arr[i].id, id))
			return (int)i;
	}
	return -1;
}

// ==================== 1. 采编入库 ====================

void LibraryAdd(Library* pl)
{
	BookInfo info;
	info.is_borrowed = 0;
	info.borrower_id[0] = '\0';
	info.borrower_name[0] = '\0';

	printf("请输入书号:>");
	scanf("%s", info.id);

	int pos = FindById(pl, info.id);
	if (pos != -1)
	{
		int addNum;
		printf("该书已存在，当前总库存:%d，现存量:%d\n",
			pl->arr[pos].total, pl->arr[pos].current);
		printf("请输入增加数量:>");
		scanf("%d", &addNum);
		pl->arr[pos].total += addNum;
		pl->arr[pos].current += addNum;
		printf("入库成功!\n");
		return;
	}

	printf("请输入书名:>");
	scanf("%s", info.name);
	printf("请输入作者:>");
	scanf("%s", info.author);
	printf("请输入出版社:>");
	scanf("%s", info.press);
	printf("请输入总库存量:>");
	scanf("%d", &info.total);
	info.current = info.total;
	printf("请输入入库时间(YYYY-MM-DD):>");
	scanf("%s", info.entry_time);

	SLPushBack(pl, info);
	printf("新书入库成功!\n");
}

// ==================== 2. 删除图书（清除库存） ====================

void LibraryDel(Library* pl)
{
	char id[ID_MAX];
	printf("请输入需要删除的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("要删除的图书不存在!\n");
		return;
	}

	printf("确认删除《%s》? (1=是/0=否):>", pl->arr[pos].name);
	int confirm;
	scanf("%d", &confirm);
	if (confirm != 1)
	{
		printf("已取消删除.\n");
		return;
	}

	// 前移
	for (size_t i = pos; i < pl->size - 1; i++)
	{
		pl->arr[i] = pl->arr[i + 1];
	}
	pl->size--;
	printf("删除成功!\n");
}

// ==================== 3. 查找图书（含借阅者信息） ====================

void LibraryFind(Library* pl)
{
	int choice;
	printf("请选择查找方式:\n");
	printf("1.按书号查找\n");
	printf("2.按书名查找\n");
	printf("请选择:>");
	scanf("%d", &choice);

	if (choice == 1)
	{
		char id[ID_MAX];
		printf("请输入需要查找的书号:>");
		scanf("%s", id);
		int pos = FindById(pl, id);
		if (pos == -1)
		{
			printf("未找到该书号的图书!\n");
			return;
		}

		// --- 基本信息 ---
		printf("\n========== 图书详细信息 ==========\n");
		printf("书号:    %s\n", pl->arr[pos].id);
		printf("书名:    %s\n", pl->arr[pos].name);
		printf("作者:    %s\n", pl->arr[pos].author);
		printf("出版社:  %s\n", pl->arr[pos].press);
		printf("总库存:  %d\n", pl->arr[pos].total);
		printf("现存量:  %d\n", pl->arr[pos].current);
		printf("入库时间: %s\n", pl->arr[pos].entry_time);

		// --- 借阅信息 ---
		if (pl->arr[pos].is_borrowed > 0)
		{
			printf("\n--- 当前借阅信息 ---\n");
			printf("借阅者:       %s\n", pl->arr[pos].borrower_name);
			printf("借阅者证件号: %s\n", pl->arr[pos].borrower_id);
		}
		else
		{
			printf("\n当前状态: 未借出\n");
		}
		printf("===================================\n");
	}
	else if (choice == 2)
	{
		char name[NAME_MAX];
		printf("请输入需要查找的书名:>");
		scanf("%s", name);
		int found = 0;
		printf("%-10s %-18s %-12s %-18s %-6s %-6s\n",
			"书号", "书名", "作者", "出版社", "总库存", "现存量");
		for (size_t i = 0; i < pl->size; i++)
		{
			if (0 == strcmp(pl->arr[i].name, name))
			{
				printf("%-10s %-18s %-12s %-18s %-6d %-6d\n",
					pl->arr[i].id, pl->arr[i].name,
					pl->arr[i].author, pl->arr[i].press,
					pl->arr[i].total, pl->arr[i].current);
				found = 1;
			}
		}
		if (!found)
			printf("未找到该名称的图书!\n");
	}
	else
	{
		printf("选择无效!\n");
	}
}

// ==================== 4. 修改图书信息 ====================

void LibraryModify(Library* pl)
{
	char id[ID_MAX];
	printf("请输入需要修改的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("要修改的图书不存在!\n");
		return;
	}
	printf("请输入新书名(原:%s):>", pl->arr[pos].name);
	scanf("%s", pl->arr[pos].name);
	printf("请输入新作者(原:%s):>", pl->arr[pos].author);
	scanf("%s", pl->arr[pos].author);
	printf("请输入新出版社(原:%s):>", pl->arr[pos].press);
	scanf("%s", pl->arr[pos].press);
	printf("请输入新总库存(原:%d):>", pl->arr[pos].total);
	scanf("%d", &pl->arr[pos].total);
	printf("请输入新现存量(原:%d):>", pl->arr[pos].current);
	scanf("%d", &pl->arr[pos].current);
	printf("请输入新入库时间(原:%s):>", pl->arr[pos].entry_time);
	scanf("%s", pl->arr[pos].entry_time);
	printf("修改成功!\n");
}

// ==================== 5. 显示全部图书 ====================

void LibraryShow(Library* pl)
{
	if (pl->size == 0)
	{
		printf("图书馆暂无图书!\n");
		return;
	}
	printf("%-10s %-18s %-12s %-18s %-6s %-6s %-10s %-6s\n",
		"书号", "书名", "作者", "出版社",
		"总库存", "现存量", "入库时间", "状态");
	printf("----------------------------------------"
		"---------------------------------------\n");
	for (size_t i = 0; i < pl->size; i++)
	{
		char status[20];
		if (pl->arr[i].is_borrowed > 0)
			sprintf(status, "借出%d", pl->arr[i].is_borrowed);
		else
			strcpy(status, "在架");

		printf("%-10s %-18s %-12s %-18s %-6d %-6d %-10s %-6s\n",
			pl->arr[i].id,
			pl->arr[i].name,
			pl->arr[i].author,
			pl->arr[i].press,
			pl->arr[i].total,
			pl->arr[i].current,
			pl->arr[i].entry_time,
			status);
	}
	printf("共 %zu 种图书\n", pl->size);
}

// ==================== 6. 借阅图书（记录借阅者信息） ====================

void LibraryBorrow(Library* pl)
{
	char id[ID_MAX];
	printf("请输入需要借阅的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("该图书不存在!\n");
		return;
	}

	BookInfo* book = &pl->arr[pos];
	printf("书号:%s  书名:%s  现存量:%d  总库存:%d\n",
		book->id, book->name, book->current, book->total);

	if (book->current <= 0)
	{
		printf("该书现存量不足，无法借阅!\n");
		return;
	}

	// 登记借阅者信息
	char reader_id[READER_ID_MAX];
	char reader_name[READER_NAME_MAX];

	printf("请输入借阅者证件号:>");
	scanf("%s", reader_id);
	printf("请输入借阅者姓名:>");
	scanf("%s", reader_name);

	strcpy(book->borrower_id, reader_id);
	strcpy(book->borrower_name, reader_name);
	book->is_borrowed++;
	book->current--;

	printf("借阅成功!\n");
	printf("书名:%s  借阅者:%s\n",
		book->name, reader_name);
}

// ==================== 7. 归还图书（注销借阅者信息） ====================

void LibraryReturn(Library* pl)
{
	char id[ID_MAX];
	printf("请输入需要归还的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("该图书不存在于本馆，无法归还!\n");
		return;
	}

	BookInfo* book = &pl->arr[pos];

	if (book->current >= book->total)
	{
		printf("该书库存已满，归还失败!\n");
		return;
	}

	if (book->is_borrowed <= 0)
	{
		printf("该书没有被借出记录，无需归还!\n");
		return;
	}

	// 恢复库存
	book->current++;
	book->is_borrowed--;

	printf("归还成功!《%s》现存量:%d\n", book->name, book->current);

	// 注销借阅者信息
	book->borrower_id[0] = '\0';
	book->borrower_name[0] = '\0';
}

// ==================== 8. 按作者查找全部著作 ====================

void LibraryFindByAuthor(Library* pl)
{
	char author[AUTHOR_MAX];
	printf("请输入作者姓名:>");
	scanf("%s", author);

	int found = 0;
	printf("\n========== %s 的著作 ==========\n", author);
	printf("%-10s %-18s %-12s %-18s %-6s %-6s %-10s\n",
		"书号", "书名", "作者", "出版社",
		"总库存", "现存量", "入库时间");
	for (size_t i = 0; i < pl->size; i++)
	{
		if (0 == strcmp(pl->arr[i].author, author))
		{
			printf("%-10s %-18s %-12s %-18s %-6d %-6d %-10s\n",
				pl->arr[i].id, pl->arr[i].name,
				pl->arr[i].author, pl->arr[i].press,
				pl->arr[i].total, pl->arr[i].current,
				pl->arr[i].entry_time);
			found++;
		}
	}
	if (!found)
		printf("未找到该作者的著作!\n");
	else
		printf("共找到 %d 本著作\n", found);
	printf("==================================\n");
}
