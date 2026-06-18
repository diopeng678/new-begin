#include <functional>
#include <iostream>
using namespace std;  
using namespace placeholders;  // 有这个东西  _1,  _2   才不会报错
//bind函数的作用：绑定参数，调整参数顺序，绑死参数等 通俗来讲就是把一个函数的参数进行重新组合，形成一个新的函数对象。它可以用来调整参数顺序，绑死某些参数，或者创建一个新的函数对象来调用原函数。

int Sub(int a, int b) { return (a - b) * 10; }

//int main() {
//    // 正常调用
//    auto sub1 = bind(Sub, _1, _2);
//    cout << sub1(10, 5) << endl;   // 50
//
//    // 调整参数顺序
//    auto sub2 = bind(Sub, _2, _1);
//    cout << sub2(10, 5) << endl;   // -50
//
//    // 绑死第一个参数
//    auto sub3 = bind(Sub, 100, _1);
//    cout << sub3(5) << endl;       // 950
//
//    // 绑死第二个参数
//    auto sub4 = bind(Sub, _1, 100);
//    cout << sub4(5) << endl;       // -950
//
//}