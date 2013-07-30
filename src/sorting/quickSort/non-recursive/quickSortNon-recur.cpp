#include <iostream>
#include <vector>
#include <stack>
using namespace std;

stack<int> stackLeft;
stack<int> stackRight;

void swap(int& a, int&b)
{
    int temp = a;
    a = b;
    b = temp;
}
int partition(int a[], int left, int right)
{
    int i = left;
    for(int j = i; j < right; ++j)
    {
        if(a[j] < a[right])
            swap(a[j], a[i++]);
    }
    swap(a[i], a[right]);
    return i;
}
void quickSortNR(int a[], int left, int right)
{
    stackLeft.push(left);
    stackRight.push(right);
    int topL, topR;
    while((stackLeft.size() == stackRight.size() ) && !stackLeft.empty())
    {
        topL = stackLeft.top();
        topR = stackRight.top();
        stackLeft.pop();
        stackRight.pop();
        if(topL < topR)
        {
            int middle = partition(a, topL, topR);
            cout<<"middle: "<<middle<<endl;
            stackLeft.push(middle+1);
            stackRight.push(topR);

            stackLeft.push(topL);
            stackRight.push(middle-1);
        }
    }
}
int main()
{
    int myInt[] = {5, 4, 7, 2, 10, 1, 9};
    vector<int> vecInt(myInt, myInt + sizeof(myInt)/sizeof(int));
    stack<int, vector<int> > inS(vecInt);
    cout<<"stack size: "<<inS.size()<<endl;
    cout<<"stack top: "<<inS.top()<<endl;

    quickSortNR(myInt, 0, sizeof(myInt)/sizeof(int) -1);
    for(int i =0; i < sizeof(myInt)/sizeof(int); ++i)
        cout<<myInt[i]<<",";
    cout<<endl;

    return 0;

}
