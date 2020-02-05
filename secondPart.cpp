//
// Created by owlet on 25.11.2019.
//
/*
Напишите программу, порождающую два потока, конкурентно
инкрементирующих разделяемую переменную (для каждого потока задается
свое количество раз инкремента переменной. Например, первый поток
инкрементирует 3 раза, второй – 4).
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <mutex>

using namespace std;

mutex mut;

void Increment(int &value, int num_incrementation){
    for(int i = 0; i < num_incrementation; i++){
        this_thread::sleep_for(chrono::milliseconds(10));
        mut.lock();
        value++;
        cout << "Номер потока: " << this_thread::get_id() << " | Значение переменной: " << value << endl;
        mut.unlock();
    }
    this_thread::sleep_for(chrono::milliseconds(10));
}

void secondTask(){
    srand(time(NULL));
    int share_value = 0;
    cout << "\n\nВторая задача, конкуррентное инкрементирование переменной" << endl;
    thread th7(Increment, ref(share_value), 5);
    thread th8(Increment, ref(share_value), 7);
    th7.join();
    th8.join();

}
