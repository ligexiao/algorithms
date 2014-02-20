#include<iostream>
using namespace std;
#include<queue>
#include<set>
#include "DLGPLUSNode.h"
#define ITEM_SIZE 20
#define maxThreadNum 8
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#ifdef linux
#include <sys/time.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

class DLGPLUSDemo
{
public:
	DLGPLUSDemo();
	~DLGPLUSDemo();
	void printGraph();
	void printarcs();
	void printkvec();
	void printVTable();
	void createTransaction();
	void showReverseTrans();
	void reverseTransaction();
	void showTransaction();
	void createGraph();
	void saveFrequentItemSet(int vi, vector< vector<int> > kvec);
	void createFreqItemSet(int vi, vector< vector<int> > kvec);
	int  kcandSup(vector<int> vec);
	void checkDegree(int vi, int degree);
	bool existEdge(int vi, int start, int end);

	void print_all(string outFileName);

	static DWORD WINAPI ThreadProc(LPVOID lpParam);

	void file2transactionMatrix(string inFileName);

	void setExecuteTime(double executeTime)
	{
		this->executeTime = executeTime;
	}
	double getExecuteTime(){ return executeTime; }

	void setMinsup(int minsup){ MIN_SUP = minsup; }
	int getMinsup(){ return MIN_SUP; }

	void associationRuleMining(int minsup, string inFileName, string outFileName);

	void initializeALLDataStructures();

	void combineALlFrequentItems();

	void itemsInit();

private:
	VNode** VTable;
	vector<vector<int> > transactionMatrix;
	vector<vector<int> > reverseTrans;
	int arcs[ITEM_SIZE + 1][ITEM_SIZE + 1];
	vector< vector<int> > kvec[ITEM_SIZE + 1];
	set<vector<int> > set_vec;
	set<vector<int> > set_vecA[ITEM_SIZE];

	int transactionSize;
	int MIN_SUP;
	double executeTime;
	void k_freq_to_k1_cand(int vi, vector< vector<int> > kvec);
	//bool *visited;
	char items[ITEM_SIZE];
};