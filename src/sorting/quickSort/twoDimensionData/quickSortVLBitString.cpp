#include <iostream>
#include <cstring>
using namespace std;
const int N =4;

struct VarLenBit
{
    int length;
    char* bit;
};
void InitialStrcut(VarLenBit x[], char bitArray[][10])
{
    for(int i = 0; i< 4; i++)
    {
        x[i].length = strlen(bitArray[i]);
        x[i].bit = bitArray[i];
    }

}
void PrintStruct(VarLenBit x[])
{
    for(int i = 0; i<N; ++i)
    {
        cout<<x[i].bit<<endl;
    }
}
void bsort(VarLenBit x[],int l, int u, int depth)
{
    if(l >= u)
        return;
    int i=l;
    for(; i <=u; ++i)
    {
        if(x[i].length < depth)
        {
            swap(x[i], x[l]);
            l++;
        }
    }
    int m = l;
    for(i =l; i<=u; ++i)
    {
        if(x[i].bit[depth-1]== '0')
            swap(x[i], x[m++]);
    }
    bsort(x, l, m-1,depth+1);
    bsort(x, m, u, depth+1);
}


int main()
{
    VarLenBit x[N];
    char bitArray[4][10]={"1101", "01", "1010","10"};
    InitialStrcut(x, bitArray);

    bsort(x,0,N-1,1);

    PrintStruct(x);
    return 0;
}
