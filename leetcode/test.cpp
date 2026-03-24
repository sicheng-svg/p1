#include <iostream>
#include <vector>
#include <string>
#include <functional>

// 布隆过滤器
class BloomFilter {
public:
    // m: 位数组长度, k: 哈希函数个数
    BloomFilter(size_t m, size_t k) : bits_(m, false), m_(m), k_(k) {}

    void add(const std::string& key) {
        for (size_t i = 0; i < k_; ++i) {
            size_t pos = hash(key, i) % m_;
            bits_[pos] = true;
        }
    }

    bool mightContain(const std::string& key) const {
        for (size_t i = 0; i < k_; ++i) {
            size_t pos = hash(key, i) % m_;
            if (!bits_[pos]) {
                return false;  // 有一位是0，一定不存在
            }
        }
        return true;  // 全部为1，可能存在（也可能误判）
    }

private:
    // 用种子区分不同的哈希函数
    // 每个 i 代表第 i 个哈希函数，通过不同的种子让结果不同
    size_t hash(const std::string& key, size_t i) const {
        // std::hash 只能产生一个固定值
        // 我们把 i 拼到 key 后面，让每次哈希的输入不同
        // 这是最简单的做法，生产环境会用 MurmurHash 等
        return std::hash<std::string>{}(key + std::to_string(i));
    }

    std::vector<bool> bits_;  // 位数组
    size_t m_;                // 位数组长度
    size_t k_;                // 哈希函数个数
};

int main() {
    // 创建一个位数组长度100、3个哈希函数的布隆过滤器
    BloomFilter bf(100, 3);

    // 插入一些元素
    bf.add("apple");
    bf.add("banana");
    bf.add("cherry");

    // 查询
    std::cout << "=== 查询已插入的元素 ===" << std::endl;
    std::cout << "apple:  " << bf.mightContain("apple")  << std::endl;  // 1 (一定能查到)
    std::cout << "banana: " << bf.mightContain("banana") << std::endl;  // 1
    std::cout << "cherry: " << bf.mightContain("cherry") << std::endl;  // 1

    std::cout << "\n=== 查询未插入的元素 ===" << std::endl;
    std::cout << "grape:  " << bf.mightContain("grape")  << std::endl;  // 大概率0，小概率1(误判)
    std::cout << "melon:  " << bf.mightContain("melon")  << std::endl;
    std::cout << "peach:  " << bf.mightContain("peach")  << std::endl;

    // 测试误判率：插入1000个元素，查询10000个不存在的元素
    std::cout << "\n=== 误判率测试 ===" << std::endl;
    BloomFilter bf2(10000, 7);  // 更大的位数组，7个哈希函数

    for (int i = 0; i < 1000; ++i) {
        bf2.add("user:" + std::to_string(i));
    }

    int false_positive = 0;
    int test_count = 10000;
    for (int i = 1000; i < 1000 + test_count; ++i) {
        if (bf2.mightContain("user:" + std::to_string(i))) {
            ++false_positive;
        }
    }

    std::cout << "位数组长度: 10000, 哈希函数数: 7, 已插入: 1000" << std::endl;
    std::cout << "测试 " << test_count << " 个不存在的元素" << std::endl;
    std::cout << "误判数: " << false_positive << std::endl;
    std::cout << "误判率: " << (double)false_positive / test_count * 100 << "%" << std::endl;

    return 0;
}