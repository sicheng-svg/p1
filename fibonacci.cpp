#include <iostream>

int main()
{
    int a = 0, b = 1;
    while (a <= 1000) {
        std::cout << a << std::endl;
        int next = a + b;
        a = b;
        b = next;
    }
    return 0;
}
