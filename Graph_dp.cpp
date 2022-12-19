#pragma warning(disable:4996)

#include <iostream>
#include <vector>

#include "Graph.h"

#include "allDefine.h"

using namespace std;

void Graph::initDP() {
    memset(f, -1, sizeof(f));
    memset(path, 0, sizeof(path));
    for (int i = 1; i <= len_n; i++)
    {
        f[0][1 << (i - 1)][i] = 0;
        f[1][1 << (i - 1)][i] = 0;
    }
}

void Graph::SC(std::vector<int> tour,std::vector<int>& v,int type) {
    //cout << "开始规划..." << endl;

    initDP();

    if (D_F[type] == NULL)
        Floyed(1, 2, type);

    //压缩城市编号至一个状态位
    int allCity=0;

    for (int i : tour)
        allCity |= (1 << (i - 1));

    int bestS = -1, bestP = -1;
    
    for (int s = 0; s < (1 << len_n); s++)
    {
        if ((s & allCity) != allCity)
            continue;
        for (int i : tour)
        {
            if (bestS == -1 && bestP == -1)
            {
                bestS = s;
                bestP = i;
            }
            else if (DP(bestS, bestP, type) > DP(s, i, type))
            {
                bestS = s;
                bestP = i;
            }
        }
    }
    
    if (DP(bestS, bestP, type) >= INF)
        return ;

    outPutPath(bestS, bestP, v, type);

    return ;
}

int Graph::DP(int s, int p, int type) {
    if (f[type][s][p] != -1)
        return f[type][s][p];
    f[type][s][p] = INF;

    int pre = s ^ (1 << (p - 1));

    for (int i=1;i<=len_n;i++)
    {
        if (pre >> (i - 1) & 1)
        {
            int dis = DP(pre, i, type) + D_F[type][(i - 1) * len_n + p - 1];
            if (dis < f[type][s][p])
            {
                f[type][s][p] = dis;
                path[type][s][p] = i;
            }
        }
    }
    return f[type][s][p];
}

void Graph::outPutPath(int s, int p, std::vector<int> &v, int type) {
    if (p == 0)
        return;
    outPutPath(s ^ (1 << (p - 1)), path[type][s][p], v, type);
    v.push_back(p);

}