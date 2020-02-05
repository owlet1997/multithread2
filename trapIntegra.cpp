//
// Created by owlet on 17.10.19.
//
#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;
const double eps = 0.00001;
const double delim = 1000000;

double f(double x){ return x*x+3*x; }

double recursionTrapezIntegral(double a, double b, double fa, double fb, double area){
    double mid = (a+b)/2;
    double fmid = f(mid);
    double leftArea = (fa + fmid) * (mid - a)/2;
    double rightArea = (fmid + fb) * (b - mid)/2;
    double res;
    if(abs(leftArea + rightArea - area) > eps){
        {
            #pragma omp task shared(leftArea)
            leftArea = recursionTrapezIntegral(a, mid, fa, fmid, leftArea);
        }
        {
            #pragma omp task shared( rightArea)
            rightArea = recursionTrapezIntegral(mid, b, fmid, fb, rightArea);
        }
    #pragma omp taskwait
    res = leftArea + rightArea;
    }
    return res;
}

void integral(){
    double a = 100, b = 100000, result = 0;

    auto start = chrono::steady_clock::now( );
omp_set_nested(true);
#pragma omp parallel shared(a,b)
   {
#pragma omp single
   result = recursionTrapezIntegral(a,b,f(a),f(b),(f(a)+f(b))*(b-a)/2);
   }
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>( chrono::steady_clock::now( ) - start);
    cout<<"Время работы параллельного алгоритма: "<<elapsed.count()/delim<<" мксекунд"<<endl;

}
