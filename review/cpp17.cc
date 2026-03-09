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
int main(){
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
    a.emplace(args...);     // 析构旧值，原地构造新值
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
    opt.value_or(expensive_func());  // 即使 opt 有值，expensive_func 也会被调用

    // 真正的懒求值：
    auto val = opt ? *opt : expensive_func();

    // 7.直接在内存上构造
    // in_place 是构造时的原地构造，emplace 是构造后的原地构造，底层机制完全一样，都是 placement new 直接在内部 storage 上建对象。
    return 0;
}