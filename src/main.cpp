#include "consumer.hpp"
#include "producer.hpp"
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

    int tasks_per_producer = (NTASKS + 1) / NPRODUCERS;
    int remaining_tasks = (NTASKS + 1) % NPRODUCERS;

    for (int i = 0; i < NPRODUCERS; ++i)
    {
        int tasks_for_this = tasks_per_producer + (i < remaining_tasks ? 1 : 0);
        producers.emplace_back(produce, std::ref(Q), tasks_for_this, i);
    }

    int tasks_per_consumer = (NTASKS + 1) / NCONSUMERS;
    int remaining_for_consumers = (NTASKS + 1) % NCONSUMERS;

    for (int i = 0; i < NCONSUMERS; ++i)
    {
        int tasks_for_this = tasks_per_consumer + (i < remaining_for_consumers ? 1 : 0);
        consumers.emplace_back(consume, std::ref(Q), tasks_for_this, i);
    }

    for (auto& t : producers)
        t.join();

    Q.wake_and_done();

    for (auto& t : consumers)
        t.join();

    std::cout << "\n===Results ===" << std::endl;
    std::cout << "Total consumed: " << Consumed.size() << " items" << std::endl;

    if (Consumed.size() == static_cast<size_t>(NTASKS + 1))
        std::cout << "SUCCESS: All tasks consumed!" << std::endl;
    else
        std::cout << "ERROR: Expected " << NTASKS + 1 << ", got " << Consumed.size() << std::endl;

    return 0;
}
