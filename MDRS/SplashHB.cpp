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
	//ע�ᴰ����
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);
	//��������
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
	// TODO: �ڴ˴������Ϣ����������  
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()  
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
	//����λ����Ļ����
	CenterWindow();
	//����������ʾʱ��
	SetTimer(1, 3000, NULL);
	//����ͼƬ1.2�룬���ڽ���
	::AnimateWindow(GetSafeHwnd(), 1200, AW_BLEND);
	return 0;
}

void CSplashHB::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//����ͼƬ0.8�룬���ڽ���
	::AnimateWindow(GetSafeHwnd(), 800, AW_BLEND | AW_HIDE); 
	CWnd::OnClose();
}

BOOL CSplashHB::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//��ȡӦ�ó���ģ����
	HINSTANCE hApp = ::GetModuleHandle(NULL);
	//����λͼ
	HBITMAP hBmp = ::LoadBitmap(hApp, MAKEINTRESOURCE(IDB_SPLASH));
	//��������DC��Ϊ�ڴ�DC
	HDC hCompDC = ::CreateCompatibleDC(pDC->GetSafeHdc());
	//��λͼѡ���ڴ�DC
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(hCompDC, hBmp);
	//���ڴ�DC�е����ݿ������豸DC����ʾ
	::BitBlt(pDC->GetSafeHdc(), 0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, hCompDC, 0, 0, SRCCOPY);
	//�����Է��ڴ�й¶
	::SelectObject(hCompDC, hBmpOld);
	::DeleteObject(hBmp);
	::DeleteDC(hCompDC);

	return CWnd::OnEraseBkgnd(pDC);
}


void CSplashHB::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//���ٶ�ʱ��
	KillTimer(1);
	//�ر���������
	PostMessage(WM_CLOSE);
	CWnd::OnTimer(nIDEvent);
}
