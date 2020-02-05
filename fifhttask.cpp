#include <cstdio>
#include <string>
#include <iostream>

using namespace std;



template<typename T>
void sortBin(T *arr, int arrSize) {

    T buffer;
    bool firstIsOne, secondIsOne, meetOne;

    for (int i = 0; i < arrSize - 1; i++) {

        for (int j = 0; j < arrSize - i - 1; j++) {

            meetOne = false;

            for (int k = sizeof(T) * 8 - 1; k >= 0; k--) {

                firstIsOne = arr[j] & (int(1) << k);
                secondIsOne = arr[j + 1] & (int(1) << k);


                if(firstIsOne || secondIsOne) {
                    meetOne = true;
                }

                if(!firstIsOne && secondIsOne && meetOne) {
                    break;
                }

                if (firstIsOne && !secondIsOne)
                {
                    buffer = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = buffer;
                    break;
                }
            }
        }
    }
}


//int main() {
//
//    int arraySize = getArraySize();
//
//    int *array = new int[arraySize];
//
//    srand(time(NULL));
//
//    for (int i = 0; i < arraySize; i++) {
//        array[i] = rand() % 100;
//        cout << array[i] << " ";
//    }
//
//    sortBin(array, arraySize);
//
//    cout << endl;
//    for (int i = 0; i < arraySize; i++) {
//        cout << array[i] << " ";
//    }
//
//
//
//    return 0;
//}