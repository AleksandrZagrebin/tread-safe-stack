#include "ts_stack.hpp"
#include <iostream>
#include <thread>
#include <vector>

int NTasks;
std::mutex TaskMut;

std::vector<int> Consumed;
std::mutex ConsMut;

int main()
{
constexpr int NTASKS = 20;
constexpr int BUFSZ = 5;
constexpr int NPRODUCERS = 2;
constexpr int NCONSUMERS = 2;

NTasks = NTASKS;
Consumed.clear();

ts_stack<int> Q(BUFSZ);

std::vector<std::thread> producers;
std::vector<std::thread> consumers;


for (int i = 0; i < NPRODUCERS; ++i)
    producers.emplace_back(produce, std::ref(Q),i);
for (int i = 0; i < NCONSUMERS; ++i)
    consumers.emplace_back(consume, std::ref(Q), i);

for (auto& t : producers)
    t.join();

Q.wake_and_done();

for (auto& t : consumers)
    t.join();

std::cout << "\n=== Results ===" << std::endl;
std::cout << "Total consumed: " << Consumed.size() << " items" << std::endl;

if (Consumed.size() == static_cast<size_t>(NTASKS + 1))
    std::cout << "SUCCESS: All tasks consumed!" << std::endl;
else
    std::cout << "ERROR: Expected " << NTASKS + 1 << ", got " << Consumed.size() << std::endl;

return 0;
}
