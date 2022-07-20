/***********************************************
#
#      Filename: false_sharing.cpp
#
#        Author: LaaaaaaaaaaaPP
#        Create: 2022-07-20 10:15:20
***********************************************/

#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

/// The value of 64 bytes is typical for x86/x64 architectures.
const size_t CacheLineSize = 64;

#define LOOP_NUMS 10000000

class nFlag {
public:
    nFlag() {
        flag1_.clear();
        flag2_.clear();
        flag3_.clear();
    }

    atomic_flag flag1_;
    atomic_flag flag2_;
    atomic_flag flag3_;
};

class nPadFlag {
public:
    nPadFlag() {
        flag1_.clear();
        flag2_.clear();
        flag3_.clear();
    }

    uint8_t m_pad1[CacheLineSize - sizeof(atomic_flag)];
    atomic_flag flag1_;
    uint8_t m_pad2[CacheLineSize - sizeof(atomic_flag)];
    atomic_flag flag2_;
    uint8_t m_pad3[CacheLineSize - sizeof(atomic_flag)];
    atomic_flag flag3_;
    uint8_t m_pad4[CacheLineSize - sizeof(atomic_flag)];
};

void f_add(int *num, atomic_flag *flag) {
    for(int i = 0; i < LOOP_NUMS; ++i) {
        while(flag->test_and_set(std::memory_order_acquire));

        *num += 1;

        flag->clear(std::memory_order_release);
    }
}

int main() {
    int a = 0, b = 0, c = 0;

    nFlag nor_flag;
    nPadFlag pad_flag;
    {
        auto s_time = chrono::steady_clock::now();
        for(int loop = 0; loop < 20; ++loop) {
            thread th1 = thread(f_add, &a, &(pad_flag.flag1_));
            thread th2 = thread(f_add, &b, &(pad_flag.flag2_));
            thread th3 = thread(f_add, &c, &(pad_flag.flag3_));

            th1.join();
            th2.join();
            th3.join();
        }
        auto t_elapse = chrono::steady_clock::now() - s_time;
        cout << "with pad: " << t_elapse.count() / 1000000 << endl;
    }
    {
        auto s_time = chrono::steady_clock::now();
        for(int loop = 0; loop < 20; ++loop) {
            thread th1 = thread(f_add, &a, &(nor_flag.flag1_));
            thread th2 = thread(f_add, &b, &(nor_flag.flag2_));
            thread th3 = thread(f_add, &c, &(nor_flag.flag3_));

            th1.join();
            th2.join();
            th3.join();
        }

        auto t_elapse = chrono::steady_clock::now() - s_time;
        cout << "without: " << t_elapse.count() / 1000000 << endl;
    }
    getchar();

    return 0;
}
