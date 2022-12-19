#pragma once

#include <cstdio>
#include <vector>
#include <stack>
#include <string>

#include "allDefine.h"

typedef enum DIRECTION{N,NE,E,ES,S,SW,W,NW}DIRECTION;   //从北方为0，顺时针旋转。

typedef struct EdgeInfo {
    int type;           //路径类型,绘图时需要用
    int distance;       //距离（权值）
    DIRECTION direction;//路径方向信息（记录两点间方向而非路径连接处切线）
    POINT pts[4];       //绘图时贝塞尔曲线的控制坐标
    POINT ptdis;        //路径距离在平面图中标注位置
    COLORREF color;     //路径颜色
}EdgeInfo;

typedef struct Edge{
	int mark;           //搜索标记
	int ivex;           //依附结点i
	Edge* ilink;
	int jvex;           //依附结点j
	Edge* jlink;
	EdgeInfo* info;     //边信息
    Edge* next;         //总边链表指针
}Edge;

typedef struct NodeInfo {
    int num;            //编号
    int Edge_num;       //邻接边数，0号头结点为总边数
    int mark;           //搜索标记
    char* name;         //景点名称
    char* description;  //景点简介
    int Dtop;           //以下四个域为图形化界面的坐标，矩形长度，颜色等参数
    int Dleft;
    int Dlen;
    COLORREF color;
}NodeInfo;

typedef struct Node {
    Edge* next;         //邻接表表头
    NodeInfo* info;     //结点信息
}Head;

class Graph {
public:
	Graph(FILE* file);
    Graph();
    ~Graph();
    Node* getNode(int i);
    int getLen_n() { return len_n; }
    int getLen_e() { return len_e; }
    Edge* getEdge() { return nodeList->next; }
    //边的生成和连接
    void createEdge(FILE* file);    //生成边
    void insertEdge(Edge* e);       //将设置过依附点的边连入邻接多重表
    //邻接矩阵生成
    void createArcs(int type);      
    //调试函数，用来打印邻接矩阵和Floyed算法的D阵和P阵
    void showM(unsigned int * ptr[], int n, int type);
    //Floyed算法，返回值为sd最短距离
    int Floyed(int s, int d, int type);
    //以Floyed生成的路径矩阵为依据，打印sd之间的最短路径，将结果存在v中
    void printPMatrix(int s, int d, std::vector<int>& v, int type);     //调用printWay并记录起点和终点
    void printWay(int s, int d, std::vector<int>& v, int type);         //递归寻找最短路径涉及到的所有点
    //DFS回溯法查找所有路径
    std::string AllTheWay(int s, int d, int type);                      //调用DFS算法
    void DFS(std::stack<int>& st,std::string& output, int s, int d, int& way_num, int type);
    //动态规划，解决多点最短串联问题
    void SC(std::vector<int> tour, std::vector<int>& v, int type);  //动态规划启动函数
    void initDP();                                                  //初始化相关矩阵
    int DP(int s, int p, int type);                                 //动态规划函数，返回值为以p为终点的s集的最短路径规划长度
    void outPutPath(int s, int p, std::vector<int> &v, int type);   //打印路径规划结果
    //获得所有邻接点
    std::vector<std::string> ADJSdescription(int idx);  

private:
	Node* nodeList;     //节点线性表

	int len_n;          //总结点数
	int	len_e;          //总边数

    int c_n;            //有车行路相连的景点数目
    int c_e;            //车行路边数

    //Floyed算法辅助数组
    unsigned int* arcs[2];           //保存邻接矩阵    
    unsigned int* D_F[2];            //保存floyed距离
    int* P_F[2];                     //保存路径

    //DP中用到的辅助数组
    //第一个下标为type，第二个为景点集，第三个为终点
    
    int f[2][1<<MAXCITY][MAXCITY];          //记录距离
    int path[2][1 << MAXCITY][MAXCITY];     //记录抵达终点前倒第二个终点
    
};

