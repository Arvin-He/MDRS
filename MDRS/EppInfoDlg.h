#pragma once
#include <string>
using namespace std;

// CEppInfoDlg dialog

class CEppInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEppInfoDlg)

public:
	CEppInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEppInfoDlg();

// Dialog Data
	enum { IDD = IDD_EPP_INFO_DLG };

	void AddEppMachineInfo();
	void AddEppChannelInfo();

private:
string eppDataBase[2][10];	//保存EPP的TXT文件中基本信息，用于显示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnShowEppInfo();
};
