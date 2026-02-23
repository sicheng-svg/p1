#include <iostream>
#include <fstream>
#include <string>
#include "contacts.pb.h"

int main() {
    contacts::Contacts contacts;
    // 读取本地已经有的通讯录文件
    std::fstream fs("contacts.bin", std::fstream::in | std::fstream::binary);
    if(fs.is_open()) {
        if(!contacts.ParseFromIstream(&fs)) {
            std::cerr << "parse error" << std::endl;
            return -1;
            fs.close();
        }
        std::cout << "parse success!" << std::endl;
    }

    // 反序列化通讯录内容，并打印
    int n = contacts.people_size();
    std::cout << "n: " << n << std::endl;
    for(int i=0; i<n; ++i) {
        auto people = contacts.people(i); // 获取指定下标的联系人

        std::cout << "-------------- 联系人" << i + 1 << "--------------------" << std::endl;
        std::cout << "name: " << people.name() << std::endl;
        std::cout << "age: " << people.age() << std::endl;

        int m = people.phone_size();
        std::cout << "m: " << m << std::endl;
        for(int j=0; j<m; ++j) {
            auto phone = people.phone(j); // 指定联系人的电话信息
            std::cout << "phone " << j + 1 << ": " << phone.number() << ": " << contacts::PhoneType_Name(phone.type()) << std::endl;
        }
    }


    return 0;
}