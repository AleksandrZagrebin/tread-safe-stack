#include "consumer.hpp"
#include <iostream>
#include <mutex>

extern int NTasks;
extern std::mutex TaskMut;
extern std::vector<int> Consumed;
extern std::mutex ConsMut;

void consume(ts_stack<int>& Q, int consumer_id)
{
        int consumed_count = 0;
    for(;;)
    {
        {
            std::lock_guard<std::mutex> Lk{ TaskMut };
            if (NTasks < 0 && Q.is_empty_and_done())
                break;
        }

        int N;
        bool Succ = Q.wait_and_pop(N);

        if (Succ)
        {
            {
                std::lock_guard<std::mutex> Lk{ ConsMut };
                Consumed.push_back(N);
                consumed_count++;
            }
#ifdef VISUALIZE
            std::cout << "Consumer " << consumer_id << " popped: " << N << std::endl;
#endif
        }
    }
}
