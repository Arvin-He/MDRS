// EppInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDRS.h"
#include "EppInfoDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <fstream>

#include <iostream>

using namespace std;

extern float	* pData;

// CEppInfoDlg dialog

IMPLEMENT_DYNAMIC(CEppInfoDlg, CDialogEx)

CEppInfoDlg::CEppInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEppInfoDlg::IDD, pParent)
{

}

CEppInfoDlg::~CEppInfoDlg()
{
}

void CEppInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEppInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SHOW_EPP_INFO, &CEppInfoDlg::OnBnClickedBtnShowEppInfo)
END_MESSAGE_MAP()


// CEppInfoDlg message handlers
void CEppInfoDlg::AddEppMachineInfo()
{
	CListBox *pEppInfoList = (CListBox*)GetDlgItem(IDC_EPP_INFO_LIST);
	if(pEppInfoList->GetCount() == 0)
	{
		pEppInfoList->AddString("0机箱 0通道");
		pEppInfoList->AddString("0机箱 1通道");
	}
	else
	{
// 		for (int i=0; i<=pEppInfoList->GetCount(); i++)
// 		{
// 			UpdateData(FALSE);
// 			pEppInfoList->DeleteString(i);
// 		}
		pEppInfoList->ResetContent();
	}
	
}


void CEppInfoDlg::OnBnClickedBtnShowEppInfo()
{
	// TODO: Add your control notification handler code her
	AddEppMachineInfo();
	AddEppChannelInfo();
}


void CEppInfoDlg::AddEppChannelInfo()
{
	int lineNum = 0;
	ifstream fin("database.txt", ios::in);
	char line[1024]={0};
	while(fin.getline(line, sizeof(line)))
	{
		stringstream data(line);
		if (lineNum == 0)
		{
			for (int i=0; i<10; i++)
			{
				data >> eppDataBase[0][i];
			}	
		}
		else if (lineNum == 1)
		{
			for (int i=0; i<10; i++)
			{
				data >> eppDataBase[1][i];
			}
		}
		lineNum++;
	}
	fin.clear();
	fin.close();
	
	UpdateData(FALSE);
	CString strEppData = _T("");
	strEppData = eppDataBase[0][2].c_str();
	SetDlgItemText(IDC_INTERVAL_MIN0, strEppData);
	strEppData = eppDataBase[0][3].c_str();
	SetDlgItemText(IDC_INTERVAL_MAX0, strEppData);
	strEppData = eppDataBase[0][4].c_str();
	SetDlgItemText(IDC_DA_MIN0, strEppData);
	strEppData = eppDataBase[0][5].c_str();
	SetDlgItemText(IDC_DA_MAX0, strEppData);
	strEppData = eppDataBase[0][6].c_str();
	SetDlgItemText(IDC_VOLTAGE_MIN0, strEppData);
	strEppData = eppDataBase[0][7].c_str();
	SetDlgItemText(IDC_VOLTAGE_MAX0, strEppData);
	strEppData = eppDataBase[0][8].c_str();
	SetDlgItemText(IDC_AD_MIN0, strEppData);
	strEppData = eppDataBase[0][9].c_str();
	SetDlgItemText(IDC_AD_MAX0, strEppData);
	
	strEppData = eppDataBase[1][2].c_str();
	SetDlgItemText(IDC_INTERVAL_MIN1, strEppData);	
	strEppData = eppDataBase[1][3].c_str();
	SetDlgItemText(IDC_INTERVAL_MAX1, strEppData);
	strEppData = eppDataBase[1][4].c_str();
	SetDlgItemText(IDC_DA_MIN1, strEppData);	
	strEppData = eppDataBase[1][5].c_str();
	SetDlgItemText(IDC_DA_MAX1, strEppData);	
	strEppData = eppDataBase[1][6].c_str();
	SetDlgItemText(IDC_VOLTAGE_MIN1, strEppData);
	strEppData = eppDataBase[1][7].c_str();
	SetDlgItemText(IDC_VOLTAGE_MAX1, strEppData);	
	strEppData = eppDataBase[1][8].c_str();
	SetDlgItemText(IDC_AD_MIN1, strEppData);
	strEppData = eppDataBase[1][9].c_str();
	SetDlgItemText(IDC_AD_MAX1, strEppData);	
}