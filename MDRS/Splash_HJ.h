#pragma once

// CSplash_HJ

class CSplash_HJ : public CWnd
{
	DECLARE_DYNAMIC(CSplash_HJ)

public:
	CSplash_HJ();
	virtual ~CSplash_HJ();

	CBitmap m_bitmap;  
	void Create(UINT nBitmapID);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint(); 
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


