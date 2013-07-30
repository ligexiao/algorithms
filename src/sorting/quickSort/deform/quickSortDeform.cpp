#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
const int N = 1000;
//int a[N+1] = {-1, 3, 4, 2, 1, 5, 3};
int a[N+1]={0};
const int CUT_OFF = 50;

void swap(int& a, int& b)
{
    if(a != b)
    {
        int temp =a;
        a = b;
        b = temp;
    }
}
int RandInt(int l, int r)
{
    return (rand()%(r-l+1) + l) ;
}
void InsertSort(int a[], int left, int right)
{
    if(left >= right)
        return;
    int i = left+1;
    for( ; i<= right; ++i)
    {
        int temp = a[i];
        int j = i;
        for(; (j>left) && (temp < a[j-1]) ; --j)
        {
            a[j] = a[j-1];
        }
        a[j] = temp ;
    }
}

/*
 * quick sortint that sorting lots of numbers to be ordered generally 
 * then, insert sotring that put those numbers to be ordered strictly
 * CUT_OFF: 30~70, I set it to 50 in this file.
 * */
void QSortWithCutoff(int a[], int left, int right)
{//from right to left that use a[left] as a sentinel to accelerate the algorithm
    if(right - left < CUT_OFF)
        return;
    int m = right+1;
    for(int i = right; i>= left; --i)
    {
        if(a[i] >= a[left])
            swap(a[--m],a[i]);
    }

    QSortWithCutoff(a,left,m-1);
    QSortWithCutoff(a,m+1,right);
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
    int i =1;
    srand((unsigned int)time(0)); //seed
    for(; i<= N; ++i)
    {
        a[i] = RandInt(1,N);
    }   
    cout<<"Before Sorting:\n";
    myPrint();
    QSortWithCutoff(a,1,N);
    cout<<"After Quick Sorting:\n";
    myPrint(); 
    InsertSort(a,1,N);
    cout<<"After Insert Sorting:\n";
    myPrint();
    return 0;
}
