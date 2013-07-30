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
    int pivotValue = a[low];
    int i = low+1;
    int j = high;
    while(1)
    {//detection at the same time
        while(i < j && pivotValue < a[j]) j--;
        while(i < j && pivotValue > a[i]) i++;
        if(i < j)
        {
            swap(a[i],a[j]);
            i++; j--;
        }
        else //i >= j
        {
            if(pivotValue < a[i])
                i--;
            break;
        }
    }
    
    a[low] = a[i];
    a[i] = pivotValue;
    
    return i;
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

