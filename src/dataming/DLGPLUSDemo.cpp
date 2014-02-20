#include "DLGPLUSDemo.h"
#include <set>
#include <map>
using namespace std;
HANDLE hMutex;
DWORD threadIds[ITEM_SIZE + 1];
map<int, DWORD> map_subGraphIndex;

DLGPLUSDemo::DLGPLUSDemo()
{
	for (int i = 1; i <= ITEM_SIZE; i++)//初始化邻接矩阵 - 邻接矩阵只是为了求节点的度方面而建立 - 频繁2项集
	{
		for (int j = 1; j <= ITEM_SIZE; j++)
		{
			arcs[i][j] = 0;
		}
	}

	VTable = new VNode*[ITEM_SIZE + 1];
	for (int vi = 0; vi <= ITEM_SIZE; vi++)
	{

		VTable[vi] = new VNode[ITEM_SIZE + 1];//初始化邻接表 - 各关联子图
		for (int ii = 0; ii <= ITEM_SIZE; ii++)
		{
			VTable[vi][ii].firstarc = NULL;
			VTable[vi][ii].info = 0;
		}
	}

}
void DLGPLUSDemo::itemsInit()
{
	if (ITEM_SIZE > 26)
	{
		cout << "ERROR!!项目数过多！" << endl;
		return;
	}
	char x = 'a';
	for (int i = 0; i < ITEM_SIZE; ++i,++x)
	{
		items[i] = x;
	}
}

void DLGPLUSDemo::file2transactionMatrix(string fileName)
{
	
	//初始化的项目集
	itemsInit();

	ifstream  input = ifstream(fileName.c_str());
	if (!input) //文件打开失败!
	{
		cout << "存放事务的文件transction不存在，请先手工建立" << endl;

		return;
	}
	else
	{
		while (input)
		{
			string  strFileLine;
			getline(input, strFileLine, '\n'); //得到一行事务项集

			if (strFileLine != "")//最后有一空行，所以在此用if消除
			{
				int i = 0;  //事务中：项目标号
				int j = 0;  //项目集：项目标号
				vector<int> vecFileLine;
				while (i < ITEM_SIZE)
				{
					if (items[i] == strFileLine[j])
					{
						vecFileLine.push_back(1);
						i++;
						j++;
					}
					else
					{
						vecFileLine.push_back(0);
						i++;
					}
				}
				transactionMatrix.push_back(vecFileLine);
			}
		}
	}
	transactionSize = transactionMatrix.size();  //事务数
}
void DLGPLUSDemo::initializeALLDataStructures()
{
	for (int i = 1; i <= ITEM_SIZE; i++)//清空邻接矩阵
	{
		for (int j = 1; j <= ITEM_SIZE; j++)
		{
			arcs[i][j] = 0;
		}
	}

	for (int vi = 0; vi <= ITEM_SIZE; vi++)
	{

		VTable[vi] = new VNode[ITEM_SIZE + 1];//清空邻接表
		for (int ii = 0; ii <= ITEM_SIZE; ii++)
		{
			VTable[vi][ii].firstarc = NULL;
			VTable[vi][ii].info = 0;
		}
	}

	transactionMatrix.clear();
	reverseTrans.clear();
	for (int i = 0; i < ITEM_SIZE; i++)
	{
		set_vecA[i].clear();
	}

}
DLGPLUSDemo::~DLGPLUSDemo()
{
	//	cout<<"~DLGPLUSDemo()"<<endl;

	for (int i = 0; i <= ITEM_SIZE; i++)
	{
		for (int j = 0; j <= ITEM_SIZE; j++)
		{
			ArcNode* ptr = VTable[i][j].firstarc;
			while (ptr != NULL)
			{
				ArcNode* temp = ptr;
				ptr = ptr->nextarc;
				delete temp;
				//				cout<<"deleted a arc node"<<endl;
			}
		}
		delete[] VTable[i];
	}
	delete[] VTable;
}
void DLGPLUSDemo::reverseTransaction() //邻接矩阵的逆置: 作为挖掘的输入
{
	for (int j = 0; j < ITEM_SIZE; j++)
	{
		vector<int> vec;
		for (int i = 0; i < transactionMatrix.size(); i++)
		{
			vec.push_back(transactionMatrix[i][j]); //[i][0]		
		}
		reverseTrans.push_back(vec);
	}

}

void DLGPLUSDemo::showTransaction()
{
	cout << "数据库中的事务集合：" << endl;
	for (int ii = 0; ii < transactionMatrix.size(); ++ii)
	{
		for (int m = 0; m < transactionMatrix[ii].size(); ++m)
		{
			cout << transactionMatrix[ii][m] << " ";
		}
		cout << endl;
	}
}
void DLGPLUSDemo::showReverseTrans()
{
	cout << "转换后的事务集合：" << endl;
	for (int i = 0; i < reverseTrans.size(); ++i)
	{
		for (int j = 0; j< reverseTrans[i].size(); ++j)
		{
			cout << reverseTrans[i][j] << " ";
		}
		cout << endl;
	}
}

/*全体频繁2项集构成的关联图 - 邻接矩阵形式*/
void DLGPLUSDemo::printarcs()
{
	for (int i = 1; i <= ITEM_SIZE; i++)
	{
		for (int j = 1; j <= ITEM_SIZE; j++)
			cout << arcs[i][j] << " ";
		cout << endl;
	}
}

/*各子图频繁2项集构成的关联图 - 邻接表形式*/
void DLGPLUSDemo::printVTable()
{
	cout << "邻接表:" << endl;  //频繁2项集
	for (int i = 1; i < ITEM_SIZE; i++)
	{
		cout << "The " << i << "st node: " << endl;
		for (int j = 1; j <= ITEM_SIZE; j++)
		{		
			ArcNode*p = VTable[i][j].firstarc;
			if (!p) continue;
			cout << j << " ";
			while (p != NULL)
			{
				cout << p->adjvex << " ";
				p = p->nextarc;
			}
			cout << endl;
		}
	}
}
void DLGPLUSDemo::createGraph()
{
	for (int i = 0; i<reverseTrans.size(); i++)
	{
		for (int inext = i + 1; inext < reverseTrans.size(); inext++) //对于邻接矩阵，相当于只考虑“上三角”矩阵。
		{
			int num = 0;
			vector<int> temp;
			for (int k = 0; k < transactionSize; k++) //对所有的项目向量进行与操作
			{
				if (reverseTrans[i][k] * reverseTrans[inext][k] == 1) //Vi^Vinext---逻辑“与”
					num++;
			}
			if (num >= MIN_SUP)//minsup
			{
				arcs[i + 1][inext + 1] = 1; //构造邻接矩阵（包含了所有频繁2项集的所有信息）------------生成各完整子图的证据!
			}

		}
	}

	/*下面是分治的思想*/
	//vector< vector<int> > kvec[ITEM_SIZE+1];
	vector< int > temp_vertex;
	vector< int > temp_subGraph_vertex;
	for (int s_i = 1; s_i <= ITEM_SIZE; s_i++)  //扫描全局矩阵-----建立子图的邻接表和子频繁2项集
	{
		temp_vertex.push_back(s_i);
		for (int s_j = 1; s_j <= ITEM_SIZE; s_j++)
		{
			if (arcs[s_i][s_j] == 1)   //只保存结点i相邻的节点编号--------------点:vertex
			{
				temp_vertex.push_back(s_j);
			}
		}
		for (int k_i = 0; k_i < temp_vertex.size(); k_i++)
		{
			int k_ii = temp_vertex[k_i];
			for (int k_j = k_i + 1; k_j < temp_vertex.size(); k_j++)
			{
				int k_jj = temp_vertex[k_j];
				if (arcs[k_ii][k_jj] == 1)    //结点相邻----------------------------边:edge
				{

					temp_subGraph_vertex.push_back(k_ii);  //同一子图的 频繁2项集
					temp_subGraph_vertex.push_back(k_jj);
					kvec[s_i].push_back(temp_subGraph_vertex);  //加入频繁2项集全集中去
					/*生成关子联图: 保全了子图的所有信息*/
					ArcNode *s = new ArcNode; //生成一个弧结点

					s->nextarc = VTable[s_i][k_ii].firstarc;      //插入到邻接表中
					s->adjvex = k_jj;


					VTable[s_i][k_ii].firstarc = s;
					VTable[s_i][k_ii].info += 1;
					VTable[s_i][k_jj].info += 1;
					//	cout<<VTable[s_i][k_ii].info<<endl;
					temp_subGraph_vertex.clear();           //清空数组
				}
			}

		}

		temp_vertex.clear();                               //清空数组

	}

}

void DLGPLUSDemo::checkDegree(int vi, int degree)
{
	for (int i = 1; i<ITEM_SIZE + 1; i++)
	{
		if (VTable[vi][i].info < degree)
		{
			ArcNode* pre = VTable[vi][i].firstarc;
			while (pre != NULL)
			{
				ArcNode* temp = pre;
				pre = pre->nextarc;
				delete temp;
				//	cout<<"deleted a arc node"<<endl;
			}

			VTable[vi][i].firstarc = NULL;
		}
	}
}
void DLGPLUSDemo::printkvec()
{
	for (int i = 1; i <= ITEM_SIZE; i++)
	{
		for (int j = 0; j < kvec[i].size(); j++)
		{
			for (int k = 0; k < kvec[i][j].size(); k++)
			{
				cout << kvec[i][j][k] << " ";
			}
			cout << ",";
		}
		cout << endl;
	}
	cout << endl;
}
void DLGPLUSDemo::k_freq_to_k1_cand(int vi, vector < vector<int> > kvec)
{
	
	//保存频繁 k 项集 至 第vi个频繁项集合中
	//DWORD before=GetTickCount();
	saveFrequentItemSet(vi, kvec);
	//	DWORD after=GetTickCount();
	//cout<<"saveFrequentItemSet time:"<<double(after - before)<<endl;
	
	//当涉及该条目的没有任何频繁项集时，则退出！
	if (kvec.size() == 0)return;
	
	int degreeK = kvec[0].size();
	checkDegree(vi, degreeK);  //定理2：若此k项集的中某个节点的度小于k,则删除该节点及其相邻的边。

	vector< vector<int> > cank1vec;  //频繁K+1项集 
	for (int i = 0; i < kvec.size(); i++)
	{
		int kvec_i_Len = kvec[i].size();
		ArcNode* p = VTable[vi][kvec[i][kvec_i_Len - 1]].firstarc;  //k扩展至k+1
		while (p != NULL)
		{
			int num = 0;
			for (int j = 0; j < kvec[i].size(); j++)  //定理3
			{
				if (existEdge(vi, kvec[i][j], p->adjvex))  //求节点p的入度
				{
					num++;
				}
			}
			if (num == kvec_i_Len)
			{
				vector<int> temp(kvec[i]);
				temp.push_back(p->adjvex);
				//if(!set_vec.count(temp))   //此候选项集 不在 已挖掘出的频繁项集之中
				//{
				cank1vec.push_back(temp);
				//}
			}
			p = p->nextarc;
		}
	}
	createFreqItemSet(vi, cank1vec);  //候选项集-->频繁项集（k+1）	

}

void DLGPLUSDemo::saveFrequentItemSet(int vi, vector< vector<int> > kvec)  //合并所有的频繁项集
{
	//cout<<"vi: "<<vi<<endl;

	int i;
	if (vi > ITEM_SIZE)
	{
		cout << "the vi in saveFrequentItemSet ERROR!!!" << endl;
		return;
	}
	for (i = 0; i < kvec.size(); i++)
	{
		set_vecA[vi].insert(kvec[i]);
	}
	
}
bool DLGPLUSDemo::existEdge(int vi, int start, int end)
{
	ArcNode*p = VTable[vi][start].firstarc;
	while (p != NULL)
	{
		if (p->adjvex == end)
			return true;
		p = p->nextarc;
	}
	return false;

}
void DLGPLUSDemo::createFreqItemSet(int vi, vector< vector<int> > kvec)
{
	if (kvec.size() == 0)
	{
		return;
	}
	for (int i = 0; i < kvec.size(); i++)
	{
		vector< vector<int> >::iterator veciter = kvec.begin() + i;
		if (kcandSup(kvec[i])<MIN_SUP)
		{
			kvec.erase(veciter);
			i--;
		}
	}

	if (kvec.size() != 0)
	{
		k_freq_to_k1_cand(vi, kvec);
	}
}
int DLGPLUSDemo::kcandSup(vector<int> vec)  //将候选k+1项集所有项对应的列向量进行"与"操作，求出其支持度
{
	int num = 0;
	vector<int> temp(transactionSize, 1);//temp:初值----	transactionSize: transaction.size()

	int i = 0;
	for (int j = 0; j < transactionSize; j++)
	{
		while (reverseTrans[vec[i] - 1][j])
		{
			i++;
			if (i == vec.size())
			{
				num++;
				break;
			}
		}
		i = 0;

	}
	/*
	for(int i=0;i<vec.size();i++)   //对候选k+1项集进行“与操作”
	{
	for(int j=0;j< transactionSize;j++)
	{
	temp[j]=temp[j]*reverseTrans[vec[i]-1][j];
	}
	}
	for(i=0;i<temp.size();i++)  //计算支持度
	{
	if(temp[i]==1)
	{
	num++;
	}
	}*/
	return num;
}
void DLGPLUSDemo::print_all(string outFileName)
{
	ofstream  outfile = ofstream(outFileName.c_str(), ofstream::trunc);
	if (!outfile)
	{
		cout << "out file error!!" << endl;
		return;
	}
	int count = 0;
	set<vector <int> >::iterator iter_set_vec;
	iter_set_vec = set_vec.begin();

	while (iter_set_vec != set_vec.end())
	{
		// cout<<"频繁"<<(*iter_set_vec).size()<<"项集："<<endl;
		for (int i = 0; i<(*iter_set_vec).size(); i++)
		{
			outfile << (*iter_set_vec)[i] << " ";
		}
		count++;
		outfile << endl;
		iter_set_vec++;
	}
	outfile << "count值：" << count << endl;
	//	cout<<"count值："<<count<<endl;
}
void DLGPLUSDemo::combineALlFrequentItems()
{
	set<vector<int> >::iterator set_iter;
	//	cout<<"set_vec.size(): "<<set_vec.size()<<endl;
	
	for (int i = 0; i < ITEM_SIZE; i++)
	{
		set_iter = set_vecA[i].begin();
		while (set_iter != set_vecA[i].end())
		{
			set_vec.insert(*set_iter++);
		}
	}


}
DWORD WINAPI DLGPLUSDemo::ThreadProc(LPVOID lpParam)
{
	DLGPLUSDemo* pDlgplus = (DLGPLUSDemo*) lpParam;
	DWORD threadId = GetCurrentThreadId();
	int vi = 0;
	for (int index = 1; index <= ITEM_SIZE; index++)
	{
		if (map_subGraphIndex[index] == threadId)
		{
			vi = index;
		}
	}
	/*
	DWORD dwWaitResult = WaitForSingleObject(
	hMutex,	//signaled initially
	INFINITE);
	switch(dwWaitResult)
	{
	case WAIT_OBJECT_0:

	//核心
	pDlgplus->k_freq_to_k1_cand(vi ,pDlgplus->kvec[vi]);

	if(!ReleaseMutex(hMutex))
	{
	cout<<"Release Mutex error: "<<GetLastError()<<endl;
	}
	break;
	default:
	cout<<"wait error: "<<GetLastError()<<endl;
	ExitThread(0);
	}*/

	pDlgplus->k_freq_to_k1_cand(vi, pDlgplus->kvec[vi]);

	return 1;
}
void DLGPLUSDemo::associationRuleMining(int minsup, string inFileName, string outFileName)
{
//	DWORD start = GetTickCount();
#ifdef linux
	timeval start, end;
	gettimeofday(&start, NULL);
#endif
#ifdef _WIN32
	DWORD start = GetTickCount();
#endif

	setMinsup(minsup);

	//input file transaction Records to transaction matrix
	file2transactionMatrix(inFileName.c_str());

	//dlgplus.showTransaction();

	//verticle format
	reverseTransaction();

	//showReverseTrans();

	createGraph();

	//输出全体的频繁2项集构成的关联图
	//printarcs();
	
	//输出全体的频繁2项集
	//printkvec();
	
	//输出各个子关联图
	//printVTable();

	//Multithread
	hMutex = CreateMutex(
		NULL,	//default security attribute
		FALSE,	//not belong to current process
		NULL);	//unnamed
	if (hMutex == NULL)
	{
		cout << "CreateMutex error: " << GetLastError() << endl;
	}

	HANDLE tHandles[ITEM_SIZE + 1];
	HANDLE* THandles = tHandles + 1;
	int i = 1;
	for (; i <= maxThreadNum; i++) // (1)只发起 maxThreadNum 个线程
	{

		tHandles[i] = CreateThread(
			NULL,
			0,
			ThreadProc,
			this,
			0,
			//	&threadIds[i]);
			&map_subGraphIndex[i]); //associate threadId with index
	}

	//(2)剩下的：maxThreadNum + 1至ITEM_SIZE个组成的子关联图则均在主线程中执行
	for (int j = maxThreadNum + 1; j <= ITEM_SIZE; j++)
	{
		this->k_freq_to_k1_cand(j, this->kvec[j]);
	}

	DWORD dwWaitResult = WaitForMultipleObjects(
		maxThreadNum,
		THandles,
		TRUE,
		INFINITE);
	switch (dwWaitResult)
	{
	case WAIT_OBJECT_0:
		break;
	default:
		cout << "\nWait error" << GetLastError() << endl;
	}

	for (i = 1; i <= maxThreadNum; i++)
	{
		CloseHandle(tHandles[i]);
	}

	//combine
//	DWORD before = GetTickCount();
	combineALlFrequentItems();
///	DWORD after = GetTickCount();
//	cout << "combineALlFrequentItems time:" << double(after - before) << endl;
	
	//frequent items to output file
	print_all(outFileName.c_str());

	ofstream  outfile = ofstream(outFileName.c_str(), ofstream::app);
	if (!outfile)
	{
		cout << "out file error!!" << endl;
		return;
	}

//	DWORD end = GetTickCount();
	double totalTime=0.0;
#ifdef linux
	gettimeofday(&end, NULL);
	totalTime = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
#endif
#ifdef _WIN32
	DWORD end = GetTickCount();
	totalTime = (end - start);
#endif
	setExecuteTime(totalTime);
	outfile << "execute time:" << getExecuteTime() << endl;
	cout << "execute time:" << getExecuteTime() << endl;
}
void main()
{
	DLGPLUSDemo dlgplus;

	//set min support
	dlgplus.setMinsup(180);
	string my_inFileName("transction9000.txt");
	////string my_inFileName("test1.txt");
	string my_outFileName("outfile9000.txt");
	string strInc = my_outFileName;
	
	//int count = 5;
	//for(int i = 0; i < count; i++ )
	//{
	//char temp[16];
	//sprintf_s(temp,"%d%s%d",i+1,"-",dlgplus.getMinsup());
	//strInc = temp + strInc;
	////cout<<strInc<<endl;
	//dlgplus.associationRuleMining(dlgplus.getMinsup(),my_inFileName,strInc);

	//dlgplus.initializeALLDataStructures();
	//strInc = my_outFileName;
	//}
	
	dlgplus.associationRuleMining(dlgplus.getMinsup(), my_inFileName, my_outFileName);

}
                                                                                                                                                                                                                                                                                                                                                                                 