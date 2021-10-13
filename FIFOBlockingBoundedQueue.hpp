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

// uisng Mesa Style
// wiht conditional variable queue
/**
 * create a coditoina variable for every waiter
 * put condition variable on a queue in FIFO order
 * sigal wakes up the thread in the front of the queue
 * be careful about the spurious wakeups
 * 
 **/
class FIFOBlockingBoundedQueue{



public:






private:





};