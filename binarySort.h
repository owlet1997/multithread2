//
// Created by owlet on 17.10.19.
//

#ifndef MULTITHREAD3_BINARYSORT_H
#define MULTITHREAD3_BINARYSORT_H
using namespace std;

vector<int> init();
template<class T>
void show(T& arr);
string to_binary(int inumber);
vector<string> to_binary(vector<int> array);
void rsort(vector<string> & bin_array, int charge);
vector<int> to_int(vector<string> sarray);
void binarySort();

#endif //MULTITHREAD3_BINARYSORT_H
