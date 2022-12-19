#pragma warning(disable:4996)

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdio>
#include <stack>

#include "Graph.h"

#include "allDefine.h"

using namespace std;

const char* directionList[8] = { "����","������", "����", "���Ϸ�", "�Ϸ�", "���Ϸ�", "����", "������" };

Graph::Graph() {}

Graph:: Graph(FILE* file) {

    //�ļ���ȡGraph������ͱ���
	int fs_num=fscanf(file, "%d %d", &this->len_n, &this->len_e);				

    //����NodeList�ռ�
	nodeList = (Node *)malloc((len_n + 1) * SIZE_NODE);		

    if (nodeList==NULL)
    {
        std::cerr << ERROR_MALLOC << std::endl;
        exit(-1);
    }

	for (int i = 0; i <= len_n; i++)	//��ʼ��NodeList
	{
		memset(nodeList + i, 0, SIZE_NODE);
        (nodeList + i)->info = new NodeInfo;
        
        if ((nodeList + i)->info == NULL)
        {
            std::cerr << "ERROR: malloc fail!" << std::endl;
            exit(-1);
        }

        (nodeList + i)->next = NULL;            //�ڽӱ��ֵΪ��
		(nodeList + i)->info->num = i;          //���
        (nodeList + i)->info->mark = 0;
        (nodeList + i)->info->Edge_num = 0;     //�ڽӱ߳�ֵΪ0
		(nodeList + i)->info->description = (char*)malloc(0x100);
		(nodeList + i)->info->name = (char*)malloc(0x20);

        if ((nodeList + i)->info->description == 0 || (nodeList + i)->info->name == 0)
        {
            std::cerr << ERROR_MALLOC << std::endl;
            exit(-1);
        }

		//���ļ��ж�ȡ�����Ϣ����źͼ��
        fs_num=fscanf(
			file, 
			"%d %d %d %d %[^ ] %[^\n]",
			&(nodeList + i)->info->num,
            &(nodeList + i)->info->Dtop,
            &(nodeList + i)->info->Dleft,
            &(nodeList + i)->info->Dlen,
            (nodeList + i)->info->name,
            (nodeList + i)->info->description
        );

        (nodeList + i)->info->color = BLACK;

        //std::cout << (nodeList + i)->info->num << ":" << (nodeList + i)->info->description << " " << std::endl;
	}


	for (int i = 0; i < len_e; i++) 
		createEdge(file);

    arcs[0] = NULL;
    arcs[1] = NULL;
    P_F[0] = NULL;
    D_F[0] = NULL;
    P_F[1] = NULL;
    D_F[1] = NULL;

}

Graph::~Graph() {

}

void 
Graph::createEdge(
	FILE * file	)
{
	int ivex, jvex, type, distance, direction;

    int fs_num=fscanf(file,"%d %d %d %d %d",&ivex,&jvex,&type,&distance,&direction);

	Edge *e = new Edge;

	e->mark = 0;
	e->ivex = ivex;
	e->jvex = jvex;
    e->next = e->ilink = e->jlink = NULL;
	e->info = new EdgeInfo;

	e->info->type = type;
	e->info->distance = distance;
    e->info->direction = (DIRECTION)direction;

    for (int i = 0; i < 4; i++)
        fs_num = fscanf(file, "%d %d", &e->info->pts[i].x, &e->info->pts[i].y);
    fs_num = fscanf(file, "%d %d", &e->info->ptdis.x, &e->info->ptdis.y);

    e->info->color = BLACK;

	insertEdge(e);
}

void
Graph::insertEdge(Edge * e)
{
	Node* ivex_node = nodeList + e->ivex;
	Node* jvex_node = nodeList + e->jvex;
	if (ivex_node->next != NULL)
		e->ilink = ivex_node->next;
	ivex_node->next = e;
	ivex_node->info->Edge_num++;

	if (jvex_node->next != NULL)
		e->jlink = jvex_node->next;
	ivex_node->next = e;
	jvex_node->info->Edge_num++;

    if (nodeList->next != NULL)
        e->next = nodeList->next;
    nodeList->next = e;
    nodeList->info->Edge_num++;

}

Node* Graph::getNode(int i) {
    return nodeList + i;
}

//�ڽӾ��������������Ҫʹ��floyed�㷨������ֱ�ӽ��ڽӾ����ָ�뷵�ط���ʹ��
void Graph::createArcs(int type) {
    int n;

    //����ǰ���ɹ����ڽӾ���ֱ�ӽ����������ɣ������ٴ�����
    if (arcs[type] != NULL)
        return;

    //��ʼ���ڽӾ���arcs              
    n = len_n;

    arcs[type] = (unsigned int*)malloc(n * n * sizeof(unsigned int));

    if (arcs[type] == NULL)
    {
        cerr << ERROR_MALLOC << endl;
        exit(-1);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            arcs[type][i * n + j] = INF;

    Edge* e = nodeList->next;
    while (e != NULL)
    {
        if (type == CTYPE && e->info->type == PWAY)
        {
            e = e->next;
            continue;
        }
        arcs[type][(e->ivex - 1) * n + e->jvex - 1] = e->info->distance;
        arcs[type][(e->jvex - 1) * n + e->ivex - 1] = e->info->distance;
        e = e->next;
    }
    for (int i = 0; i < n; i++)
        arcs[type][i * n +i] = 0;

    return;
}

int Graph::Floyed(int s,int d,int type) {
    int n = len_n;

    //��D���Ѿ�ʵ�ּ�����ˣ��Ǿ�ֱ�ӷ��ؼ���
    if (D_F[type] != NULL && P_F[type] != NULL)
    {
        return D_F[type][(s - 1) * n + d - 1];
    }

    //��ʼ��D��
    createArcs(type);       //�������ڽӾ���
    D_F[type] = (unsigned int*)malloc(n * n * sizeof(unsigned int));
    if (D_F[type] == NULL)
    {
        cerr << ERROR_MALLOC << endl;
        exit(-1);
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            D_F[type][i * n + j] = arcs[type][i * n + j];
    //�����д�ӡ��һ��D��
    //showArcs(D_F,n,type);

    //��ʼ��P��
    P_F[type] = (int*)malloc(n * n * sizeof(int));
    if (P_F[type] == NULL)
    {
        cerr << ERROR_MALLOC << endl;
        exit(-1);
    }
    memset(P_F[type], 0, n * n * sizeof(int));
    Edge* e = nodeList->next;
    while (e != NULL)
    {
        P_F[type][(e->ivex - 1) * n + e->jvex - 1] = INF;
        P_F[type][(e->jvex - 1) * n + e->ivex - 1] = INF;
        e = e->next;
    }
    for (int i = 0; i < n; i++)
        P_F[type][i * n + i] = INF;

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                if (D_F[type][i * n + k] + D_F[type][k * n + j] < D_F[type][i * n + j])
                {
                    D_F[type][i * n + j] = D_F[type][i * n + k] + D_F[type][k * n + j];
                    P_F[type][i * n + j] = k;
                }
            }

    //�������
    //showM(arcs, n, type);

    return D_F[type][(s - 1) * n + d - 1];
}

void Graph::showM(unsigned int *ptr[],int n,int type) {
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (ptr[type][i * n + j] == INF)
                cout << setw(5) << "inf";
            else
                cout << setw(5) << ptr[type][i * n + j];
        }
        cout << endl;
    }
    cout << endl;

}

void Graph::printPMatrix(int s, int d, std::vector<int>& v, int type) {
    if (P_F == NULL)
    {
        exit(-1);
        return;
    }

    v.push_back(s);

    printWay(s - 1, d - 1, v, type);
    v.push_back(d);
}

void Graph::printWay(int s, int d, std::vector<int>& v, int type) {
    int n = len_n;
    int k = *(P_F[type] + s * n + d);
    if (k == INF)
        return;
    printWay(s, k, v, type);
    //cout << "-" << k + 1;
    v.push_back(k + 1);
    printWay(k, d, v, type);
}

string Graph::AllTheWay(int s, int d,int type) {
    int way_num = 1;
    string output="";
    stack<int> st;
    DFS(st, output, s, d, way_num, type);
    if (way_num == 1)
        output.append("�޷��ִ��תΪ����\n");
    return output;
}
 
void Graph::DFS(stack<int> &st,string& output, int s, int d, int& way_num,int type) {
    if (s!=d)
    {
        st.push(s);
        Node* ptr = nodeList + s;
        ptr->info->mark = 1;

        for (Edge* e = nodeList->next; e != NULL; e = e->next)
        {
            if (type == CTYPE && e->info->type == PWAY)
                continue;
            if (s == e->ivex || s == e->jvex) {
                if ((nodeList + another(e->ivex, e->jvex, s))->info->mark == 0)
                    DFS(st, output, another(e->ivex, e->jvex, s), d, way_num, type);
            }
        }
        ptr->info->mark = 0;
        st.pop();
    }
    else
    {
        stack<int> temp;
        while (!st.empty())
        {
            temp.push(st.top());
            st.pop();
        }
        //cout<< "·��" << way_num++ << "Ϊ��";
        output.append("·��");
        output += to_string(way_num++);
        output.append("Ϊ��");
        while (!temp.empty())
        {
            st.push(temp.top());
            //cout << temp.top() << " -> ";
            output += (nodeList+temp.top())->info->name;
            output += " -> ";
            temp.pop();
        }
        //cout << s << endl;
        output += (nodeList + s)->info->name;
        output += "\r\n";
    }
}

std::vector<std::string> Graph::ADJSdescription(int idx) {
    vector<string> v;
    for (Edge* e = nodeList->next; e != NULL; e = e->next)
    {
        if (e->ivex != idx && e->jvex != idx)
            continue;
        string s="";
        Node* ptr = nodeList + another(e->ivex, e->jvex, idx);

        DIRECTION d= (idx == e->ivex)?(e->info->direction):(DIRECTION)((e->info->direction+4)%8);

        char buf[0x10];
        itoa(e->info->distance, buf, 10);

        s.append(ptr->info->name);
        s.append(",  ");
        s.append(directionList[d]);
        s.append(",  ");
        s.append(buf);
        s.append("m");
        s.append(",  ");
        s.append(WAYTYPE(e->info->type)==PS_SOLID?"���е�":"���е�");

        v.push_back(s);

    }
    return v;
}