#include "my_shared_ptr.h"
#include <iostream>

int main() {

    auto ptr = custom::make_shared<int>(321);

    {
        auto ptr2 = ptr;

        std::cout << "p1 " << *ptr << std::endl;
        std::cout << "p2 " << *ptr2 << std::endl;
    }

    std::cout << "p1 again: " << *ptr << std::endl;

    return 0;
}