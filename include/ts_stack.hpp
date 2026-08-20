#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>
#include <stdexcept>
template <typename T> class ts_stack
{
    std::vector<T> Buffer;
    int NCur = -1;
    bool Done = false;
    mutable std::mutex Mut;
    std::condition_variable CondCons, CondProd;

    bool full() const { return NCur >= static_cast<int>(Buffer.size() - 1); }
    bool empty() const { return NCur < 0; }
    bool done() const { return Done; }

public:
    ts_stack(int BufSize) : Buffer(BufSize)
    {
        if (BufSize > (1 << 30))
            throw std::runtime_error("unsupported buffer size");
    }

    void push(T Data)
    {
        std::unique_lock<std::mutex> Lk{ Mut };
        CondProd.wait(Lk, [this] { return !full(); });

        int NewCur = NCur + 1;
        Buffer[NewCur] = std::move(Data);
        NCur = NewCur;
        Lk.unlock();
        CondCons.notify_one();
    }

    bool wait_and_pop(T& Data)
    {
        std::unique_lock<std::mutex> Lk{ Mut };
        CondCons.wait(Lk, [this] { return !empty() || done(); });
        if (empty())
            return false;
        Data = Buffer[NCur];
        NCur -= 1;
        Lk.unlock();
        CondProd.notify_one();
        return true;
    }

    void wake_and_done()
    {
        std::unique_lock<std::mutex> Lk{ Mut };
        Done = true;
        Lk.unlock();
        CondCons.notify_all();
    }

    bool is_empty_and_done() const
    {
        std::unique_lock<std::mutex> Lk{ Mut };
        return (NCur < 0) && Done;
    }
};
