//
// Created by owlet on 05.11.2019.
//

/*
Задание 1. (10%)
Написать программу, копирующую одну матрицу в другую, с
использованием буферной переменной.
*/

#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <iomanip>
#include <array>
#include <omp.h>

using namespace std;

omp_lock_t copy_lock;

int getMatrixSize() {
    int matrixSize;

    cout << "Введите размер матрицы: ";
    cin >> matrixSize;

    return matrixSize;
}

int** createMass(int n){
    int **mas;

    mas = (int**)malloc(n*sizeof(int*));
    for(int i=0;i<n;i++)
        mas[i]=(int*)malloc(n*sizeof(int));

    return mas;
}

int** fullMass(int **mas, int n){

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mas[i][j] = rand()%10+1;
    return mas;
}


void producer_actions(int **&matrix, int n, string & buffer){
    int save_i = 0;
    int save_j = 0;
    int i = 0;
    int j = 0;
    {
        this_thread::sleep_for(chrono::nanoseconds(100 + rand() % 100));
        while (true) {


            omp_set_lock(&copy_lock);

            if (buffer == "") {
                buffer = to_string(matrix[i][j]);
            } else {
                i = save_i;
                j = save_j;
            }
            save_i = i;
            save_j = j;

            if (j < n - 1) {
                j++;
            } else {
                j = 0;
                if (i < n - 1)
                    i++;
                else {
                    omp_unset_lock(&copy_lock);
                    break;
                }
            }
            omp_unset_lock(&copy_lock);
            this_thread::sleep_for(chrono::nanoseconds(100 + rand() % 50));
        }
    }
}


void consumer_actions(int **&matrix, int n, string & buffer){
    int save_i = 0;
    int save_j = 0;
    int i = 0;
    int j = 0;
    {
        this_thread::sleep_for(chrono::nanoseconds(100 + rand() % 100));
        while (true) {
            omp_set_lock(&copy_lock);
          //  cout << omp_get_thread_num() << endl;

            if (buffer != "") {
                matrix[i][j] = atoi(buffer.c_str());
                buffer = "";
            } else {
                if (i == 0 && j == 0) {
                    omp_unset_lock(&copy_lock);
                    continue;
                }
                i = save_i;
                j = save_j;
            }
            save_i = i;
            save_j = j;

            if (j < n - 1) {
                j++;
            } else {
                j = 0;
                if (i < n - 1)
                    i++;
                else {
                    omp_unset_lock(&copy_lock);
                    break;
                }
            }
            omp_unset_lock(&copy_lock);
            this_thread::sleep_for(chrono::nanoseconds(100 + rand() % 50));
        }
    }
}

void lookMatrix(int **mas1, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << mas1[i][j] << " ";
        cout << endl;
    }
}

void copyMatrix(){
    srand(time(NULL));
    const int size_m = 5;
    string buffer;
    int n = getMatrixSize();
    cout << "Размер матрицы: "<<size_m<<endl;

    int **m1 = createMass(n);
    fullMass(m1,n);
    int **m2 = createMass(n);
    fullMass(m2,n);

    cout << "Матрица А:"<< endl;
    lookMatrix(m1,n);

    omp_init_lock(&copy_lock);
#pragma omp parallel sections num_threads(2)
    {
#pragma omp section
        {
            producer_actions(m1,n, buffer);
        }

#pragma omp section
        {
            consumer_actions(m2,n, buffer);
        }
    }
    omp_destroy_lock(&copy_lock);

    cout << "\nМатрица Б:"<< endl;
    lookMatrix(m2,n);

}
