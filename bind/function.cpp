#include <functional>
#include<iostream>
using namespace std;


int add(int a, int b) { return a + b; }

struct Mul {
    int operator()(int a, int b) { return a * b; }
};

int main() {
    function<int(int, int)> f1 = add;           // 函数指针
    function<int(int, int)> f2 = Mul();         // 仿函数
	function<int(int, int)> f3 = [](int a, int b) { return a - b; };  // Lambda中括号里面是捕获列表，()里面是参数列表，{}里面是函数体

    cout << f1(1, 2) << endl;   // 3
    cout << f2(2, 3) << endl;   // 6
    cout << f3(5, 2) << endl;   // 3
    return 0;
}