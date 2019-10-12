#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;

const double delim = 1000000;

int compareBig(int *arr, int n,  int numberThread){
    int i;
    int max = arr[0];
    omp_set_num_threads(numberThread);

    auto start = chrono::steady_clock::now( );

#pragma omp parallel for shared(arr,n) private(i)
    for (i = 1; i < n; i++) {
        if (arr[i] > max) {
#pragma omp critical(max)

            max = arr[i];
        }
    }
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start );
    cout<<"\n Параллельный алгоритм поиска максимума на "<<numberThread<<" потока: "<<elapsed.count()/delim<<" секунд"<<endl;

    return max;
}

int compareFew(int *arr, int n,  int numberThread){
    int i;
    int min = arr[0];
    omp_set_num_threads(numberThread);

    auto start = chrono::steady_clock::now( );
#pragma omp parallel for shared(arr,n) private(i)
    for (i = 1; i < n; i++) {
        if (arr[i] < min){
#pragma omp critical(min)
            min = arr[i];
        }
    }
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start );
    cout<<"\n Параллельный алгоритм поиска минимума на "<<numberThread<<" потока: "<<elapsed.count()/delim<<" секунд"<<endl;

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

    compareBig(arr,n,2);

    compareBig(arr,n,4);

    auto start = chrono::steady_clock::now( );
    for (auto i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);

    cout<<"Время работы последовательного алгоритма: "<<elapsed.count()/delim<<" секунд"<<endl;

    cout<<"\nНахождение минимального элемента "<<endl;

    compareFew(arr,n,2);

    compareFew(arr,n,4);

    auto start_2 = chrono::steady_clock::now( );
    for (auto i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    auto elapsed_2 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start_2);

    cout<<"Время работы последовательного алгоритма: "<<elapsed_2.count()/delim<<" секунд"<<endl;

}
