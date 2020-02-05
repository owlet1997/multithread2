#include <iostream>
#include "quickSort.h"
#include "trapIntegra.h"
#include "treeBinary.h"
#include "binarySort.h"

using namespace std;

int main() {
    /*
 Задание 1. (25%)
Написать программу, реализующую
определенного интеграла методом трапеций.
рекуррентное
вычисление*/
    integral();
    cout<<"========================"<<endl;
    /*
Задание 2. (25%)
Написать программу, реализующую быструю сортировку.*/
    int n = getArraySize();
    sort(n);
    cout<<"========================"<<endl;

    /*
Задание 3. (25%)
Написать программу, реализующую поразрядную сортировку.*/
    binarySort();
    /*
 Задание 4. (25%)
 Написать программу, вычисляющую сумму элементов бинарного
 дерева.
  */
    binaryTree();
    return 0;
}