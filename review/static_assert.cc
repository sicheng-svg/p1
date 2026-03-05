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
auto add(T a, U b) ->decltype(a + b) {
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