#include <tuple>
#include <string>
#include <iostream>

int main(){
    // tuple 创建
    std::tuple<int, std::string, double> p1(1, "Gemini", 3.1);
    auto p2 = std::make_tuple(2, "ChatGpt", 5.1);

    // tuple 访问
    // 索引必须是编译期常量
    std::cout << std::get<0>(p1) << std::endl;
    std::cout << std::get<1>(p1) << std::endl;
    std::cout << std::get<2>(p1) << std::endl;

    // 使用std::tie将tuple解码到现有变量中
    int id = 0;
    std::string name = "";
    double version = 0.0;
    std::tie(id, name, version) = p2;
    std::cout << id << " " << name << " " << version << std::endl;

    // 可以使用std::ignore占位，忽略你不想要的变量
    std::tie(std::ignore, name, version) = p1;
    std::cout << id << " " << name << " " << version << std::endl;

    // 使用std::tuple_size 获取指定tuple的大小，要指定tuple的类型
    constexpr size_t size = std::tuple_size<decltype(p1)>::value;
    std::cout << "tuple_size-> " << size << std::endl;

    // 使用std::tuple_element 获取特定位置tuple的元素类型
    // 下面使用了这个类型定义了变量
    std::tuple_element<1, decltype(p1)>::type t = std::get<1>(p1);
    std::cout << "t-> " << t << std::endl;

    // 使用std::tuple_cat 拼接tuple，得到一个更大的
    auto p3 = std::tuple_cat(p1, p2);
    std::cout << "tuple_cat->" << std::tuple_size<decltype(p3)>::value << std::endl;

    // C++17可以使用结构化绑定，更简便的将tuple中的元素解析到变量中
    auto [a, b, c] = p1;
    std::cout << "a-> " << a << "\nb-> " << b << "\nc->" << c << std::endl;

    return 0;
}