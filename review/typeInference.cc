#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <string>
#include <memory>

std::string demangle(const char* name) {
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };
    return (status == 0) ? res.get() : name;
}

void Auto(){
    // 推导基础类型
    auto x = 5;
    auto y = 3.14;
    auto z = "hello";

    std::cout << demangle(typeid(x).name()) << std::endl;
    std::cout << demangle(typeid(y).name()) << std::endl;
    std::cout << demangle(typeid(z).name()) << std::endl;

    // 在传递过程中推导，会丢失引用和const修饰符
    int a = 10;
    int& ref = a;
    const int b = 20;

    auto c = ref;
    auto d = b;
    std::cout << demangle(typeid(c).name()) << std::endl;
    std::cout << demangle(typeid(d).name()) << std::endl; 

    // 如果不想丢失，则需要手动加上
    auto& e = ref;
    const auto f = b;
    std::cout << demangle(typeid(e).name()) << std::endl;
    std::cout << demangle(typeid(f).name()) << std::endl; 
}

auto add(int a, int b) -> decltype(a + b){
    return a + b;
}

// C++14引入了返回类型推导，可以省略函数的返回类型，编译器会根据函数体自动推导出返回类型
// 需要注意的是，C++14这种auto推导函数返回值，遵循的是模板的推导规则，会丢失引用和const修饰符
auto sub(int a, int b){
    return a - b;
}

decltype(auto) mul(int a, int b){
    return a * b;
}

int main() {
    // Auto();
    add(1, 2);
    return 0;
}