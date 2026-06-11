#include "library.h"

// ==================== 顺序表实现 ====================

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

// 容量检查与扩容
static void CheckCapacity(SL* ps)
{
	if (ps->capacity == ps->size)
	{
		int newcapacity = ps->capacity == 0 ? 4 : ps->capacity * 2;
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

// 尾插
void SLPushBack(SL* ps, SLDataType x)
{
	assert(ps);
	CheckCapacity(ps);
	ps->arr[ps->size] = x;
	ps->size++;
}

// 头插
void SLPushFront(SL* ps, SLDataType x)
{
	assert(ps);
	CheckCapacity(ps);
	// 数据后移
	for (int i = ps->size - 1; i >= 0; i--)
	{
		ps->arr[i + 1] = ps->arr[i];
	}
	ps->arr[0] = x;
	ps->size++;
}

// 打印顺序表（原始实现，图书系统使用 LibraryShow 代替）
void SLPrint(SL ps)
{
	for (size_t i = 0; i < ps.size; i++)
	{
		printf("%d ", ps.arr[i]);
	}
	printf("\n");
}

// ==================== 图书馆功能实现 ====================

// 初始化
void LibraryInit(Library* pl)
{
	SLInit(pl);
}

// 销毁
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
		{
			return (int)i;
		}
	}
	return -1;
}

// 采编入库
void LibraryAdd(Library* pl)
{
	BookInfo info;
	printf("请输入书号:>");
	scanf("%s", info.id);

	int pos = FindById(pl, info.id);
	if (pos != -1)
	{
		int addNum;
		printf("该书已存在，当前总库存:%d，现存量:%d\n", pl->arr[pos].total, pl->arr[pos].current);
		printf("请输入新增数量:>");
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
	printf("请输入入库数量:>");
	scanf("%d", &info.total);
	info.current = info.total;

	SLPushBack(pl, info);
	printf("采编入库成功!\n");
}

// 删除图书
void LibraryDel(Library* pl)
{
	char id[ID_MAX];
	printf("请输入要删除的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("要删除的图书不存在!\n");
		return;
	}
	for (size_t i = pos; i < pl->size - 1; i++)
	{
		pl->arr[i] = pl->arr[i + 1];
	}
	pl->size--;
	printf("删除成功!\n");
}

// 显示全部图书
void LibraryShow(Library* pl)
{
	if (pl->size == 0)
	{
		printf("图书馆暂无图书!\n");
		return;
	}
	printf("%-15s %-20s %-15s %-20s %-8s %-8s\n", "书号", "书名", "作者", "出版社", "总库存", "现存量");
	for (size_t i = 0; i < pl->size; i++)
	{
		printf("%-15s %-20s %-15s %-20s %-8d %-8d\n",
			pl->arr[i].id,
			pl->arr[i].name,
			pl->arr[i].author,
			pl->arr[i].press,
			pl->arr[i].total,
			pl->arr[i].current);
	}
}

// 修改图书信息
void LibraryModify(Library* pl)
{
	char id[ID_MAX];
	printf("请输入要修改的图书书号:>");
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
	printf("请输入新总库存量(原:%d):>", pl->arr[pos].total);
	scanf("%d", &pl->arr[pos].total);
	printf("请输入新现存量(原:%d):>", pl->arr[pos].current);
	scanf("%d", &pl->arr[pos].current);
	printf("修改成功!\n");
}

// 查找图书
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
		printf("请输入要查找的书号:>");
		scanf("%s", id);
		int pos = FindById(pl, id);
		if (pos == -1)
		{
			printf("未找到该书号的图书!\n");
			return;
		}
		printf("%-15s %-20s %-15s %-20s %-8s %-8s\n", "书号", "书名", "作者", "出版社", "总库存", "现存量");
		printf("%-15s %-20s %-15s %-20s %-8d %-8d\n",
			pl->arr[pos].id,
			pl->arr[pos].name,
			pl->arr[pos].author,
			pl->arr[pos].press,
			pl->arr[pos].total,
			pl->arr[pos].current);
	}
	else if (choice == 2)
	{
		char name[NAME_MAX];
		printf("请输入要查找的书名:>");
		scanf("%s", name);
		int found = 0;
		printf("%-15s %-20s %-15s %-20s %-8s %-8s\n", "书号", "书名", "作者", "出版社", "总库存", "现存量");
		for (size_t i = 0; i < pl->size; i++)
		{
			if (0 == strcmp(pl->arr[i].name, name))
			{
				printf("%-15s %-20s %-15s %-20s %-8d %-8d\n",
					pl->arr[i].id,
					pl->arr[i].name,
					pl->arr[i].author,
					pl->arr[i].press,
					pl->arr[i].total,
					pl->arr[i].current);
				found = 1;
			}
		}
		if (!found)
		{
			printf("未找到该书名的图书!\n");
		}
	}
	else
	{
		printf("选择无效!\n");
	}
}

// 借阅图书
void LibraryBorrow(Library* pl)
{
	char id[ID_MAX];
	printf("请输入要借阅的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("该图书不存在!\n");
		return;
	}
	if (pl->arr[pos].current <= 0)
	{
		printf("该书现存量不足，无法借阅!\n");
		return;
	}
	pl->arr[pos].current--;
	printf("借阅成功!《%s》剩余现存量:%d\n", pl->arr[pos].name, pl->arr[pos].current);
}

// 归还图书
void LibraryReturn(Library* pl)
{
	char id[ID_MAX];
	printf("请输入要归还的图书书号:>");
	scanf("%s", id);
	int pos = FindById(pl, id);
	if (pos == -1)
	{
		printf("该图书不属于本馆，请检查书号!\n");
		return;
	}
	if (pl->arr[pos].current >= pl->arr[pos].total)
	{
		printf("该书库存已满，归还失败!\n");
		return;
	}
	pl->arr[pos].current++;
	printf("归还成功!《%s》剩余现存量:%d\n", pl->arr[pos].name, pl->arr[pos].current);
}
