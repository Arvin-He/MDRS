#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <conio.h>
//using namespace std;
using namespace cv;
// CPage1 dialog

class CPage1 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPage1();
	BOOL SetInitialCardsPara(int cardIndex, int nCurrAxis);
	BOOL OnClipRobotOpera(UINT nID);
	BOOL OnGlueRobotOpera(UINT nID);
	BOOL OnCCDStageOpera(UINT nID);
	BOOL OnSampleStageOpera(UINT nID);
	void InitialMicroStage();
	BOOL OnMicroStageOpera(UINT nID);
	void CheckAxisStatus(int cardIndex, int nCurrAxis);
	void GetAxisPosition(int nCurrAxis, UINT nID);
//归零（回原点）
	BOOL OnClipRobotGoHome(UINT nID);
	BOOL OnGlueRobotGoHome(UINT nID);
	BOOL OnCCDStageGoHome(UINT nID);
	BOOL OnSampleStageGoHome(UINT nID);
	BOOL OnRotateAxisGoHome(UINT nID);
//复位（当电机超出限位，不能被驱动时，进行电机复位操作）
	BOOL OnClipRobotReset(UINT nID);
	BOOL OnGlueRobotReset(UINT nID);
	BOOL OnCCDRobotReset(UINT nID);

// 	BOOL InitList();
// 	BOOL UpdateList(int iSel);
// Dialog Data
	enum { IDD = IDD_PAGE1 };


private:
	
	DWORD	m_cardArrary[16];
	int		m_sampleStageDir;

	bool    m_bLightGreen;
	bool    m_bLightYellow;
	bool	    m_bLightRed;
	bool		m_bBeepWarn;

private:
//	FILE	* streami;
// 	float	* pData;
// 	int	    iItemNum;

	int		m_iMachine0;			//机箱号
	int		m_iChannel0;			//通道号
	float	m_MinVoltage0;		//最小电压
	float	m_MaxVoltage0;		//最大电压
	float	m_MinInterval0;		//最小位移
	float	m_MaxInterval0;	//最大位移
	float	m_MinDA0;		//最小DA
	float	m_MaxDA0;		//最大DA
	float	m_MinAD0;		//最小AD
	float	m_MaxAD0;		//最大AD

	int		m_iChannel1;			//通道号
	float	m_MinVoltage1;		//最小电压
	float	m_MaxVoltage1;		//最大电压
	float	m_MinInterval1;		//最小位移
	float	m_MaxInterval1;	//最大位移
	float	m_MinDA1;		//最小DA
	float	m_MaxDA1;	//最大DA
	float	m_MinAD1;	//最小AD
	float	m_MaxAD1;		//最大AD
	string eppDataBase[2][10];	//保存EPP的TXT文件中基本信息，用于显示

	float    m_currInternalPos;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClipRobotAllCheck();
	afx_msg void OnBnClickedGlueRobotAllCheck();
	afx_msg void OnBnClickedCcdRobotAllCheck();
	afx_msg void OnBnClickedSampleStageAllCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFrapClaw();
	afx_msg void OnBnClickedPuffClaw();
	afx_msg void OnBnClickedDroppingGlue();
	afx_msg void OnBnClickedSuckChip();
	afx_msg void OnBnClickedClipRobotAllCheckReset();
	afx_msg void OnBnClickedGlueRobotAllCheckReset();
	afx_msg void OnBnClickedCcdRobotAllCheckReset();
	afx_msg void OnBnClickedSampleStageAllCheckReset();
	afx_msg void OnBnClickedRotateAxisAllCheck();
	afx_msg void OnBnClickedSampleStageSetZero();
	afx_msg void OnBnClickedRotateAxisSetZero();
	afx_msg void OnBnClickedReleaseChip();
	afx_msg void OnBnClickedGreenLight();
	afx_msg void OnBnClickedYellowLight();
	afx_msg void OnBnClickedRedLight();
	afx_msg void OnBnClickedWarning();
	afx_msg void OnBnClickedBtnMicroStageInit();
	afx_msg void OnBnClickedBtnCheckEppInfo();
};
