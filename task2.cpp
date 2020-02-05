//
// Created by owlet on 05.11.2019.
//

/*
Задание 2. (40%)
Один процесс читает строки из файла, содержащего код программы, и
кладет их в одиночный буфер, второй обрабатывает их и выводит на экран.
Программа должна выводить лог работы.
*/

#include <iostream>
#include <omp.h>
#include <queue>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>

using namespace std;

omp_lock_t lock;

void log_output(char action, string message){
    ofstream fout;
    string str;
    fout.open("/home/owlet/CLionProjects/multithread4/logfile.txt", ios::app);
    if(fout.is_open()){
        switch(action){
            case 'r':
                str = "Производитель получил строку `" + message + "` из файла и пытается записать в буфер.\n";
                break;
            case 'w':
                str = "Потребитель пытается считать строку из буфера.\n";
                break;
            case 'i':
                str = "Производитель успешно записал строку `" + message + "` буфер.\n";
                break;
            case 'e':
                str = "Потребитель успешно считал строку `" + message + "` из буфера.\n";
                break;
            case 'p':
                str = "Буфер заполнен. Производитель ожидает освобождение буфера.\n";
                break;
            case 'c':
                str = "Буфер пуст. Потребитель ожидает пополнения буфера.\n";
                break;
            case 'E':
                str = "Производитель встретил конец файла.\n";
                break;
            case 'F':
                str = "Потребитель закончил работу.\n";
                break;
            default:
                exit(-1);
                break;
        }
        fout << str << endl;
    }
    fout.close();
}

void producer_actions(string &buffer, ifstream &filecpp) {
    string str;
    streampos pos;

    {
        this_thread::sleep_for(chrono::nanoseconds(100 + rand()%100));
        while(getline(filecpp,str)){
            omp_set_lock(&lock);
            log_output('r', str);
            if(buffer == ""){

                buffer = str;
                cout << buffer << endl;
                log_output('i', buffer);
            }
            else{
                log_output('p', buffer);
                filecpp.seekg(pos, ios::ios_base::beg);
            }
            pos = filecpp.tellg();
            omp_unset_lock(&lock);
            this_thread::sleep_for(chrono::nanoseconds(100 + rand()%50));
        }
        cout << str << endl;
    }
    log_output('E', buffer);
    this_thread::sleep_for(chrono::nanoseconds(200));
    buffer = "EOF";
}

void consumer_actions(string &buffer) {
    {
        while(true){
            this_thread::sleep_for(chrono::nanoseconds(100 + rand()%100));
            omp_set_lock(&lock);
            log_output('w', buffer);
            if(buffer != "" && buffer != "EOF"){
                log_output('e', buffer);

                buffer = "";
            }
            else{
                if(buffer == "EOF"){
                    log_output('E', buffer);
                    omp_unset_lock(&lock);
                    break;
                }
                else{
                    log_output('c', buffer);
                }
            }
            omp_unset_lock(&lock);
            this_thread::sleep_for(chrono::nanoseconds(100 + rand()%50));
        }
    }
}

int copyFile(){
    srand(time(NULL));
    int totalProducers = 3;
    int totalConsumers = 3;
    string buffer;
    omp_init_lock(&lock);
    fstream clear_file("/home/owlet/CLionProjects/multithread4/logfile.txt", ios::out);
    clear_file.close();
    ifstream filecpp("/home/owlet/CLionProjects/multithread4/copyMatrix.cpp", ios::binary);
#pragma omp parallel sections num_threads(2)
    {
#pragma omp section
        {
            producer_actions(buffer, filecpp);
        }

#pragma omp section
        {
            consumer_actions(buffer);
        }
    }
    omp_destroy_lock(&lock);
    filecpp.close();
    return 0;
}
