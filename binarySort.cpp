/*
Задание 3. (25%)
Написать программу, реализующую поразрядную сортировку.
*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
#include <cmath>
#include <omp.h>

using namespace std;

int max_bit_number;

vector<int> init(){
    vector<int> array;
    int n;
    cout << "Введите количество элементов:";
    cin >> n;
    for(int i = 0; i < n; ++i){
        array.push_back(rand()%1001);
    }
    return array;
}

template<class T>
void show(T& arr){
    for(int i = 0; i < arr.size(); ++i){
        cout << arr[i] << endl;
    }
    cout << "----------------\n";
}

string to_binary(int inumber){
    string snumber = "";
    int divider;
    int max_size = 0;
    divider = inumber;
    while (divider >= 2){
        snumber = to_string(divider % 2) + snumber;
        divider = divider/2;
    }
    snumber = to_string(divider) + snumber;
    if(snumber.size() < max_size){
        while(snumber.size() != max_size){
            snumber = "0" + snumber;
        }
    }
}

vector<string> to_binary(vector<int> array){
    vector<string> bin_array;
    string number = "";
    int divider;
    int max_size = 0;
    for(int i = 0; i < array.size(); ++i){
        divider = array[i];
        while (divider >= 2){
            number = to_string(divider % 2) + number;
            divider = divider/2;
        }
        number = to_string(divider) + number;
        bin_array.push_back(number);
        if(max_size < number.size())
            max_size = number.size();
        number = "";
    }

    for(int i = 0; i < bin_array.size(); ++i){
        if(bin_array[i].size() < max_size){
            while(bin_array[i].size() != max_size){
                bin_array[i] = "0" + bin_array[i];
            }
        }
    }
    max_bit_number = max_size;
    return bin_array;
}

void rsort(vector<string> & bin_array, int charge){
    int extend = bin_array[0].size() - charge;
#pragma omp parallel for shared(bin_array) num_threads(bin_array.size())
    for(int j = 1; j < bin_array.size(); ++j){
        for(int n = 0; n < bin_array.size()-j; n++){
            if(bin_array[n][extend] > bin_array[n+1][extend]){
                string temp = bin_array[n];
                bin_array[n] = bin_array[n+1];
                bin_array[n+1] = temp;
            }
        }
    }
    if(extend > 0)
#pragma omp task shared(bin_array)
        rsort(bin_array, ++charge);
#pragma omp taskwait
}

vector<int> to_int(vector<string> sarray){

    //10110110 = (1·2^7)+(0·2^6)+(1·2^5)+(1·2^4)+(0·2^3)+(1·2^2)+(1·2^1)+(0·2^0) = 128+32+16+4+2 = 182
    vector<int> iarray;
    string buffer;
    for(int i = 0; i < sarray.size(); i++){
        int size = sarray[0].size()-1;
        int summ = 0;
        for(int j = 0; j < sarray[0].length(); j++){
            buffer = sarray[i][j];

            summ += atoi(buffer.c_str())*pow(2, size);
            size--;
        }
        iarray.push_back(summ);
    }
    return iarray;
}

void binarySort(){
    srand(time(NULL));
    double t1, t2;
    vector<int> array = init();
    cout << "Сгенерированные элементы:\n";
    show(array);
    vector<string> bin_array = to_binary(array);
    cout << "Элементы в двоичном виде:\n";
    show(bin_array);
#pragma omp parallel
#pragma omp single nowait
    {
        t1 = omp_get_wtime();
        rsort(bin_array, 1);
        t2 = omp_get_wtime();
    }
    array = to_int(bin_array);
    show(bin_array);
    cout << "Элементы после поразрядной сортировки:\n";
    show(array);
    printf("Время параллельной поразрядной сортировки: %0.8f\n", t2-t1);

}