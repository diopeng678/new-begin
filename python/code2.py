#算术运算符

#print(10/0)#除零异常

#//取整除
print(10//3)
print(-10//3)#负数取小的

#**乘方开方操作
print(10**2)

a = 10
b = 11
print(a<b)#返回值是布尔类型
#字符串比大小是以字典序大的顺序为准的比首字母

c = "peng"
d = "wang"
print(c<d)

#浮点数在比较相等时需要用范围进行比较
a1 = 0.1
b1 = 0.2
c1 = 0.3
print(a1+b1 == c1)
print(-0.000001 < c1 - a1 - b1 < 0.0000001)

#逻辑运算符 and or not
print(a1 < b1 and b1 < c1)


