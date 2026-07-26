#include "library.h"

#define ADMIN_PASSWORD "admin123"   // ????????????

// ==================== ???????? ====================

void reader_menu()
{
	printf("\n");
	printf("***********************************\n");
	printf("*****   ???????? - ??????   *****\n");
	printf("***********************************\n");
	printf("***** 1.???????    2.??????? *****\n");
	printf("***** 3.?臄???    4.?????? *****\n");
	printf("***** 5.???????????             *****\n");
	printf("***** 0.?????????               *****\n");
	printf("***********************************\n");
}

// ==================== ???????? ====================

void reader_mode(Library* pl)
{
	int input = 0;
	do
	{
		reader_menu();
		printf("?????:>");
		if (scanf("%d", &input) != 1)
		{
			printf("??????完!\n");
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
		default: printf("?????完???????????!\n"); break;
		}
	} while (input);
}

// ==================== ???????? ====================

int admin_login()
{
	char pwd[50];
	int attempts = 3;
	while (attempts > 0)
	{
		printf("??????????????(???%d??):>", attempts);
		scanf("%s", pwd);
		if (strcmp(pwd, ADMIN_PASSWORD) == 0)
		{
			printf("??????!\n");
			return 1;
		}
		attempts--;
		printf("???????!\n");
	}
	printf("???????????????????????.\n");
	return 0;
}

// ==================== ???????? ====================

void admin_menu()
{
	printf("\n");
	printf("***********************************\n");
	printf("*****   ???????? - ?????   *****\n");
	printf("***********************************\n");
	printf("*****  1.??????    2.?????? *****\n");
	printf("*****  3.???????    4.?????? *****\n");
	printf("*****  5.??????    6.??????? *****\n");
	printf("*****  7.?臄???    8.???????? *****\n");
	printf("*****  0.?????????               *****\n");
	printf("***********************************\n");
}

// ==================== ??????? ====================

void admin_mode(Library* pl)
{
	if (!admin_login())
		return;

	int input = 0;
	do
	{
		admin_menu();
		printf("?????:>");
		if (scanf("%d", &input) != 1)
		{
			printf("??????完!\n");
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
		default: printf("?????完???????????!\n"); break;
		}
	} while (input);
}

// ==================== ????? ====================

void main_menu()
{
	printf("\n");
	printf("***********************************\n");
	printf("*****     ????????????     *****\n");
	printf("***********************************\n");
	printf("*****  1.????????             *****\n");
	printf("*****  2.?????????             *****\n");
	printf("*****  0.?????               *****\n");
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
		printf("?????:>");
		if (scanf("%d", &input) != 1)
		{
			printf("??????完!\n");
			while (getchar() != '\n');
			continue;
		}
		switch (input)
		{
		case 1: admin_mode(&lib);  break;
		case 2: reader_mode(&lib); break;
		case 0:
			printf("?????????!\n");
			break;
		default:
			printf("?????完???????????!\n");
			break;
		}
	} while (input);

	LibraryDesTory(&lib);
	return 0;
}
