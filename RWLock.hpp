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


using namespace std;

class RWLock{

public:

    RWLock();
    ~RWLock();

    void StartRead(){
        unique_lock<mutex> lk(cv_mutex);

        ++waiting_readers_;
        read_cv_.wait(lk, [this]{return active_writers_==0 && waiting_writers_==0;});
        --waiting_readers_;

        ++active_readers_;
    
        lk.unlock();
    }

    void DoneRead(){
        unique_lock<mutex> lk(cv_mutex);

        --active_readers_;
        if(active_readers_ == 0 && waiting_writers_ >0){
            write_cv_.notify_one();
        }

        lk.unlock();
    }

    void StartWrite(){
        unique_lock<mutex> lk(cv_mutex);

        ++waiting_writers_;

        write_cv_.wait(lk, [this]{return  active_writers_==0 && active_readers_==0;});
        --waiting_writers_;
        ++active_writers_;

        lk.unlock();


    }


    void DoneWrite(){
        unique_lock<mutex> lk(cv_mutex);

        --active_writers_;

        if(waiting_writers_>0){
            write_cv_.notify_one();
        } else {
            read_cv_.notify_all();
        }

        lk.unlock();

    }



private:



    bool ReadShouldWait(){


    }


    bool WriteShouldWait(){



    }


    mutex cv_mutex;

    condition_variable read_cv_;
    condition_variable write_cv_;

    int active_writers_{0};
    int waiting_writers_{0};
    int active_readers_{0};
    int waiting_readers_{0};






};