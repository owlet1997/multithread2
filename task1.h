//
// Created by owlet on 11.10.19.
//

#ifndef MULTITHREAD2_TASK1_H
#define MULTITHREAD2_TASK1_H

double trapezeIntegralParallel(double a, double b, int intervals, int numThreads);
double trapezeIntegral(double a, double b, int intervals);
double Simpson_integral_parallel(double a, double b, int n, int numThreads);
double Simpson_integral(double a, double b, int n);
void simpsonIntegral(double a,double b, int n);
void Task1();
double f(double x);

#endif //MULTITHREAD2_TASK1_H
