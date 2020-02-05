/*
Используя материал лекции [1], напишите программу, выполняющуюся
в два потока. Каждый поток в качестве параметров получает начальное
значение приватной переменной и количество итераций ее инкремента.
Программа выводит лог своей работы. Реализуйте потоки следующим
способами:
• указатель на функцию;
• указатель на объект-функцию;
• указатель на функцию-член класса.
*/


#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m;

void FIncrement(int value, int num_incrementation){
    for(int i = 0; i < num_incrementation; i++){
        this_thread::sleep_for(chrono::milliseconds(10));

        m.lock();
        value++;
        cout << "Указатель на функцию | Номер потока: " << this_thread::get_id() << " | Значение переменной: " << value << endl;
        m.unlock();
    }
    this_thread::sleep_for(chrono::milliseconds(10));

}

class FOIncrement{
public:
    static void Increment(int a, int b){
        for(int i = 0; i < b; i++){
            this_thread::sleep_for(chrono::milliseconds(10));

            m.lock();
            a++;
            cout << "Указатель на объект-функцию | Номер потока: " << this_thread::get_id() << " | Значение переменной: " << a << endl;
            m.unlock();
        }
        this_thread::sleep_for(chrono::milliseconds(10));

    }
};

class CIncrement{
private:
    int a;
    int b;
public:
    CIncrement(int a, int b){this->a = a; this->b = b;}
    void Increment(int value, int num_incrementation){
        for (int i = 0; i < num_incrementation; i++){
            this_thread::sleep_for(chrono::milliseconds(10));

            m.lock();
            value++;
            cout << "Указатель на функцию-член класса | Номер потока: " << this_thread::get_id() << " | Значение переменной: " << value << endl;
            m.unlock();
        }
        this_thread::sleep_for(chrono::milliseconds(10));

    }
    int get_a(){return a;}
    int get_b(){return b;}
    ~CIncrement(){a = 0; b = 0;}
};

void firstPart(){
    int a = 0;
    int b = 0;

    // Указатель на функцию
    void (*FIptr)(int, int) = &FIncrement;
    thread th1(FIptr, a, 5);
    thread th2(FIptr, b, 7);
    th1.join();
    th2.join();

    cout << "\n\n";

    // Указатель на объект-функцию
    //FOIncrement FOI;
    CIncrement CI(0, 0);
    void(CIncrement::*CIptr)(int, int) = &CIncrement::Increment;
    thread th3(CIptr, CI, CI.get_a(), 5);
    thread th4(CIptr, CI, CI.get_b(), 7);
    th3.join();
    th4.join();

    cout << "\n\n";

    // Указатель на функцию-член класса
    thread th5(FOIncrement::Increment, a, 5);
    thread th6(FOIncrement::Increment, b, 7);
    th5.join();
    th6.join();

}