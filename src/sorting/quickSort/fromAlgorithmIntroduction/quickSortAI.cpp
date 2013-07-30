#include <iostream>
using namespace std;
const int N = 5;
int a[N+1] = {-1, 3, 4, 2, 1, 5};

void swap(int& a, int& b)
{
    if(a != b)
    {
        int temp =a;
        a = b;
        b = temp;
    }
}
int Partition(int a[], int low, int high)
{
    int pivotValue = a[high];
    int i = low-1;
    for(int j = low; j <= high-1; j++)
    {
        if(a[j] <= pivotValue)
        {
            i = i+1;
            swap(a[i], a[j]);
        }
    }
    swap(a[i+1], a[high]);
    return i+1;
}
void QuickSort(int a[], int left, int right)
{
    if(left < right)
    {
        int pivotPos = Partition(a, left, right);
        QuickSort(a, left, pivotPos-1);
        QuickSort(a, pivotPos+1, right);
    }
}
void myPrint()
{
    for(int i =1; i <= N; i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<endl;
}

int main()
{
    myPrint();
    QuickSort(a,1,N);
    myPrint();
    return 0;
}
