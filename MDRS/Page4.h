#pragma once


// CPage4 dialog

class CPage4 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPage4();

// Dialog Data
	enum { IDD = IDD_PAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
