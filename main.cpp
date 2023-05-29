#include <iostream>
#include "source/cshared_ptr.h"
#include <thread>

using namespace std;

void test_fnc2(const plh::cshared_ptr<int> tmp) {
    auto ptr = tmp.get_ptr();
    if (ptr != nullptr) {
        std::cout << "Value = " << *(int*)ptr << std::endl;
    }
    ++(*(int*)ptr);
}

void test_fnc(const plh::cshared_ptr<int>& tmp) {
    auto test = tmp;
    test_fnc2(test);
}

void xxx()
{
    plh::cshared_ptr<int> abc(2);
    test_fnc(abc);

//    plh::cshared_ptr<int> test = abc;
//    std::cout << "last value = " << *test.get_ptr() << std::endl;
}

int main() {
    auto thre = std::thread(xxx);
    thre.join();
    _sleep(3000);
}
