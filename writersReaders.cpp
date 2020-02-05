//
// Created by owlet on 25.11.2019.
//

#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <clocale>
#include <thread>
#include <cstring>
#include <queue>

using namespace std;

FILE *stream;
int message = 0;

char * settime(struct tm *u)
{
    char s[40];
    char *tmp;
    for (int i = 0; i<40; i++) s[i] = 0;
    strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);
    tmp = (char*)malloc(sizeof(s));
    strcpy(tmp, s);
    return(tmp);
}

void printTimeStamp()
{
    struct tm *u;
    char *f;
    const time_t timer = time(nullptr);
    u = localtime(&timer);
    f = settime(u);
    cout << f;
    fprintf(stream, "%s", f);
}


void writer(queue<int>& storage, int totalWriters, int& activeReaders, omp_lock_t& writeLock, bool& readLock)
{
#pragma omp parallel num_threads(totalWriters)
    {
        bool flag;

        int writerNumber = omp_get_thread_num();

        while(true)
        {
            flag = false;
            this_thread::sleep_for(chrono::milliseconds(rand() * writerNumber % 12000 + 3000));

#pragma omp critical
            {
                printTimeStamp();
                printf(": Писатель %d обратился к хранилищу...\n", writerNumber);
                fprintf(stream, ": Писатель %d обратился к хранилищу...\n", writerNumber);
                fflush(stream);

            }
            if (!omp_test_lock(&writeLock))
            {
#pragma omp critical
                {
                    printf("Другой писатель обратился к хранилищу раньше. Писатель %d ожидает.\n", writerNumber);
                    fprintf(stream, "Другой писатель обратился к хранилищу раньше. ");
                    fprintf(stream, "Писатель %d ожидает.\n", writerNumber);
                    fflush(stream);

                }
                omp_set_lock(&writeLock);
            }
            readLock = true;

            while(activeReaders != 0)
            {
                if (!flag)
                {
#pragma omp critical
                    {
                        printf("Писатель %d ожидает, пока активные читатели закончат работу с хранилищем.\n", writerNumber);
                        fprintf(stream, "Писатель %d ожидает, пока ", writerNumber);
                        fprintf(stream, "активные читатели закончат работу с хранилищем.\n");
                        fflush(stream);

                    }
                }
                flag = true;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
#pragma omp critical
            {
                printTimeStamp();
                printf(": Писатель %d получил доступ к хранилищу.\n", writerNumber);
                fprintf(stream, ": Писатель %d получил доступ к хранилищу.\n", writerNumber);
                fflush(stream);

            }
            this_thread::sleep_for(chrono::milliseconds(3000));
            storage.push(rand() + writerNumber);

#pragma omp critical
            {
                printTimeStamp();
                printf(": Писатель %d записал в хранилище ", writerNumber);
                printf("сообщение %d и закончил работу.\n", storage.back());
                fprintf(stream, ": Писатель %d записал в хранилище ", writerNumber);
                fprintf(stream, "сообщение %d и закончил работу.\n", storage.back());
                fflush(stream);

            }
            readLock = false;
            omp_unset_lock(&writeLock);
        }
    }
}

void reader(queue<int>& storage, int totalReaders, int& activeReaders,bool& readLock, int &read)
{
#pragma omp parallel num_threads(totalReaders)
    {
        bool flag;

        int readerNumber = omp_get_thread_num();
        while(true)
        {
            flag = false;
            this_thread::sleep_for(chrono::milliseconds(rand() * readerNumber % 10000 + 3000));
#pragma omp critical
            {
                printTimeStamp();
                printf(": Читатель %d обратился к хранилищу...\n", readerNumber);
                fprintf(stream, ": Читатель %d обратился к хранилищу...\n", readerNumber);
                fflush(stream);

            }
            while(readLock)
            {
                if (!flag)
                {
#pragma omp critical
                    {
                        printf("Доступ к хранилищу заблокирован. Читатель %d ожидает.\n", readerNumber);
                        fprintf(stream, "Доступ к хранилищу заблокирован. ");
                        fprintf(stream, "Читатель %d ожидает.\n", readerNumber);
                        fflush(stream);

                    }
                }
                flag = true;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
#pragma omp critical
            {
                printTimeStamp();
                printf(": Читатель %d получил доступ к хранилищу.\n", readerNumber);
                fprintf(stream, ": Читатель %d получил доступ к хранилищу.\n", readerNumber);
                fflush(stream);
            }
#pragma omp atomic
            activeReaders++;
            this_thread::sleep_for(chrono::milliseconds(3000));
#pragma omp critical
            {
                printTimeStamp();
                if (storage.empty())
                    message = 0;
                else {
                    message = storage.front();
                    read++;
                    if (read==totalReaders){
                        storage.pop();
                        read=0;
                    }
                }
                printf(": Читатель %d прочитал из хранилища ", readerNumber);
                printf("сообщение %d и закончил работу.\n", message);
                fprintf(stream, ": Читатель %d прочитал из хранилища ", readerNumber);
                fprintf(stream, "сообщение %d и закончил работу.\n", message);
                fflush(stream);

            }
#pragma omp atomic
            activeReaders--;
        }
    }
}

void writersReaders(){
    setlocale (LC_ALL,"Russian");
    srand (time(NULL));
    queue<int> storage;
    int totalWriters = 0,
            totalReaders = 0;
    int activeReaders = 0;
    int read = 0;

    omp_lock_t writeLock;
    bool readLock = false;

    stream = fopen("/home/owlet/CLionProjects/multithread5/log.txt", "w");
    omp_init_lock(&writeLock);
    omp_set_nested(true);

    cout << "Введите количество писателей: \n";

    cin >> totalWriters;
    cout << "Введите количество читателей: \n";
    cin >> totalReaders;
    cout <<"\nМоделирование началось. \n";

#pragma omp parallel sections
    {
#pragma omp section
        {
            writer(storage, totalWriters, activeReaders, writeLock, readLock);
        }
#pragma omp section
        {
            reader(storage, totalReaders, activeReaders, readLock, read);
        }
    }
    fclose(stream);
}