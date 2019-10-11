#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <random>
#include <malloc.h>
#include <time.h>
#include <chrono>


using namespace std;

void task1(){
    cout << "Task 1" << endl;
    if (_OPENMP)
        cout << "OpenMP is supported!" << endl;
    unordered_map<int,string> map{
            {200505,"2.5"},{200805,"3.0"},{201107,"3.1"},{201307,"4.0"},{201511,"4.5"}};
    cout<<"We have OpenMP "<<map.at(_OPENMP).c_str()<<endl;

    cout << "Amount of processors = " << omp_get_max_threads() << endl;
}

int** createMass(int n){
    int **mas;


    mas = (int**)malloc(n*sizeof(int*));
    for(int i=0;i<n;i++)
       mas[i]=(int*)malloc(n*sizeof(int));

    return mas;
}

int** fullMass(int **mas, int n){
    default_random_engine generator;
    uniform_int_distribution<int> distribution(1, 100);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mas[i][j] = distribution(generator);
    return mas;
}

void lookMatrix(int **mas1, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << mas1[i][j] << " ";
        cout << endl;
    }
}


void task2_1(){
    cout << endl<< "Поэлементное перемножение"<< endl;
    int i,j;
    int n = 3000;
    int count = 0;
    int **mas1 = createMass(n);
    int **mas2 = createMass(n);
    fullMass(mas1,n);
    fullMass(mas2,n);

    int **arr3 = createMass(n);
    omp_set_nested(true);
    omp_set_num_threads(2);

    auto startMP = chrono::steady_clock::now( );
    #pragma omp parallel if(n>2000)
    {
    #pragma omp for private(i,j)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                arr3[i][j] = mas1[i][j] * mas2[i][j];
            }
    }

    auto elapsedParallel = chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now( ) - startMP );

    auto start = chrono::steady_clock::now( );
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            arr3[i][j] = mas1[i][j] * mas2[i][j];
            count++;
        }
    auto elapsed = chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now( ) - start);


    double a = double(elapsed.count())/double (elapsedParallel.count());

    cout<<"\n Time of working parallel program is: "<<elapsedParallel.count()<<" milliseconds"<<endl;
    cout<<"\n Time of working line program is: "<<elapsed.count()<<" milliseconds"<<endl;
    cout<<"\n A = "<<a<<endl;
    delete [] mas1;
    delete [] mas2;
    delete [] arr3;

}

void task2_2(){
    cout << endl <<"Ленточная схема"<<endl;
    int i,j,k;
    int n = 500;
    int **arr1 = createMass(n);
    int **arr2 = createMass(n);
    fullMass(arr1,n);
    fullMass(arr2,n);

    int **result = createMass(n);

    auto start = chrono::steady_clock::now( );


    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            result[i][j] = 0;
            for (k = 0; k < n; k++) {
                result[i][j] += arr1[i][k] * arr2[k][j];
            }
        }

    }

    auto elapsed = chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now( ) - start);


    omp_set_nested(true);
    omp_set_num_threads(4);
    auto startMP = chrono::steady_clock::now( );

    #pragma omp parallel for shared(n, arr1, arr2, result) private(i, j, k)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            result[i][j] = 0;
            for (k = 0; k < n; k++) {
                result[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }

    auto elapsedParallel = chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now( ) - startMP );

    double a = double(elapsed.count())/double (elapsedParallel.count());

    cout<<"\n Time of working parallel program is: "<<elapsedParallel.count()<<" milliseconds"<<endl;
    cout<<"\n Time of working line program is: "<<elapsed.count()<<" milliseconds"<<endl;
    cout<<"\n A = "<<a<<endl;


    delete [] arr1;
    delete [] arr2;
    delete [] result;
    }




