#include <iostream>
#include <regex>
#include <string>
#include <vector>

// 打印分隔线
void printSection(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
}

int main(){

    // ============================================================
    // 测试目标：手机号
    // 规则：1开头，第二位3~9，后跟9位任意数字，共11位
    // ============================================================

    // std::regex 用来存储编译后的正则表达式
    std::regex phonePattern(R"(1[3-9]\d{9})");

    std::vector<std::string> testPhones = {
        "13812345678",   // ✅ 合法
        "19912345678",   // ✅ 合法
        "12312345678",   // ❌ 第二位是2，不合法
        "1381234567",    // ❌ 只有10位，不合法
        "138123456789",  // ❌ 12位，不合法
    };

    // std::regex_match 全串匹配
    printSection("1. regex_match 验证手机号格式");
    for(auto& phone : testPhones) {
        bool valid = std::regex_match(phone, phonePattern);
        std::cout << phone << " -> " << (valid ? "✅ 合法" : "❌ 不合法") << "\n";
    }

    // std::regex_search 子串搜索
    // std::smatch 用来存储匹配结果，针对std::string
    printSection("2. regex_search 从文本中搜索手机号");

    std::string text = "联系人：张三，电话：13812345678，备用：无";
    std::smatch match;
    if (std::regex_search(text, match, phonePattern)) {
        std::cout << "在文本中找到手机号: " << match[0] << "\n";
        std::cout << "手机号前面的内容:   " << match.prefix() << "\n";
        std::cout << "手机号后面的内容:   " << match.suffix() << "\n";
    }

    // -------------------------------------------------------
    // 3. 捕获组 —— 拆解手机号的组成部分
    //    分组：(1[3-9])  前两位
    //          (\d{4})   中间四位
    //          (\d{5})   最后五位
    // -------------------------------------------------------
    // std::smatch 会存储完整匹配，以及根据正则表达式的分组内容。
    printSection("3. 捕获组 拆解手机号");

    std::regex phoneGrouped(R"((1[3-9])(\d{4})(\d{5}))");
    std::string phone = "13812345678";
    std::smatch groups;

    if (std::regex_match(phone, groups, phoneGrouped)) {
        std::cout << "完整匹配:   " << groups[0] << "\n";
        std::cout << "前两位:     " << groups[1] << "\n";
        std::cout << "中间四位:   " << groups[2] << "\n";
        std::cout << "后五位:     " << groups[3] << "\n";
    }

    // -------------------------------------------------------
    // 4. sregex_iterator —— 找出文本中所有手机号
    // -------------------------------------------------------
    printSection("4. sregex_iterator 提取所有手机号");

    std::string contacts = "张三:13812345678 李四:19987654321 王五:12312345678(无效) 赵六:15566667777";

    auto it  = std::sregex_iterator(contacts.begin(), contacts.end(), phonePattern);
    auto end = std::sregex_iterator();

    int count = 0;
    for (; it != end; ++it) {
        std::cout << "找到第 " << ++count << " 个手机号: " << (*it)[0] << "\n";
    }
    std::cout << "共找到 " << count << " 个合法手机号\n";

    // -------------------------------------------------------
    // 5. regex_replace —— 将手机号中间四位打码
    //    替换格式：前两位 + **** + 后五位
    // -------------------------------------------------------
    printSection("5. regex_replace 手机号脱敏打码");

    // 对于这个正则表达式来说，有括号表示会记住，后面可以使用$1..来引用这一部分。如果没有括号，只会匹配，但不会记住该部分。
    std::regex phoneForReplace(R"((1[3-9])\d{4}(\d{5}))");
    std::string result = std::regex_replace(contacts, phoneForReplace, "$1*****$2");
    std::cout << "原始: " << contacts << "\n";
    std::cout << "打码: " << result   << "\n";

    // -------------------------------------------------------
    // 6. regex_error —— 捕获非法正则表达式的异常
    // -------------------------------------------------------
    printSection("6. regex_error 异常处理");

    try {
        std::regex badPattern("[unclosed");  // 故意写错
    } catch (const std::regex_error& e) {
        std::cout << "捕获正则异常: " << e.what() << "\n";
    }

    return 0;
}