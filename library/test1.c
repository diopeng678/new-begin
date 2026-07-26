#include "library1.h"

#define ADMIN_PASSWORD "1234666"   // 管理员默认密码

// ==================== 借阅者菜单 ====================

void reader_menu()
{
	printf("\n");
	printf("***********************************\n");
	printf("*****   图书管理系统 - 借阅者   *****\n");
	printf("***********************************\n");
	printf("***** 1.查找图书    2.借阅图书 *****\n");
	printf("***** 3.归还图书    4.显示全部 *****\n");
	printf("***** 5.作者著作查询             *****\n");
	printf("***** 0.返回主菜单               *****\n");
	printf("***********************************\n");
}

// ==================== 借阅者模式 ====================

void reader_mode(Library* pl)
{
	int input = 0;
	do
	{
		reader_menu();
		printf("请选择:>");
		if (scanf("%d", &input) != 1)
		{
			printf("输入无效!\n");
			while (getchar() != '\n');
			continue;
		}
		switch (input)
		{
		case 1: LibraryFind(pl);         break;
		case 2: LibraryBorrow(pl);       break;
		case 3: LibraryReturn(pl);       break;
		case 4: LibraryShow(pl);         break;
		case 5: LibraryFindByAuthor(pl); break;
		case 0: break;
		default: printf("选择无效，请重新选择!\n"); break;
		}
	} while (input);
}

// ==================== 管理员登录 ====================

int admin_login()
{
	char pwd[50];
	int attempts = 3;
	while (attempts > 0)
	{
		printf("请输入管理员密码(剩余%d次):>", attempts);
		scanf("%s", pwd);
		if (strcmp(pwd, ADMIN_PASSWORD) == 0)
		{
			printf("登录成功!\n");
			return 1;
		}
		attempts--;
		printf("密码错误!\n");
	}
	printf("密码错误次数过多，返回主菜单.\n");
	return 0;
}

// ==================== 管理员菜单 ====================

void admin_menu()
{
	printf("\n");
	printf("***********************************\n");
	printf("*****   图书管理系统 - 管理员   *****\n");
	printf("***********************************\n");
	printf("*****  1.采编入库    2.删除图书 *****\n");
	printf("*****  3.查找图书    4.修改图书 *****\n");
	printf("*****  5.显示全部    6.借阅图书 *****\n");
	printf("*****  7.归还图书    8.作者著作 *****\n");
	printf("*****  0.返回主菜单               *****\n");
	printf("***********************************\n");
}

// ==================== 管理员模式 ====================

void admin_mode(Library* pl)
{
	if (!admin_login())
		return;

	int input = 0;
	do
	{
		admin_menu();
		printf("请选择:>");
		if (scanf("%d", &input) != 1)
		{
			printf("输入无效!\n");
			while (getchar() != '\n');
			continue;
		}
		switch (input)
		{
		case 1: LibraryAdd(pl);           break;
		case 2: LibraryDel(pl);           break;
		case 3: LibraryFind(pl);          break;
		case 4: LibraryModify(pl);        break;
		case 5: LibraryShow(pl);          break;
		case 6: LibraryBorrow(pl);        break;
		case 7: LibraryReturn(pl);        break;
		case 8: LibraryFindByAuthor(pl);  break;
		case 0: break;
		default: printf("选择无效，请重新选择!\n"); break;
		}
	} while (input);
}

// ==================== 主菜单 ====================

void main_menu()
{
	printf("\n");
	printf("***********************************\n");
	printf("*****     图书信息管理系统     *****\n");
	printf("***********************************\n");
	printf("*****  1.管理员登录             *****\n");
	printf("*****  2.借阅者入口             *****\n");
	printf("*****  0.退出系统               *****\n");
	printf("***********************************\n");
}

int main()
{
	Library lib;
	LibraryInit(&lib);

	int input = 0;
	do
	{
		main_menu();
		printf("请选择:>");
		if (scanf("%d", &input) != 1)
		{
			printf("输入无效!\n");
			while (getchar() != '\n');
			continue;
		}
		switch (input)
		{
		case 1: admin_mode(&lib);  break;
		case 2: reader_mode(&lib); break;
		case 0:
			printf("欢迎下次使用!\n");
			break;
		default:
			printf("选择无效，请重新选择!\n");
			break;
		}
	} while (input);

	LibraryDesTory(&lib);
	return 0;
}
