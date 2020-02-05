//
// Created by owlet on 17.10.19.
//
#include <iostream>
#include <omp.h>
#include <random>
#include <malloc.h>
#include <chrono>

using namespace std;
const double delim = 1000000;

int getArraySize() {
    int arraySize;

    cout << "Input array size: ";
    cin >> arraySize;

    return arraySize;
}

int* createMas(int n){
    int* mas;
    mas = (int*)malloc(n*sizeof(int*));

    default_random_engine generator;
    uniform_int_distribution<int> distribution(1, 10);

    for (int i = 0; i < n; i++)
        mas[i] = distribution(generator);

    return mas;
}


void quickSortParallel(int *numbers, int left,int right){
    int letElement;
    int l = left;
    int r = right;
    letElement = numbers[(l+r)/2];
    while (l < r)
    {
        while (numbers[l]<letElement)
            l++;

        while (numbers[r]>letElement)
            r--;

        if (l <= r)
        {
            swap(numbers[l++],numbers[r--]);
        }
    }
#pragma omp task shared(numbers) firstprivate(left,r)
    if (left < r)
        quickSortParallel(numbers, left, r);
#pragma omp task shared(numbers) firstprivate(l,right)
    if (right > l)
        quickSortParallel(numbers, l, right);
#pragma omp taskwait

}


void sort(int n){
    int *mas1 = createMas(n);

    auto start = chrono::steady_clock::now( );

#pragma omp parallel shared(mas1)
    {
#pragma omp single
        quickSortParallel(mas1,0,n-1);
    }
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"Время работы параллельного алгоритма: "<<elapsed.count()/delim<<" мксекунд"<<endl;


    delete [] mas1;
}