#include <iostream>
#include <queue>
using namespace std;
const int N = 6;

typedef priority_queue<int, vector<int> > pqueueInt;
void GetLeastNumbers(const vector<int>& data, pqueueInt& pq, unsigned int k)
{
    vector<int>::const_iterator it = data.begin();
    while(it != data.end())
    {
        if(pq.size() < k)
            pq.push(*it);
        else
        {
            if(*it < pq.top())
            {
                pq.pop();
                pq.push(*it);
            }
        }
        ++it;
    }
}
int main()
{
    int a[N] = {3, 1, 5, 4, 2, 6};
    vector<int> data;
    int i=0;
    for(; i<N; ++i)
    {
        data.push_back(a[i]);
    }
    
    priority_queue<int, vector<int> > pq;
    
    unsigned int k =1;
    for(; k <=N; ++k)
    {
        GetLeastNumbers(data, pq ,k);
        while(pq.size() != 0)
        {
            cout<<pq.top()<<" ";
            pq.pop();
        }
        cout<<endl;
    }
    return 0;
}
