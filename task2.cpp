//
// Created by owlet on 11.10.19.
//
#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;

double funcPi(double x) {return 4/(pow(x,2)+1);}


double Simpson_integral_PI(double a, double b, int n) {

    const double h = (b-a)/(1.0 * n);
    double	 k1 = 0, k2 = 0;
    int i;
    for(i = 1; i < n; i += 2){
        k1 += funcPi(a + i * h);
        k2 += funcPi(a + (i + 1) * h);
    }
    double s = h/3*(funcPi(a) + 4*k1 + 2*k2 - funcPi(b));
    return s;
}


bool IsPointInCircle(double R, double x, double y) {
    //функция возвращает true, если точка находится в единичной окружности
    return ((pow(x,2) + pow(y,2)) < pow(R,2));
}

void Task2() {

    int n=0, pointsInCircle=0;
    cout <<"\nЗадание 2 [Вычисление значения pi]" << endl;
    cout << "\nВведите число точек: ";
    cin >> n;
    cout << "Var.1: По правому верхнему квадранту единичной окружности" << endl;

    for (auto i = 0; i < n; ++i) {
        if(IsPointInCircle( 1.0, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX) ) {
            pointsInCircle++;
        }
    }
    cout << "Значение Pi: "<<pointsInCircle*4.0/n << endl;
    cout << "Var.2: Как значение интеграла" << endl;

    cout << "Введите количество шагов: ";
    cin >> n;

    double pi = Simpson_integral_PI(0,1,n);
    cout << "PI = "<< pi<< endl;
}