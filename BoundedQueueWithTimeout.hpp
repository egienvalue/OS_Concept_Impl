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


template<typename DataT>
class DataPipe {

public:

    DataPipe(size_t buffer_size) : buffer_size_(buffer_size) {
        static_assert(is_shared_ptr<DataT>::value==true, "DataT needs to be shared pointer");

    }

    DataPipe() : DataPipe(10){

    }

    virtual ~DataPipe(){
        commit_cond_.notify_all();
    }

    virtual DataT Fetch(uint32_t timeout_ms=500){
        std::unique_lock<std::mutex> lk(data_mutex_);

        if(!data_queue_.empty()){
            auto front = std::move(data_queue_.front());
            data_queue_.pop();
            return front;
        }

        if(timeout_ms == UINT32_MAX){
            commit_cond_.wait(lk);
        } else {
            auto timeout_in_ms = std::chrono::milliseconds(timeout_ms);
            commit_cond_.wait_for(lk, timeout_in_ms);     
        }

        if(!data_queue_.empty()){
            auto front = std::move(data_queue_.front());
            data_queue_.pop();
            return front;
        }

        MW_ERROR("Timeout Null Data Returned ");
        return nullptr;

    } 

    virtual void Commit(DataT & data){

        std::unique_lock<std::mutex> lk(data_mutex_);
        data_queue_.emplace(data);
        while(data_queue_.size() > buffer_size_){
            MW_WARN("Data Dropped in Queue due to limited Size");
            data_queue_.pop();
        }
        commit_cond_.notify_all();

    }

    std::mutex data_mutex_;
    std::condition_variable commit_cond_;
    std::queue<DataT> data_queue_;
    size_t buffer_size_{10};

};
