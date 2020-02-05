//
// Created by owlet on 05.11.2019.
//

/*
Задание 3. (50%)
Есть два (достаточно больших) типизированных файла (пусть int)
которые могут читать M (не менее трех) процессов производителей.
Производители читают блок по К (K>=5) чисел и передают его в кольцевой
буфер из N (N>=7) элементов. S процессов-потребителей (S>=5) читают по
одному числу из буфера. Период записи производителя примерно в пять раз
больше периода чтения потребителя.
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

omp_lock_t read_lock;

void gen_files(){
    fstream file1("/home/owlet/CLionProjects/multithread4/file1.txt", ios::out);
    fstream file2("/home/owlet/CLionProjects/multithread4/file2.txt", ios::out);
    int size = 20;
    for(int i = 0; i < size; i++){
        file1 << rand()%10000 << endl;
        file2 << rand()%10000 << endl;
    }
    file1.close();
    file2.close();
}

void log_output_read(char action, string message){
    ofstream fout;
    string str;
    fout.open("/home/owlet/CLionProjects/multithread4/logfile1.txt", ios::app);
    if(fout.is_open()){
        switch(action){
            case 'w':
                str = "Производитель получил числа из файла и пытается записать в буфер.\n";
                break;
            case 'r':
                str = "Потребитель пытается считать число из буфера.\n";
                break;
            case 'W':
                str = "Производитель успешно записал числа `" + message + "` в буфер.\n";
                break;
            case 'R':
                str = "Потребитель успешно считал число `" + message + "` из буфера.\n";
                break;
            case 'p':
                str = "В буфере не хватает места для записи блока чисел. Производитель ожидает появления места в буфере.\n";
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

void producer_actions(const int max_buffer_size, queue<int> &buffer, bool &end_file) {
    queue<int> numbers;
    int number;
    int block_size = 0;
    string str = "";
    streampos pos;
    ifstream file1("/home/owlet/CLionProjects/multithread4/file1.txt", ios::binary);
    ifstream file2("/home/owlet/CLionProjects/multithread4/file2.txt", ios::binary);

    {
        this_thread::sleep_for(chrono::milliseconds(100 + rand()%100));
        block_size = 5 + rand()%2;
        while(true){
            omp_set_lock(&read_lock);
            for(int i = 0; i < block_size; i++){
                if (i%2==0) getline(file1, str);
                else getline(file2,str);
                if(str == "" || str == "\0"){
                    block_size = numbers.size();
                    break;
                }
                number = atoi(str.c_str());
                numbers.push(number);
                str = "";
            }
            if(block_size == 0){
                end_file = true;
                omp_unset_lock(&read_lock);
                break;
            }
            log_output_read('w', str);
            if((buffer.size() + block_size) <= max_buffer_size){
                for(int i = 0; i < block_size; i++){
                    number = numbers.front();
                    numbers.pop();
                    buffer.push(number);
                    str += to_string(number) + " ";
                }
                log_output_read('W', str);
                str = "";
            }
            else{
                log_output_read('p', str);
                while(!numbers.empty()){
                    numbers.pop();
                }
                file1.seekg(pos, ios::ios_base::beg);
                file2.seekg(pos, ios::ios_base::beg);
            }

            pos = file1.tellg();
            omp_unset_lock(&read_lock);
            this_thread::sleep_for(chrono::milliseconds(500 + rand()%100));
        }
        file1.close();
        file2.close();
    }
    log_output_read('E', str);
}

void consumer_actions(queue<int> &buffer, bool &end_file) {
    string str;

    {
        this_thread::sleep_for(chrono::milliseconds(100 + rand()%50));
        while(!end_file || (end_file && !buffer.empty())){
            omp_set_lock(&read_lock);
            log_output_read('r', str);
            if(!buffer.empty()){
                str = to_string(buffer.front());
                buffer.pop();
                log_output_read('R', str);
                str = "";
            }
            else{
                log_output_read('c', str);
            }
            omp_unset_lock(&read_lock);
            this_thread::sleep_for(chrono::milliseconds(100 + rand()%100));
        }
        log_output_read('F', str);
    }
}

int readConsumer(){
    srand(time(NULL));
    int totalProducers = 3 + rand()%2;
    int totalConsumers = 5 + rand()%2;
    int max_buffer_size = 7 + rand()%3;

    queue<int> buffer;
    bool end_file = 0;
    gen_files();
    omp_init_lock(&read_lock);
    fstream clear_file("/home/owlet/CLionProjects/multithread4/logfile1.txt", ios::out);
    clear_file.close();
#pragma omp parallel sections
    {
#pragma omp section
        {
            producer_actions(max_buffer_size, buffer, end_file);
        }

#pragma omp section
        {
            consumer_actions(buffer, end_file);
        }
    }
    omp_destroy_lock(&read_lock);
    return 0;
}
