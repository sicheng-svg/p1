#include <utility>
#include <array>
#include <tuple>
#include <iostream>

template<size_t... I>
void print(std::index_sequence<I...>) {
    // 包展开，逗号表达式逐个打印
    int dummy[] = { (std::cout << I << " ", 0)... };
    (void)dummy;
}

template<typename Tuple, size_t... I>
void print_tuple_impl(Tuple& t, std::index_sequence<I...>) {
    int dummy[] = { (std::cout << std::get<I>(t) << " ", 0)... };
    (void)dummy;
}

template<typename Tuple>
void print_tuple(Tuple& t) {
    constexpr size_t N = std::tuple_size<Tuple>::value;
    print_tuple_impl(t, std::make_index_sequence<N>{});
}

// 编译期生成数组
template<size_t... I>
constexpr auto make_squares(std::index_sequence<I...>) {
    return std::array<int, sizeof...(I)>{ (I * I)... };
}

int main() {
    // print(std::make_index_sequence<5>{});
    // 输出：0 1 2 3 4

    // auto t = std::make_tuple(42, 3.14, "hello");
    // print_tuple(t);
    // 输出：42 3.14 hello

    // constexpr auto arr = make_squares(std::make_index_sequence<6>{});

    // for (auto x : arr) std::cout << x << " ";

    constexpr std::index_sequence<5> arr; 
}