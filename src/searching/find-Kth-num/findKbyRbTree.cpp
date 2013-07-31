#include <iostream>
#include <vector>
#include <set>
using namespace std;
const int N = 6;

typedef multiset<int, greater<int> > intSet;
typedef multiset<int, greater<int> >::iterator setIterator;
void GetLeastNumbers(const vector<int>& data, intSet& leastNum, unsigned int k)
{
    if(data.size() < k || k<1)
        return;
    vector<int>::const_iterator iter = data.begin();
    
    for(; iter != data.end(); ++iter)
    {
        if(leastNum.size() < k)
            leastNum.insert(*iter);
        else
        {
            setIterator iterGreatest = leastNum.begin();
            if(*iter < *(leastNum.begin()))
            {
                leastNum.erase(iterGreatest);
                leastNum.insert(*iter);
            }

        }
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
    intSet leastNum;
    
    unsigned int k =1;
    for(; k <=N; ++k)
    {
        GetLeastNumbers(data, leastNum,k);
        for(setIterator iter=leastNum.begin(); iter != leastNum.end(); ++iter)
        {
            cout<<*iter<<" ";
        }
        cout<<endl;
        leastNum.clear();
    }
    return 0;
}
