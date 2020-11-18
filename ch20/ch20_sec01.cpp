#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <map>
#include <set>
#include <forward_list>
#include <algorithm>
#include <unordered_set>
#include <functional>
#include <queue>
using namespace std;

// -------------------------------
// 函数调用运算符与std::function类模板
// --------------------------------

// (1) 函数对象, 调用形式, 可调用调用

// 重载 operator()的类, 该类的对象叫做"函数对象"

// 调用形式: 返回值 + 参数, 一种调用形式对应一个函数类型.

// 可调用对象
// 1. 函数
// 2. 重载operator()类的对象.

// (2) std::function

namespace _nmsp1
{
	class A
	{
	public:
		int m_i;

		A(int i) : m_i(i) {}

		int operator()(int value) const // 重载 函数调用运算符, 类A的对象叫做"函数对象"
		{
			return value < 0 ? 0 : value;
		}
	};

	int func(int value) // 该函数和类A的operator()有相同的 调用形式.
	{
		value++;
		return value;
	}

} // namespace _nmsp1

// 函数重载时, std::function的行为
namespace _nmsp2
{
	int func(int value) // 同 _nmsp1::func()
	{
		value++;
		return value;
	}

	// func的重载版本
	int func(int value1, int value2)
	{
		value1 += value2;
		return value1;
	}
} // namespace _nmsp2

void test01()
{
	int i = 10;
	_nmsp1::A a(100);	  // a(100): 初始化
	cout << a(i) << endl; // a(i): 函数调用, 等价于如下
	cout << a.operator()(i) << endl;
}

void test02()
{
	_nmsp1::A a1(100);

	std::map<std::string, int (*)(int)> func_map;
	func_map["aaa"] = _nmsp1::func; // 可以放函数指针, 允许_nmsp1::func有重载版本
	// func_map["bbb"] = &a1;			// 报错, 不可以放函数对象

	cout << func_map["aaa"](10) << endl;
}

// std::function 类模板可以解决 函数指针 不能放 函数对象 的问题.
void test03()
{
	_nmsp1::A a1(100);

	std::map<std::string, std::function<int(int)>> func_map;

	std::function<int(int)> f1 = a1;		   // 可以放函数对象
	std::function<int(int)> f2 = _nmsp1::func; // 可以放函数指针, 注意: _nmsp1::func不能有重载版本, 如果放_nmsp2::func就会报错.
	func_map["aaa"] = f1;
	func_map["bbb"] = f2;

	cout << func_map["aaa"](-10) << endl;
	cout << func_map["bbb"](100) << endl;
}

// 通过定义函数指针, 解决 std::function 不能处理函数 重载 的问题.
void test04()
{
	int (*fp1)(int) = _nmsp2::func;
	int (*fp2)(int, int) = _nmsp2::func; // 直接定义函数指针是可以识别重载版本的.

	std::map<std::string, std::function<int(int)>> func_map;
	func_map["aaa"] = fp1; // 通过直接定义函数指针, 解决 std::function 不能处理函数重载的问题.

	cout << func_map["aaa"](20) << endl;
}

int main()
{
	// test01();
	// test02();
	// test03();
	test04();

	return 0;
}
