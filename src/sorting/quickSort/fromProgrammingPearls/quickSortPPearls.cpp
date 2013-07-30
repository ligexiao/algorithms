#include <iostream>
using namespace std;
const int N = 6;
int a[N+1] = {-1, 3, 4, 2, 1, 5, 3};

void swap(int& a, int& b)
{
    if(a != b)
    {
        int temp =a;
        a = b;
        b = temp;
    }
}
void QSortFromPPerls(int a[], int left, int right)
{
    if(left >= right)
        return;
    int m = left;
    for(int j = left+1; j<= right; ++j)
    {
        if(a[j] < a[left])
            swap(a[++m],a[j]);
    }
    swap(a[left],a[m]);
    QSortFromPPerls(a,left,m-1);
    QSortFromPPerls(a,m+1,right);
}
void QSortFromPPerlsM1(int a[], int left, int right)
{//from right to left that use a[left] as a sentinel to accelerate the algorithm
    if(left >= right)
        return;
    int m = right+1;
    for(int i = right; i>= left; --i)
    {
        if(a[i] >= a[left])
            swap(a[--m],a[i]);
    }

    QSortFromPPerls(a,left,m-1);
    QSortFromPPerls(a,m+1,right);
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
    //QSortFromPPerls(a,1,N);
    QSortFromPPerlsM1(a,1,N);
    myPrint();
      
    return 0;
}
