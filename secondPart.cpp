#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <sstream>
#include <mutex>


using namespace std;

mutex m;

class Counter {
    long long mId = 0;
public:
    long long inc_id() {
        ++mId;
        return mId;
    }
    long long dec_id() {
        --mId;
        return mId;
    }

    long long get_id(){
        return mId;
    }
};
void increment(int num_increments, Counter *counter){

    for (int i=0; i<num_increments; i++){
        m.lock();
        counter->inc_id();
        m.unlock();
    }

}

void decrement(int num_increments, Counter *counter){
    for (int i=0; i<num_increments; i++){
        m.lock();
        counter->dec_id();
        m.unlock();
    }
}


int secondPart() {

    Counter *counter =new Counter();
    thread th1(increment, 100000, ref(counter));
    thread th2(decrement, 100000, ref(counter));

    th1.join();
    th2.join();

    cout << counter->get_id();

    return 0;
}