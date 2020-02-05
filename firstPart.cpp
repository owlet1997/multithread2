
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
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
        counter->inc_id();
    }

}

void decrement(int num_increments, Counter *counter){
    for (int i=0; i<num_increments; i++){
        counter->dec_id();
    }
}


int firstPart() {

    Counter *counter =new Counter();
    thread th1(increment, 100000, ref(counter));
    thread th2(decrement, 100000, ref(counter));

    th1.join();
    th2.join();

    cout << counter->get_id();

    return 0;
}