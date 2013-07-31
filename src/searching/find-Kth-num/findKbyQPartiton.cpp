#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
const int N = 6;

int RandInt(int l, int r)
{
    return (rand()%(r-l+1) + l);
}
int RandomPartition(int a[], int l, int r)
{
    swap(a[l], a[RandInt(l,r)]);
    int i=r;
    int m = r+1;
    for(; i >=l; --i)
    {
        if(a[i] >= a[l])
            swap(a[i], a[--m]);
    }
    return m;
}
int GetKthNumbers(int a[], int l, int r, int k)
{
    if(a == NULL || l>r || k>r-l+1)
        return -1;
    if(l ==r)
        return l;
    int m = RandomPartition(a, l, r);
    int num = m-l+1;
    if(num == k)
        return m;
    else if(num > k)
        return GetKthNumbers(a, l, m-1,k);
    else
        return GetKthNumbers(a, m+1, r,k-num);

}

int main()
{
    int a[N] = {3, 1, 5, 4, 2, 6};
    int b[N]; //auxillary array because this algorithm can modify array a
    int k =1;
    srand((unsigned int)time(0));
    for(; k <=N; ++k)
    {
        for(int j=0; j<N; ++j)
            b[j] = a[j];
        int index = GetKthNumbers(b, 0, N-1, k);
        for(int i = 0; i< k; ++i)
            cout<<b[i]<<" ";
        cout<<endl;
    }
    return 0;
}
