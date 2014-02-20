#include "DLGPLUSDemo.h"
#include <set>
#include <map>
using namespace std;
HANDLE hMutex;
DWORD threadIds[ITEM_SIZE + 1];
map<int, DWORD> map_subGraphIndex;

DLGPLUSDemo::DLGPLUSDemo()
{
	for (int i = 1; i <= ITEM_SIZE; i++)//��ʼ���ڽӾ��� - �ڽӾ���ֻ��Ϊ����ڵ�Ķȷ�������� - Ƶ��2�
	{
		for (int j = 1; j <= ITEM_SIZE; j++)
		{
			arcs[i][j] = 0;
		}
	}

	VTable = new VNode*[ITEM_SIZE + 1];
	for (int vi = 0; vi <= ITEM_SIZE; vi++)
	{

		VTable[vi] = new VNode[ITEM_SIZE + 1];//��ʼ���ڽӱ� - ��������ͼ
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
		cout << "ERROR!!��Ŀ�����࣡" << endl;
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
	
	//��ʼ������Ŀ��
	itemsInit();

	ifstream  input = ifstream(fileName.c_str());
	if (!input) //�ļ���ʧ��!
	{
		cout << "���������ļ�transction�����ڣ������ֹ�����" << endl;

		return;
	}
	else
	{
		while (input)
		{
			string  strFileLine;
			getline(input, strFileLine, '\n'); //�õ�һ�������

			if (strFileLine != "")//�����һ���У������ڴ���if����
			{
				int i = 0;  //�����У���Ŀ���
				int j = 0;  //��Ŀ������Ŀ���
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
	transactionSize = transactionMatrix.size();  //������
}
void DLGPLUSDemo::initializeALLDataStructures()
{
	for (int i = 1; i <= ITEM_SIZE; i++)//����ڽӾ���
	{
		for (int j = 1; j <= ITEM_SIZE; j++)
		{
			arcs[i][j] = 0;
		}
	}

	for (int vi = 0; vi <= ITEM_SIZE; vi++)
	{

		VTable[vi] = new VNode[ITEM_SIZE + 1];//����ڽӱ�
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
void DLGPLUSDemo::reverseTransaction() //�ڽӾ��������: ��Ϊ�ھ������
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
	cout << "���ݿ��е����񼯺ϣ�" << endl;
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
	cout << "ת��������񼯺ϣ�" << endl;
	for (int i = 0; i < reverseTrans.size(); ++i)
	{
		for (int j = 0; j< reverseTrans[i].size(); ++j)
		{
			cout << reverseTrans[i][j] << " ";
		}
		cout << endl;
	}
}

/*ȫ��Ƶ��2����ɵĹ���ͼ - �ڽӾ�����ʽ*/
void DLGPLUSDemo::printarcs()
{
	for (int i = 1; i <= ITEM_SIZE; i++)
	{
		for (int j = 1; j <= ITEM_SIZE; j++)
			cout << arcs[i][j] << " ";
		cout << endl;
	}
}

/*����ͼƵ��2����ɵĹ���ͼ - �ڽӱ���ʽ*/
void DLGPLUSDemo::printVTable()
{
	cout << "�ڽӱ�:" << endl;  //Ƶ��2�
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
		for (int inext = i + 1; inext < reverseTrans.size(); inext++) //�����ڽӾ����൱��ֻ���ǡ������ǡ�����
		{
			int num = 0;
			vector<int> temp;
			for (int k = 0; k < transactionSize; k++) //�����е���Ŀ�������������
			{
				if (reverseTrans[i][k] * reverseTrans[inext][k] == 1) //Vi^Vinext---�߼����롱
					num++;
			}
			if (num >= MIN_SUP)//minsup
			{
				arcs[i + 1][inext + 1] = 1; //�����ڽӾ��󣨰���������Ƶ��2���������Ϣ��------------���ɸ�������ͼ��֤��!
			}

		}
	}

	/*�����Ƿ��ε�˼��*/
	//vector< vector<int> > kvec[ITEM_SIZE+1];
	vector< int > temp_vertex;
	vector< int > temp_subGraph_vertex;
	for (int s_i = 1; s_i <= ITEM_SIZE; s_i++)  //ɨ��ȫ�־���-----������ͼ���ڽӱ����Ƶ��2�
	{
		temp_vertex.push_back(s_i);
		for (int s_j = 1; s_j <= ITEM_SIZE; s_j++)
		{
			if (arcs[s_i][s_j] == 1)   //ֻ������i���ڵĽڵ���--------------��:vertex
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
				if (arcs[k_ii][k_jj] == 1)    //�������----------------------------��:edge
				{

					temp_subGraph_vertex.push_back(k_ii);  //ͬһ��ͼ�� Ƶ��2�
					temp_subGraph_vertex.push_back(k_jj);
					kvec[s_i].push_back(temp_subGraph_vertex);  //����Ƶ��2�ȫ����ȥ
					/*���ɹ�����ͼ: ��ȫ����ͼ��������Ϣ*/
					ArcNode *s = new ArcNode; //����һ�������

					s->nextarc = VTable[s_i][k_ii].firstarc;      //���뵽�ڽӱ���
					s->adjvex = k_jj;


					VTable[s_i][k_ii].firstarc = s;
					VTable[s_i][k_ii].info += 1;
					VTable[s_i][k_jj].info += 1;
					//	cout<<VTable[s_i][k_ii].info<<endl;
					temp_subGraph_vertex.clear();           //�������
				}
			}

		}

		temp_vertex.clear();                               //�������

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
	
	//����Ƶ�� k � �� ��vi��Ƶ�������
	//DWORD before=GetTickCount();
	saveFrequentItemSet(vi, kvec);
	//	DWORD after=GetTickCount();
	//cout<<"saveFrequentItemSet time:"<<double(after - before)<<endl;
	
	//���漰����Ŀ��û���κ�Ƶ���ʱ�����˳���
	if (kvec.size() == 0)return;
	
	int degreeK = kvec[0].size();
	checkDegree(vi, degreeK);  //����2������k�����ĳ���ڵ�Ķ�С��k,��ɾ���ýڵ㼰�����ڵıߡ�

	vector< vector<int> > cank1vec;  //Ƶ��K+1� 
	for (int i = 0; i < kvec.size(); i++)
	{
		int kvec_i_Len = kvec[i].size();
		ArcNode* p = VTable[vi][kvec[i][kvec_i_Len - 1]].firstarc;  //k��չ��k+1
		while (p != NULL)
		{
			int num = 0;
			for (int j = 0; j < kvec[i].size(); j++)  //����3
			{
				if (existEdge(vi, kvec[i][j], p->adjvex))  //��ڵ�p�����
				{
					num++;
				}
			}
			if (num == kvec_i_Len)
			{
				vector<int> temp(kvec[i]);
				temp.push_back(p->adjvex);
				//if(!set_vec.count(temp))   //�˺�ѡ� ���� ���ھ����Ƶ���֮��
				//{
				cank1vec.push_back(temp);
				//}
			}
			p = p->nextarc;
		}
	}
	createFreqItemSet(vi, cank1vec);  //��ѡ�-->Ƶ�����k+1��	

}

void DLGPLUSDemo::saveFrequentItemSet(int vi, vector< vector<int> > kvec)  //�ϲ����е�Ƶ���
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
int DLGPLUSDemo::kcandSup(vector<int> vec)  //����ѡk+1��������Ӧ������������"��"�����������֧�ֶ�
{
	int num = 0;
	vector<int> temp(transactionSize, 1);//temp:��ֵ----	transactionSize: transaction.size()

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
	for(int i=0;i<vec.size();i++)   //�Ժ�ѡk+1����С��������
	{
	for(int j=0;j< transactionSize;j++)
	{
	temp[j]=temp[j]*reverseTrans[vec[i]-1][j];
	}
	}
	for(i=0;i<temp.size();i++)  //����֧�ֶ�
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
		// cout<<"Ƶ��"<<(*iter_set_vec).size()<<"���"<<endl;
		for (int i = 0; i<(*iter_set_vec).size(); i++)
		{
			outfile << (*iter_set_vec)[i] << " ";
		}
		count++;
		outfile << endl;
		iter_set_vec++;
	}
	outfile << "countֵ��" << count << endl;
	//	cout<<"countֵ��"<<count<<endl;
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

	//����
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

	//���ȫ���Ƶ��2����ɵĹ���ͼ
	//printarcs();
	
	//���ȫ���Ƶ��2�
	//printkvec();
	
	//��������ӹ���ͼ
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
	for (; i <= maxThreadNum; i++) // (1)ֻ���� maxThreadNum ���߳�
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

	//(2)ʣ�µģ�maxThreadNum + 1��ITEM_SIZE����ɵ��ӹ���ͼ��������߳���ִ��
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
                                                                                                                                                                                                                                                                                                                                                                                 