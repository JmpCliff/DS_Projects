#pragma once

#include <cstdio>
#include <vector>
#include <stack>
#include <string>

#include "allDefine.h"

typedef enum DIRECTION{N,NE,E,ES,S,SW,W,NW}DIRECTION;   //�ӱ���Ϊ0��˳ʱ����ת��

typedef struct EdgeInfo {
    int type;           //·������,��ͼʱ��Ҫ��
    int distance;       //���루Ȩֵ��
    DIRECTION direction;//·��������Ϣ����¼����䷽�����·�����Ӵ����ߣ�
    POINT pts[4];       //��ͼʱ���������ߵĿ�������
    POINT ptdis;        //·��������ƽ��ͼ�б�עλ��
    COLORREF color;     //·����ɫ
}EdgeInfo;

typedef struct Edge{
	int mark;           //�������
	int ivex;           //�������i
	Edge* ilink;
	int jvex;           //�������j
	Edge* jlink;
	EdgeInfo* info;     //����Ϣ
    Edge* next;         //�ܱ�����ָ��
}Edge;

typedef struct NodeInfo {
    int num;            //���
    int Edge_num;       //�ڽӱ�����0��ͷ���Ϊ�ܱ���
    int mark;           //�������
    char* name;         //��������
    char* description;  //������
    int Dtop;           //�����ĸ���Ϊͼ�λ���������꣬���γ��ȣ���ɫ�Ȳ���
    int Dleft;
    int Dlen;
    COLORREF color;
}NodeInfo;

typedef struct Node {
    Edge* next;         //�ڽӱ��ͷ
    NodeInfo* info;     //�����Ϣ
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
    //�ߵ����ɺ�����
    void createEdge(FILE* file);    //���ɱ�
    void insertEdge(Edge* e);       //�����ù�������ı������ڽӶ��ر�
    //�ڽӾ�������
    void createArcs(int type);      
    //���Ժ�����������ӡ�ڽӾ����Floyed�㷨��D���P��
    void showM(unsigned int * ptr[], int n, int type);
    //Floyed�㷨������ֵΪsd��̾���
    int Floyed(int s, int d, int type);
    //��Floyed���ɵ�·������Ϊ���ݣ���ӡsd֮������·�������������v��
    void printPMatrix(int s, int d, std::vector<int>& v, int type);     //����printWay����¼�����յ�
    void printWay(int s, int d, std::vector<int>& v, int type);         //�ݹ�Ѱ�����·���漰�������е�
    //DFS���ݷ���������·��
    std::string AllTheWay(int s, int d, int type);                      //����DFS�㷨
    void DFS(std::stack<int>& st,std::string& output, int s, int d, int& way_num, int type);
    //��̬�滮����������̴�������
    void SC(std::vector<int> tour, std::vector<int>& v, int type);  //��̬�滮��������
    void initDP();                                                  //��ʼ����ؾ���
    int DP(int s, int p, int type);                                 //��̬�滮����������ֵΪ��pΪ�յ��s�������·���滮����
    void outPutPath(int s, int p, std::vector<int> &v, int type);   //��ӡ·���滮���
    //��������ڽӵ�
    std::vector<std::string> ADJSdescription(int idx);  

private:
	Node* nodeList;     //�ڵ����Ա�

	int len_n;          //�ܽ����
	int	len_e;          //�ܱ���

    int c_n;            //�г���·�����ľ�����Ŀ
    int c_e;            //����·����

    //Floyed�㷨��������
    unsigned int* arcs[2];           //�����ڽӾ���    
    unsigned int* D_F[2];            //����floyed����
    int* P_F[2];                     //����·��

    //DP���õ��ĸ�������
    //��һ���±�Ϊtype���ڶ���Ϊ���㼯��������Ϊ�յ�
    
    int f[2][1<<MAXCITY][MAXCITY];          //��¼����
    int path[2][1 << MAXCITY][MAXCITY];     //��¼�ִ��յ�ǰ���ڶ����յ�
    
};

