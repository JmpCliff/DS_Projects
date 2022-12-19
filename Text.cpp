//#pragma warning(disable:4996)
//
//#include <iostream>
//#include <iomanip>
//#include <cstdio>
//#include <cstdlib>
//
//#include "Graph.h"
//#include "allDefine.h"
//
//using namespace std;
//
//void Menu();
//void Search(Graph& graph);
//void theShortestWay(Graph& graph);
//void manyWay(Graph& graph);
//void allTheWay(Graph& graph);
//void DP(Graph& graph);
//void Change();
//
//int type = PTYPE;
//int type = CTYPE;
//
//int Xmain()
//{
//    int choice;
//
//    FILE* file = fopen("Data.txt","r");
//    if (file == NULL)
//    {
//        cerr << "error: open failed!" << endl;
//        exit(-1);
//    }
//    Graph graph(file);
//    
//    while (1)
//    {
//        Menu();
//        cin >> choice;
//        system("cls");
//        switch (choice)
//        {
//        case 1:
//            Search(graph);
//            break;
//        case 2:
//            theShortestWay(graph);
//            break;
//        case 3:
//            manyWay(graph);
//            break;
//        case 4:
//            allTheWay(graph);
//            break;
//        case 5:
//            DP(graph);
//            break;
//        case 6:
//            Change();
//            break;
//        case 0:
//            cout << "ByeBye~" << endl;
//            exit(0);
//            break;
//        default:
//            cout << "你他妈给老子好好输入！" << endl;
//        }
//        cout << endl;
//    }
//
//}
//
//void Menu() {
//    cout << "------Graph------" << endl;
//    cout << "1.景点查询" << endl;
//    cout << "2.两点最短路径" << endl;
//    cout << "3.多点游览路线导航" << endl;
//    cout << "4.所有路径" << endl;
//    cout << "5.多点路径游览规划" << endl;
//    cout << "6.调整游览模式" << endl;
//
//    cout << "0.退出" << endl;
//    cout << "请输入你的选择:";
//
//}
//
//void Search(Graph& graph) {
//    int n;
//    cout << "输入景点编号";
//    cin >> n;
//    if (n > graph.getLen_n() || n < 0)
//    {
//        cout << "景点编号不存在" << endl;
//        return;
//    }
//
//    cout << "# 景点信息:" << endl;
//
//    Node* node = graph.getNode(n);
//    cout << "name:" << node->info->name << endl;
//    cout << "discription:" << node->info->description << endl;
//    cout << "Edge_num:" << node->info->Edge_num << endl;
//    cout << "All the Edges:" << endl;
//    Edge* e = graph.getNode(0)->next;
//    if (n == 0) {
//        for (int i = 1; e != NULL; i++, e = e->next)
//        {
//            cout << setw(2) << i << setw(4) << e->ivex << " to " << another(e->ivex, e->jvex, e->ivex) << endl;
//            cout << "distance: " << e->info->distance << endl;
//        }
//    }
//    else {
//        for (int i = 0; e != NULL; e = e->next)
//        {
//            if (n == e->ivex || n == e->jvex) {
//                cout << setw(2) << i++ << setw(4) << n << " to " << another(e->ivex, e->jvex, n) << endl;
//                cout << "distance: " << e->info->distance << endl;
//            }
//        }
//    }
//}
//
//void theShortestWay(Graph& graph) {
//    int s, d;
//
//    cout << "请输入起点：" << endl;
//    cin >> s;
//    if (s > graph.getLen_n() || s < 0)
//    {
//        cout << "景点编号不存在" << endl;
//        return;
//    }
//    cout << "请输入终点：" << endl;
//    cin >> d;
//    if (d > graph.getLen_n() || d < 0)
//    {
//        cout << "景点编号不存在" << endl;
//        return;
//    }
//
//    int distance=graph.Floyed(s,d,type);
//    if (distance == INF)
//    {
//        cout << "无法抵达！" << endl;
//        return;
//    }
//    graph.printPMatrix(s, d, type);
//    cout << "路程长度为 : " << distance << endl;
//}
//
//void manyWay(Graph& graph) {
//
//    graph.Floyed(1,2,type);
//    int* x = (int*)malloc((graph.getLen_n() + 1) * sizeof(int));
//
//    if (x == NULL)
//    {
//        std::cerr << ERROR_MALLOC << std::endl;
//        exit(-1);
//    }
//
//    memset(x, 0, (graph.getLen_n() + 1) * sizeof(int));
//
//    cout << "请输入起点：" << endl;
//    cin >> *x;
//    if (*x > graph.getLen_n() || *x < 0)
//    {
//        cout << "景点编号不存在" << endl;
//        return;
//    }
//    for (int i = 1; *(x + i - 1) != 0; i++)
//    {
//        cout << "请输入第" << i + 1 << "个景点（输入0来结束输入）:" << endl;
//        cin >> x[i];
//        if (x[i] > graph.getLen_n() || x[i] < 0)
//        {
//            cout << "景点编号不存在" << endl;
//            return;
//        }
//    }
//    for (int i = 0; x[i + 1] != 0; i++)
//        if (graph.Floyed(x[i], x[i + 1], type) == INF)
//        {
//            cout << "路径中途存在点无法抵达，请转为步行游览" << endl;
//            return;
//        }
//    for (int i = 0; x[i + 1] != 0; i++)
//        graph.printPMatrix(x[i], x[i + 1], type);
//
//}
//
//void allTheWay(Graph& graph) {
//    int s, d;
//    cout << "请输入起点：" << endl;
//    cin >> s;
//    if (s > graph.getLen_n() || s < 0)
//    {
//        cout << "景点编号不存在" << endl;
//        return;
//    }
//    cout << "请输入终点：" << endl;
//    cin >> d;
//    if (d > graph.getLen_n() || d < 0)
//    {
//        cout << "景点编号不存在" << endl;
//        return;
//    }
//    graph.AllTheWay(s, d, type);
//}
//
//void DP(Graph& graph) {
//    int n;
//    vector<int> v;
//    while (1) {
//        cout << "请输入景点编号（0代表结束输入）:" << endl;
//        cin >> n;
//        if (n > graph.getLen_n() || n < 0)
//        {
//            cout << "景点编号不存在" << endl;
//            return;
//        }
//        if (n == 0)
//            break;
//        v.push_back(n);
//    }
//    graph.SC(v, type);
//}
//
//
//
//void Change() {
//    int choice;
//    cout << "1.步行模式" << endl;
//    cout << "2.车行模式" << endl;
//    cin >> choice;
//    if (choice != 1 && choice!=2)
//    {
//        cout << "请正确输入" << endl;
//        return;
//    }
//    type = choice - 1;
//}