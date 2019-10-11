#include <iostream>
#include <random>
#include <malloc.h>
#include <chrono>
#include <omp.h>

using namespace std;

const double delim = 1000000;

double f(double x) {return 3*x;}

double trapezeIntegralParallel(double a, double b, int intervals, int numThreads) {
    int i;
    double S = 0;
    double h = (b - a) / intervals;

    omp_set_nested(true);
    omp_set_num_threads(numThreads);

#pragma omp parallel for reduction(+:S) private(i)

    for (i = 0; i < intervals; i++)
    {
        S += (f(a+i*h)+f(a+(i+1)*h))*h/2;
    }
    return abs(S);
}

double trapezeIntegral(double a, double b, int intervals){
    int i;
    double S = 0;
    double h = (b - a) / intervals;

    for (i = 0; i < intervals; i++)
    {
        S += (f(a+i*h)+f(a+(i+1)*h))*h/2;
    }
    return abs(S);

}

double Simpson_integral_parallel(double a, double b, int n, int numThreads) {

    double h = (b-a)/(1.0 * n);
    double	 k1 = 0, k2 = 0;
    int i;

    omp_set_nested(true);
    omp_set_num_threads(numThreads);
#pragma omp parallel for reduction(+:k1,k2) private(i) shared(a,h,n)
    for(i = 1; i < n; i += 2){
        k1 += f(a + i * h);
        k2 += f(a + (i + 1) * h);
    }
    return h/3*(f(a) + 4*k1 + 2*k2 - f(b));
}

double Simpson_integral(double a, double b, int n) {

    const double h = (b-a)/(1.0 * n);
    double	 k1 = 0, k2 = 0;
    int i;
    for(i = 1; i < n; i += 2){
        k1 += f(a + i * h);
        k2 += f(a + (i + 1) * h);
    }
    return h/3*(f(a) + 4*k1 + 2*k2 - f(b));
}

void simpsonIntegral(double a,double b, int n){

    cout << "\n Подсчет интеграла итеративным методом Симпсона"<<endl;

    double s, s1,s2;

    auto start = chrono::steady_clock::now( );
    s = Simpson_integral(a,b,n);
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"\nПоследовательный алгоритм метода Симпсона = "<<s<<endl;

    auto startMP2 = chrono::steady_clock::now( );
    s1 = Simpson_integral_parallel(a,b,n,2);
    auto elapsedMP2 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP2);
    cout<<"Параллельный алгоритм метода Симпсона на 2 потока= "<<s1<<endl;

    auto startMP4 = chrono::steady_clock::now( );
    s2 = Simpson_integral_parallel(a,b,n,4);
    auto elapsedMP4 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP4);
    cout<<"Параллельный алгоритм метода Симпсона на 4 потока= "<<s2<<endl;

    double a2 = double(elapsed.count())/double (elapsedMP2.count());
    double a4 = double(elapsed.count())/double (elapsedMP4.count());

    cout<<"\nВремя работы параллельного алгоритма метода Симпсона на 2 потока: "<<elapsedMP2.count()/delim<<" мс"<<endl;
    cout<<"Время работы параллельного алгоритма метода Симпсона на 4 потока: "<<elapsedMP4.count()/delim<<" мс"<<endl;
    cout<<"Время работы последовательного алгоритма метода Симпсона: "<<elapsed.count()/delim<<" мс"<<endl;
    cout<<"Ускорение A2 = "<<a2<<endl;
    cout<<"Ускорение A4 = "<<a4<<endl;
}

void Task1(){

    cout << "Задание 1" << endl;

    double s, s1, s2;
    double a = 0;
    double b = 100;
    int intervals = 1000000;

  //  cout<<"Введите количество интервалов:"; cin>>intervals;

    cout << "\nПодсчет интеграла итеративным методом трапеций"<<endl;

    auto startMP2 = chrono::steady_clock::now( );
    s = trapezeIntegralParallel(a,b,intervals,2);
    auto elapsedParallel2 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP2 );
    cout<<"\nПараллельный алгоритм метода трапеций на 2 потока = "<<s<<endl;

    auto startMP4 = chrono::steady_clock::now( );
    s1 = trapezeIntegralParallel(a,b,intervals,4);
    auto elapsedParallel4 = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - startMP4 );
    cout<<"Параллельный алгоритм метода трапеций на 4 потока = "<<s1<<endl;

    auto start = chrono::steady_clock::now( );
    s2 = trapezeIntegral(a,b,intervals);
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"Последовательный алгоритм метода трапеций = "<<s2<<endl;

    double a2 = double(elapsed.count())/double (elapsedParallel2.count());
    double a4 = double(elapsed.count())/double (elapsedParallel4.count());

    cout<<"\n Время работы параллельного алгоритма метода трапеций на 2 потока: "<<elapsedParallel2.count()/delim<<" мс"<<endl;
    cout<<"Время работы параллельного алгоритма метода трапеций на 4 потока: "<<elapsedParallel4.count()/delim<<" мс"<<endl;
    cout<<"Время работы последовательного алгоритма метода трапеций: "<<elapsed.count()/delim<<" мс"<<endl;
    cout<<"Ускорение A2 = "<<a2<<endl;
    cout<<"Ускорение A4 = "<<a4<<endl;

    simpsonIntegral(a,b,intervals);
}
