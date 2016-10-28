// SplashHB.cpp : implementation file
//

#include "stdafx.h"
#include "MDRS.h"
#include "SplashHB.h"


// CSplashHB
#define SPLASH_WIDTH 755
#define SPLASH_HEIGHT 529

IMPLEMENT_DYNAMIC(CSplashHB, CWnd)

CSplashHB::CSplashHB()
{
	//注册窗口类
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);
	//创建窗口
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, lpszWndClass, NULL, WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, SPLASH_WIDTH, SPLASH_HEIGHT, NULL, NULL, NULL);
}

CSplashHB::~CSplashHB()
{
}


BEGIN_MESSAGE_MAP(CSplashHB, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSplashHB message handlers
void CSplashHB::Create(UINT nBitmapID)  
{  
	m_bitmap.LoadBitmap(nBitmapID);  
	BITMAP bitmap;  
	m_bitmap.GetBitmap(&bitmap);  
	//CreateEx(0,AfxRegisterWndClass(0),"",WS_POPUP|WS_VISIBLE|WS_BORDER,0,0,bitmap.bmWidth,bitmap.bmHeight,NULL,0);  
	CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),  NULL, 
		WS_POPUP | WS_VISIBLE, 0, 0, bitmap.bmWidth, bitmap.bmHeight, NULL , NULL);  
} 

void CSplashHB::OnPaint()  
{  
	// TODO: 在此处添加消息处理程序代码  
	// 不为绘图消息调用 CWnd::OnPaint()  
	CPaintDC dc(this); // device context for painting  
	BITMAP bitmap;  
	m_bitmap.GetBitmap(&bitmap);  
	CDC dcComp;  
	dcComp.CreateCompatibleDC(&dc);  
	dcComp.SelectObject(&m_bitmap);  
	// draw bitmap  
	dc.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcComp,0,0,SRCCOPY);  
} 

int CSplashHB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	//窗口位于屏幕中心
	CenterWindow();
	//启动画面显示时间
	SetTimer(1, 3000, NULL);
	//淡入图片1.2秒，窗口渐现
	::AnimateWindow(GetSafeHwnd(), 1200, AW_BLEND);
	return 0;
}

void CSplashHB::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//淡出图片0.8秒，窗口渐隐
	::AnimateWindow(GetSafeHwnd(), 800, AW_BLEND | AW_HIDE); 
	CWnd::OnClose();
}

BOOL CSplashHB::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//获取应用程序模块句柄
	HINSTANCE hApp = ::GetModuleHandle(NULL);
	//加载位图
	HBITMAP hBmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));
	//创建兼容DC作为内存DC
	HDC hCompDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
	//将位图选入内存DC
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(hCompDC, hBmp);
	//将内存DC中的内容拷贝到设备DC以显示
	::BitBlt(pDC->GetSafeHdc(), 0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, hCompDC, 0, 0, SRCCOPY);
	//清理，以防内存泄露
	::SelectObject(hCompDC, hBmpOld);
	::DeleteObject(hBmp);
	::DeleteDC(hCompDC);

	return CWnd::OnEraseBkgnd(pDC);
}


void CSplashHB::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//销毁定时器
	KillTimer(1);
	//关闭启动画面
	PostMessage(WM_CLOSE);
	CWnd::OnTimer(nIDEvent);
}
