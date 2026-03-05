#include <iostream>
#include <string>

class Person {
    std::string name;
    int age;
    std::string email;

public:
    // 主构造函数
    Person(std::string name, int age, std::string email)
        : name(std::move(name)), age(age), email(std::move(email)) {
        std::cout << "主构造\n";
    }

    // 委托构造函数：调用主构造函数
    Person(std::string name, int age)
        : Person(std::move(name), age, "") {}  // 委托

    Person(std::string name)
        : Person(std::move(name), 0) {}  // 委托

    void print() const {
        std::cout << name << ", " << age << ", " << email << "\n";
    }
};

class Employee : public Person {
    std::string company;

public:
    // 继承 Person 的所有构造函数
    using Person::Person;

    Employee(std::string name, int age, std::string company)
        : Person(name, age), company(std::move(company)) {}
};

int main() {
    Person p1("Alice", 30, "alice@example.com");
    Person p2("Bob", 25);         // 委托构造
    Person p3("Charlie");         // 委托构造

    Employee e1("Dave", 28);      // 继承自 Person 的构造函数
    Employee e2("Eve", 25, "ACME");

    return 0;
}