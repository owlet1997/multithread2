#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;

const double delim = 1000000;

double f(double x) {return pow(x,4)+4*x;}

double trapezeIntegralParallel(double a, double b, int intervals, int numThreads) {
    int i;
    double S = 0;
    double h = (b - a) / intervals;

    omp_set_nested(true);
    omp_set_num_threads(numThreads);

    auto start = chrono::steady_clock::now( );

#pragma omp parallel for reduction(+:S) private(i)

    for (i = 0; i < intervals; i++)
    {
        S += (f(a+i*h)+f(a+(i+1)*h))*h/2;
    }

    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"\nПараллельный алгоритм на "<<numThreads<<" потока = "<<S<<endl;
    cout<<"Время выполнения = "<<elapsed.count()/delim<<" ms"<<endl;

    return S;
}

double trapezeIntegral(double a, double b, int intervals){
    int i;
    double S = 0;
    double h = (b - a) / intervals;

    auto start = chrono::steady_clock::now( );

    for (i = 0; i < intervals; i++)
    {
        S += (f(a+i*h)+f(a+(i+1)*h))*h/2;
    }

    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"\nПоследовательный алгоритм  = "<<S<<endl;
    cout<<"Время выполнения = "<<elapsed.count()/delim<<" ms"<<endl;

    return S;

}

double Simpson_integral_parallel(double a, double b, int n, int numThreads) {

    double h = (b-a)/(1.0 * n);
    double	 k1 = 0, k2 = 0;
    int i;

    omp_set_nested(true);
    omp_set_num_threads(numThreads);

    auto start = chrono::steady_clock::now( );

#pragma omp parallel for reduction(+:k1,k2) private(i) shared(a,h,n)
    for(i = 1; i < n; i += 2){
        k1 += f(a + i * h);
        k2 += f(a + (i + 1) * h);
    }

    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"\nПараллельный алгоритм Симпсона на "<<numThreads<<" потока = "<<h/3*(f(a) + 4*k1 + 2*k2 - f(b))<<endl;
    cout<<"Время выполнения = "<<elapsed.count()/delim<<" ms"<<endl;

    return h/3*(f(a) + 4*k1 + 2*k2 - f(b));
}

double Simpson_integral(double a, double b, int n) {

    const double h = (b-a)/(1.0 * n);
    double	 k1 = 0, k2 = 0;
    int i;

    auto start = chrono::steady_clock::now( );

    for(i = 1; i < n; i += 2){
        k1 += f(a + i * h);
        k2 += f(a + (i + 1) * h);
    }

    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"\nПоследовательный алгоритм Симпсона = "<<h/3*(f(a) + 4*k1 + 2*k2 - f(b))<<endl;
    cout<<"Время выполнения = "<<elapsed.count()/delim<<" ms"<<endl;

    return h/3*(f(a) + 4*k1 + 2*k2 - f(b));
}

void simpsonIntegral(double a,double b, int n){

    cout << "\n Подсчет интеграла итеративным методом Симпсона"<<endl;

    Simpson_integral(a,b,n);

    Simpson_integral_parallel(a,b,n,2);

    Simpson_integral_parallel(a,b,n,4);

}

void Task1(){

    cout << "Задание 1" << endl;

    double a = 0;
    double b = 10;
    int intervals = 1000000;

    cout << "\nПодсчет интеграла итеративным методом трапеций"<<endl;


    trapezeIntegralParallel(a,b,intervals,2);

    trapezeIntegralParallel(a,b,intervals,4);

    trapezeIntegral(a,b,intervals);

    simpsonIntegral(a,b,intervals);
}
