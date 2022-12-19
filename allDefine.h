#pragma once

#include "framework.h"

#define SIZE_EDGE sizeof(Edge)
#define SIZE_EDGEINFO sizeof(EdgeInfo)
#define SIZE_NODE sizeof(Node)
#define SIZE_NODEINFO sizeof(NodeInfo)

#define ERROR_MALLOC "ERROR: malloc fail!"

#define INF 0x3f3f3f3f

#define CWAY 1          //���е���־
#define PWAY 0          //���е���־

#define CTYPE 1         //��������ģʽ
#define PTYPE 0         //��������ģʽ

#define MAXCITY 11      //��̬�滮����Ĵ�С����Χ�Դ��ڽ����

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