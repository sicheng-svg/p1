# 现代 C++ 语法特性详解（C++11 / 14 / 17 / 20）

> 本文档覆盖 C++11、C++14、C++17、C++20 中引入的核心语言与标准库特性，每个特性附有完整示例和原理说明。

---

## 目录

1. [C++11 特性](#c11-特性)
   - [auto 类型推导](#1-auto-类型推导)
   - [decltype](#2-decltype)
   - [nullptr](#3-nullptr)
   - [范围 for 循环](#4-范围-for-循环)
   - [初始化列表](#5-初始化列表)
   - [Lambda 表达式](#6-lambda-表达式)
   - [右值引用与移动语义](#7-右值引用与移动语义)
   - [智能指针](#8-智能指针)
   - [可变参数模板](#9-可变参数模板)
   - [constexpr](#10-constexpr)
   - [static_assert](#11-static_assert)
   - [类型别名 using](#12-类型别名-using)
   - [委托构造函数与继承构造函数](#13-委托构造函数与继承构造函数)
   - [override 与 final](#14-override-与-final)
   - [枚举类 enum class](#15-枚举类-enum-class)
   - [原始字符串字面量](#16-原始字符串字面量)
   - [noexcept](#17-noexcept)
   - [线程支持库](#18-线程支持库)
   - [std::tuple](#19-stdtuple)
   - [std::function 与 std::bind](#20-stdfunction-与-stdbind)
   - [无序容器](#21-无序容器)
   - [数组 std::array](#22-数组-stdarray)
   - [正则表达式](#23-正则表达式)
   - [移动语义相关：std::move / std::forward](#24-移动语义相关stdmove--stdforward)
   - [explicit 转换运算符](#25-explicit-转换运算符)
   - [用户定义字面量](#26-用户定义字面量)
2. [C++14 特性](#c14-特性)
   - [通用 Lambda](#1-通用-lambda)
   - [Lambda 捕获初始化](#2-lambda-捕获初始化)
   - [函数返回类型推导](#3-函数返回类型推导)
   - [变量模板](#4-变量模板)
   - [constexpr 放宽限制](#5-constexpr-放宽限制)
   - [std::make_unique](#6-stdmake_unique)
   - [二进制字面量与数字分隔符](#7-二进制字面量与数字分隔符)
   - [std::integer_sequence](#8-stdinteger_sequence)
   - [std::exchange](#9-stdexchange)
3. [C++17 特性](#c17-特性)
   - [结构化绑定](#1-结构化绑定)
   - [if / switch 初始化语句](#2-if--switch-初始化语句)
   - [std::optional](#3-stdoptional)
   - [std::variant](#4-stdvariant)
   - [std::any](#5-stdany)
   - [std::string_view](#6-stdstring_view)
   - [折叠表达式](#7-折叠表达式)
   - [if constexpr](#8-if-constexpr)
   - [类模板参数推导（CTAD）](#9-类模板参数推导ctad)
   - [内联变量](#10-内联变量)
   - [嵌套命名空间](#11-嵌套命名空间)
   - [[[nodiscard]] / [[maybe_unused]] / [[fallthrough]]](#12-nodiscard--maybe_unused--fallthrough)
   - [std::filesystem](#13-stdfilesystem)
   - [并行算法](#14-并行算法)
   - [std::byte](#15-stdbyte)
   - [std::invoke 与 std::apply](#16-stdinvoke-与-stdapply)
4. [C++20 特性](#c20-特性)
   - [概念（Concepts）](#1-概念concepts)
   - [范围库（Ranges）](#2-范围库ranges)
   - [协程（Coroutines）](#3-协程coroutines)
   - [模块（Modules）](#4-模块modules)
   - [三路比较运算符 <=>](#5-三路比较运算符-)
   - [consteval 与 constinit](#6-consteval-与-constinit)
   - [Lambda 增强](#7-lambda-增强)
   - [聚合初始化改进](#8-聚合初始化改进)
   - [std::span](#9-stdspan)
   - [std::format](#10-stdformat)
   - [std::jthread](#11-stdjthread)
   - [std::source_location](#12-stdsource_location)
   - [std::bit_cast](#13-stdbit_cast)
   - [using enum](#14-using-enum)

---

## C++11 特性

### 1. auto 类型推导

**原理：** 编译器根据初始化表达式自动推导变量类型，减少冗余的类型书写。`auto` 遵循模板参数推导规则，会去掉顶层 `const` 和引用。

```cpp
#include <vector>
#include <map>
#include <iostream>

int main() {
    // 基础类型推导
    auto i = 42;          // int
    auto d = 3.14;        // double
    auto s = "hello";     // const char*
    auto b = true;        // bool

    // 保留 const/引用需要显式声明
    const auto ci = 42;   // const int
    auto& ri = i;         // int&
    const auto& cri = i;  // const int&

    // 容器场景大幅减少冗余
    std::map<std::string, std::vector<int>> m;
    // 不用 auto:
    // std::map<std::string, std::vector<int>>::iterator it = m.begin();
    auto it = m.begin();  // 简洁

    // 范围 for 中使用 auto
    std::vector<int> v = {1, 2, 3, 4, 5};
    for (auto x : v) {
        std::cout << x << " ";
    }

    // auto 推导函数返回类型（C++14 起支持，C++11 可配合 -> 尾置返回）
    auto add = [](int a, int b) -> int { return a + b; };
    return 0;
}
```

**注意：** `auto` 推导 `std::initializer_list` 时需要小心；`auto` 不能用于函数参数（C++20 的缩写函数模板支持此功能）。

---

### 2. decltype

**原理：** `decltype(expr)` 在编译期获取表达式的精确类型，不会执行表达式，常用于模板元编程和尾置返回类型。

```cpp
#include <iostream>
#include <vector>

int main() {
    int x = 0;
    double y = 3.14;

    decltype(x) a = 5;          // int
    decltype(y) b = 2.71;       // double
    decltype(x + y) c = x + y;  // double（表达式类型）

    // decltype 区分左值和右值
    int arr[5];
    decltype(arr[0]) ref = arr[0];  // int&（arr[0] 是左值）
    decltype(42) val = 42;          // int（42 是右值）

    // 配合 auto 实现完美类型推导（C++11 尾置返回）
    auto func = [](auto a, auto b) -> decltype(a + b) {
        return a + b;
    };

    // decltype(auto)：C++14 引入，保留引用/const 语义
    // auto x2 = func(1, 2.0);          // double（auto 去引用）
    // decltype(auto) x3 = func(1, 2.0); // 保留原始类型

    std::vector<int> v = {1, 2, 3};
    decltype(v)::value_type elem = 10;  // int
    std::cout << elem << std::endl;

    return 0;
}
```

---

### 3. nullptr

**原理：** `nullptr` 是类型 `std::nullptr_t` 的字面量，专门用于表示空指针，解决了旧式 `NULL`（通常是整数 `0`）导致的重载歧义问题。

```cpp
#include <iostream>

void f(int) { std::cout << "f(int)\n"; }
void f(int*) { std::cout << "f(int*)\n"; }

int main() {
    f(0);       // 调用 f(int)
    f(nullptr); // 调用 f(int*)，不歧义

    // 旧问题：f(NULL) 可能调用 f(int)，因为 NULL == 0

    int* p = nullptr;   // 更安全、语义清晰
    if (p == nullptr) {
        std::cout << "p is null\n";
    }

    // nullptr 可以转换为任意指针类型
    char* cp = nullptr;
    void* vp = nullptr;

    // 但不能隐式转换为整数
    // int n = nullptr; // 编译错误

    return 0;
}
```

---

### 4. 范围 for 循环

**原理：** 基于范围的 `for` 循环是语法糖，编译器展开为迭代器调用（`begin()`/`end()`），适用于任何提供这两个函数的类型（包括数组）。

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>

int main() {
    // 数组
    int arr[] = {1, 2, 3, 4, 5};
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    // vector（值拷贝）
    std::vector<std::string> v = {"hello", "world"};
    for (auto s : v) std::cout << s << " ";  // 拷贝

    // 引用避免拷贝
    for (auto& s : v) s += "!";  // 修改原始元素

    // const 引用只读
    for (const auto& s : v) std::cout << s << " ";
    std::cout << "\n";

    // map 遍历 pair
    std::map<std::string, int> m = {{"a", 1}, {"b", 2}};
    for (const auto& kv : m) {
        std::cout << kv.first << "=" << kv.second << " ";
    }

    // 编译器展开等价为：
    // auto&& __range = v;
    // auto __begin = begin(__range);
    // auto __end = end(__range);
    // for (; __begin != __end; ++__begin) { auto s = *__begin; ... }

    return 0;
}
```

---

### 5. 初始化列表

**原理：** 统一初始化语法（`{}`）和 `std::initializer_list<T>` 允许用大括号初始化任意对象，消除了不同初始化方式之间的不一致性，并防止窄化转换。

```cpp
#include <iostream>
#include <vector>
#include <initializer_list>

class Point {
public:
    int x, y;
    // 列表初始化构造函数
    Point(int x, int y) : x(x), y(y) {}
};

class Config {
public:
    std::vector<int> values;
    Config(std::initializer_list<int> il) : values(il) {}
};

int main() {
    // 统一初始化
    int a{42};
    double d{3.14};
    int arr[]{1, 2, 3};

    // 防止窄化转换（编译错误）
    // int x{3.14}; // 错误：double -> int 是窄化

    Point p{10, 20};  // 聚合初始化

    Config c{1, 2, 3, 4, 5};
    for (int v : c.values) std::cout << v << " ";
    std::cout << "\n";

    // std::vector 初始化
    std::vector<int> v{1, 2, 3, 4, 5};
    std::vector<std::pair<int,int>> vp{{1,2}, {3,4}};

    // 注意：vector<int>(5) 创建5个0，vector<int>{5} 创建含1的向量
    std::vector<int> v1(5);   // {0,0,0,0,0}
    std::vector<int> v2{5};   // {5}

    return 0;
}
```

---

### 6. Lambda 表达式

**原理：** Lambda 是匿名函数对象（闭包），编译器为其生成一个独一无二的匿名类，重载了 `operator()`。捕获列表控制如何访问外部变量。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

int main() {
    // 基本语法: [捕获列表](参数列表) -> 返回类型 { 函数体 }
    auto add = [](int a, int b) -> int { return a + b; };
    std::cout << add(3, 4) << "\n";  // 7

    // 返回类型可以省略（自动推导）
    auto mul = [](int a, int b) { return a * b; };

    // 捕获外部变量
    int factor = 3;
    auto scale = [factor](int x) { return x * factor; };  // 值捕获（拷贝）
    auto scaleRef = [&factor](int x) { return x * factor; };  // 引用捕获
    auto scaleMod = [&factor](int x) { factor = x; };  // 修改外部变量

    // = 捕获所有（值），& 捕获所有（引用）
    auto all_val = [=](int x) { return x * factor; };
    auto all_ref = [&](int x) { factor = x; return factor; };

    // mutable：允许修改值捕获的副本（不影响原始变量）
    int count = 0;
    auto inc = [count]() mutable { return ++count; };  // count 的副本被修改
    inc(); inc(); inc();
    std::cout << count << "\n";  // 仍然是 0

    // Lambda 在算法中
    std::vector<int> v{5, 3, 1, 4, 2};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a < b; });
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << " "; });

    // Lambda 赋值给 std::function
    std::function<int(int)> fn = [factor](int x) { return x * factor; };

    // 立即调用（IIFE）
    auto result = [](int x, int y) { return x + y; }(3, 4);
    std::cout << result << "\n";

    return 0;
}
```

**底层原理：** 编译器将 Lambda 转换为类似以下的代码：
```cpp
// [factor](int x) { return x * factor; }
// 等价于：
struct __lambda_1 {
    int factor;  // 捕获的成员
    __lambda_1(int f) : factor(f) {}
    int operator()(int x) const { return x * factor; }
};
```

---

### 7. 右值引用与移动语义

**原理：** 右值引用 `T&&` 绑定到右值（临时对象），使得移动构造/移动赋值可以"窃取"资源而非复制，将 O(n) 的拷贝变成 O(1) 的指针转移。

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <utility>

class Buffer {
public:
    size_t size;
    int* data;

    // 普通构造
    Buffer(size_t n) : size(n), data(new int[n]) {
        std::cout << "构造 size=" << n << "\n";
    }

    // 拷贝构造（深拷贝）
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + size, data);
        std::cout << "拷贝构造\n";
    }

    // 移动构造（O(1)，窃取资源）
    Buffer(Buffer&& other) noexcept
        : size(other.size), data(other.data) {
        other.data = nullptr;  // 置空，防止析构时 double-free
        other.size = 0;
        std::cout << "移动构造\n";
    }

    // 拷贝赋值
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // 移动赋值
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~Buffer() { delete[] data; }
};

Buffer makeBuffer(size_t n) {
    Buffer b(n);
    return b;  // NRVO 或移动
}

int main() {
    Buffer b1(100);             // 构造
    Buffer b2(b1);              // 拷贝构造（深拷贝）
    Buffer b3(std::move(b1));   // 移动构造（b1 之后不可用）
    Buffer b4 = makeBuffer(50); // 移动构造（临时对象）

    // std::string 和 vector 等都实现了移动语义
    std::string s1 = "hello world";
    std::string s2 = std::move(s1);  // s1 变为空串，O(1)

    std::vector<Buffer> vec;
    vec.push_back(Buffer(200));     // 移动（临时对象）
    vec.emplace_back(300);          // 直接在容器内构造，更高效

    return 0;
}
```

**左值 vs 右值：**

| 类别 | 描述 | 示例 |
|------|------|------|
| 左值（lvalue） | 有名字，有地址，可取地址 | `x`, `arr[0]`, `*p` |
| 右值（rvalue） | 临时的，无持久地址 | `42`, `x+y`, `func()` |
| 右值引用 `T&&` | 绑定右值，允许移动 | `int&& r = 42;` |

---

### 8. 智能指针

**原理：** RAII（资源获取即初始化）封装原始指针，析构时自动释放资源。三种智能指针满足不同所有权语义。

```cpp
#include <memory>
#include <iostream>

struct Resource {
    int id;
    Resource(int id) : id(id) { std::cout << "构造 " << id << "\n"; }
    ~Resource() { std::cout << "析构 " << id << "\n"; }
    void use() { std::cout << "使用 " << id << "\n"; }
};

// ===== unique_ptr =====
// 独占所有权，不可拷贝，可移动
void demo_unique() {
    auto up1 = std::make_unique<Resource>(1);
    up1->use();

    // 转移所有权
    auto up2 = std::move(up1);  // up1 变为 nullptr
    if (!up1) std::cout << "up1 为空\n";

    // 自定义删除器
    auto del = [](Resource* r) { std::cout << "自定义删除\n"; delete r; };
    std::unique_ptr<Resource, decltype(del)> up3(new Resource(3), del);
}  // 离开作用域自动释放

// ===== shared_ptr =====
// 引用计数共享所有权，线程安全的引用计数操作
void demo_shared() {
    auto sp1 = std::make_shared<Resource>(2);
    std::cout << "引用计数: " << sp1.use_count() << "\n";  // 1

    {
        auto sp2 = sp1;  // 拷贝，引用计数 +1
        std::cout << "引用计数: " << sp1.use_count() << "\n";  // 2
        sp2->use();
    }  // sp2 析构，引用计数 -1

    std::cout << "引用计数: " << sp1.use_count() << "\n";  // 1
}  // sp1 析构，资源释放

// ===== weak_ptr =====
// 不参与引用计数，用于打破循环引用
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // 用 weak_ptr 避免循环引用
    ~Node() { std::cout << "Node 析构\n"; }
};

void demo_weak() {
    auto n1 = std::make_shared<Node>();
    auto n2 = std::make_shared<Node>();
    n1->next = n2;
    n2->prev = n1;  // weak_ptr，不增加引用计数

    // 使用 weak_ptr 前需要 lock()
    if (auto locked = n2->prev.lock()) {
        std::cout << "n1 仍存在\n";
    }
}

int main() {
    demo_unique();
    demo_shared();
    demo_weak();
    return 0;
}
```

---

### 9. 可变参数模板

**原理：** 模板参数包（`...`）允许模板接受任意数量、任意类型的参数，通过递归展开或折叠表达式（C++17）处理参数包。

```cpp
#include <iostream>
#include <string>

// 递归终止（基本情况）
void print() { std::cout << "\n"; }

// 可变参数模板函数
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) std::cout << ", ";
    print(rest...);  // 递归展开
}

// 计算参数个数
template<typename... Args>
constexpr size_t count(Args...) { return sizeof...(Args); }

// 求和（C++11 递归实现）
template<typename T>
T sum(T v) { return v; }

template<typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// 完美转发与可变模板：make_unique 的简化实现
template<typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 类型列表
template<typename... Types>
struct TypeList {
    static constexpr size_t size = sizeof...(Types);
};

int main() {
    print(1, "hello", 3.14, true);     // 1, hello, 3.14, 1
    std::cout << count(1, 2.0, "a") << "\n";  // 3
    std::cout << sum(1, 2, 3, 4, 5) << "\n";  // 15

    TypeList<int, double, std::string> list;
    std::cout << TypeList<int, double, std::string>::size << "\n";  // 3

    return 0;
}
```

---

### 10. constexpr

**原理：** `constexpr` 声明函数或变量可以在**编译期**求值，从而将运算从运行时移到编译时，提高性能并允许用于模板参数、数组大小等需要编译期常量的场景。

```cpp
#include <iostream>
#include <array>

// constexpr 函数（C++11：只能有单一 return 语句）
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n-1) + fibonacci(n-2);
}

// constexpr 变量
constexpr double PI = 3.14159265358979;
constexpr int MAX_SIZE = factorial(5);  // 编译期计算 = 120

int main() {
    // 用于数组大小（必须是编译期常量）
    std::array<int, factorial(5)> arr;  // std::array<int, 120>
    std::cout << arr.size() << "\n";    // 120

    // 编译期 vs 运行时
    constexpr int ct = factorial(10);   // 编译期求值
    int n = 10;
    int rt = factorial(n);              // 运行时求值（n 不是常量）

    std::cout << ct << "\n";   // 3628800
    std::cout << rt << "\n";   // 3628800

    // constexpr if（C++17，见后文）
    // C++14 放宽了 constexpr 函数限制，允许循环、局部变量等

    static_assert(factorial(5) == 120, "编译期断言");

    return 0;
}
```

---

### 11. static_assert

**原理：** 编译期断言，条件为 `false` 时产生编译错误，常用于模板约束和平台验证。

```cpp
#include <type_traits>
#include <cstdint>

// 验证平台假设
static_assert(sizeof(int) == 4, "需要 32 位 int");
static_assert(sizeof(void*) >= 4, "需要至少 32 位指针");

template<typename T>
class SafeArray {
    static_assert(std::is_trivially_copyable<T>::value,
                  "T 必须是可平凡复制的类型");
    T data[100];
};

template<typename T, typename U>
auto add(T a, U b) {
    static_assert(std::is_arithmetic<T>::value, "T 必须是算术类型");
    static_assert(std::is_arithmetic<U>::value, "U 必须是算术类型");
    return a + b;
}

int main() {
    SafeArray<int> ok;
    // SafeArray<std::string> err;  // 编译错误："T 必须是可平凡复制的类型"

    add(1, 2.0);
    // add("a", "b"); // 编译错误

    return 0;
}
```

---

### 12. 类型别名 using

**原理：** `using` 别名替代 `typedef`，语法更直观，且支持模板别名（`typedef` 不支持）。

```cpp
#include <vector>
#include <map>
#include <functional>

// 等价于 typedef int MyInt;
using MyInt = int;

// 指针类型（比 typedef 更清晰）
using IntPtr = int*;
// typedef int* IntPtr; // 等价但不够直观

// 函数类型
using Comparator = bool(*)(int, int);
using Handler = std::function<void(int, int)>;

// 模板别名（typedef 无法做到）
template<typename T>
using Vec = std::vector<T>;

template<typename K, typename V>
using Dict = std::map<K, V>;

// 成员类型别名
template<typename T>
class Container {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
};

int main() {
    Vec<int> v = {1, 2, 3};
    Dict<std::string, int> d;
    d["age"] = 25;

    Container<double>::value_type x = 3.14;
    return 0;
}
```

---

### 13. 委托构造函数与继承构造函数

```cpp
#include <iostream>
#include <string>

class Person {
    std::string name;
    int age;
    std::string email;

public:
    // 主构造函数
    Person(std::string name, int age, std::string email)
        : name(std::move(name)), age(age), email(std::move(email)) {
        std::cout << "主构造\n";
    }

    // 委托构造函数：调用主构造函数
    Person(std::string name, int age)
        : Person(std::move(name), age, "") {}  // 委托

    Person(std::string name)
        : Person(std::move(name), 0) {}  // 委托

    void print() const {
        std::cout << name << ", " << age << ", " << email << "\n";
    }
};

class Employee : public Person {
    std::string company;

public:
    // 继承 Person 的所有构造函数
    using Person::Person;

    Employee(std::string name, int age, std::string company)
        : Person(name, age), company(std::move(company)) {}
};

int main() {
    Person p1("Alice", 30, "alice@example.com");
    Person p2("Bob", 25);         // 委托构造
    Person p3("Charlie");         // 委托构造

    Employee e1("Dave", 28);      // 继承自 Person 的构造函数
    Employee e2("Eve", 25, "ACME");

    return 0;
}
```

---

### 14. override 与 final

**原理：** 显式标记虚函数重写，让编译器检查签名是否与基类一致，防止因拼写错误或签名不匹配导致的静默错误。

```cpp
#include <iostream>

class Base {
public:
    virtual void speak() const { std::cout << "Base\n"; }
    virtual void greet(int n) { std::cout << "Base greet " << n << "\n"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    // override 显式声明重写，签名不匹配则编译错误
    void speak() const override { std::cout << "Derived\n"; }

    // void greet(double n) override {}  // 编译错误：基类没有此签名

    // final 禁止进一步重写
    void greet(int n) final { std::cout << "Derived greet " << n << "\n"; }
};

// final 类：禁止继承
class NonInheritable final : public Base {
    void speak() const override {}
};

// class TryInherit : public NonInheritable {};  // 编译错误

int main() {
    Base* b = new Derived();
    b->speak();    // Derived
    b->greet(42);  // Derived greet 42
    delete b;
    return 0;
}
```

---

### 15. 枚举类 enum class

**原理：** 强类型枚举，枚举值不隐式转换为整数，枚举名有作用域，解决了传统 `enum` 的命名污染和隐式转换问题。

```cpp
#include <iostream>

// 传统 enum 的问题
// enum Color { Red, Green, Blue };
// enum Fruit { Apple, Banana, Red };  // 错误！Red 重复定义

// enum class 有作用域
enum class Color { Red, Green, Blue };
enum class Fruit { Apple, Banana, Red };  // 不冲突

// 指定底层类型
enum class Status : uint8_t {
    Ok = 0,
    Error = 1,
    Pending = 2
};

int main() {
    Color c = Color::Red;
    Fruit f = Fruit::Red;  // 不冲突

    // 不能隐式转换
    // if (c == 0) {}  // 错误
    // int n = c;      // 错误

    // 显式转换
    int n = static_cast<int>(c);
    std::cout << n << "\n";  // 0

    // switch
    switch (c) {
        case Color::Red:   std::cout << "红色\n"; break;
        case Color::Green: std::cout << "绿色\n"; break;
        case Color::Blue:  std::cout << "蓝色\n"; break;
    }

    // 底层类型
    Status s = Status::Ok;
    auto raw = static_cast<uint8_t>(s);
    std::cout << (int)raw << "\n";  // 0

    return 0;
}
```

---

### 16. 原始字符串字面量

```cpp
#include <iostream>
#include <string>

int main() {
    // 传统字符串需要转义
    std::string old = "C:\\Users\\Alice\\file.txt\nLine2";

    // 原始字符串：R"(内容)" 内容原样保留
    std::string raw = R"(C:\Users\Alice\file.txt
Line2)";

    // 分隔符：R"delim(内容)delim"
    std::string sql = R"sql(
        SELECT *
        FROM users
        WHERE name = "Alice"
        AND age > 18
    )sql";

    // 正则表达式极其实用
    std::string regex_old = "\\d+\\.\\d+";
    std::string regex_raw = R"(\d+\.\d+)";  // 更清晰

    std::cout << raw << "\n";
    std::cout << regex_raw << "\n";

    return 0;
}
```

---

### 17. noexcept

**原理：** `noexcept` 标记函数不抛出异常，使编译器可以进行更激进的优化（如移动语义的条件性使用），STL 容器在 `noexcept` 移动构造时会选择移动而非拷贝。

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

void safe_func() noexcept {
    // 如果此函数抛出异常，程序调用 std::terminate()
    // 使用 noexcept 是一种"承诺"
}

// noexcept(expr)：条件性 noexcept
template<typename T>
void swap_vals(T& a, T& b) noexcept(noexcept(T(std::move(a)))) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

class MyVec {
    int* data;
    size_t sz;
public:
    MyVec(size_t n) : data(new int[n]), sz(n) {}

    // 移动构造声明 noexcept，vector 扩容时才会用移动而非拷贝
    MyVec(MyVec&& other) noexcept : data(other.data), sz(other.sz) {
        other.data = nullptr;
        other.sz = 0;
    }

    ~MyVec() { delete[] data; }
};

int main() {
    // 编译期查询
    std::cout << std::boolalpha;
    std::cout << noexcept(safe_func()) << "\n";  // true

    MyVec a(10), b(20);
    swap_vals(a, b);

    // vector 扩容时：若移动构造是 noexcept，使用移动；否则用拷贝
    std::vector<MyVec> v;
    v.emplace_back(5);   // 触发可能的扩容
    v.emplace_back(10);

    return 0;
}
```

---

### 18. 线程支持库

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <chrono>

std::mutex mtx;
int counter = 0;
std::atomic<int> atomic_counter{0};

// 基本线程
void worker(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "线程 " << id << " 运行\n";
    ++counter;
}

// 原子操作
void atomic_worker() {
    for (int i = 0; i < 1000; ++i)
        ++atomic_counter;  // 原子递增，无需锁
}

// std::async + std::future（异步任务）
int compute(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

int main() {
    // 创建线程
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    t1.join();
    t2.join();
    std::cout << "counter = " << counter << "\n";

    // 原子操作
    std::thread t3(atomic_worker);
    std::thread t4(atomic_worker);
    t3.join(); t4.join();
    std::cout << "atomic_counter = " << atomic_counter << "\n";  // 2000

    // 异步任务
    auto future = std::async(std::launch::async, compute, 10);
    std::cout << "做其他事情...\n";
    int result = future.get();  // 阻塞直到结果可用
    std::cout << "结果: " << result << "\n";  // 100

    // promise/future 手动控制
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread setter([&prom] { prom.set_value(42); });
    std::cout << "promise 值: " << fut.get() << "\n";
    setter.join();

    return 0;
}
```

---

### 19. std::tuple

```cpp
#include <iostream>
#include <tuple>
#include <string>

// 函数返回多个值
std::tuple<int, std::string, double> getInfo() {
    return {42, "Alice", 3.14};
}

int main() {
    // 创建 tuple
    auto t = std::make_tuple(1, "hello", 3.14);

    // 访问元素
    std::cout << std::get<0>(t) << "\n";  // 1
    std::cout << std::get<1>(t) << "\n";  // hello

    // tie 解包
    int id; std::string name; double score;
    std::tie(id, name, score) = getInfo();
    std::cout << id << " " << name << " " << score << "\n";

    // std::ignore 忽略某个值
    std::tie(id, std::ignore, score) = getInfo();

    // C++17 结构化绑定（更优雅）
    auto [a, b, c] = getInfo();
    std::cout << a << " " << b << " " << c << "\n";

    // tuple 比较（字典序）
    auto t1 = std::make_tuple(1, 2, 3);
    auto t2 = std::make_tuple(1, 2, 4);
    std::cout << (t1 < t2) << "\n";  // true

    // tuple_size, tuple_element
    constexpr size_t sz = std::tuple_size<decltype(t)>::value;
    std::cout << sz << "\n";  // 3

    return 0;
}
```

---

### 20. std::function 与 std::bind

```cpp
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

int add(int a, int b) { return a + b; }

struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    // std::function：通用函数包装器
    std::function<int(int, int)> f1 = add;
    std::function<int(int, int)> f2 = [](int a, int b) { return a * b; };
    std::function<int(int)> f3 = Multiplier{3};

    std::cout << f1(3, 4) << "\n";  // 7
    std::cout << f2(3, 4) << "\n";  // 12
    std::cout << f3(5) << "\n";     // 15

    // std::bind：绑定参数，生成新的可调用对象
    using namespace std::placeholders;
    auto add5 = std::bind(add, _1, 5);   // 绑定第二个参数为 5
    std::cout << add5(10) << "\n";       // 15

    auto add_rev = std::bind(add, _2, _1);  // 交换参数顺序
    std::cout << add_rev(3, 7) << "\n";     // 10

    // 成员函数绑定
    struct Counter {
        int n = 0;
        void inc() { ++n; }
        int get() const { return n; }
    };
    Counter c;
    auto inc = std::bind(&Counter::inc, &c);
    inc(); inc(); inc();
    std::cout << c.get() << "\n";  // 3

    // 函数对象向量
    std::vector<std::function<int(int)>> transforms = {
        [](int x) { return x + 1; },
        [](int x) { return x * 2; },
        std::bind(add, _1, 10)
    };
    int val = 5;
    for (auto& fn : transforms) val = fn(val);
    std::cout << val << "\n";  // ((5+1)*2)+10 = 22

    return 0;
}
```

---

### 21. 无序容器

**原理：** 基于哈希表实现，平均 O(1) 查找，但无序；有序容器（`map`/`set`）基于红黑树，O(log n) 查找但有序。

```cpp
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

// 自定义类型的哈希
struct Point { int x, y; };

struct PointHash {
    size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

struct PointEq {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

int main() {
    std::unordered_map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores.emplace("Charlie", 92);

    // 平均 O(1) 查找
    if (auto it = scores.find("Alice"); it != scores.end()) {
        std::cout << "Alice: " << it->second << "\n";
    }

    std::unordered_set<int> s = {1, 2, 3, 4, 5};
    s.insert(3);  // 重复，不插入
    std::cout << s.count(3) << "\n";  // 1

    // 自定义哈希
    std::unordered_map<Point, std::string, PointHash, PointEq> pointMap;
    pointMap[{0, 0}] = "原点";
    pointMap[{1, 0}] = "x轴";
    std::cout << pointMap[{0, 0}] << "\n";

    // 桶操作
    std::cout << "桶数: " << scores.bucket_count() << "\n";
    std::cout << "负载因子: " << scores.load_factor() << "\n";

    return 0;
}
```

---

### 22. 数组 std::array

```cpp
#include <array>
#include <iostream>
#include <algorithm>

int main() {
    // 固定大小数组，大小是类型的一部分
    std::array<int, 5> arr = {1, 2, 3, 4, 5};

    // 支持标准算法
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    // 边界检查（at() 抛异常，[] 不检查）
    std::cout << arr.at(2) << "\n";
    // arr.at(10); // 抛出 std::out_of_range

    // 编译期大小
    constexpr size_t sz = arr.size();

    // 结构化绑定（C++17）
    std::array<int, 3> a3 = {10, 20, 30};
    auto [x, y, z] = a3;
    std::cout << x << " " << y << " " << z << "\n";

    // 多维数组
    std::array<std::array<int, 3>, 3> matrix = {{{1,2,3},{4,5,6},{7,8,9}}};
    for (const auto& row : matrix) {
        for (int v : row) std::cout << v << " ";
        std::cout << "\n";
    }

    return 0;
}
```

---

### 23. 正则表达式

```cpp
#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string text = "John: 25, Jane: 30, Bob: 22";

    // 匹配
    std::regex pattern(R"((\w+): (\d+))");
    std::smatch match;

    // 搜索第一个匹配
    if (std::regex_search(text, match, pattern)) {
        std::cout << "完整: " << match[0] << "\n";
        std::cout << "姓名: " << match[1] << "\n";
        std::cout << "年龄: " << match[2] << "\n";
    }

    // 迭代所有匹配
    auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        std::cout << (*it)[1] << " -> " << (*it)[2] << "\n";
    }

    // 替换
    std::string result = std::regex_replace(text, pattern, "$1=$2");
    std::cout << result << "\n";  // John=25, Jane=30, Bob=22

    // 完全匹配
    std::regex email(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::string addr = "user@example.com";
    std::cout << std::regex_match(addr, email) << "\n";  // true

    return 0;
}
```

---

### 24. 移动语义相关：std::move / std::forward

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// 完美转发：保留参数的值类别（左值/右值）
template<typename T>
void wrapper(T&& arg) {
    // std::forward<T>(arg) 将 arg 以原始的值类别转发
    process(std::forward<T>(arg));
}

void process(int& x)  { std::cout << "左值引用\n"; }
void process(int&& x) { std::cout << "右值引用\n"; }

template<typename T, typename... Args>
T* my_new(Args&&... args) {
    return new T(std::forward<Args>(args)...);  // 完美转发构造
}

int main() {
    int x = 42;
    wrapper(x);         // T=int&，转发左值
    wrapper(42);        // T=int，转发右值
    wrapper(std::move(x));  // T=int，转发右值

    // std::move 只是静态转型，不"移动"任何东西
    // 它将左值转为右值引用，激活移动语义
    std::string s1 = "hello";
    std::string s2 = std::move(s1);  // s1 资源转移给 s2
    // s1 现在是有效但未指定的状态（通常为空串）
    std::cout << "s1: " << s1 << "\n";  // 空
    std::cout << "s2: " << s2 << "\n";  // hello

    return 0;
}
```

---

### 25. explicit 转换运算符

```cpp
#include <iostream>

class SafeBool {
    bool value;
public:
    SafeBool(bool v) : value(v) {}
    // explicit 防止隐式转换到 bool
    explicit operator bool() const { return value; }
};

class Fraction {
    int num, den;
public:
    Fraction(int n, int d) : num(n), den(d) {}
    explicit operator double() const { return (double)num / den; }
};

int main() {
    SafeBool sb(true);

    if (sb) std::cout << "ok\n";  // 允许：在布尔上下文中显式
    // int n = sb;                 // 错误：隐式转换被阻止

    Fraction f(1, 3);
    double d = static_cast<double>(f);  // 显式转换
    // double d2 = f;              // 错误：需要显式

    return 0;
}
```

---

### 26. 用户定义字面量

```cpp
#include <iostream>
#include <chrono>
#include <complex>

// 用户定义字面量
constexpr long double operator""_km(long double km) { return km * 1000.0; }
constexpr long double operator""_m(long double m) { return m; }
constexpr long double operator""_cm(long double cm) { return cm / 100.0; }

constexpr unsigned long long operator""_KB(unsigned long long kb) { return kb * 1024; }
constexpr unsigned long long operator""_MB(unsigned long long mb) { return mb * 1024 * 1024; }

int main() {
    auto distance = 1.5_km + 200.0_m + 50.0_cm;  // 全部转换为米
    std::cout << distance << " 米\n";

    auto filesize = 4_MB + 512_KB;
    std::cout << filesize << " 字节\n";

    // 标准库自带字面量
    using namespace std::literals::chrono_literals;
    auto t = 1h + 30min + 45s;
    std::cout << t.count() << " 秒\n";

    using namespace std::complex_literals;
    auto c = 1.0 + 2.0i;  // std::complex<double>
    std::cout << c << "\n";

    using namespace std::string_literals;
    auto s = "hello"s;  // std::string（不是 const char*）

    return 0;
}
```

---

## C++14 特性

### 1. 通用 Lambda

**原理：** C++14 允许 Lambda 参数使用 `auto`，编译器为每种调用类型生成对应的 `operator()`，等效于函数模板。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // 通用 Lambda（参数用 auto）
    auto add = [](auto a, auto b) { return a + b; };
    std::cout << add(1, 2) << "\n";       // int
    std::cout << add(1.5, 2.3) << "\n";   // double
    std::cout << add(std::string("a"), std::string("b")) << "\n";  // string

    // 通用 Lambda 实现打印
    auto print = [](const auto& v) {
        for (const auto& x : v) std::cout << x << " ";
        std::cout << "\n";
    };

    std::vector<int> vi = {1, 2, 3};
    std::vector<double> vd = {1.1, 2.2, 3.3};
    print(vi);
    print(vd);

    // 通用 Lambda 递归（需要 std::function 或 Y 组合子）
    // 配合 auto&&：完美转发通用 Lambda
    auto forward_call = [](auto&& fn, auto&&... args) {
        return fn(std::forward<decltype(args)>(args)...);
    };
    forward_call(add, 3, 4);

    return 0;
}
```

---

### 2. Lambda 捕获初始化

**原理：** C++14 引入广义 Lambda 捕获，允许在捕获列表中初始化新变量（包括移动捕获），解决了 C++11 无法移动捕获的问题。

```cpp
#include <iostream>
#include <memory>
#include <string>

int main() {
    // 移动捕获（C++11 不支持）
    auto ptr = std::make_unique<int>(42);
    auto fn = [p = std::move(ptr)]() {
        std::cout << *p << "\n";
    };
    fn();  // 42
    // ptr 已经被移动，ptr == nullptr

    // 捕获时计算并存储
    int x = 10, y = 20;
    auto sum_fn = [s = x + y]() { return s; };
    std::cout << sum_fn() << "\n";  // 30，即使 x,y 之后改变也不影响

    // 重命名捕获
    std::string long_name = "hello world";
    auto fn2 = [ln = std::move(long_name)]() {
        std::cout << ln << "\n";
    };
    fn2();
    std::cout << (long_name.empty() ? "已移动" : long_name) << "\n";

    // 捕获 this 的成员（避免悬空引用）
    struct Widget {
        int value = 42;
        auto getFunc() {
            return [v = value]() { return v; };  // 拷贝成员而非 this
        }
    };
    Widget w;
    auto f = w.getFunc();
    std::cout << f() << "\n";  // 42

    return 0;
}
```

---

### 3. 函数返回类型推导

**原理：** C++14 允许普通函数（非 Lambda）用 `auto` 作为返回类型，编译器从 `return` 语句推导，多个 `return` 必须类型一致。

```cpp
#include <iostream>
#include <vector>

// 返回类型自动推导
auto add(int a, int b) {
    return a + b;  // 推导为 int
}

// 递归函数需要前向声明或 if constexpr
auto fibonacci(int n) -> int {  // 也可以尾置返回
    if (n <= 1) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

// decltype(auto)：保留引用和 cv 限定
int global = 0;
decltype(auto) getGlobal() {
    return (global);  // 注意括号！返回 int&
}

// 模板函数
template<typename Container>
auto front(Container& c) -> decltype(c.front()) {
    return c.front();
}

template<typename Container>
decltype(auto) front2(Container& c) {  // C++14：保留引用
    return c.front();
}

int main() {
    std::cout << add(3, 4) << "\n";

    std::vector<int> v = {10, 20, 30};
    front2(v) = 99;  // 返回引用，可以赋值
    std::cout << v[0] << "\n";  // 99

    getGlobal() = 42;  // 返回引用
    std::cout << global << "\n";  // 42

    return 0;
}
```

---

### 4. 变量模板

**原理：** C++14 允许变量也使用模板参数，最常见的应用是为不同类型提供数学常量。

```cpp
#include <iostream>
#include <type_traits>
#include <cmath>

// 数学常量变量模板
template<typename T>
constexpr T pi = T(3.14159265358979323846);

template<typename T>
constexpr T e = T(2.71828182845904523536);

// 类型特征简化（C++17 标准库广泛使用此模式）
template<typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template<typename T>
constexpr bool is_floating_v = std::is_floating_point<T>::value;

template<typename T>
T circle_area(T r) {
    return pi<T> * r * r;
}

int main() {
    std::cout << pi<float> << "\n";   // float 精度
    std::cout << pi<double> << "\n";  // double 精度

    std::cout << circle_area(1.0f) << "\n";
    std::cout << circle_area(1.0) << "\n";

    std::cout << is_integral_v<int> << "\n";    // 1
    std::cout << is_floating_v<double> << "\n"; // 1

    return 0;
}
```

---

### 5. constexpr 放宽限制

**原理：** C++11 的 `constexpr` 函数只能有单一 `return` 语句；C++14 放宽限制，允许局部变量、循环、条件语句、修改局部变量等。

```cpp
#include <iostream>
#include <array>

// C++11 只能写成递归，C++14 可以用循环
constexpr int factorial(int n) {
    int result = 1;          // 允许局部变量
    for (int i = 2; i <= n; ++i)  // 允许循环
        result *= i;
    return result;
}

constexpr bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

// 编译期生成素数表
template<int N>
constexpr std::array<int, N> make_primes() {
    std::array<int, N> result{};
    int count = 0, num = 2;
    while (count < N) {
        if (is_prime(num)) result[count++] = num;
        ++num;
    }
    return result;
}

int main() {
    constexpr int f10 = factorial(10);  // 编译期 = 3628800
    static_assert(f10 == 3628800, "");

    constexpr auto primes = make_primes<10>();
    for (int p : primes) std::cout << p << " ";
    std::cout << "\n";  // 2 3 5 7 11 13 17 19 23 29

    return 0;
}
```

---

### 6. std::make_unique

```cpp
#include <memory>
#include <iostream>

struct Resource {
    int id;
    Resource(int id) : id(id) { std::cout << "构造 " << id << "\n"; }
    ~Resource() { std::cout << "析构 " << id << "\n"; }
};

int main() {
    // C++11 只有 make_shared，C++14 补上了 make_unique
    auto p1 = std::make_unique<Resource>(1);

    // 为什么要用 make_unique 而非 new？
    // 1. 异常安全（避免 new 和构造之间抛异常的泄漏）
    // 2. 代码简洁
    // 3. 不暴露原始指针

    // 数组版本
    auto arr = std::make_unique<int[]>(10);
    for (int i = 0; i < 10; ++i) arr[i] = i;

    std::cout << arr[5] << "\n";  // 5

    return 0;
}
```

---

### 7. 二进制字面量与数字分隔符

```cpp
#include <iostream>
#include <cstdint>

int main() {
    // 二进制字面量（前缀 0b 或 0B）
    int bin = 0b1010'1010;  // 170
    uint8_t flags = 0b0001'1110;  // 30

    // 数字分隔符（单引号，仅用于可读性）
    long long million = 1'000'000;
    double avogadro = 6.022'140'76e23;
    int hex = 0xFF'FF'FF;
    int oct = 0777'777;

    std::cout << bin << "\n";      // 170
    std::cout << million << "\n";  // 1000000
    std::cout << hex << "\n";      // 16777215

    // 常用于位标志
    const uint32_t READ    = 0b0000'0001;
    const uint32_t WRITE   = 0b0000'0010;
    const uint32_t EXECUTE = 0b0000'0100;

    uint32_t perms = READ | WRITE;
    std::cout << std::boolalpha << (bool)(perms & READ) << "\n";    // true
    std::cout << std::boolalpha << (bool)(perms & EXECUTE) << "\n"; // false

    return 0;
}
```

---

### 8. std::integer_sequence

```cpp
#include <iostream>
#include <tuple>
#include <utility>
#include <array>

// 用于在编译期展开 tuple
template<typename Tuple, std::size_t... I>
void print_tuple_impl(const Tuple& t, std::index_sequence<I...>) {
    ((std::cout << std::get<I>(t) << " "), ...);  // C++17 折叠表达式
    std::cout << "\n";
}

template<typename... Args>
void print_tuple(const std::tuple<Args...>& t) {
    print_tuple_impl(t, std::index_sequence_for<Args...>{});
}

// 编译期数组初始化
template<std::size_t... I>
constexpr auto make_array(std::index_sequence<I...>) {
    return std::array<int, sizeof...(I)>{{I * I...}};
}

int main() {
    auto t = std::make_tuple(1, "hello", 3.14);
    print_tuple(t);  // 1 hello 3.14

    constexpr auto squares = make_array(std::make_index_sequence<5>{});
    for (int v : squares) std::cout << v << " ";  // 0 1 4 9 16
    std::cout << "\n";

    return 0;
}
```

---

### 9. std::exchange

```cpp
#include <iostream>
#include <utility>
#include <string>

int main() {
    // std::exchange(obj, new_val) 将 obj 设为 new_val，返回 obj 的旧值
    int x = 5;
    int old = std::exchange(x, 10);
    std::cout << "old: " << old << ", x: " << x << "\n";  // old:5, x:10

    // 在移动构造中很有用
    struct Resource {
        int* data;
        Resource(int n) : data(new int(n)) {}
        Resource(Resource&& other) noexcept
            : data(std::exchange(other.data, nullptr)) {}  // 原子地取走并清零
        ~Resource() { delete data; }
    };

    Resource r1(42);
    Resource r2(std::move(r1));
    // r1.data == nullptr，r2.data == 42 所在内存

    return 0;
}
```

---

## C++17 特性

### 1. 结构化绑定

**原理：** 允许一次性将 `tuple`、`pair`、结构体、数组的成员绑定到独立变量，是语法糖，编译器自动展开。

```cpp
#include <iostream>
#include <map>
#include <tuple>
#include <array>

struct Point3D { int x, y, z; };

std::tuple<int, std::string, double> getData() {
    return {1, "Alice", 9.5};
}

int main() {
    // tuple/pair 解包
    auto [id, name, score] = getData();
    std::cout << id << " " << name << " " << score << "\n";

    // pair（map 插入返回值）
    std::map<std::string, int> m;
    auto [it, inserted] = m.insert({"key", 42});
    std::cout << "插入成功: " << inserted << "\n";

    // 结构体
    Point3D p{1, 2, 3};
    auto [x, y, z] = p;
    std::cout << x << " " << y << " " << z << "\n";

    // 数组
    int arr[3] = {10, 20, 30};
    auto [a, b, c] = arr;

    // map 遍历（最常见用法之一）
    std::map<std::string, int> ages = {{"Alice", 30}, {"Bob", 25}};
    for (const auto& [name2, age] : ages) {
        std::cout << name2 << ": " << age << "\n";
    }

    // 引用绑定（修改原始数据）
    auto& [rx, ry, rz] = p;
    rx = 100;
    std::cout << p.x << "\n";  // 100

    return 0;
}
```

---

### 2. if / switch 初始化语句

**原理：** 允许在 `if` 和 `switch` 的条件判断中声明并初始化变量，变量的作用域限制在 `if/switch` 语句块内，避免污染外部命名空间。

```cpp
#include <iostream>
#include <map>
#include <string>
#include <mutex>

std::map<std::string, int> registry = {{"alpha", 1}, {"beta", 2}};
std::mutex mtx;

int risky() { return 42; }

int main() {
    // if 初始化语句：变量作用域限制在 if 块
    if (auto it = registry.find("alpha"); it != registry.end()) {
        std::cout << "找到: " << it->second << "\n";
    }
    // it 在这里不可用

    // switch 初始化语句
    switch (auto val = risky(); val) {
        case 42: std::cout << "答案是 42\n"; break;
        default: std::cout << "其他: " << val << "\n";
    }

    // 锁作用域精确控制
    if (std::lock_guard<std::mutex> lock(mtx); true) {
        std::cout << "持有锁\n";
    }  // 锁在这里释放

    // 结合结构化绑定
    if (auto [it, ok] = registry.insert({"gamma", 3}); ok) {
        std::cout << "插入成功: " << it->first << "\n";
    }

    return 0;
}
```

---

### 3. std::optional

**原理：** 表示"可能有值或没有值"的类型，替代 `nullptr`、哨兵值（`-1`、`""`）或 `bool+out-param` 三种粗糙模式，语义清晰且零开销（栈分配）。

```cpp
#include <optional>
#include <iostream>
#include <string>
#include <vector>

std::optional<int> parseInt(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::nullopt;  // 表示"没有值"
    }
}

std::optional<std::string> findUser(int id) {
    if (id == 1) return "Alice";
    if (id == 2) return "Bob";
    return std::nullopt;
}

int main() {
    auto r1 = parseInt("42");
    auto r2 = parseInt("abc");

    if (r1) std::cout << "解析结果: " << *r1 << "\n";
    if (!r2) std::cout << "解析失败\n";

    // value_or：提供默认值
    std::cout << r2.value_or(-1) << "\n";  // -1

    // value()：有值则返回，无值抛 std::bad_optional_access
    try {
        std::cout << r2.value() << "\n";
    } catch (const std::bad_optional_access& e) {
        std::cout << "异常: " << e.what() << "\n";
    }

    // 链式操作
    auto user = findUser(1);
    if (user) {
        std::cout << "用户: " << user.value() << "\n";
    }

    // 作为成员变量
    struct Config {
        std::string host;
        int port;
        std::optional<std::string> proxy;  // 可选代理
        std::optional<int> timeout;
    };

    Config cfg{"localhost", 8080, std::nullopt, 30};
    if (cfg.proxy) std::cout << "代理: " << *cfg.proxy << "\n";
    else std::cout << "无代理\n";

    return 0;
}
```

---

### 4. std::variant

**原理：** 类型安全的联合（`union`），在运行时持有多个类型之一，比 `union` 更安全，比继承体系更轻量，是"代数数据类型"（Sum Type）在 C++ 中的体现。

```cpp
#include <variant>
#include <iostream>
#include <string>
#include <vector>

using Value = std::variant<int, double, std::string, bool>;

// std::visit + 重载 visitor 模式
template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;  // CTAD

int main() {
    Value v = 42;
    std::cout << std::get<int>(v) << "\n";

    v = 3.14;
    std::cout << v.index() << "\n";  // 1（double 在第 1 位）

    v = std::string("hello");

    // std::visit：对当前存储的类型调用对应的函数
    std::visit([](const auto& x) {
        std::cout << x << "\n";
    }, v);

    // 更完整的 visitor
    std::visit(overloaded{
        [](int x)          { std::cout << "int: " << x << "\n"; },
        [](double x)       { std::cout << "double: " << x << "\n"; },
        [](const std::string& x) { std::cout << "string: " << x << "\n"; },
        [](bool x)         { std::cout << "bool: " << x << "\n"; },
    }, v);

    // holds_alternative：类型检查
    if (std::holds_alternative<std::string>(v)) {
        std::cout << "是字符串\n";
    }

    // 处理错误的例子
    using Result = std::variant<std::string, int>;  // 成功=string，失败=int错误码
    Result r = std::string("成功结果");
    // Result err = 404;

    // 解析器示例：JSON 值类型
    using JsonValue = std::variant<
        std::nullptr_t,
        bool,
        int64_t,
        double,
        std::string
        // 实际还有 array 和 object
    >;

    return 0;
}
```

---

### 5. std::any

**原理：** 类型擦除的容器，可以存储任意类型的值（类似动态类型），但访问时需要显式 `any_cast`，保证类型安全。

```cpp
#include <any>
#include <iostream>
#include <string>
#include <vector>
#include <map>

int main() {
    std::any a = 42;
    std::cout << std::any_cast<int>(a) << "\n";

    a = std::string("hello");
    std::cout << std::any_cast<std::string>(a) << "\n";

    a = std::vector<int>{1, 2, 3};

    // 类型检查
    if (a.type() == typeid(std::vector<int>)) {
        auto& v = std::any_cast<std::vector<int>&>(a);
        for (int x : v) std::cout << x << " ";
        std::cout << "\n";
    }

    // 转型失败抛 std::bad_any_cast
    try {
        std::any_cast<int>(a);  // 当前是 vector<int>
    } catch (const std::bad_any_cast& e) {
        std::cout << "类型错误: " << e.what() << "\n";
    }

    // 指针版本不抛异常（返回 nullptr）
    auto* p = std::any_cast<int>(&a);
    if (!p) std::cout << "不是 int\n";

    // 异构容器
    std::map<std::string, std::any> props;
    props["name"] = std::string("Alice");
    props["age"] = 30;
    props["score"] = 99.5;

    std::cout << std::any_cast<std::string>(props["name"]) << "\n";

    return 0;
}
```

---

### 6. std::string_view

**原理：** 非拥有（non-owning）的字符串引用，只包含指针和长度，不进行内存分配，适合只读字符串操作，避免不必要的拷贝。

```cpp
#include <string_view>
#include <string>
#include <iostream>
#include <algorithm>

// 接受 string_view 可以无缝接受 std::string 和 const char*
void processStr(std::string_view sv) {
    std::cout << "长度: " << sv.size() << "\n";
    std::cout << "首字母: " << sv.front() << "\n";

    // 子串（不分配内存）
    auto sub = sv.substr(0, 5);
    std::cout << "前5个: " << sub << "\n";
}

std::string_view getExtension(std::string_view filename) {
    auto pos = filename.rfind('.');
    if (pos == std::string_view::npos) return {};
    return filename.substr(pos);  // 返回原字符串的视图
}

int main() {
    std::string s = "Hello, World!";
    const char* cs = "Hello, C-string!";
    std::string_view sv = "Hello, string_view!";

    processStr(s);   // 无拷贝
    processStr(cs);  // 无拷贝
    processStr(sv);  // 无拷贝

    // 字符串解析
    std::string_view data = "name=Alice;age=30;city=Beijing";
    while (!data.empty()) {
        auto sep = data.find(';');
        auto token = (sep == std::string_view::npos) ? data : data.substr(0, sep);

        auto eq = token.find('=');
        auto key = token.substr(0, eq);
        auto val = token.substr(eq + 1);
        std::cout << key << " -> " << val << "\n";

        data = (sep == std::string_view::npos) ? "" : data.substr(sep + 1);
    }

    // 文件扩展名
    std::cout << getExtension("photo.jpg") << "\n";  // .jpg
    std::cout << getExtension("archive.tar.gz") << "\n";  // .gz

    // 注意：string_view 的生命周期不能超过被引用字符串
    // 危险示例：
    // std::string_view bad = std::string("temp");  // 悬空视图！

    return 0;
}
```

---

### 7. 折叠表达式

**原理：** C++17 提供对参数包进行折叠操作的语法，避免了可变模板函数的递归写法，编译器将参数包展开为二元运算符链。

```cpp
#include <iostream>
#include <string>

// 左折叠 ((a + b) + c) + d
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // 一元左折叠
}

// 右折叠 a + (b + (c + d))
template<typename... Args>
auto sum_right(Args... args) {
    return (args + ...);  // 一元右折叠
}

// 二元折叠（带初始值）
template<typename... Args>
auto sum_with_init(Args... args) {
    return (0 + ... + args);  // 二元左折叠，初始值 0
}

// 逗号折叠：依次执行
template<typename... Args>
void print_all(Args&&... args) {
    (std::cout << ... << args);  // 依次输出
    std::cout << "\n";
}

// && 和 || 折叠
template<typename... Args>
bool all_true(Args... args) {
    return (... && args);
}

template<typename T, typename... Args>
bool any_of(T val, Args... args) {
    return ((val == args) || ...);
}

// 向容器插入
template<typename Container, typename... Args>
void push_all(Container& c, Args&&... args) {
    (c.push_back(std::forward<Args>(args)), ...);
}

int main() {
    std::cout << sum(1, 2, 3, 4, 5) << "\n";       // 15
    std::cout << sum(1.0, 2.5, 3.5) << "\n";       // 7
    std::cout << sum_with_init() << "\n";           // 0（空参数包安全）

    print_all(1, " + ", 2, " = ", 3);  // 1 + 2 = 3

    std::cout << all_true(true, true, true) << "\n";   // 1
    std::cout << all_true(true, false, true) << "\n";  // 0

    std::cout << any_of(3, 1, 2, 3, 4) << "\n";  // 1

    std::vector<int> v;
    push_all(v, 1, 2, 3, 4, 5);
    for (int x : v) std::cout << x << " ";

    return 0;
}
```

---

### 8. if constexpr

**原理：** 编译期条件判断，根据编译期布尔值选择性编译分支，未选择的分支完全不编译（不同于普通 `if` 的两个分支都编译），使模板特化更简洁。

```cpp
#include <iostream>
#include <type_traits>
#include <string>
#include <vector>

// 替代模板特化的更简洁写法
template<typename T>
void print_type_info(T val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "整数: " << val << " (二进制: ";
        for (int i = sizeof(T)*8-1; i >= 0; --i)
            std::cout << ((val >> i) & 1);
        std::cout << ")\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "浮点数: " << val << "\n";
    } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "字符串: \"" << val << "\" (长度=" << val.size() << ")\n";
    } else {
        std::cout << "未知类型\n";
    }
}

// 递归可变模板的终止条件
template<typename T>
T sum(T v) { return v; }

template<typename T, typename... Args>
auto sum(T first, Args... rest) {
    return first + sum(rest...);
}

// C++17 用 if constexpr 实现（更优雅）
template<typename T, typename... Args>
auto sum2(T first, Args... rest) {
    if constexpr (sizeof...(rest) == 0) {
        return first;
    } else {
        return first + sum2(rest...);
    }
}

// 序列化：不同类型不同处理
template<typename T>
std::string serialize(const T& val) {
    if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(val);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "\"" + val + "\"";
    } else {
        static_assert(false, "不支持的类型");
    }
}

int main() {
    print_type_info(42);
    print_type_info(3.14);
    print_type_info(std::string("hello"));

    std::cout << sum2(1, 2, 3, 4, 5) << "\n";  // 15

    std::cout << serialize(42) << "\n";
    std::cout << serialize(std::string("world")) << "\n";

    return 0;
}
```

---

### 9. 类模板参数推导（CTAD）

**原理：** C++17 允许从构造函数参数推导类模板参数，不再需要像 `make_pair`、`make_tuple` 那样的辅助函数。

```cpp
#include <iostream>
#include <vector>
#include <pair>
#include <tuple>
#include <mutex>

// 自定义类的 CTAD
template<typename T>
class Wrapper {
    T value;
public:
    Wrapper(T v) : value(v) {}
    T get() const { return value; }
};

// 自定义推导指引
template<typename T>
Wrapper(T) -> Wrapper<T>;

// 从容器推导
template<typename Iter>
class Range {
    Iter begin_, end_;
public:
    Range(Iter b, Iter e) : begin_(b), end_(e) {}
};

int main() {
    // C++11/14 需要写：std::pair<int, std::string>
    std::pair p{42, std::string("hello")};  // CTAD: pair<int, string>
    auto [a, b] = p;
    std::cout << a << " " << b << "\n";

    // tuple
    std::tuple t{1, 2.0, "three"};  // tuple<int, double, const char*>

    // vector 从初始化列表推导
    std::vector v{1, 2, 3, 4, 5};  // vector<int>
    std::vector v2{1.1, 2.2, 3.3}; // vector<double>

    // lock_guard
    std::mutex mtx;
    std::lock_guard lock(mtx);      // lock_guard<std::mutex>

    // 自定义类
    Wrapper w{42};    // Wrapper<int>
    Wrapper w2{3.14}; // Wrapper<double>
    std::cout << w.get() << " " << w2.get() << "\n";

    return 0;
}
```

---

### 10. 内联变量

**原理：** `inline` 变量（特别是 `inline static` 成员）允许在头文件中定义，不需要单独的 `.cpp` 文件提供定义，解决了 ODR（One Definition Rule）问题。

```cpp
// config.h
struct Config {
    inline static int maxConnections = 100;    // C++17
    inline static const std::string version = "1.0.0";
    inline static std::atomic<int> counter{0};
};

// 全局内联变量（头文件中定义不违反 ODR）
inline constexpr double PI = 3.14159265358979;
inline int globalId = 0;

// 使用
#include <iostream>
int main() {
    std::cout << Config::maxConnections << "\n";
    Config::maxConnections = 200;
    std::cout << Config::version << "\n";
    std::cout << PI << "\n";
    return 0;
}
```

---

### 11. 嵌套命名空间

```cpp
// C++11/14
namespace Outer {
    namespace Inner {
        namespace Deep {
            void func() {}
        }
    }
}

// C++17 简化
namespace Outer::Inner::Deep {
    void func2() {}
    class MyClass {};
}

// 使用
int main() {
    Outer::Inner::Deep::func();
    Outer::Inner::Deep::func2();
    Outer::Inner::Deep::MyClass obj;
    return 0;
}
```

---

### 12. [[nodiscard]] / [[maybe_unused]] / [[fallthrough]]

```cpp
#include <iostream>

// [[nodiscard]]：调用者必须使用返回值
[[nodiscard]] int divide(int a, int b) {
    if (b == 0) return -1;
    return a / b;
}

[[nodiscard("忽略错误码可能导致问题")]] // C++20 可带消息
int openFile(const char* path);

// [[maybe_unused]]：告知编译器变量可能未使用（抑制警告）
void func([[maybe_unused]] int debug_only_param) {
    [[maybe_unused]] int unused_var = 42;
    // 不产生"未使用变量"警告
}

// [[fallthrough]]：显式标记 switch 贯穿
void process(int n) {
    switch (n) {
        case 1:
            std::cout << "1\n";
            [[fallthrough]];  // 显式说明"故意贯穿"
        case 2:
            std::cout << "2\n";
            break;
        case 3:
            std::cout << "3\n";
            break;
    }
}

int main() {
    // divide(10, 2);  // 警告：忽略了 [[nodiscard]] 返回值
    auto r = divide(10, 2);  // 正确
    std::cout << r << "\n";

    process(1);  // 输出 1 和 2
    process(3);  // 只输出 3

    return 0;
}
```

---

### 13. std::filesystem

```cpp
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

int main() {
    // 路径操作
    fs::path p = "/home/user/documents/file.txt";
    std::cout << "文件名: " << p.filename() << "\n";
    std::cout << "扩展名: " << p.extension() << "\n";
    std::cout << "父目录: " << p.parent_path() << "\n";
    std::cout << "无扩展: " << p.stem() << "\n";

    // 路径拼接
    fs::path dir = "/home/user";
    fs::path full = dir / "documents" / "file.txt";  // operator/
    std::cout << full << "\n";

    // 文件操作
    fs::path tmpDir = fs::temp_directory_path() / "cpp17_test";
    fs::create_directories(tmpDir);

    fs::path tmpFile = tmpDir / "test.txt";
    std::ofstream(tmpFile) << "Hello, filesystem!\n";

    // 文件信息
    if (fs::exists(tmpFile)) {
        std::cout << "文件大小: " << fs::file_size(tmpFile) << " 字节\n";
        auto ftime = fs::last_write_time(tmpFile);
    }

    // 目录遍历
    for (const auto& entry : fs::directory_iterator(tmpDir)) {
        std::cout << entry.path() << "\n";
    }

    // 递归遍历
    for (const auto& entry : fs::recursive_directory_iterator(".")) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            std::cout << entry.path() << "\n";
        }
    }

    // 复制/移动/删除
    fs::copy(tmpFile, tmpDir / "backup.txt");
    fs::rename(tmpDir / "backup.txt", tmpDir / "renamed.txt");
    fs::remove(tmpDir / "renamed.txt");
    fs::remove_all(tmpDir);

    return 0;
}
```

---

### 14. 并行算法

```cpp
#include <algorithm>
#include <execution>
#include <vector>
#include <numeric>
#include <iostream>
#include <chrono>

int main() {
    std::vector<int> v(10'000'000);
    std::iota(v.begin(), v.end(), 0);

    // 顺序执行（原有行为）
    auto t1 = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::seq, v.begin(), v.end(), std::greater<int>());
    auto t2 = std::chrono::high_resolution_clock::now();

    // 并行执行（自动利用多核）
    std::iota(v.begin(), v.end(), 0);
    auto t3 = std::chrono::high_resolution_clock::now();
    std::sort(std::execution::par, v.begin(), v.end(), std::greater<int>());
    auto t4 = std::chrono::high_resolution_clock::now();

    // 并行+向量化
    std::iota(v.begin(), v.end(), 0);
    std::sort(std::execution::par_unseq, v.begin(), v.end());

    // 并行 reduce
    long long total = std::reduce(std::execution::par, v.begin(), v.end(), 0LL);
    std::cout << "总和: " << total << "\n";

    // 并行 transform
    std::vector<int> result(v.size());
    std::transform(std::execution::par,
                   v.begin(), v.end(), result.begin(),
                   [](int x) { return x * x; });

    // 执行策略：
    // seq      顺序
    // par      并行
    // par_unseq 并行+SIMD 向量化（最强，但要求操作无副作用）

    return 0;
}
```

---

### 15. std::byte

```cpp
#include <cstddef>
#include <iostream>

int main() {
    // std::byte 是类型安全的字节类型，不能隐式转换为整数
    std::byte b{0xFF};

    // 只支持位操作
    auto b2 = b & std::byte{0x0F};  // 与
    auto b3 = b | std::byte{0x01};  // 或
    auto b4 = b ^ std::byte{0xAA};  // 异或
    auto b5 = ~b;                    // 非
    auto b6 = b >> 4;               // 右移
    auto b7 = b << 2;               // 左移

    // 转换为整数
    int val = std::to_integer<int>(b);
    unsigned char raw = static_cast<unsigned char>(b);
    std::cout << val << "\n";  // 255

    // 不支持算术运算（类型安全）
    // b + std::byte{1};  // 编译错误

    // 典型用途：原始内存操作
    std::byte buffer[1024];
    std::fill(std::begin(buffer), std::end(buffer), std::byte{0});

    return 0;
}
```

---

### 16. std::invoke 与 std::apply

```cpp
#include <functional>
#include <tuple>
#include <iostream>
#include <string>

struct Obj {
    int value = 10;
    int get(int x) const { return value + x; }
};

int add(int a, int b) { return a + b; }

int main() {
    // std::invoke：统一调用任何可调用对象
    // 普通函数
    std::cout << std::invoke(add, 3, 4) << "\n";  // 7

    // Lambda
    auto lam = [](int x) { return x * 2; };
    std::cout << std::invoke(lam, 5) << "\n";  // 10

    // 成员函数
    Obj obj;
    std::cout << std::invoke(&Obj::get, obj, 5) << "\n";  // 15

    // 成员变量
    std::cout << std::invoke(&Obj::value, obj) << "\n";  // 10

    // std::apply：将 tuple 展开为函数参数
    auto args = std::make_tuple(3, 4);
    std::cout << std::apply(add, args) << "\n";  // 7

    auto args2 = std::make_tuple(std::string("hello"), ' ', std::string("world"));
    auto cat = [](const std::string& a, char sep, const std::string& b) {
        return a + sep + b;
    };
    std::cout << std::apply(cat, args2) << "\n";  // hello world

    return 0;
}
```

---

## C++20 特性

### 1. 概念（Concepts）

**原理：** 概念是对模板参数的命名约束，让模板错误信息更清晰，让 API 意图更明确。概念是一个编译期 `bool` 表达式。

```cpp
#include <concepts>
#include <iostream>
#include <string>
#include <vector>

// 定义概念
template<typename T>
concept Printable = requires(T t) {
    std::cout << t;  // T 必须支持 << 运算符
};

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;  // + 的结果可转换为 T
};

template<typename T>
concept Container = requires(T c) {
    c.begin();
    c.end();
    c.size();
    typename T::value_type;  // 必须有 value_type 类型成员
};

template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

// 使用概念约束
template<Printable T>
void print(const T& val) {
    std::cout << val << "\n";
}

template<Addable T>
T add(T a, T b) { return a + b; }

// requires 子句（另一种约束方式）
template<typename T>
requires Number<T>
T square(T x) { return x * x; }

// 简化写法（缩写函数模板）
auto multiply(Number auto a, Number auto b) {
    return a * b;
}

// 概念约束 + if constexpr
template<typename T>
void describe(const T& val) {
    if constexpr (std::integral<T>) {
        std::cout << "整数: " << val << "\n";
    } else if constexpr (std::floating_point<T>) {
        std::cout << "浮点: " << val << "\n";
    } else if constexpr (Container<T>) {
        std::cout << "容器, 大小=" << val.size() << "\n";
    }
}

// requires 表达式详解
template<typename T>
concept Serializable = requires(T obj) {
    // 简单表达式：T 支持该操作
    obj.serialize();
    // 类型约束：serialize() 返回 std::string
    { obj.serialize() } -> std::same_as<std::string>;
    // 嵌套类型
    typename T::id_type;
    // 编译期常量
    requires std::is_copyable_v<T>;
};

int main() {
    print(42);
    print(std::string("hello"));
    // print(std::vector<int>{});  // 编译错误：vector 不支持 <<

    std::cout << add(1, 2) << "\n";
    std::cout << add(1.5, 2.5) << "\n";
    // add(std::string("a"), std::string("b")); // 需要 string 满足 Addable

    std::cout << square(5) << "\n";
    std::cout << multiply(3, 4.0) << "\n";

    describe(42);
    describe(3.14);
    describe(std::vector<int>{1,2,3});

    return 0;
}
```

---

### 2. 范围库（Ranges）

**原理：** 对 STL 算法的现代封装，支持惰性求值、管道操作符 `|`、范围适配器，消除了迭代器对的冗余写法。

```cpp
#include <ranges>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 传统 STL
    std::vector<int> result1;
    std::copy_if(v.begin(), v.end(), std::back_inserter(result1),
                 [](int x) { return x % 2 == 0; });

    // Ranges 风格
    auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });
    for (int x : evens) std::cout << x << " ";  // 2 4 6 8 10
    std::cout << "\n";

    // 管道链式操作（惰性求值）
    auto result = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);

    for (int x : result) std::cout << x << " ";  // 4 16 36
    std::cout << "\n";

    // 常用视图
    auto squares = std::views::iota(1, 11)        // 1..10
                 | std::views::transform([](int x) { return x * x; });

    // 字符串处理
    std::string text = "hello world cpp ranges";
    auto words = text | std::views::split(' ');
    for (auto word : words) {
        std::string w(word.begin(), word.end());
        std::cout << w << "\n";
    }

    // ranges 算法（不需要 begin/end）
    std::vector<int> data = {5, 2, 8, 1, 9, 3};
    std::ranges::sort(data);
    std::ranges::for_each(data, [](int x) { std::cout << x << " "; });
    std::cout << "\n";

    // reverse_view, drop, take
    auto last3 = data | std::views::reverse | std::views::take(3);
    for (int x : last3) std::cout << x << " ";  // 9 8 5
    std::cout << "\n";

    // zip（C++23）
    // auto zipped = std::views::zip(v1, v2);

    return 0;
}
```

---

### 3. 协程（Coroutines）

**原理：** 协程是可以暂停和恢复的函数，通过 `co_await`、`co_yield`、`co_return` 实现，编译器将协程转换为状态机。常用于异步 I/O、生成器、任务调度。

```cpp
#include <coroutine>
#include <iostream>
#include <optional>

// ===== 生成器实现 =====
template<typename T>
struct Generator {
    struct promise_type {
        T current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;

    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }

    // 迭代器支持
    bool next() {
        handle.resume();
        return !handle.done();
    }

    T value() { return handle.promise().current_value; }
};

// 斐波那契生成器
Generator<long long> fibonacci() {
    long long a = 0, b = 1;
    while (true) {
        co_yield a;
        auto tmp = a;
        a = b;
        b = tmp + b;
    }
}

// 范围生成器
Generator<int> range(int start, int end, int step = 1) {
    for (int i = start; i < end; i += step) {
        co_yield i;
    }
}

int main() {
    // 斐波那契数列
    auto fib = fibonacci();
    std::cout << "Fibonacci: ";
    for (int i = 0; i < 10 && fib.next(); ++i) {
        std::cout << fib.value() << " ";
    }
    std::cout << "\n";  // 0 1 1 2 3 5 8 13 21 34

    // 范围生成
    auto r = range(0, 20, 3);
    std::cout << "Range(0,20,3): ";
    while (r.next()) std::cout << r.value() << " ";
    std::cout << "\n";  // 0 3 6 9 12 15 18

    return 0;
}
```

---

### 4. 模块（Modules）

**原理：** 取代 `#include` 头文件机制，编译一次生成二进制接口，避免头文件反复解析，加快编译速度，解决宏污染和符号泄漏问题。

```cpp
// === math_utils.ixx（模块接口单元）===
export module math_utils;  // 声明模块

import <iostream>;
import <cmath>;

export namespace MathUtils {
    // 导出的函数（对外可见）
    double circleArea(double r) {
        return M_PI * r * r;
    }

    template<typename T>
    T clamp(T val, T lo, T hi) {
        return val < lo ? lo : (val > hi ? hi : val);
    }

    // 导出的类
    export class Vector2D {
    public:
        double x, y;
        Vector2D(double x, double y) : x(x), y(y) {}
        double length() const { return std::sqrt(x*x + y*y); }
    };
}

// 内部实现（不导出）
namespace {
    double helper() { return 42.0; }
}

// === main.cpp ===
import math_utils;       // 导入模块（无宏污染）
import <iostream>;
import <vector>;

int main() {
    using namespace MathUtils;

    std::cout << circleArea(5.0) << "\n";
    std::cout << clamp(15, 0, 10) << "\n";

    Vector2D v{3.0, 4.0};
    std::cout << v.length() << "\n";  // 5

    return 0;
}
```

---

### 5. 三路比较运算符 <=>

**原理：** 宇宙飞船运算符（Spaceship Operator）一次定义三种比较语义（小于/等于/大于），编译器可以自动从 `<=>` 生成所有六个比较运算符。

```cpp
#include <compare>
#include <iostream>
#include <string>
#include <vector>

struct Point {
    int x, y;

    // 自动生成所有比较运算符
    auto operator<=>(const Point&) const = default;
};

struct Version {
    int major, minor, patch;

    // 自定义三路比较
    std::strong_ordering operator<=>(const Version& other) const {
        if (auto cmp = major <=> other.major; cmp != 0) return cmp;
        if (auto cmp = minor <=> other.minor; cmp != 0) return cmp;
        return patch <=> other.patch;
    }

    bool operator==(const Version&) const = default;
};

struct FloatWrapper {
    double value;
    // 浮点返回 partial_ordering（NaN 不参与排序）
    std::partial_ordering operator<=>(const FloatWrapper& other) const {
        return value <=> other.value;
    }
};

int main() {
    Point p1{1, 2}, p2{1, 3}, p3{1, 2};

    std::cout << (p1 < p2) << "\n";   // true
    std::cout << (p1 == p3) << "\n";  // true
    std::cout << (p2 > p1) << "\n";   // true

    Version v1{1, 2, 3}, v2{1, 3, 0}, v3{1, 2, 3};
    std::cout << (v1 < v2) << "\n";   // true（minor: 2 < 3）
    std::cout << (v1 == v3) << "\n";  // true

    // 三种排序类别
    // std::strong_ordering  ：等价即相同（整数）
    // std::weak_ordering    ：等价不一定相同（大小写不敏感字符串）
    // std::partial_ordering ：部分有序（浮点，NaN != NaN）

    auto cmp = 5 <=> 10;  // strong_ordering::less
    if (cmp < 0) std::cout << "小于\n";
    if (cmp == 0) std::cout << "等于\n";
    if (cmp > 0) std::cout << "大于\n";

    return 0;
}
```

---

### 6. consteval 与 constinit

```cpp
#include <iostream>

// consteval：立即函数，必须在编译期求值（不能运行时调用）
consteval int square(int n) {
    return n * n;
}

// constinit：变量必须在编译期初始化（但可以在运行时修改）
// 防止"静态初始化顺序惨案"（Static Initialization Order Fiasco）
constinit int globalVal = square(5);  // 编译期初始化，值 = 25

// 对比：
// constexpr：编译期或运行时都可以
// consteval：只能编译期
// constinit：编译期初始化，不限制后续修改

constexpr int maybe_compile_time(int n) {
    return n * 2;  // 可能是运行时
}

consteval int must_compile_time(int n) {
    return n * 2;  // 必须是编译期
}

int main() {
    constexpr int a = square(10);  // 编译期
    // int x = 5;
    // int b = square(x);  // 错误！consteval 不能运行时调用

    std::cout << a << "\n";        // 100
    std::cout << globalVal << "\n"; // 25

    globalVal = 100;  // constinit 不阻止运行时修改
    std::cout << globalVal << "\n"; // 100

    return 0;
}
```

---

### 7. Lambda 增强

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // C++20：Lambda 支持模板参数
    auto typed_print = []<typename T>(const T& val) {
        std::cout << val << "\n";
    };
    typed_print(42);
    typed_print(std::string("hello"));

    // Lambda 模板 + 约束
    auto sum_all = []<typename T>(std::vector<T> v) {
        T total{};
        for (const auto& x : v) total += x;
        return total;
    };
    std::cout << sum_all(std::vector<int>{1,2,3,4,5}) << "\n";

    // C++20：无捕获的有状态 Lambda（用 static）
    auto counter = []() {
        static int n = 0;
        return ++n;
    };
    std::cout << counter() << counter() << counter() << "\n";  // 123

    // C++20：[=, this] 修正（C++17 [=] 隐式捕获 this，C++20 弃用）
    struct Widget {
        int value = 42;
        auto getFunc() {
            return [=, this]() { return value; };  // C++20 显式
        }
    };

    // Lambda 可以是 consteval
    auto sq = [](int n) consteval { return n * n; };
    constexpr int r = sq(5);
    std::cout << r << "\n";  // 25

    return 0;
}
```

---

### 8. 聚合初始化改进

```cpp
#include <iostream>
#include <string>

// C++20 指定初始化器（Designated Initializers）
struct Config {
    std::string host = "localhost";
    int port = 8080;
    bool ssl = false;
    int timeout = 30;
};

struct Point3D {
    int x = 0, y = 0, z = 0;
};

int main() {
    // 按名称初始化（不需要按顺序，但字段名必须存在且有序）
    Config c1{.host = "example.com", .port = 443, .ssl = true};
    // 未指定的字段使用默认值
    std::cout << c1.host << ":" << c1.port << " ssl=" << c1.ssl << "\n";
    std::cout << "timeout=" << c1.timeout << "\n";  // 30（默认）

    Config c2{.port = 9090};  // 只覆盖 port
    std::cout << c2.host << ":" << c2.port << "\n";  // localhost:9090

    Point3D p{.x = 1, .z = 3};  // y 默认 0
    std::cout << p.x << " " << p.y << " " << p.z << "\n";  // 1 0 3

    // 注意：必须按声明顺序，不能跳序（.z = 1, .x = 2 编译错误）

    return 0;
}
```

---

### 9. std::span

**原理：** `std::span<T>` 是对连续内存序列的非拥有视图（类似 `string_view` 之于字符串），适配数组、`vector`、`array` 等，避免传递指针+大小的繁琐模式。

```cpp
#include <span>
#include <vector>
#include <array>
#include <iostream>
#include <numeric>
#include <algorithm>

// 接受任意连续序列，无需模板
void printSpan(std::span<const int> s) {
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";
}

double average(std::span<const double> data) {
    if (data.empty()) return 0.0;
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

void fillWithIndex(std::span<int> s) {
    std::iota(s.begin(), s.end(), 0);
}

int main() {
    // 接受 C 数组
    int arr[] = {1, 2, 3, 4, 5};
    printSpan(arr);

    // 接受 vector
    std::vector<int> v = {6, 7, 8, 9, 10};
    printSpan(v);

    // 接受 std::array
    std::array<int, 3> a = {11, 12, 13};
    printSpan(a);

    // 子视图（不分配内存）
    std::span<int> full(v);
    auto first3 = full.first(3);   // 前3个
    auto last2 = full.last(2);     // 后2个
    auto middle = full.subspan(1, 3);  // 从索引1开始的3个

    printSpan(first3);   // 6 7 8
    printSpan(last2);    // 9 10
    printSpan(middle);   // 7 8 9

    // 修改（非 const span）
    fillWithIndex(v);
    printSpan(v);  // 0 1 2 3 4

    // 静态大小 span
    std::span<int, 5> fixed(arr);  // 编译期固定大小
    std::cout << fixed.size() << "\n";  // 5

    // 字节视图
    std::span<const std::byte> bytes = std::as_bytes(std::span(arr));
    std::cout << "字节数: " << bytes.size() << "\n";  // 20

    return 0;
}
```

---

### 10. std::format

**原理：** 类型安全的格式化库（类似 Python 的 f-string），替代 `printf` 和 `std::stringstream`，编译期检查格式字符串。

```cpp
#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

int main() {
    // 基本用法
    std::string s1 = std::format("Hello, {}!", "World");
    std::cout << s1 << "\n";  // Hello, World!

    // 位置参数
    std::string s2 = std::format("{0} + {1} = {2}", 1, 2, 3);
    std::cout << s2 << "\n";  // 1 + 2 = 3

    // 格式规范
    std::cout << std::format("{:>10}", "right") << "\n";   // 右对齐
    std::cout << std::format("{:<10}", "left") << "\n";    // 左对齐
    std::cout << std::format("{:^10}", "center") << "\n";  // 居中
    std::cout << std::format("{:*^10}", "hi") << "\n";     // 填充字符

    // 数值格式
    std::cout << std::format("{:.3f}", 3.14159) << "\n";  // 3.142
    std::cout << std::format("{:08.2f}", 3.14) << "\n";   // 00003.14
    std::cout << std::format("{:+d}", 42) << "\n";         // +42
    std::cout << std::format("{:b}", 42) << "\n";          // 101010（二进制）
    std::cout << std::format("{:x}", 255) << "\n";         // ff（十六进制）
    std::cout << std::format("{:#x}", 255) << "\n";        // 0xff

    // 直接输出到流（std::print，C++23）
    // std::print("Hello, {}!\n", "C++23");

    // 自定义类型格式化
    struct Color { uint8_t r, g, b; };
    // 需要特化 std::formatter<Color>（略）

    // 格式化表格
    std::vector<std::pair<std::string, int>> data = {
        {"Alice", 95}, {"Bob", 87}, {"Charlie", 92}
    };
    std::cout << std::format("{:<10} {:>5}\n", "Name", "Score");
    std::cout << std::string(16, '-') << "\n";
    for (const auto& [name, score] : data) {
        std::cout << std::format("{:<10} {:>5}\n", name, score);
    }

    return 0;
}
```

---

### 11. std::jthread

**原理：** "Joining thread"——析构时自动 `join()`，并支持协作式中断（`stop_token`），解决了 `std::thread` 忘记 `join` 导致 `terminate` 的问题。

```cpp
#include <thread>
#include <stop_token>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main() {
    // jthread：析构时自动 join
    {
        std::jthread t([]() {
            std::cout << "线程运行\n";
        });
    }  // 自动 join，不会 terminate

    // stop_token：协作式取消
    std::jthread worker([](std::stop_token st) {
        int count = 0;
        while (!st.stop_requested()) {  // 检查是否请求停止
            std::cout << "工作中... " << ++count << "\n";
            std::this_thread::sleep_for(200ms);
        }
        std::cout << "优雅退出\n";
    });

    std::this_thread::sleep_for(700ms);
    worker.request_stop();  // 请求停止
    // jthread 析构时自动 join

    // stop_callback：停止时回调
    std::jthread bg([](std::stop_token st) {
        std::stop_callback cb(st, []() {
            std::cout << "收到停止信号，清理资源\n";
        });
        while (!st.stop_requested()) {
            std::this_thread::sleep_for(100ms);
        }
    });
    std::this_thread::sleep_for(300ms);
    bg.request_stop();

    return 0;
}
```

---

### 12. std::source_location

```cpp
#include <source_location>
#include <iostream>
#include <string>

// 替代 __FILE__, __LINE__, __FUNCTION__ 宏
void log(const std::string& msg,
         const std::source_location& loc = std::source_location::current()) {
    std::cout << "[" << loc.file_name() << ":"
              << loc.line() << " "
              << loc.function_name() << "] "
              << msg << "\n";
}

template<typename T>
T debug_return(T val,
               const std::source_location& loc = std::source_location::current()) {
    log("返回值: " + std::to_string(val), loc);
    return val;
}

class Logger {
public:
    static void info(const std::string& msg,
                     std::source_location loc = std::source_location::current()) {
        std::cout << "INFO [" << loc.file_name() << ":"
                  << loc.line() << "]: " << msg << "\n";
    }
};

int compute() {
    return debug_return(42);  // 自动记录调用位置
}

int main() {
    log("程序启动");       // 记录调用位置
    compute();
    Logger::info("初始化完成");

    auto loc = std::source_location::current();
    std::cout << "当前位置: " << loc.file_name() << ":" << loc.line() << "\n";

    return 0;
}
```

---

### 13. std::bit_cast

**原理：** 类型安全的位重新解释（替代 `memcpy`/`reinterpret_cast` 的 UB 用法），在编译期验证大小一致，C++20 可以在 `constexpr` 中使用。

```cpp
#include <bit>
#include <cstdint>
#include <iostream>
#include <array>

int main() {
    // 安全地将 float 的位模式解释为 uint32_t
    float f = 1.0f;
    uint32_t bits = std::bit_cast<uint32_t>(f);
    std::cout << std::hex << bits << "\n";  // 3f800000（IEEE 754 的 1.0）

    // 反向
    uint32_t raw = 0x40490FDB;  // π 的近似位模式
    float pi_approx = std::bit_cast<float>(raw);
    std::cout << std::fixed << pi_approx << "\n";  // ~3.14159

    // 双精度
    double d = 3.14;
    uint64_t d_bits = std::bit_cast<uint64_t>(d);
    std::cout << std::hex << d_bits << "\n";

    // 数组转换
    std::array<uint8_t, 4> bytes = {0x00, 0x00, 0x80, 0x3F};  // 小端 1.0f
    float from_bytes = std::bit_cast<float>(bytes);
    std::cout << from_bytes << "\n";

    // 编译期使用
    constexpr float cf = 1.0f;
    constexpr uint32_t cbits = std::bit_cast<uint32_t>(cf);
    static_assert(cbits == 0x3F800000);

    // 其他位操作函数（C++20 <bit>）
    uint32_t n = 0b10110100;
    std::cout << std::popcount(n) << "\n";          // 4（1的个数）
    std::cout << std::countl_zero(n) << "\n";       // 24（前导0）
    std::cout << std::countr_zero(n) << "\n";       // 2（尾随0）
    std::cout << std::bit_width(n) << "\n";         // 8（最高位+1）
    std::cout << std::has_single_bit(n) << "\n";    // 0（不是2的幂）
    std::cout << std::bit_ceil(n) << "\n";          // 192（向上取最近2的幂）
    std::cout << std::bit_floor(n) << "\n";         // 128（向下取最近2的幂）
    std::cout << std::rotl(n, 2) << "\n";           // 循环左移2位

    return 0;
}
```

---

### 14. using enum

```cpp
#include <iostream>

enum class Color { Red, Green, Blue };
enum class Status { Ok, Error, Pending };

void handleColor(Color c) {
    // C++20：using enum 将枚举成员引入当前作用域
    using enum Color;
    switch (c) {
        case Red:   std::cout << "红色\n"; break;   // 不用写 Color::Red
        case Green: std::cout << "绿色\n"; break;
        case Blue:  std::cout << "蓝色\n"; break;
    }
}

class Traffic {
    Color light;
public:
    using enum Color;  // 类作用域内引入
    Traffic() : light(Red) {}

    void next() {
        switch (light) {
            case Red:   light = Green; break;
            case Green: light = Blue;  break;
            case Blue:  light = Red;   break;
        }
    }
};

int main() {
    handleColor(Color::Green);

    {
        using enum Status;
        Status s = Ok;
        if (s == Ok) std::cout << "状态正常\n";
    }

    Traffic t;
    t.next();

    return 0;
}
```

---

## 特性总览对比表

| 特性 | C++11 | C++14 | C++17 | C++20 |
|------|:-----:|:-----:|:-----:|:-----:|
| auto 推导 | ✓ | ✓扩展 | - | - |
| Lambda | ✓ | 通用Lambda | - | 模板Lambda |
| 移动语义 | ✓ | - | - | - |
| 智能指针 | ✓ | make_unique | - | - |
| 可变模板 | ✓ | - | 折叠表达式 | - |
| constexpr | ✓基础 | ✓放宽 | ✓更多 | consteval/constinit |
| 结构化绑定 | - | - | ✓ | - |
| optional/variant/any | - | - | ✓ | - |
| string_view | - | - | ✓ | - |
| if constexpr | - | - | ✓ | - |
| CTAD | - | - | ✓ | - |
| 并行算法 | - | - | ✓ | - |
| Ranges | - | - | - | ✓ |
| Concepts | - | - | - | ✓ |
| Coroutines | - | - | - | ✓ |
| Modules | - | - | - | ✓ |
| <=> 运算符 | - | - | - | ✓ |
| std::format | - | - | - | ✓ |
| std::span | - | - | - | ✓ |
| jthread | - | - | - | ✓ |
| std::filesystem | - | - | ✓ | - |

---

## 编译命令参考

```bash
# C++11
g++ -std=c++11 main.cpp -o main

# C++14
g++ -std=c++14 main.cpp -o main

# C++17
g++ -std=c++17 main.cpp -o main

# C++20
g++ -std=c++20 main.cpp -o main

# C++23（部分编译器支持）
g++ -std=c++23 main.cpp -o main

# 开启所有警告 + 优化
g++ -std=c++20 -Wall -Wextra -O2 main.cpp -o main

# MSVC
cl /std:c++20 /W4 main.cpp

# Clang
clang++ -std=c++20 -Wall main.cpp -o main
```

---

*本文档涵盖 C++11/14/17/20 核心特性，建议配合 [cppreference.com](https://cppreference.com) 查阅完整 API 文档。*