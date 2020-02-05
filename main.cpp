#include <iostream>
#include <random>
#include <malloc.h>

using namespace std;
const double eps = 0.001;
static int count = 0;

int getArraySize() {
    int arraySize;

    cout << "Input array size: ";
    cin >> arraySize;

    return arraySize;
}

void lookMatrix(int **mas1, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << mas1[i][j] << " ";
        cout << endl;
    }
}
void lookMass(int *mas,int n){
    for (int i = 0; i< n; i++)
        cout<<mas[i]<<"\t";
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
    uniform_int_distribution<int> distribution(100, 1000);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mas[i][j] = distribution(generator);
    return mas;
}

int* createMas(int n){
    int* mas;
    mas = (int*)malloc(n*sizeof(int*));

    default_random_engine generator;
    uniform_int_distribution<int> distribution(1, 10);

    for (int i = 0; i < n; i++)
        mas[i] = distribution(generator);

    return mas;
}

void multiply(int n) {

    int **mas1 = createMass(n);
    int **mas2 = createMass(n);
    fullMass(mas1,n);
    fullMass(mas2,n);

    lookMatrix(mas1,n);
    lookMatrix(mas2,n);

    int **result = createMass(n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;

            for (int k = 0; k < n; k++)
                result[i][j] += mas1[i][k] * mas2[k][j];
        }

    lookMatrix(result,n);

    delete [] mas1;
    delete [] mas2;
    delete [] result;

}

double f(double x){ return x+3; }

void trapezeIntegral(double a, double b, int intervals){
        double S = 0, S1 = 0, x = a;
        double h = (b-a)/intervals;
        do {
        S1 = S;
        S = f(a) + f(b);
        while (x < b-h){
            x += h;
            S += 2 * f(x);
        }
        S *= h/2;
        } while(fabs(S-S1) < h);

        cout<<"Trapeze integral = "<<abs(S)<<endl;
    }

void rectangleIntegral(double a, double b, int intervals){
        double x;
        double h = (b-a)/intervals;
        double result = 0;

        for(int i = 1; i <= intervals; i++){
            x = a + (i-0.5)*h;
            result += h*f(x);
        }
        cout<<"Rectangle integral = "<<abs(result)<<endl;
    }

double recursionTrapezIntegral(double a, double b, double fa, double fb, double area){
        double mid = (a+b)/2;
        double fmid = f(mid);
        double leftArea = (fa + fmid) * (mid - a)/2;
        double rightArea = (fmid + fb) * (b - mid)/2;
        if(abs(leftArea + rightArea - area) > eps){
            leftArea = recursionTrapezIntegral(a, mid, fa, fmid, leftArea);
            rightArea = recursionTrapezIntegral(mid, b, fmid, fb, rightArea);
            count++;
        }
        return leftArea + rightArea;
}

double recursionRectangleIntegral(double a, double b, double fa, double area){
    double mid = (a+b)/2;
    double fmid = f(mid);
    double leftArea = fa  * (mid - a);
    double rightArea = fmid * (b - mid);
    if(abs(leftArea + rightArea - area) > eps){
        leftArea = recursionRectangleIntegral(a, mid, fa, leftArea);
        rightArea = recursionRectangleIntegral(mid, b, fmid, rightArea);
        count++;
    }
    return leftArea + rightArea;
}

void integral(){
    cout<<"Enter a:"; double a; cin>>a;
    cout<<"Enter b:"; double b; cin>>b;
    cout<<"Enter number intervals:"; int intervals; cin>>intervals;
    trapezeIntegral(a,b,intervals);
    rectangleIntegral(a,b,intervals);
    cout<<"Recursion trapecion integral = "<<abs(recursionTrapezIntegral(a, b, f(a), f(b), (f(a)+f(b))*(b-a)/2))<<endl;
    cout<<"Recursion rectangle integral = "<<abs(recursionRectangleIntegral(a, b, f(a), (f(a)+f(b))*(b-a)/2))<<endl;

    count = 0;
}

void copy(int n){

    int **mas1 = createMass(n);
    fullMass(mas1,n);

    lookMatrix(mas1,n);
    int **mas2 = createMass(n);

    int *buf = new int;
    for (int i=0;i<n;i++)
        for (int j=0;j<n;j++)
        {
            *buf = mas1[i][j];
            mas2[i][j] = *buf;
        }
    cout << "Matrix-copy" << endl;

    lookMatrix(mas2,n);

    delete [] mas1;
    delete [] mas2;
    delete buf;
}

void quickSort(int *numbers, int left,int right){
    int letElement;
    int l = left;
    int r = right;
    letElement = numbers[(l+r)/2];
    while (l < r)
    {
        while (numbers[l]<letElement)
            l++;

        while (numbers[r]>letElement)
            r--;

        if (l <= r)
        {
            swap(numbers[l++],numbers[r--]);
        }
    }
    if (left < r)
        quickSort(numbers, left, r);
    if (right > l)
        quickSort(numbers, l, right);
}

void fourthTask(int n){
    int *mas1 = createMas(n);
    lookMass(mas1,n);
    quickSort(mas1,0,n-1);
    cout<<"\nResult"<<endl;
    lookMass(mas1,n);
    delete [] mas1;
}

template<typename T>
void binarySort(T *arr, int n){
    T buf;
    bool firstOne, secondOne, thereIsOne;
    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            thereIsOne = false;

            for (int k = sizeof(T) * 8 - 1; k >= 0; k--) {

                firstOne = arr[j] & (T(1) << k);
                secondOne = arr[j + 1] & (T(1) << k);

                if(firstOne || secondOne) {
                    thereIsOne = true;
                }

                if(!firstOne && secondOne && thereIsOne) {
                    break;
                }

                if (firstOne && !secondOne)
                {
                    buf = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = buf;
                    break;
                }

            }
        }
    }
}

void fifthTask(int n){
    int *array = createMas(n);

    lookMass(array,n);
    binarySort(array,n);

    cout << endl;

    lookMass(array,n);

    delete [] array;
}


int main() {
        int n = getArraySize();

        cout<<"First task"<<endl;
        multiply(n);
        cout<<"Second task"<<endl;
        integral();
        cout<<"Third task"<<endl;
        copy(n);
        cout<<"Forth task"<<endl;
        fourthTask(n);
        cout<<"\nFifth task"<<endl;
        fifthTask(n);

  return 0;
}

