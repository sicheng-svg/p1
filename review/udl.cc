#include <iostream>
#include <algorithm>
#include <string>

// 用户定义字面量 udl 就是给字面量带上后缀，让其自带含义。在复杂场景下，简单的由变量名不能解决，可以使用udl来解决。
// 例如： double distance = 12.01_km 一看表示的就是千米
// 自定义后缀必须以_开头，不含_的后缀是标准库保留的，会与标准库发生冲突。
// 对于udl来说，参数类型只有以下几种：
// 整型 unsigned long long
// 浮点数 long double
// 字符 char
// 字符串 const char*, size_t
// 返回值可以是任意类型

// 1.物理单位
constexpr double operator "" _km(long double v) { return v * 1000.0; }
constexpr double operator "" _m (long double v) { return v; }
constexpr double operator "" _cm(long double v) { return v / 100.0; }

// 2.字符串增强
// "hello"_upper  →  "HELLO"
std::string operator "" _upper(const char* str, size_t len) {
    std::string result(str, len);
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// "hello"_repeat3  重复3次（演示整数后缀作用于字符串的另一种做法）
std::string operator "" _s(const char* str, size_t len) {
    return std::string(str, len);  // 方便构造 std::string
}

// 打印分隔线
void printSection(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

int main() {
    printSection("1.物理单位");
    double d1 = 100.0_km;
    double d2 = 100.0_m;
    double d3 = 100.0_cm;

    std::cout << d1 << std::endl;
    std::cout << d2 << std::endl;
    std::cout << d3 << std::endl;

    printSection("2.字符串增强");
    std::string str = "hello"_upper;
    auto ret = "bye"_s;
    std::cout << "str: " << str << std::endl;
    std::cout << "ret: " << ret << std::endl;


    return 0;
}