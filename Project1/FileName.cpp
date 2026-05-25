#include <iostream>
using namespace std;

// 除法函数：除数为0时抛出异常
double divide(int a, int b) {
    if (b == 0) {
        throw "除数不能为0！";  // 抛出字符串常量
    }
    return (double)a / b;
}

int main() {
    int x, y;
    cout << "请输入两个整数: ";
    cin >> x >> y;
    //在执行除法操作时，可能会抛出异常，因此使用try-catch块来捕获异常并处理
    //如果有异常发生，程序不会崩溃，而是会输出错误信息并继续执行后续代码 错误信息就是catch块中的代码

    try {
        double result = divide(x, y);
        cout << "结果: " << result << endl;
    }

    catch (const char* e) {  // 捕获字符串异常
        cout << "发生错误: " << e << endl;
    }

    cout << "程序继续执行..." << endl;
    //jixi
    return 0;
}