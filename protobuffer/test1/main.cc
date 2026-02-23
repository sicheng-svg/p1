#include <iostream>
#include "contacts.pb.h"

int main()
{
    std::string str;
    // 使用pb协议，尽心序列化，并打印结果
    contacts::peopleInof p1;
    p1.set_name("zhangsan");
    p1.set_age(29);
    if(!p1.SerializePartialToString(&str)) {
        std::cerr << "serialize error\n";
        return -1;
    }
    std::cout << str << std::endl;

    // 使用pb协议，将序列化的内容，反序列化
    contacts::peopleInof p2;
    if(!p2.ParseFromString(str)) return -1;
    std::cout << p2.name() << std::endl;
    std::cout << p2.age() << std::endl;
    
    return 0;
}