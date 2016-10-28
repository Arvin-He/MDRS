#pragma once


//IMPLEMENT_DYNAMIC(CSplashHB, CWnd)
// CSplashHB

class CSplashHB : public CWnd
{
	DECLARE_DYNAMIC(CSplashHB)

public:
	CSplashHB();
	virtual ~CSplashHB();

	CBitmap m_bitmap;  
	void Create(UINT nBitmapID);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint(); 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


