#include "source/cshared_ptr.h"
#include "source/ChainFunctor.h"
#include "source/QueueFreeLock.h"
#include "source/ThreadPool.h"

#include <iostream>
#include <thread>
#include <stdio.h>
#include <chrono>
using namespace std::chrono;

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

void test_queue() {
    plh::queue_free_lock<int> test;
    test.push(12);
    auto a = test.pop();
    std::cout << "queue size = " << test.size() << std::endl;
    std::cout << "value pop = " << (*a) << std::endl;
}

int testPool1(int a, int b) {
    std::cout << "Pool 1..." << std::endl;
    std::this_thread::sleep_for(1s);
    return a*b;
}

void testPool2() {
    std::cout << "Pool 2..." << std::endl;
    std::this_thread::sleep_for(2s);
}

void poolTester()
{
    auto start = high_resolution_clock::now();
    /* Compute square of numbers. */
    plh::PoolExecutor pool;
    pool.startPool(4);
    std::vector<std::future<int>> results;

    for (int i = 0; i < 16; ++i)
    {
        auto future = pool.executeTask([i] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;
        });
        results.emplace_back(std::move(future));
    }
    pool.executeTask([]()->void{cout << "Test void function in pool!!\n";});

    for (auto &result : results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time = " << duration.count() << endl;
}

int main() {
//    plh::Callable<std::function<void()>> a(&test_queue);
//    a.execute();
    poolTester();



    getchar();
}
