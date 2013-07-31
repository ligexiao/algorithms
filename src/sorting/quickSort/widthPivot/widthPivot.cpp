#include <iostream>
using namespace std;
const int N = 6;
void WidthPivot(int a[], int l, int u)
{
    if(l >= u)
        return;
    
    bool bHasFirstEqual = false;
    
    int n=u+1;
    int m=u+1;
    int i=u;
    for(; i>= l; --i)
    {
        if(a[i] == a[l])
        {
            swap(a[i],a[--m]);
            if(!bHasFirstEqual)
                bHasFirstEqual = true;
        }
        else if(a[i] > a[l])
        {
            swap(a[i], a[--n]);
            if(bHasFirstEqual)
                swap(a[i], a[--m]);
            else
                m = n;
        }
    }

    WidthPivot(a, l, m-1);
    WidthPivot(a,n,u);
}

void Print(int a[])
{
    int i =0;
    for(; i<N; ++i)
    {
        cout<<a[i]<<" ";
    }
    cout<<endl;
}
int main()
{
    int a[N] ={ 3, 1, 2,3, 5,3};
    WidthPivot(a, 0, N-1);
    Print(a);
    return 0;
}
