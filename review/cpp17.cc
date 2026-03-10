#include <variant>
#include <optional>
#include <thread>
#include <mutex>
#include <tuple>
#include <queue>
#include <map>
#include <string>
#include <iostream>

class MyPair {
    int a_; double b_;
public:
    MyPair(int a, double b) : a_(a), b_(b) {}
    
    template<std::size_t I>
    auto get() const {
        if constexpr (I == 0) return a_;
        else return b_;
    }
};

// 特化 std::tuple_size 和 std::tuple_element
namespace std {
    template<> struct tuple_size<MyPair> : integral_constant<size_t, 2> {};
    template<> struct tuple_element<0, MyPair> { using type = int; };
    template<> struct tuple_element<1, MyPair> { using type = double; };
}

MyPair mp{1, 3.14};
auto [a, b] = mp;  // a=1, b=3.14

int main1(){
    // 基本语法
    // auto [var1, var2, ...] = expression;

    // 三种绑定方式
    // auto [a, b] = expr; 拷贝
    // auto& [a, b] = expr; 左值引用    
    // auto&& [a, b] = expr; 右值引用

    // 1.绑定数组
    int arr[4] = {1, 2, 3, 4};
    auto [x, y, z, h] = arr;
    std::cout << x << " " << y << " " << z << " " << h << std::endl;

    // 2.绑定tuple/pair
    std::pair<int, std::string> person(120, "ambulance");
    auto& [i, s] = person;
    std::cout << i << " " << s << std::endl;

    std::tuple<int, double, std::string> tmp(1, 2.2, "3.3");
    auto& [a, b, c] = tmp;
    std::cout << a << " " << b << " " << c << std::endl;

    // 3.绑定结构体，最常用
    struct point{int _x; int _y;};
    point p{10, 20};
    auto [x1, y1] = p;
    std::cout << x1 << " " << y1 << std::endl;
    // 如果绑定的成员变量是私有的，不能直接绑定，需要使用tuple-like协议
    // class point{int _x; int _y; };
    // point p{10, 20};
    // auto [x1, y1] = p;
     

    // 4.遍历map
    std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};

    // C++17 之前
    for (const auto& kv : scores) {
        std::cout << kv.first << ": " << kv.second;
    }
    
    // C++17 之后
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score;
    }

    // 5.结构化绑定对于自定义类型有两种绑定路径
    // 5.1 聚合类型，直接绑定。编译期直接按照成员变量顺序进行绑定，条件是成员必须是public
    // 5.2 tuple-like协议，简介绑定
    // 当成员是private时，编译期无法直接访问，必须你手动告诉它：
    // 有几个元素(tuple_size)
    // 每个元素是什么类型(tuple_element)
    // 怎么取到每个元素(get<I>())
    MyPair pair(1, 2);
    auto [p1, p2] = pair;


    return 0;
}

int main2(){
    // if/switch 初始化语句
    // 可以在if/switch中初始化变量，并且将该变量的生命周期限制在if/switch内，避免污染后面的空间

    // 1.map查找
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}};

    if (auto it = m.find(2); it != m.end()) {
        std::cout << it->second;  // "two"
    }

    // 2.结合结构化绑定
    if (auto [it, ok] = m.insert({3, "three"}); ok) {
        std::cout << "插入成功: " << it->second;
    } else {
        std::cout << "key 已存在";
    }

    // // 3.锁的生命周期精确限定在 if 块内
    // if (std::lock_guard<std::mutex> lock(mtx); !q.empty()) {
    //     process(q.front());
    //     q.pop();
    // }
    // // lock 在此析构，自动释放锁 ✓

    // // 4.switch初始化
    // enum class Status{OK, Error};
    // switch (auto status = getStatus(); status) {
    //     case Status::OK:    handle_ok();    break;
    //     case Status::Error: handle_error(); break;
    //     default:            break;
    // }
    // status 不泄漏到外部

    return 0;
}

// std::optional<T>
int main3(){
    std::optional<int> opt;
    if(opt.has_value()) {
        std::cout << "yes" << std::endl;
    }else{
        std::cout << "no" << std::endl;
    }

    // 1.构造/赋值
    std::optional<int> a;                  // 空
    std::optional<int> b = std::nullopt;   // 显式为空
    std::optional<int> c = 42;             // 有值
    std::optional<int> d(std::in_place, 42); // 原地构造（避免拷贝）

    a = 42;                 // 赋值
    a = std::nullopt;       // 清空
    a.emplace(42);          // 原地构造，比 = 更高效（对复杂对象）
    a.reset();              // 清空，等价于 a = nullopt

    // 2.检查是否有值
    if (a.has_value()) { }  // 显式检查
    if (a) { }              // 隐式 bool 转换，等价
    if (!a) { }             // 为空

    // 3.取值
    a.value();               // 有值返回，无值抛 std::bad_optional_access
    *a;                      // 有值返回，无值是 UB（不检查，性能更好）
    a.operator->();          // 访问成员，等价于 (*a).member，无值是 UB
    // a.value_or(默认值);      // 无值时返回默认值，最安全常用

    // 4.修改
    a.reset();              // 析构内部值，置为空
    // a.emplace(args...);     // 析构旧值，原地构造新值
    a.swap(b);              // 和另一个 optional 交换

    // 5.比较
    // optional 之间比较 ,空的 optional 小于任何有值的 optional。
    std::optional<int> x = 1, y = 2;
    x < y;    // true（有值的按值比较）
    x == y;   // false

    // 和 nullopt 比较
    x == std::nullopt;  // false（x 有值）

    // 和 T 直接比较
    x == 1;   // true
    x < 2;    // true

    // 6.懒求值和急求值
    // value_or 的参数是立即求值的！
    // opt.value_or(expensive_func());  // 即使 opt 有值，expensive_func 也会被调用

    // 真正的懒求值：
    // auto val = opt ? *opt : expensive_func();

    // 7.直接在内存上构造
    // in_place 是构造时的原地构造，emplace 是构造后的原地构造，底层机制完全一样，都是 placement new 直接在内部 storage 上建对象。
    return 0;
}

void test1(){
    std::variant<int, double, std::string> v;

    // 1. 直接赋值 —— 自动推导类型
    v = 42;
    v = 3.14;
    v = std::string("hello");

    // 2. emplace<T>(args...) —— 原地构造，销毁旧值
    v.emplace<std::string>(5, 'x');    // 持有 "xxxxx"

    // 3. emplace<Index>(args...) —— 按索引原地构造
    v.emplace<0>(100);                 // 索引0 是 int，持有 100

    // 4. swap
    std::variant<int, std::string> a = 1, b = std::string("hi");
    a.swap(b);   // a 持有 "hi"，b 持有 1
    // 也可以 std::swap(a, b);
}

void test2(){
    std::variant<int, double, std::string> v = "hello";

    // 1. std::get<T>(v) —— 按类型取，不匹配抛 std::bad_variant_access
    std::string s = std::get<std::string>(v);  // OK
    // std::get<int>(v);  // 抛异常

    // 2. std::get<Index>(v) —— 按索引取
    std::string s2 = std::get<2>(v);  // 索引2 是 string，OK

    // 3. std::get_if<T>(&v) —— 按类型取指针，不匹配返回 nullptr
    if (auto* p = std::get_if<std::string>(&v)) {
        std::cout << *p << "\n";   // "hello"
    }
    // 注意参数是指针，不是引用

    // 4. std::get_if<Index>(&v) —— 按索引取指针
    if (auto* p = std::get_if<2>(&v)) {
        std::cout << *p << "\n";
    }
}

// 状态查询
void test3(){
    std::variant<int, double, std::string> v = 3.14;

    // 1. index() —— 返回当前持有类型的索引
    v.index();  // 1（double 在模板参数列表中排第二）

    // 2. std::holds_alternative<T>(v) —— 判断当前是否持有类型 T
    std::holds_alternative<double>(v);       // true
    std::holds_alternative<int>(v);          // false

    // 3. valueless_by_exception() —— 是否处于无值状态
    v.valueless_by_exception();  // false（正常情况下都是 false）
}

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void test4(){
    std::variant<int, double, std::string> v = 42;

    // 1. 泛型 lambda
    std::visit([](auto&& val) {
        std::cout << val << "\n";
    }, v);

    // 2. 带 if constexpr 的泛型 lambda（按类型分支处理）
    std::visit([](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>)
            std::cout << "int: " << val << "\n";
        else if constexpr (std::is_same_v<T, double>)
            std::cout << "double: " << val << "\n";
        else
            std::cout << "string: " << val << "\n";
    }, v);

    // 3. overloaded pattern（最优雅）
    std::visit(overloaded{
        [](int i)                { std::cout << "int: " << i << "\n"; },
        [](double d)             { std::cout << "double: " << d << "\n"; },
        [](const std::string& s) { std::cout << "string: " << s << "\n"; }
    }, v);

    // 4. 多 variant 联合 visit（笛卡尔积分发）
    std::variant<int, std::string> a = 1;
    std::variant<double, char> b = 'x';
    std::visit([](auto&& x, auto&& y) {
        std::cout << x << ", " << y << "\n";
    }, a, b);
    // 编译器会为 2×2=4 种组合都生成代码
}

// std::variant
int main() {
    // 默认构造：构造第一个备选类型的默认值，如果没有默认构造则会抛出异常
    std::variant<int, std::string> v1; // int(0)

    // 直接初始化：自动推导匹配的备选类型
    std::variant<int, double, std::string> v2(42); // 持有int
    std::variant<int, double, std::string> v3(3.13); // 持有double
    std::variant<int, double, std::string> v4("hello"); // 持有std::string

    // std::in_place_type 显示指定类型构造，原地构造
    std::variant<int, std::string> v5(std::in_place_type<std::string>, 5, 'a');
    // 持有std::string("aaaaa");

    // std::in_place_index —— 按索引指定类型，原地构造
    std::variant<int, std::string> v6(std::in_place_index<1>, "world");
    // 索引1是string，持有std::string("world")

    // 拷贝构造/移动构造
    auto v7 = v5;
    auto v8 = std::move(v5); 

    return 0;
}