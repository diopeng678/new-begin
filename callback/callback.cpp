#include<stdio.h>
#include<iostream>
using namespace std;

void buy() 
{
	cout << "buying..." << endl;
}

void make(void(*callback)())//回调函数，参数是一个函数指针，指向一个没有参数和返回值的函数
{
	callback();
	cout << "making..." << endl;

}
//本质相当于在在函数中调其他函数，回调函数的作用是将一个函数作为参数传递给另一个函数，在另一个函数中调用这个函数，从而实现代码的复用和灵活性。

int main()
{
	make(buy);//回调函数相当于将buy函数的地址传递给make函数，make函数在执行时会调用buy函数
	return 0;
}