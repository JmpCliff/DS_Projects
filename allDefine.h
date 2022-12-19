#pragma once

#include "framework.h"

#define SIZE_EDGE sizeof(Edge)
#define SIZE_EDGEINFO sizeof(EdgeInfo)
#define SIZE_NODE sizeof(Node)
#define SIZE_NODEINFO sizeof(NodeInfo)

#define ERROR_MALLOC "ERROR: malloc fail!"

#define INF 0x3f3f3f3f

#define CWAY 1          //车行道标志
#define PWAY 0          //人行道标志

#define CTYPE 1         //车行游览模式
#define PTYPE 0         //步行游览模式

#define MAXCITY 11      //动态规划数组的大小，范围略大于结点数

#define another(i,j,n) ((i==n)?(j):(i))

#define GREEN RGB(0,255,0)
#define RED   RGB(255,0,0)
#define BLUE  RGB(0,0,255)
#define BLACK RGB(0,0,0)

#define WAYTYPE(x) ((x)==1?(PS_SOLID):(PS_DOT))

#define ST_NULL		0
#define ST_SHORTEST 1
#define ST_ALL		2
#define ST_DP		4

#define LOCK		1
#define UNLOCK		0