#include "library.h"

void menu()
{
	printf("***********************************\n");
	printf("*****     图书信息管理系统     *****\n");
	printf("***********************************\n");
	printf("***** 1.采编入库              *****\n");
	printf("***** 2.删除图书              *****\n");
	printf("***** 3.查找图书              *****\n");
	printf("***** 4.修改图书              *****\n");
	printf("***** 5.显示全部图书          *****\n");
	printf("***** 6.借阅图书              *****\n");
	printf("***** 7.归还图书              *****\n");
	printf("***** 0.退出系统              *****\n");
	printf("***********************************\n");
}

int main()
{
	Library lib;
	LibraryInit(&lib);
	int input = 0;
	do
	{
		menu();
		printf("请选择:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			LibraryAdd(&lib);
			break;
		case 2:
			LibraryDel(&lib);
			break;
		case 3:
			LibraryFind(&lib);
			break;
		case 4:
			LibraryModify(&lib);
			break;
		case 5:
			LibraryShow(&lib);
			break;
		case 6:
			LibraryBorrow(&lib);
			break;
		case 7:
			LibraryReturn(&lib);
			break;
		case 0:
			printf("欢迎下次使用!\n");
			break;
		default:
			printf("选择错误，请重新选择!\n");
			break;
		}
	} while (input);
	LibraryDesTory(&lib);
	return 0;
}
