struct ArcNode
{
	friend class Graph;
	int adjvex;
	int weight;
	ArcNode*nextarc;
};
struct VNode
{
	friend class Graph;
	int info;
	ArcNode*firstarc;
};