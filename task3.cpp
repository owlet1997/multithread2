#include <iostream>
#include <random>
#include <malloc.h>
#include <chrono>
#include <omp.h>

using namespace std;

const double delim = 1000000000;

int compareBig(int *arr, int n,  int numberThread){
    int i;
    int max = arr[0];
    omp_set_num_threads(numberThread);
#pragma omp parallel for shared(arr,n) private(i)
    for (i = 1; i < n; i++) {
        if (arr[i] > max) {
#pragma omp critical(max)

            max = arr[i];
        }
    }
    return max;
}

int compareFew(int *arr, int n,  int numberThread){
    int i;
    int min = arr[0];
    omp_set_num_threads(numberThread);
#pragma omp parallel for shared(arr,n) private(i)
    for (i = 1; i < n; i++) {
        if (arr[i] < min){
#pragma omp critical(min)
            min = arr[i];
        }
    }
    return min;
}


void Task3() {

    cout << endl << "Задание 3 [Max и min значение массива]" << endl;

    int n = 1000000;
    int *arr = new int[n];

    srand(time(NULL));

    for (auto i = 0; i < n; i++) {
        arr[i] = rand() % 100000 + 1;
    }


    int max = arr[0]; // максимальный элемент
    int min = arr[0]; // минимальный элемент

    cout<<"\nНахождение максимального элемента "<<endl;

    auto startMP2 = chrono::steady_clock::now( );
    compareBig(arr,n,2);
    auto elapsedParallel2 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP2 );

    auto startMP4 = chrono::steady_clock::now( );
    compareBig(arr,n,4);
    auto elapsedParallel4 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP4 );

    auto start = chrono::steady_clock::now( );
    for (auto i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);

    double a2 = double(elapsed.count())/double (elapsedParallel2.count());
    double a4 = double(elapsed.count())/double (elapsedParallel4.count());

    cout<<"\n Время работы параллельного алгоритма на 2 потока: "<<elapsedParallel2.count()/delim<<" секунд"<<endl;
    cout<<"Время работы параллельного алгоритма на 4 потока: "<<elapsedParallel4.count()/delim<<" секунд"<<endl;
    cout<<"Время работы последовательного алгоритма: "<<elapsed.count()/delim<<" секунд"<<endl;
    cout<<"Ускорение A2 = "<<a2<<endl;
    cout<<"Ускорение A4 = "<<a4<<endl;

    cout<<"\nНахождение минимального элемента "<<endl;


    auto startMP2_2 = chrono::steady_clock::now( );
    compareFew(arr,n,2);
    auto elapsedParallel2_2 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP2_2);

    auto startMP4_4 = chrono::steady_clock::now( );
    compareFew(arr,n,4);
    auto elapsedParallel4_4 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP4_4 );

    auto start_2 = chrono::steady_clock::now( );
    for (auto i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    auto elapsed_2 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start_2);
    cout<<"Последовательный алгоритм  = "<<min<<endl;

    double a2_2 = double(elapsed_2.count())/double (elapsedParallel2_2.count());
    double a4_2 = double(elapsed_2.count())/double (elapsedParallel4_4.count());

    cout<<"\n Время работы параллельного алгоритма на 2 потока: "<<elapsedParallel2_2.count()/delim<<" секунд"<<endl;
    cout<<"Время работы параллельного алгоритма на 4 потока: "<<elapsedParallel4_4.count()/delim<<" секунд"<<endl;
    cout<<"Время работы последовательного алгоритма: "<<elapsed_2.count()/delim<<" секунд"<<endl;
    cout<<"Ускорение A2 = "<<a2_2<<endl;
    cout<<"Ускорение A4 = "<<a4_2<<endl;
}
