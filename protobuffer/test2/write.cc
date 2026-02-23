#include <iostream>
#include <fstream>
#include "contacts.pb.h"
    
void addNewContact(contacts::People* people)
{
    std::cout << "add new contact" << std::endl;
    std::cout << "enter contact name: ";
    std::string name;
    getline(std::cin, name);
    people->set_name(name);

    std::cout << "enter contact age: ";
    int age = 0;
    std::cin >> age;
    std::cin.ignore(256, '\n');
    people->set_age(age);

    std::cout << "enter contact phone number:";
    for(int i=0;;++i) {
        std::cout << "NO." << i+1 << ": ";
        std::string number;
        getline(std::cin, number);
        if(number.empty()) break;
        auto phone = people->add_phone();
        phone->set_number(number);

        std::cout << "select phone type: 0.unknow 1.moblie phone 2.landline: ";
        int type = 0;
        std::cin >> type;
        std::cin.ignore(256, '\n');
        if(type == 1) {
            phone->set_type(contacts::PhoneType::MOBILE_PHONE);
        }else if(type == 2) {
            phone->set_type(contacts::PhoneType::LANDLINE);
        }else{
            phone->set_type(contacts::PhoneType::UNKNOWN);
        }
    }

    // 先将地址存在address消息中，随后转换为any，存入到people中
    contacts::Address address;
    std::cout << "enter contact address:" << std::endl;
    std::cout << "home address: ";
    std::string home;
    getline(std::cin, home);
    address.set_home(home);

    std::cout << "work address: ";
    std::string work;
    getline(std::cin, work);
    address.set_work(work);
    people->mutable_date()->PackFrom(address);

    // 为联系人增加其他联系方式
    std::cout << "add other contact: 1.qq 2.wechat: ";
    int other = 0;
    std::cin >> other;
    std::cin.ignore(256, '\n');

    std::string other_contact;
    if(other == 1) {
        std::cout << "enter qq: ";
        getline(std::cin, other_contact);
        people->set_qq(other_contact);
    }else if(other == 2) {
        std::cout << "enter wechat: ";
        getline(std::cin, other_contact);
        people->set_wechat(other_contact);
    }else{
        std::cout << " 不新增其他联系方式" << std::endl;
    }

    std::cout << "add new contact success!" << std::endl;
}

int main()
{
    contacts::Contacts contacts;
    // 读取本地文件，看看是否有已经存在的通讯录文件，没有则新建
    // 如果有，则进行反序列化，读取到内存中
    std::fstream fs("contacts.bin", std::fstream::in | std::fstream::binary);
    if(fs.is_open()) {
        if(!contacts.ParseFromIstream(&fs)) {
            std::cerr << "parse error" << std::endl;
            return -1;
            fs.close();
        }
        std::cout << "parse success!" << std::endl;
    }

    // 新增联系人
    addNewContact(contacts.add_people());

    // 序列化后写入到文件中
    std::fstream output("contacts.bin", std::fstream::out | std::fstream::trunc | std::fstream::binary);
    if(!contacts.SerializePartialToOstream(&output)) {
        std::cerr << "serialize error" << std::endl;
        fs.close();
        output.close();
        return -1;
    }
    std::cout << "serialize success" << std::endl;
    fs.close();
    output.close();

    return 0;
}