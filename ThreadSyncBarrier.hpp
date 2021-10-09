#include <memory>
#include <string>
#include <atomic>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <functional>


class Barrier{


public:

    Barrier(int num_threads) : num_threads_(num_threads){


    }
    ~Barrier()=default;

    void CheckIn(){
        std::unique_lock<std::mutex> lk(cv_mutex_);

        num_entered_++;

        if(num_entered_ < num_threads_){
            all_checked_in_.wait(lk, [this](){return num_entered_ >= num_threads_;});

        } else {
            num_leaving_=0;
            all_checked_in_.notify_all();
        }


        num_leaving_++;

        if(num_leaving_ < num_threads_){
            all_leaving_.wait(lk, [this](){return num_leaving_ >= num_threads_;});
        } else {
            num_entered_=0;
            all_leaving_.notify_all();
        }

        lk.unlock();
    }

private:

    std::mutex cv_mutex_;

    std::condition_variable all_checked_in_;
    std::condition_variable all_leaving_;

    int num_threads_{0};
    int num_entered_{0};
    int num_leaving_{0};




};