#include <iostream>

// define debug to enable debug printing for unique ptr
#define DEBUG
#include "my_unique_ptr.h"


struct test_struct {
    public:
        test_struct(int x, int y) : x_(x), y_(y) {}
        ~test_struct() = default;

        int random_op() {
            return x_ + y_;
        }

    private:
        int x_;
        int y_;
};

int main() {

    custom::unique_ptr<int> test(new int(100));
    std::cout << *test << std::endl;
    *test = 200;

    custom::unique_ptr<int> test2 = std::move(test);
    std::cout << *test2 << std::endl;
    *test2 = 300;

    custom::unique_ptr<int> test3(std::move(test2));
    std::cout << *test3 << std::endl;

    std::cout << "Struct" << std::endl;
    custom::unique_ptr<test_struct> struct_ptr(new test_struct(4, 5));
    std::cout << struct_ptr->random_op() << std::endl;

    return 0;
}