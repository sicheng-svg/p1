#include <iostream>

// C++11
constexpr int add(int a, int b){
    return a + b;
}

// C++14
constexpr int sum(int a, int b){
    if(a < 0) return 0;
    return a - b;
}

// C++17
template <typename T>
auto get_value(T t) {
    // 这里的 if constexpr 会在编译期判断
    if constexpr (std::is_pointer_v<T>) {
        return *t; // 如果 T 是指针，这行保留，另一行被丢弃
    } else {
        return t;  // 如果 T 不是指针，这行保留，上一行被丢弃
    }
}

int main(){
    constexpr int ret = add(1, 2);
    std::cout << ret << std::endl;

    const int a = 100;
    constexpr int b = a;

    int c = sum(1, 2); // 可能在编译时也可能在运行时
    std::cout << c << std::endl;

    int d = 100;
    int* p = &d;

    std::cout << get_value(a) << std::endl; // 编译器只生成 return t; 的代码
    std::cout << get_value(p) << std::endl; // 编译器只生成 return *t; 的代码

    auto addL = [](int a, int b) { return a + b; };
    // C++17 中，这行是合法的！编译器会自动把 add 看作 constexpr
    constexpr int sum = addL(2, 3);

    auto multiply = [](int a, int b) constexpr { // 显式声明
        return a * b;
    };
    return 0;
}