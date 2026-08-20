#include "producer.hpp"
#include <iostream>
#include <mutex>

extern int NTasks;
extern std::mutex TaskMut;

void produce(ts_stack<int>& Q, int tasks_to_produce, int producer_id)
{
    for (int i = 0; i < tasks_to_produce; ++i)
    {
        int N;
        {
            std::lock_guard<std::mutex> Lk{ TaskMut };
            if (NTasks < 0)
                break;
            N = NTasks;
            NTasks -= 1;
        }

        Q.push(N);
#ifdef VISUALIZE
        std::cout << "Producer " << producer_id << " pushed: " << N << std::endl;
#endif
    }
}
