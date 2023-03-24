// 2048.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "2048.h"

#define MAX_LOADSTRING 100
using namespace std;

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

int     iStep;
int     iScore;
int     iCmd;

HWND    hDlg;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    iCmd = nCmdShow;

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);
    lstrcpy(szWindowClass, TEXT("2048"));
    lstrcpy(szTitle, szWindowClass);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        //if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        //if(hDlg == 0 || !IsDialogMessage(hDlg, &msg))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    /*WNDCLASSEX wcex2;

    wcex2.cbSize = sizeof(WNDCLASSEX);

    wcex2.style = CS_HREDRAW | CS_VREDRAW;
    wcex2.lpfnWndProc = WndProc2;
    wcex2.cbClsExtra = 0;
    wcex2.cbWndExtra = 0;
    wcex2.hInstance = hInstance;
    wcex2.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048));
    wcex2.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex2.lpszMenuName = nullptr;
    wcex2.lpszClassName = szWindowClass;
    wcex2.hIconSm = LoadIcon(wcex2.hInstance, MAKEINTRESOURCE(IDI_SMALL));*/
    

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

   int xWidth;
   int yHeight;
   yHeight = min(620, GetSystemMetrics(SM_CYSCREEN) * 0.8);
   xWidth = yHeight * 0.75;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       GetSystemMetrics(SM_CXSCREEN) / 2 - xWidth / 2 , GetSystemMetrics(SM_CYSCREEN) * 0.5 - yHeight * 0.5,
       xWidth, yHeight, nullptr, nullptr, hInstance, nullptr);

   /*HWND hWnd2 = CreateWindowW(szWindowClass, TEXT("第二个窗口"), WS_OVERLAPPEDWINDOW,
       GetSystemMetrics(SM_CXSCREEN) / 2 - xWidth / 2, GetSystemMetrics(SM_CYSCREEN) * 0.5 - yHeight * 0.5,
       xWidth, yHeight, nullptr, nullptr, hInstance, nullptr);*/

   if (!hWnd)
   {
      return FALSE;
   }

   hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   /*ShowWindow(hWnd2, nCmdShow);
   UpdateWindow(hWnd2);*/

   return TRUE;
}

void SetMyRect(POINT apt[], int x, int y, int iWidth, int iHeight)
{
    apt[0].x = x;
    apt[0].y = y;
    apt[1].x = apt[0].x + iWidth;
    apt[1].y = apt[0].y;
    apt[2].x = apt[1].x;
    apt[2].y = apt[1].y + iHeight;
    apt[3].x = apt[0].x;
    apt[3].y = apt[0].y + iHeight;
}

void SetFrame(POINT ptStart, RECT* rect,int iSquareWidth, int iDimen, int iFrameWidth)
{
    rect->left = ptStart.x;
    rect->right = rect->left + iDimen * iSquareWidth + (iDimen + 1)*iFrameWidth;
    rect->top = ptStart.y;
    rect->bottom = rect->top + iDimen * iSquareWidth + (iDimen + 1)*iFrameWidth;
}

COLORREF GetSquareColor(UINT uType)
{
    switch (uType)
    {
    case 0:
        return RGB(206, 194, 180);
    case 2:
        return RGB(238, 228, 216);
    case 4:
        return RGB(234, 224, 199);
    case 8:
        return RGB(242, 177, 119);
    case 16:
        return RGB(245, 151, 97);
    case 32:
        return RGB(248, 123, 95);
    case 64:
        return RGB(243, 95, 59);
    case 128:
        return RGB(234, 209, 109);
    case 256:
        return RGB(235, 205, 95);
    case 512:
        return RGB(235, 200, 80);
    case 1024:
        return RGB(236, 196, 62);
    case 2048:
        return RGB(234, 193, 39);
    case 4096:
        return RGB(239, 103, 105);
    case 8192:
        return RGB(233, 80, 85);
    case 16384:
        return RGB(246, 63, 65);
    case 32768:
        return RGB(109, 180, 208);
    case 65536:
        return RGB(97, 158, 221);
    case 131072:
        return RGB(24, 130, 204);
    default:
        return RGB(0, 0, 0);
    }
}

COLORREF GetSquareTextColor(int iNum)
{
    if (iNum <= 4)
        return RGB(115, 104, 84);
    else
        return RGB(255, 255, 255);
}

int GetSquareTextSize(int iNum,int iSquareWidth)
{
    switch (iNum)
    {
    case 0:
        return 0;
    case 2:
    case 4:
    case 8:
        return iSquareWidth * 0.75;
    case 16:
    case 32:
    case 64:
        return iSquareWidth * 0.75 * 0.85;
    case 128:
    case 256:
    case 512:
        return iSquareWidth * 0.75 * 0.85 * 0.85;
    case 1024:
    case 2048:
    case 4096:
    case 8192:
        return iSquareWidth * 0.75 * 0.85 * 0.85 * 0.85;
    case 16384:
    case 32768:
    case 65536:
        return iSquareWidth * 0.75 * 0.85 * 0.85 * 0.85 * 0.85;
    case 131072:
        return iSquareWidth * 0.75 * 0.85 * 0.85 * 0.85 * 0.85 * 0.85;
    default:
        return 0;
    }
}

#define GetSquareStart(iNum,x,y,iFrameWidth,iSquareWidth,iDimen) {(iNum%iDimen)*(iSquareWidth+iFrameWidth)+x+iFrameWidth,(iNum/iDimen)*(iSquareWidth+iFrameWidth)+y+iFrameWidth}

void DrawSquareText(HDC hdc, int iVal, int x, int y, int iSquareWidth)
{
    LOGFONTW    lf = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    HFONT       hf;
    TEXTMETRIC  tm;
    POINT       ptStart;
    int         iLength;
    TCHAR       szBuffer[10];

    if (iVal == 0)
        return;
    else if (iVal < 10)
        iLength = 1;
    else if (iVal < 100)
        iLength = 2;
    else if (iVal < 1000)
        iLength = 3;
    else if (iVal < 10000)
        iLength = 4;
    else if (iVal < 100000)
        iLength = 5;
    else
        iLength = 6;

    lstrcpyW(lf.lfFaceName, TEXT("Arial"));
    lf.lfHeight = GetSquareTextSize(iVal, iSquareWidth);
    hf = CreateFontIndirectW(&lf);
    SelectObject(hdc, hf);
    SetTextColor(hdc, GetSquareTextColor(iVal));
    SetBkMode(hdc, TRANSPARENT);
    GetTextMetrics(hdc, &tm);
    /*ptStart.x = x + iSquareWidth / 2 - lf.lfHeight / 2;
    ptStart.y = y + iSquareWidth / 2 - tm.tmAveCharWidth * iLength / 2;*/
    SetTextAlign(hdc, TA_CENTER | VTA_CENTER);
    TextOut(hdc, x + iSquareWidth / 2, y + iSquareWidth / 4, szBuffer, wsprintf(szBuffer, TEXT("%d"), iVal));
    DeleteObject(hf);
}

void DrawSmallSquare(HDC hdc, int iLoc, int iVal,RECT* rectLargest, int iFrameWidth, int iSquareWidth, int iDimen)
{
    HBRUSH hBrush = CreateSolidBrush(GetSquareColor(iVal));
    POINT ptDest = GetSquareStart(iLoc, rectLargest->left, rectLargest->top, iFrameWidth, iSquareWidth, iDimen);

    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, hBrush);
    RoundRect(hdc, ptDest.x, ptDest.y, ptDest.x + iSquareWidth, ptDest.y + iSquareWidth, iSquareWidth * 0.05, iSquareWidth * 0.05);
    DrawSquareText(hdc, iVal, ptDest.x, ptDest.y, iSquareWidth);
    DeleteObject(hBrush);
}

void SetRandSquare(int* ipSquareArray, int iAcc)
{
    int r;
    int iLoc = 0;
    int Array[100];
    static long isRand = time(0);
    TCHAR Buffer[128];


    for (int i = 0; i < iAcc; i++)
        if (ipSquareArray[i] == 0)
            Array[iLoc++] = i;

    srand(isRand);
    isRand = rand();
    srand(isRand);

    if (iLoc != 0)
    {
        if (rand() % 8 == 0)    //出4
        {
            srand(rand());
            ipSquareArray[Array[rand() % iLoc]] = 4;
        }
        else
        {
            srand(rand());
            ipSquareArray[Array[rand() % iLoc]] = 2;
        }
    }
}

#define Between(k,a,b) (min(a,b)<=k&&k<=max(a,b))

void GetKey(int iDimen, UINT uType, int* iStart, int* iEnd, int* iDelta, int* jDelta)
{
    switch (uType)
    {
    case VK_UP:
        *iStart = 0;
        *iEnd = iDimen - 1;
        *iDelta = 1;
        *jDelta = iDimen;
        break;
    case VK_DOWN:
        *iStart = iDimen * iDimen - iDimen;
        *iEnd = iDimen * iDimen - 1;
        *iDelta = 1;
        *jDelta = (-1) * iDimen;
        break;
    case VK_LEFT:
        *iStart = 0;
        *iEnd = iDimen * iDimen - iDimen;
        *iDelta = iDimen;
        *jDelta = 1;
        break;
    case VK_RIGHT:
        *iStart = iDimen - 1;
        *iEnd = iDimen * iDimen - 1;
        *iDelta = iDimen;
        *jDelta = -1;
        break;
    }
}

int TidyCR(int iDest, int* ipSquareArray, int iDimen, UINT Type)
{
    int     iStart;
    int     iEnd;
    int     iDelta;
    int     jDelta;
    int     iSum = 0;

    GetKey(iDimen, Type, &iStart, &iEnd, &iDelta, &jDelta);

    int i = iDest;

    for (int j = i; ((Type == VK_UP || Type == VK_DOWN) && j <= iDimen * iDimen - 1 && j >= 0) ||
        ((Type == VK_LEFT || Type == VK_RIGHT) && Between(j, i, i + 3 * jDelta)); j += jDelta)
    {
        int k = j;
        while ((((Type == VK_UP || Type == VK_DOWN) && (k < iDimen * iDimen && k >= 0)) ||
            ((Type == VK_LEFT || Type == VK_RIGHT) && min(i, i + (iDimen - 1) * jDelta) <= k && k <= max(i, i + (iDimen - 1) * jDelta))) &&
            ipSquareArray[k] == 0)
            k += jDelta;
        if ((((Type == VK_UP || Type == VK_DOWN) && (k >= iDimen * iDimen || k < 0)) ||
            ((Type == VK_LEFT || Type == VK_RIGHT) && (min(i, i + (iDimen - 1) * jDelta) > k || k > max(i, i + (iDimen - 1) * jDelta)))) ||
            k == j)
            continue;
        else
        {
            ipSquareArray[j] = ipSquareArray[k];
            ipSquareArray[k] = 0;
            iSum++;
        }
    }
    return iSum;
}


int TidySquares(int* ipSquareArray, int iDimen, UINT Type)
{
    int     iStart;
    int     iEnd;
    int     iDelta;
    int     jDelta;
    int     iSum = 0;

    GetKey(iDimen, Type, &iStart, &iEnd, &iDelta, &jDelta);

    for (int i = iStart; i <= iEnd; i += iDelta)
        iSum += TidyCR(i, ipSquareArray, iDimen, Type);
    return iSum;
}

int ComputeSquares(int* ipSquareArray, int iDimen, UINT uType)
{
    int     iStart;
    int     iEnd;
    int     iDelta;
    int     jDelta;
    int     iSum = 0;

    GetKey(iDimen, uType, &iStart, &iEnd, &iDelta, &jDelta);
    iSum += TidySquares(ipSquareArray, iDimen, uType);

    for (int i = iStart; i <= iEnd; i += iDelta)
    {
        for (int j = i; ((uType == VK_UP || uType == VK_DOWN) && j <= iDimen * iDimen - 1 && j >= 0) ||
            ((uType == VK_LEFT || uType == VK_RIGHT) && Between(j, i, i + 2 * jDelta)); j += jDelta)
        {
            if (ipSquareArray[j] == 0)
                break;
            if (ipSquareArray[j] == ipSquareArray[j + jDelta])
            {
                ipSquareArray[j] *= 2;
                iScore += ipSquareArray[j];
                ipSquareArray[j + jDelta] = 0;
                iSum++;
                iSum += TidyCR(i, ipSquareArray, iDimen, uType);
            }
        }
    }
    return iSum;
}

int WinOrLose(int* ipSquareArray, int iDimen)
{
    bool    IsWin = false;
    bool    IsLose = true;
    int     w, l;

    for (int i = 0; i < iDimen * iDimen && (IsWin == false || IsLose == true); i++)
    {
        if (ipSquareArray[i] == 2048)
            IsWin = true;   //已经获胜
        if (ipSquareArray[i] == 0)
        {
            IsLose = false;
            continue;
        }
        if (IsLose==true && (
            (Between(i - iDimen, 0, iDimen * iDimen - 1) && (ipSquareArray[i] == ipSquareArray[i - iDimen] || ipSquareArray[i - iDimen] == 0)) ||
            (Between(i + iDimen, 0, iDimen * iDimen - 1) && (ipSquareArray[i] == ipSquareArray[i + iDimen] || ipSquareArray[i + iDimen] == 0)) ||
            (i % iDimen != 0 && (ipSquareArray[i] == ipSquareArray[i - 1] || ipSquareArray[i - 1] == 0)) ||
            ((i + 1) % iDimen != 0 && (ipSquareArray[i] == ipSquareArray[i + 1] || ipSquareArray[i + 1] == 0))))

            IsLose = false;
    }
    if (IsWin == true)
        w = 1;
    else
        w = 0;
    if (IsLose == true)
        l = 1;
    else
        l = 0;
    return w * 10 + l;
}

void MyDrawText(HDC hdc, int x, int y, const TCHAR* szBuffer, const TCHAR* szFaceName, COLORREF crFontColor, int iHeight, int iWeight)
{
    LOGFONTW lf = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    HFONT    hf;

    lstrcpyW(lf.lfFaceName, szFaceName);
    lf.lfHeight = iHeight;
    lf.lfWeight = iWeight;
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, crFontColor);
    hf = CreateFontIndirectW(&lf);
    SelectObject(hdc, hf);
    TextOutW(hdc, x, y, szBuffer, lstrlen(szBuffer));
    DeleteObject(hf);
}

void MyDrawRoundRect(HDC hdc, RECT* rect, int ra, int rb, COLORREF crColor)
{
    HBRUSH hBru = CreateSolidBrush(crColor);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, hBru);

    RoundRect(hdc, rect->left, rect->top, rect->right, rect->bottom, ra, rb);
    DeleteObject(hBru);
}

void AdjArray(int* ipSquareArray, int iDimen)
{
    for(int i=0;i<iDimen*iDimen;i++)
        switch (ipSquareArray[i])
        {
        case 0:
        case 2:
        case 4:
        case 8:
        case 16:
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
        case 1024:
        case 2048:
        case 4096:
        case 8192:
        case 16384:
        case 32768:
        case 65536:
        case 131072:
            break;
        default:
            ipSquareArray[i] = 0;
        }
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
    static POINT    ptRect[5];
    static POINT    ptLargestStart; //外方框的位置
    static RECT     rectLargest;
    static RECT     rectStep;
    static RECT     rectScore;
    static POINT    ptSmall;
    static RECT     rectClient;
    static int      iSquareWidth = 100;
    static int      iDimen = 4;
    static int      iFrameWidth = 3;
    static const COLORREF crFrame = RGB(183, 173, 163);
    static int      ipSquareArray[16] = {   0,0,0,0,
                                            0,0,0,0,
                                            0,0,0,0,
                                            0,0,0,0 };
    static int      IsWon = 0;
    static int      IsLose = 0;
    static RECT     rectRestart;
    static fstream  fin, fout;
    static int      iArticle;   //是否读过文章
    int             iTemp;
    TCHAR           szBuffer[1024];

    switch (message)
    {
    case WM_CREATE:
        /*//设置大方框的起始位置
        GetClientRect(hWnd, &rectClient);
        ptLargestStart.x = (rectClient.right - rectClient.left) / 2 - (iDimen * iSquareWidth + (iDimen - 1) * iFrameWidth + iFrameWidth / 2 * 2) / 2;
        ptLargestStart.y = (rectClient.bottom - rectClient.top) / 2 - (iDimen * iSquareWidth + (iDimen - 1) * iFrameWidth + iFrameWidth / 2 * 2) / 2;
        SetFrame(ptLargestStart, &rectLargest, iSquareWidth, iDimen, iFrameWidth);*/

        //读取文件
        fin.open("2048.data", ios::in);
        if (fin)
        {
            for (int i = 0; i < iDimen * iDimen; i++)
                fin >> ipSquareArray[i];
            AdjArray(ipSquareArray, iDimen);
            fin >> iStep >> iScore >> IsWon >> IsLose >> iArticle;
            fin.close();
        }
        else    //打开失败
        {
            for (int i = 0; i < iDimen * iDimen; i++)
                ipSquareArray[i] = 0;
            SetRandSquare(ipSquareArray, 16);
            SetRandSquare(ipSquareArray, 16);
            GetClientRect(hWnd, &rectClient);
            iSquareWidth = min(100, (rectClient.right - rectClient.left - 100) / 4);
            iSquareWidth = min(iSquareWidth, (rectClient.bottom - rectClient.top - 150) / 4);
            IsWon = 0;
            IsLose = 0;
            iScore = 0;
            iStep = 0;
            iArticle = 0;
        }
        break;
    case WM_SIZE:
        //rectClient
        rectClient.right = LOWORD(lParam);
        rectClient.bottom = HIWORD(lParam);
        iSquareWidth = min(100, (rectClient.right - rectClient.left - 20) / 4);
        iSquareWidth = min(iSquareWidth, (rectClient.bottom - rectClient.top - 10) / 6);
        //测试语句
        //wsprintf(szBuffer, TEXT("注意这里，低字位%d 高 %d"), rectClient.right, rectClient.bottom);
        //MessageBoxW(hWnd, szBuffer, TEXT("测试"), 0);
        ptLargestStart.x = (rectClient.right - rectClient.left) / 2 - (iDimen * iSquareWidth + (iDimen - 1) * iFrameWidth + iFrameWidth * 2) / 2;
        ptLargestStart.y = (rectClient.bottom - rectClient.top) * 0.5 - (iDimen * iSquareWidth + (iDimen - 1) * iFrameWidth + iFrameWidth * 2) *0.3;
        ptLargestStart.y = min(ptLargestStart.y, rectClient.bottom - 4 * iSquareWidth - 10);
        iFrameWidth = iSquareWidth * 0.03;
        iFrameWidth = max(iFrameWidth, 1);
        iFrameWidth = min(iFrameWidth, 3);
        SetFrame(ptLargestStart, &rectLargest, iSquareWidth, iDimen, iFrameWidth);
        InvalidateRect(hWnd, &rectClient, FALSE);    //强制重绘
        //MessageBoxW(hWnd, TEXT("Size被触发"), TEXT("TEST"), NULL);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT     ps;
            HDC             realHdc = BeginPaint(hWnd, &ps);
            HPEN            hPen;
            HBRUSH          hBru;
            LOGFONTW        lf = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
            HFONT           hf;
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            //
            //  开始画最外边的方框
            //
            HDC hdc = CreateCompatibleDC(realHdc);
            HBITMAP bmp = CreateCompatibleBitmap(realHdc, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top);
            SelectObject(hdc, bmp);

            //网上的办法不好用，自己琢磨出来的

            //HDC hdc = GetDC(hWnd);

            FillRect(hdc, &rectClient, (HBRUSH)GetStockObject(WHITE_BRUSH));//用白色画笔填充

            //Draw

            //MoveToEx(...);

            //LineTo(...);
            //SwapBuffers(MemDC);//双缓冲（这是盖茨送的）

            //SetBkMode(hdc, TRANSPARENT);
            hPen = CreatePen(PS_SOLID, iFrameWidth, crFrame);
            hBru = CreateSolidBrush(crFrame);
            SelectObject(hdc, hPen);
            SelectObject(hdc, hBru);
            Rectangle(hdc, rectLargest.left, rectLargest.top, rectLargest.right, rectLargest.bottom);
            //SetRandSquare(ipSquareArray, 16);
            //SetRandSquare(ipSquareArray, 16);
            //wsprintf(szBuffer, TEXT("%d %d"), ipSquareArray[14], ipSquareArray[15]);
            //MessageBoxW(hWnd, szBuffer, TEXT("测试"), NULL);
            MyDrawText(hdc, rectLargest.left, rectLargest.top - iSquareWidth * 0.3, TEXT("RESTART"), TEXT("Arial"), RGB(222, 142, 47), iSquareWidth * 0.25, FW_BOLD);
            //以下是测试代码
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            rectRestart.left = rectLargest.left - iSquareWidth * 0.02;
            rectRestart.top = rectLargest.top - iSquareWidth * 0.3;
            rectRestart.right = rectLargest.left + iSquareWidth * 1.06;
            rectRestart.bottom = rectLargest.top - iSquareWidth * 0.05;
            //Rectangle(hdc, rectRestart.left, rectRestart.top, rectRestart.right, rectRestart.bottom);

            MyDrawText(hdc, rectLargest.left, rectLargest.top - iSquareWidth * 1.4, TEXT("2048"), TEXT("Arial"), RGB(108, 105, 96), iSquareWidth * 0.82, FW_THIN);
            for (int i = 0; i < iDimen * iDimen; i++)
                DrawSmallSquare(hdc, i, ipSquareArray[i], &rectLargest, iFrameWidth, iSquareWidth, iDimen);

            rectStep.left   = rectLargest.left + iSquareWidth * 1.66;
            rectStep.top    = rectLargest.top - iSquareWidth * 1.3;
            rectStep.right  = rectLargest.left + iSquareWidth * 2.86;
            rectStep.bottom = rectLargest.top - iSquareWidth * 0.65;
            rectScore.left = rectLargest.left + iSquareWidth * 2.9;
            rectScore.top = rectLargest.top - iSquareWidth * 1.3;
            rectScore.right = rectLargest.left + iSquareWidth * 4.1;
            rectScore.bottom = rectLargest.top - iSquareWidth * 0.65;

            MyDrawRoundRect(hdc, &rectStep, iSquareWidth * 0.05, iSquareWidth * 0.05, RGB(186, 173, 164));
            MyDrawRoundRect(hdc, &rectScore, iSquareWidth * 0.05, iSquareWidth * 0.05, RGB(186, 173, 164));
            SetTextAlign(hdc, TA_CENTER);
            MyDrawText(hdc, rectLargest.left + iSquareWidth * 2.26, rectLargest.top - iSquareWidth * 1.25, TEXT("STEP"), TEXT("Arial"), RGB(230, 230, 230), iSquareWidth * 0.2, FW_BOLD);
            MyDrawText(hdc, rectLargest.left + iSquareWidth * 3.5, rectLargest.top - iSquareWidth * 1.25, TEXT("SCORE"), TEXT("Arial"), RGB(230, 230, 230), iSquareWidth * 0.2, FW_BOLD);
            lstrcpyW(lf.lfFaceName, TEXT("Arial"));
            lf.lfHeight = iSquareWidth * 0.3;
            lf.lfWeight = FW_BOLD;
            hf = CreateFontIndirectW(&lf);
            SelectObject(hdc, hf);
            SetTextColor(hdc, RGB(255, 255, 255));
            TextOut(hdc, rectLargest.left + iSquareWidth * 2.26, rectLargest.top - iSquareWidth, szBuffer, wsprintfW(szBuffer, TEXT("%d"), iStep));
            TextOut(hdc, (rectScore.left + rectScore.right) / 2, rectLargest.top - iSquareWidth, szBuffer, wsprintfW(szBuffer, TEXT("%d"), iScore));
            DeleteObject(hPen);
            DeleteObject(hBru);
            DeleteObject(hf);

            BitBlt(realHdc, 0, 0, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, hdc, 0, 0, SRCCOPY);

            DeleteDC(hdc);
            DeleteObject(bmp);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
            switch (wParam)
            {
            case VK_UP:
            case VK_DOWN:
            case VK_LEFT:
            case VK_RIGHT:
                if (ComputeSquares(ipSquareArray, iDimen, wParam) > 0)
                {
                    iStep++;
                    SetRandSquare(ipSquareArray, iDimen * iDimen);
                    InvalidateRect(hWnd, &rectLargest, FALSE);
                    InvalidateRect(hWnd, &rectStep, FALSE);
                    InvalidateRect(hWnd, &rectScore, FALSE);
                }
                iTemp = WinOrLose(ipSquareArray, iDimen);
                if (iTemp / 10 == 1 && IsWon == 0)
                {
                    MessageBoxW(hWnd, TEXT("Won the Game"), TEXT("2048"), NULL);
                    if (iArticle == 0)
                        ShowWindow(hDlg, SW_SHOW);
                    iArticle = 1;
                    IsWon = 1;
                }
                if (iTemp % 10 == 1 && IsLose == 0)
                {
                    MessageBoxA(hWnd, "Game Over", "2048", NULL);
                    //InvalidateRect(hWnd, &rectLargest, TRUE);
                    IsLose = 1;

                }
                break;
            /*case VK_SPACE:
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
                ShowWindow(hDlg, SW_SHOW);
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
                break;*/
            }
            break;
    case WM_LBUTTONDOWN:
        if (Between(LOWORD(lParam), rectRestart.left, rectRestart.right) && Between(HIWORD(lParam), rectRestart.top, rectRestart.bottom))
        {
            //重置按键被触发
            if (MessageBox(hWnd, TEXT("您真的要重新开始吗？"), TEXT("2048"), MB_YESNO) == IDYES)
            {
                //MessageBox(hWnd, TEXT("他说是"), NULL, NULL);
                iStep = 0;
                iScore = 0;
                for (int i = 0; i < iDimen * iDimen; i++)
                    ipSquareArray[i] = 0;
                SetRandSquare(ipSquareArray, iDimen* iDimen);
                SetRandSquare(ipSquareArray, iDimen* iDimen);
                InvalidateRect(hWnd, &rectLargest, FALSE);
                InvalidateRect(hWnd, &rectStep, FALSE);
                InvalidateRect(hWnd, &rectScore, FALSE);
                IsWon = 0;
                IsLose = 0;
            }
        }
        break;
    case WM_DESTROY:
        fout.open("2048.data", ios::out);
        for (int i = 0; i < iDimen * iDimen; i++)
        {
            fout << ipSquareArray[i];
            if ((i + 1) % 4 == 0)
                fout << endl;
            else
                fout << ' ';
        }
        fout << iStep << ' ' << iScore << ' ' << IsWon << ' ' << IsLose << ' ' << iArticle;
        fout.close();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

TCHAR szBuffer[] = TEXT("  Congratulations! You have won the game for the first time!!!");

#define TEXTHEIGHT 27

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TEXTMETRIC   tm;
    static RECT         rectText;
    static RECT         rectClient;
    static LOGFONTW     lf;
    static SCROLLINFO   si;
    static int          iDelta;

    HFONT    hf;
    HDC      hdc;

    switch (message)
    {
    case WM_INITDIALOG:

        /*lf.lfHeight = 100;
        lstrcpyW(lf.lfFaceName, TEXT("微软雅黑"));
        hf = CreateFontIndirect(&lf);
        hdc = GetDC(hDlg);
        SelectObject(hdc, hf);

        DeleteObject(hf);
        ReleaseDC(hDlg, hdc);*/
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 30;
        si.nPage = 1;
        si.nPos = 0;
        GetClientRect(hDlg, &rectText);
        GetClientRect(hDlg, &rectClient);
        SetScrollInfo(hDlg, SB_VERT, &si, TRUE);
        return TRUE;
    case WM_VSCROLL:
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hDlg, SB_VERT, &si);
        int iPre;
        iPre = si.nPos;
        switch (LOWORD(wParam))
        {
        case SB_TOP:
            si.nPos = si.nMin;
            break;

        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;

        case SB_LINEUP:
            si.nPos -= 1;
            break;

        case SB_LINEDOWN:
            si.nPos += 1;
            break;

        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;

        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;

        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
        }

        si.fMask = SIF_POS;
        SetScrollInfo(hDlg, SB_VERT, &si, TRUE);
        GetScrollInfo(hDlg, SB_VERT, &si);
        if (si.nPos != iPre)
        {
            ScrollWindow(hDlg, 0, (iPre - si.nPos) * TEXTHEIGHT, NULL, NULL);
            //InvalidateRect(hDlg, &rectClient,TRUE);
            UpdateWindow(hDlg);
        }

        break;

    case WM_KEYDOWN:
        //GetScrollInfo(hDlg, SB_VERT, &si);
        switch (wParam)
        {
        case VK_UP:
            //MessageBox(hDlg, TEXT("tesr"), NULL, NULL);
            SendMessage(hDlg, WM_VSCROLL, SB_LINEUP, 0);
            break;
        case VK_DOWN:
            SendMessage(hDlg, WM_VSCROLL, SB_LINEDOWN, 0);
            break;
        case VK_PRIOR:
            SendMessage(hDlg, WM_VSCROLL, SB_LINEUP, 0);
            break;
        case VK_NEXT:
            SendMessage(hDlg, WM_VSCROLL, SB_LINEUP, 0);
            break;
        /*case VK_SPACE:
            TCHAR T[128];
            wsprintfW(T, TEXT("%d"), si.nPos);
            MessageBox(hDlg, T, NULL, NULL);*/
        }
        /*si.fMask = SIF_POS;
        SetScrollInfo(hDlg, SB_VERT, &si, TRUE);
        InvalidateRect(hDlg, &rectClient, TRUE);
        UpdateWindow(hDlg);*/
        break;
    case WM_MOUSEWHEEL:
        //MessageBox(hDlg, TEXT("TEST"), 0, 0);
        iDelta = (short)HIWORD(wParam);
        if (iDelta == 120)
            for (int i = 0; i < 3; i++)
                SendMessageW(hDlg, WM_VSCROLL, SB_LINEUP, 0);
        else if(iDelta==-120)
            for(int i=0;i<3;i++)
                SendMessageW(hDlg, WM_VSCROLL, SB_LINEDOWN, 0);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    case WM_PAINT:

        PAINTSTRUCT ps;

        hdc = BeginPaint(hDlg, &ps);
        GetTextMetrics(hdc, &tm);
        GetScrollInfo(hDlg, SB_VERT, &si);
        si.nPage = (tm.tmHeight + tm.tmInternalLeading);
        si.nMax = 100 * si.nPage;

        rectText.top = min(rectClient.top - TEXTHEIGHT * si.nPos, 0);
        //rectText.bottom = rectClient.bottom - 10 * si.nPos;

        lf.lfHeight = TEXTHEIGHT;
        lstrcpyW(lf.lfFaceName, TEXT("微软雅黑"));
        hf = CreateFontIndirect(&lf);
        SelectObject(hdc, hf);

        SetBkMode(hdc, TRANSPARENT);
        //TextOut(hdc, 0, 0, szBuffer, wsprintf(szBuffer, TEXT("你好")));
        /*wsprintfW(szBuffer, TEXT("这是一个彩蛋！！！\n\
很高兴你能看到这些，说明此时的你已经克服了重重困难见到了2048的模样，当然你也可能通过什么别的不正常途径到达这里（接下来我会告诉你怎么做），不过不管怎么样，总归你是花了不少心思，我很开心你一直执着地把这个小游戏玩下去。\n\
你知道，写游戏的人总会给自己的游戏留些后门（别叫我程序员，我不喜欢这个称呼），那么接下来聊聊关于这个小游戏的一些歪门邪道，如果你打开游戏所在位置的文件夹，你就会发现在那个文件夹里多了一个2048.data文件，右键点击它->打开方式->选择记事本，然后你就可以看到游戏的配置信息了，首先你会发现文件开头是一个4*4的数阵，没错，就是你想的那样，这16个数就是游戏启动时直接加载到16个方格的数值，需要注意的是，这16个数必须是2的正整数次幂，且需要小于等于2^17（因为只有16个格子，不可能比2^17更大了）。这样，通过修改这16个数的值，就可以随心所欲地更改方块的值，保存文件退出以后重新进入游戏（运行游戏的时候要先关闭配置文件），就会看到游戏界面已经发生了变化。接下来第五行有5个参数，它们分别记录当前走过的步数（Step），分数（Score），是否已经获胜（0为否，1为是），是否已经失败，最后一个参数是最有趣的，它代表着彩蛋是否已经被触发，如果是0，那么在下一次你获胜之后这段文字就会跳出来，但是在这之后那个参数就会变成1，以后就不会再出现这个彩蛋了，当然如果你想再看一次，可以在配置文件中把最后一个参数改成0，保存以后进入游戏，获胜以后就又可以看到了。总体来看，配置文件中共有21个参数，需要注意的是，这几个参数除了按照4/4/4/4/5的格式分成5行以外，每行的数之间一定要用空格隔开，参数的格式不建议乱改，如果程序读取错误，可能会导致崩溃（~...~），如果不幸言中，那就直接删除配置文件，这样游戏会自动重置，并会生成一个新的2048.data，这也是重置游戏的一种方法。\n\
也许你会问，为什么是2048，往大里说，这个游戏，像极了金融（^_^），方块的数值如滚雪球一般指数膨胀，这不正是每一个金融人所努力实现的职业轨迹吗，狭窄一点说是资本积累，往大里想，我们的一生何尝不是如此，如今的你我可能仅仅如同一个2，或者一个4那样在芸芸众生中毫不起眼，但是正如马云说的那样，平凡的人才最可能成功。因为正如这方块一样，在经过数次蜕变之后，将会成为令人瞠目结舌的庞然大物。我并不是说金融人的财富都会以指数级增长（不然我也不会离开了...），而是我深深地感到，金融更多地教会我们该以怎样的态度去理解未来，理解时间，理解付出，为我们看起来遥不可及的理想和现实之间建立一种看起来行得通的模型。这也就是为什么我走出经管院以后，更加愿意和金融学生打交道，你们视野更广，更敢想，更自信，这些比一纸学历、一份高薪的职业可贵得多，也实在得多。 "));
        */DrawText(hdc, szBuffer, -1, &rectText, DT_WORDBREAK | DT_EXPANDTABS);

        //DeleteObject(hf);
        EndPaint(hDlg, &ps);
        break;
    }
    return FALSE;
}