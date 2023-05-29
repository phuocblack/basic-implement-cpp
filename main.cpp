#include "source/cshared_ptr.h"
#include "source/ChainFunctor.h"

#include <iostream>
#include <thread>
#include <stdio.h>

using namespace std;

class testClass {
public:
    void print() {
        std::cout << "[testClass] please call me!!\n";
    }
};

void test_fnc2(const plh::cshared_ptr<int> tmp) {

    std::cout << "Value = " << *tmp << std::endl;
    std::cout << "de test_fnc2\n";
}

void test_fnc(const plh::cshared_ptr<int>& tmp) {
    auto test = tmp;
    test_fnc2(test);
    std::cout << "de test_fnc\n";
}

int xxx()
{
    plh::cshared_ptr<int> abc;
    plh::cshared_ptr<int> test(2);
    abc = test;
    test_fnc(abc);
    std::cout << "de xxx\n";
    return 1;
}

int main() {
    plh::Callable<std::function<void()>> a(&xxx);
    auto i = a.execute();
    getchar();
}
