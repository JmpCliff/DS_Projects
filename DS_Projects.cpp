// DS_Projects.cpp : 定义应用程序的入口点。
//

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <iostream>
#include "framework.h"
#include "DS_Projects.h"

#include "Graph.h"
#include "allDefine.h"

#define MAX_LOADSTRING 100


//窗口相关全局变量
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//自定义全局变量
Graph graph;                                    //Graph类，图问题处理的核心
int desIdx;                                     //保存当前选取的显示简介的景点标号                               
int status = ST_NULL;                           //标记当前功能模式
int lock = UNLOCK;
int x, y;                                       //调试语句，用于显示鼠标x，y坐标
int TYPE = PTYPE;
std::vector<int> v_input;                       //记录选取的点。
std::vector<int> v_output;                      //记录运算的点集及其顺序


//窗口相关函数声明
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ALLDlg(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    DPDlg(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam);

//自定义数据处理和绘图的函数声明
void    NodePaint(Graph& graph, HDC& hdc);              //绘制景点图标
void    EdgePaint(Graph& graph, HDC& hdc);              //绘制边
void    TYPEPaint(Graph& grahp, HDC& hdc);              //绘制游览状态
void    NodeDes(Graph& graph, HDC& hdc);                //打印结点简介
int     moveJudgeMousePlace(Graph& graph, int x, int y, HWND &hWnd);    //鼠标拖动时，判断是否在结点上，如果是的话，视情况将其选中
int     clickJudgeMousePlace(Graph& graph, int x, int y, HWND& hWnd);   //鼠标左击时，判断是否在结点上，如果是的话，视情况将其选中
WCHAR*  MtoW(char* ptr);                    //字符串映射成宽字符串
void    PaintXY(HDC& hdc);                  //打印鼠标坐标，调试语句
void    clearStatus(Graph& graph);          //清除状态
void    Shortest(int idx, HWND& hWnd);      //最短路径处理函数
void    AllTheWay(int idx, HWND& hWnd);     //所有路径处理函数
void    DP(HWND& hWnd);                     //路径规划处理函数

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    //设置Graph对象
    FILE* file = fopen("Data.txt", "r");
    if (file == NULL)
        exit(-1);
    graph=Graph(file);


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DSPROJECTS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DSPROJECTS));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DSPROJECTS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DSPROJECTS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^ WS_THICKFRAME,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            //菜单ID保存在wParam参数的低位
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            //版本信息，这个是VS自动生成的
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            //最短路径功能：
            case IDM_SHORTEST:
                clearStatus(graph);
                status = ST_SHORTEST;   //设置状态为最最短路径模式
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_ALL:
                clearStatus(graph);
                status = ST_ALL;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_DP:
                clearStatus(graph);
                status = ST_DP;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_CHANGE:
                clearStatus(graph);
                TYPE = another(PTYPE, CTYPE, TYPE);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

            //绘制分割线
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            SelectObject(hdc, pen);
            MoveToEx(hdc, 900, 50, NULL);
            LineTo(hdc, 900, 650);
            DeleteObject(pen);

            //绘制景点图标、路径以及游览方式
            NodePaint(graph, hdc);
            EdgePaint(graph, hdc);
            TYPEPaint(graph, hdc);

            //如果选中了某个景点，则在窗口中打印其简介
            if (desIdx > 0)
                NodeDes(graph, hdc);

            //调试函数
            //PaintXY(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
        
    //鼠标移动消息——对应功功能为对应景点图标变色和介绍信息展示。
    case WM_MOUSEMOVE:
        {  
            POINT pt;
            pt.x = LOWORD(lParam);      //低字节为x
            pt.y = HIWORD(lParam);      //高字节为y

            if (moveJudgeMousePlace(graph, pt.x, pt.y, hWnd) != 0)
                InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    //鼠标左击事件——对应菜单各项功能
    case WM_LBUTTONDOWN:
        {
        
        //点击
        POINT pt;
        x = pt.x = LOWORD(lParam);      //低字节为x
        y = pt.y = HIWORD(lParam);      //高字节为y

        //根据当前功能去响应对应事件
        switch (status)                 
            {

            //查询最短路径模式下的点击事件
            case ST_SHORTEST:       
                {
                    //点击了就先变红
                    int idx = clickJudgeMousePlace(graph, pt.x, pt.y, hWnd);
                    if (idx != 0)
                        InvalidateRect(hWnd, NULL, TRUE);
                    else
                        break;

                    Shortest(idx, hWnd);
                    break;
                }
            
            //计算所有路径模式下的点击事件
            case ST_ALL:
                {
                    //点击了就先变红
                    int idx = clickJudgeMousePlace(graph, pt.x, pt.y, hWnd);
                    if (idx != 0)
                        InvalidateRect(hWnd, NULL, TRUE);
                    else
                        break;

                    AllTheWay(idx, hWnd);
                    break;
                }
            //路径规划模式下的点击，仅仅起一个选取作用
            case ST_DP:
                {
                    //点击了就先变红
                    int idx = clickJudgeMousePlace(graph, pt.x, pt.y, hWnd);
                    if (idx != 0)
                        InvalidateRect(hWnd, NULL, TRUE);
                    else
                        break;
                    for (int i : v_input)
                        if (i == idx)
                            break;
                    v_input.push_back(idx);         //路径规划应该由回车键触发
                    break;              
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    clearStatus(graph);
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case VK_RETURN:
                    DP(hWnd);
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//路径规划处理函数
void DP(HWND& hWnd) {
    graph.SC(v_input, v_output, TYPE);
    if (v_output.size() == 0)
    {
        clearStatus(graph);
        MessageBox(hWnd, L"存在不可抵达的景点，请切换为步行！", L"提示", MB_OK | MB_ICONINFORMATION);
        return; 
    }
    std::vector<int> temp;
    std::string DPoutput="游览路线如下：\r\n";
    for (int i = 0; i < v_output.size() - 1; i++)
    {
        DPoutput += "路线" + std::to_string(i + 1) + ". 从 " + (graph.getNode(v_output[i]))->info->name + " 到 " + (graph.getNode(v_output[i + 1]))->info->name + ":\r\n";

        graph.printPMatrix(v_output[i], v_output[i+1], temp, TYPE);

        for (int i : temp)
        {
            graph.getNode(i)->info->color = RED;
            if (i != temp.back())
            {
                DPoutput.append(graph.getNode(i)->info->name);
                DPoutput.append(" -> ");
            }
            else
            {
                DPoutput.append(graph.getNode(i)->info->name);
                DPoutput.append("\r\n");
            }
        }
        for (int i = 0; i < temp.size() - 1; i++)
        {
            for (Edge* e = graph.getEdge(); e != NULL; e = e->next)
            {
                if (e->ivex == temp[i] && e->jvex == temp[i + 1] ||
                    e->ivex == temp[i + 1] && e->jvex == temp[i])
                {
                    e->info->color = RED;
                    break;
                }
            }
        }
        temp.clear();
    }


    lock = LOCK;
    HWND hdlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DP), hWnd, DPDlg);
    if (hdlg)
        ShowWindow(hdlg, SW_NORMAL);

    HWND hTEXTDP = GetDlgItem(hdlg, IDC_DP);
    WCHAR* wcptr = MtoW((char*)DPoutput.c_str());
    SetWindowText(hTEXTDP, wcptr);
    delete wcptr;


}


//所有路径处理函数
void AllTheWay(int idx,HWND& hWnd) {
    if (lock == LOCK)
        return;
    if (v_input.size() < 2)
        v_input.push_back(idx);
    if (v_input.size() == 2)
    {
        int distance = graph.Floyed(v_input[0], v_input[1], TYPE);
        if (distance == INF)
        {
            clearStatus(graph);
            MessageBox(hWnd, L"存在不可抵达的景点，请切换为步行！", L"提示", MB_OK | MB_ICONINFORMATION);
            return;
        }

        std::string ALLoutput = graph.AllTheWay(v_input[0],v_input[1],TYPE);
        
        lock = LOCK;
        HWND hdlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ALL), hWnd, ALLDlg);
        if (hdlg)
            ShowWindow(hdlg, SW_NORMAL);
        
        HWND hTEXTALL = GetDlgItem(hdlg, IDC_ALL);
        WCHAR* wcptr = MtoW((char*)ALLoutput.c_str());
        SetWindowText(hTEXTALL, wcptr);
        delete wcptr;
    }
}

//最短路径处理函数
void Shortest(int idx,HWND& hWnd) {
    if (v_input.size() < 2)
        v_input.push_back(idx);
    if (v_input.size() == 2)
    {
        int distance = graph.Floyed(v_input[0], v_input[1], TYPE);
        if (distance == INF)
        {
            clearStatus(graph);
            MessageBox(hWnd, L"存在不可抵达的景点，请切换为步行！", L"提示", MB_OK | MB_ICONINFORMATION);
            return;
        }
        graph.printPMatrix(v_input[0], v_input[1], v_output, TYPE);
        
        //将所有涉及到的景点和路径标红
        for (int i : v_output)
        {
            graph.getNode(i)->info->color = RED;
        }
        for (int i = 0; i < v_output.size() - 1; i++)
        {
            for (Edge* e = graph.getEdge(); e != NULL; e = e->next)
            {
                if (e->ivex == v_output[i] && e->jvex == v_output[i + 1] ||
                    e->ivex == v_output[i + 1] && e->jvex == v_output[i])
                {
                    e->info->color = RED;
                    break;
                }
            }
        }
        lock = LOCK;
    }
}

//清除状态，变为自由查询模式
void clearStatus(Graph& graph ) {
    status = ST_NULL;
    lock = UNLOCK;
    desIdx = 0;
    v_input.clear();
    v_output.clear();
    for (int i = 1; i <= graph.getLen_n(); i++)
        graph.getNode(i)->info->color = BLACK;
    for (Edge* e = graph.getEdge(); e != NULL; e = e->next)
        e->info->color = BLACK;
}

//鼠标点击时判断鼠标位置，会返回点击的景点编号
int clickJudgeMousePlace(Graph& graph, int x, int y, HWND& hWnd) {
    RECT rect;
    int height = 15;
    if (lock == LOCK)
        return 0;
    for (int i = 1; i <= graph.getLen_n(); i++)
    {
        //获取结点指针
        Node* ptr = graph.getNode(i);
        //生成矩形区域
        rect.top = ptr->info->Dtop - 5;
        rect.bottom = rect.top + height + 5;
        rect.left = ptr->info->Dleft - 5;
        rect.right = rect.left + height * ptr->info->Dlen + 2;

        if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
        {
            ptr->info->color = RED;
            return i;
        }
    }

    return 0;
}

//鼠标拖动时判断鼠标位置，会更改全局变量desIdx(当前指向的景点)
int moveJudgeMousePlace(Graph& graph ,int x,int y,HWND& hwnd) {
    RECT rect;
    int height = 15;
    desIdx = 0;
    for (int i = 1; i <= graph.getLen_n(); i++)
    {
        //获取结点指针
        Node* ptr = graph.getNode(i);
        //生成矩形区域
        rect.top = ptr->info->Dtop - 5;
        rect.bottom = rect.top + height + 5;
        rect.left = ptr->info->Dleft - 5;
        rect.right = rect.left + height * ptr->info->Dlen + 2;

        if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
        {
            if (ptr->info->color != RED)
                ptr->info->color = GREEN;
            desIdx = i;
            return i;
        }
        else
        {
            if (ptr->info->color == GREEN)
            {
                desIdx = -1;
                ptr->info->color = BLACK;
            }
        }
    }

    return desIdx;
}

void TYPEPaint(Graph& grahp, HDC& hdc) {
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    RECT rect;

    rect.top = 600;
    rect.bottom = rect.top + tm.tmHeight;
    rect.left = 700;
    rect.right = rect.left + 5 * tm.tmHeight;

    WCHAR* wcptr = MtoW((char*)(TYPE==PTYPE?"状态：步行":"状态：乘车"));
    SetTextColor(hdc, BLACK);
    DrawText(hdc, wcptr, -1, &rect, 0);

    delete wcptr;

}

void NodePaint(Graph& graph, HDC& hdc) {
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    RECT rect;

    for (int i = 1; i <= graph.getLen_n(); i++)
    {
        //获取结点指针        
        Node* ptr = graph.getNode(i);

        //矩形绘制区域初始化
        int height = 15;
        rect.top = ptr->info->Dtop;
        rect.bottom = rect.top + height;
        rect.left = ptr->info->Dleft;
        rect.right = rect.left + height * ptr->info->Dlen;

        //将字符串映射到宽字符串
        WCHAR * wcptr=MtoW(ptr->info->name);

        SetTextColor(hdc, ptr->info->color);
        DrawText(hdc, wcptr, -1, &rect, 0);
        
        delete wcptr;

        //矩形框绘制
        HPEN pen = CreatePen(PS_SOLID, 1, ptr->info->color);
        SelectObject(hdc, pen);

        MoveToEx(hdc, rect.left - 5, rect.top - 5, NULL);
        LineTo(hdc, rect.right + 2, rect.top - 5);
        LineTo(hdc, rect.right + 2, rect.bottom + 5);
        LineTo(hdc, rect.left - 5, rect.bottom + 5);
        LineTo(hdc, rect.left - 5, rect.top - 5);

        DeleteObject(pen);

    }
}

void EdgePaint(Graph& graph, HDC& hdc) {
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);

    for (Edge* e = graph.getEdge(); e != NULL; e = e->next)
    {
        HPEN pen = CreatePen(WAYTYPE(e->info->type), 1, e->info->color);
        SelectObject(hdc, pen);
        PolyBezier(hdc, e->info->pts, 4);
        DeleteObject(pen);

        RECT rect;
        char temp[0x10];
        WCHAR* wcptr;
        itoa(e->info->distance, temp, 10);
        wcptr = MtoW(temp);
        rect.top = e->info->ptdis.y;
        rect.bottom = rect.top + tm.tmHeight;
        rect.left = e->info->ptdis.x;
        rect.right = rect.left + (LONG)wcslen(wcptr) * tm.tmHeight;
        SetTextColor(hdc, e->info->color);
        DrawText(hdc, wcptr, -1, &rect, 0);
        delete wcptr;

    }
}

void NodeDes(Graph& graph, HDC& hdc) {
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    RECT rect;
    int height = 15;

    WCHAR* wcptr;
    Node* ptr = graph.getNode(desIdx);

    SetTextColor(hdc, BLACK);

    rect.top = 100;
    rect.bottom = rect.top + tm.tmHeight;
    rect.left = 1100;
    rect.right = rect.left + tm.tmHeight * ptr->info->Dlen;
    wcptr = MtoW(ptr->info->name);
    DrawText(hdc, wcptr, -1, &rect, 0);
    delete wcptr;
    wcptr = NULL;

    rect.top = 150;
    rect.bottom = rect.top + tm.tmHeight;
    rect.left = 925;
    rect.right = rect.left + tm.tmHeight * 30;
    wcptr = MtoW(ptr->info->description);
    DrawText(hdc, wcptr, -1, &rect, DT_NOCLIP|DT_WORDBREAK);
    delete wcptr;
    wcptr = NULL;

    std::vector<std::string> v=graph.ADJSdescription(desIdx);
    for (int i = 0; i < v.size(); i++)
    {
        rect.top = 250 + (i * 2 + 1) * tm.tmHeight;
        rect.bottom = rect.top + tm.tmHeight;
        rect.left = 925;
        rect.right = rect.left + tm.tmHeight * 30;
        wcptr = MtoW((char *)v[i].c_str());
        DrawText(hdc, wcptr, -1, &rect, 0);
        delete wcptr;
        wcptr = NULL;
    }

}

WCHAR* MtoW(char * ptr) {
    WCHAR* wcptr;
    int len = MultiByteToWideChar(CP_ACP, NULL,
        ptr,
        -1,
        NULL,
        0);
    wcptr = new WCHAR[len];
    MultiByteToWideChar(CP_ACP, NULL,
        ptr,
        -1,
        wcptr,
        len);
    return wcptr;
}

void PaintXY(HDC& hdc) {
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    RECT rect;

    WCHAR* wcptr;
    char temp[0x10];

    rect.top = 500;
    rect.bottom = rect.top + tm.tmHeight;
    rect.left = 1100;
    rect.right = rect.left + tm.tmHeight *5;

    SetTextColor(hdc, BLACK);

    itoa(x, temp, 10);
    wcptr = MtoW(temp);
    DrawText(hdc, wcptr, -1, &rect, 0);
    delete wcptr;
    wcptr = NULL;

    rect.top = 500+tm.tmHeight;
    rect.bottom = rect.top + tm.tmHeight;
    rect.left = 1100;
    rect.right = rect.left + tm.tmHeight * 5;
    itoa(y, temp, 10);
    wcptr = MtoW(temp);
    DrawText(hdc, wcptr, -1, &rect, 0);
    delete wcptr;
    wcptr = NULL;
}

//处理路径规划计算结果框
INT_PTR CALLBACK DPDlg(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        break;
    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE)
        {
            // 如果执行了关闭
            // 销毁对话框，将收到WM_DESTROY消息
            lock = UNLOCK;
            clearStatus(graph);
            DestroyWindow(hdlg);
        }
        break;

    }
    return (INT_PTR)FALSE;
}

//处理所有路径计算结果框
INT_PTR CALLBACK ALLDlg(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        break;
    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE)
        {
            // 如果执行了关闭
            // 销毁对话框，将收到WM_DESTROY消息
            lock = UNLOCK;
            clearStatus(graph);
            DestroyWindow(hdlg);
        }
        break;
    
    }
    return (INT_PTR)FALSE;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
