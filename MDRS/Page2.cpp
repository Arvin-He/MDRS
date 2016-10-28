// Page2.cpp : implementation file
//

#include "stdafx.h"
#include "MDRS.h"
#include "MDRSDlg.h"
#include "Page2.h"
#include "afxdialogex.h"
#include "MAC2SSPv102.h"
#include "Global.h"
#include <math.h>

// CPage2 dialog

IMPLEMENT_DYNAMIC(CPage2, CDialogEx)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage2::IDD, pParent)
{
	for (int i=0; i<8; i++)
	{
		m_bAutoSampleState[i] = false;
	}

	m_bIsRunning = false;
	m_bIsAllPosSetZero = false;

	m_bIsAllHome = false;

	m_bCarry = false;
	m_bDetectWpCenter = false;
	m_bGlueStart = false;
	m_bMountGlassCircle = false;
	m_bDetectGcCenter = false;
	m_bCarryBack = false;

	m_offsetX = 0;
	m_offsetY = 0;
	m_offsetZ = 0;
	m_posErrX = 0;
	m_posErrY = 0;
	m_posErrZ = 0;

	pAutoRunThread = NULL;
	pSemiRunThread = NULL;
}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Radio(pDX, IDC_RADIO_ONE, m_radioSampleNum);
}

BEGIN_MESSAGE_MAP(CPage2, CDialogEx)
	ON_BN_CLICKED(IDC_CLIP_HOME, &CPage2::OnBnClickedClipHome)
	ON_BN_CLICKED(IDC_SAMPLE_STAGE_HOME, &CPage2::OnBnClickedSampleStageHome)
	ON_BN_CLICKED(IDC_GLUE_HOME, &CPage2::OnBnClickedGlueHome)
	ON_BN_CLICKED(IDC_CCD_HOME, &CPage2::OnBnClickedCcdHome)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_CARD_ZERO, IDC_RADIO_CARD_TWELVE, OnRadioBtnSetCurrCard)

	ON_BN_CLICKED(IDC_INIT_CLIP, &CPage2::OnBnClickedInitClip)
	ON_BN_CLICKED(IDC_INIT_GLUE, &CPage2::OnBnClickedInitGlue)
	ON_BN_CLICKED(IDC_INIT_SAMPLE_STAGE, &CPage2::OnBnClickedInitSampleStage)
	ON_BN_CLICKED(IDC_INIT_CCD_STAGE, &CPage2::OnBnClickedInitCCDStage)

	ON_BN_CLICKED(IDC_DEMARCATE_CCD_H, &CPage2::OnBnClickedDemarcateCCDPosH)
	ON_BN_CLICKED(IDC_DEMARCATE_CCD_V, &CPage2::OnBnClickedDemarcateCCDPosV)
	ON_BN_CLICKED(IDC_DEMARCATE_SAMPLE_STAGE, &CPage2::OnBnClickedDemarcateSampleStagePos)
	ON_BN_CLICKED(IDC_DEMARCATE_CLIP_ROBOT, &CPage2::OnBnClickedDemarcateClipRobotPos)
	ON_BN_CLICKED(IDC_DEMARCATE_GLUE_PIN, &CPage2::OnBnClickedDemarcateGluePinPos)
	ON_BN_CLICKED(IDC_DEMARCATE_GLUE_SUCK, &CPage2::OnBnClickedDemarcateGlueSuckPos)

	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_AUTO_RUN, &CPage2::OnBnClickedAutoRun)
	ON_BN_CLICKED(IDC_PAUSE_OPERA, &CPage2::OnBnClickedPauseOperation)
	ON_BN_CLICKED(IDC_RESET_RESUME, &CPage2::OnBnClickedResetAndResumeRun)
	
	ON_BN_CLICKED(IDC_BTN_GLUE_BACK_ZERO, &CPage2::OnBnClickedBtnGlueBackZero)
	ON_BN_CLICKED(IDC_BTN_SAMPLE_STAGE_BACK_ZERO, &CPage2::OnBnClickedBtnSampleStageBackZero)
	ON_BN_CLICKED(IDC_INIT_GLUE_SUCK, &CPage2::OnBnClickedInitGlueSucker)
	ON_BN_CLICKED(IDC_BTN_CARRY, &CPage2::OnBnClickedBtnCarry)
	ON_BN_CLICKED(IDC_BTN_DETECT_WP_CENTER, &CPage2::OnBnClickedBtnDetectWpCenter)
	ON_BN_CLICKED(IDC_BTN_GLUE, &CPage2::OnBnClickedBtnGlueStart)
	ON_BN_CLICKED(IDC_BTN_MOUNT_GLASS, &CPage2::OnBnClickedBtnMountGlassCircle)
	ON_BN_CLICKED(IDC_BTN_DETECT_GC_CENTER, &CPage2::OnBnClickedBtnDetectGcCenter)
	ON_BN_CLICKED(IDC_BTN_CARRY_BACK, &CPage2::OnBnClickedBtnCarryBack)
	ON_BN_CLICKED(IDC_BTN_ALL_GO_HOME, &CPage2::OnBnClickedBtnAllGoHome)
	ON_BN_CLICKED(IDC_BTN_ALL_INIT, &CPage2::OnBnClickedBtnAllInit)
	ON_BN_CLICKED(IDC_BTN_PAUSE_SEMI, &CPage2::OnBnClickedPauseSemi)
	ON_BN_CLICKED(IDC_RESUME_POS_SEMI, &CPage2::OnBnClickedResumePosSemi)
END_MESSAGE_MAP()


// CPage2 message handlers
BOOL CPage2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CStatic *pSampleOne =(CStatic*)GetDlgItem(IDC_SAMPLE_ONE);
	pSampleOne->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleOne->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleTwo =(CStatic*)GetDlgItem(IDC_SAMPLE_TWO);
	pSampleTwo->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleTwo->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleThree =(CStatic*)GetDlgItem(IDC_SAMPLE_THREE);
	pSampleThree->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleThree->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleFour =(CStatic*)GetDlgItem(IDC_SAMPLE_FOUR);
	pSampleFour->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleFour->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleFive =(CStatic*)GetDlgItem(IDC_SAMPLE_FIVE);
	pSampleFive->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleFive->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleSix =(CStatic*)GetDlgItem(IDC_SAMPLE_SIX);
	pSampleSix->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleSix->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleSeven =(CStatic*)GetDlgItem(IDC_SAMPLE_SEVEN);
	pSampleSeven->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleSeven->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleEight =(CStatic*)GetDlgItem(IDC_SAMPLE_EIGHT);
	pSampleEight->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleEight->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));

	CStatic *pSampleOneAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_ONE_AUTO);
	pSampleOneAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleOneAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleTwoAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_TWO_AUTO);
	pSampleTwoAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleTwoAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleThreeAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_THREE_AUTO);
	pSampleThreeAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleThreeAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleFourAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_FOUR_AUTO);
	pSampleFourAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleFourAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleFiveAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_FIVE_AUTO);
	pSampleFiveAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleFiveAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleSixAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_SIX_AUTO);
	pSampleSixAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleSixAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleSevenAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_SEVEN_AUTO);
	pSampleSevenAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleSevenAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	CStatic *pSampleEightAuto =(CStatic*)GetDlgItem(IDC_SAMPLE_EIGHT_AUTO);
	pSampleEightAuto->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	pSampleEightAuto->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	
	//先将点胶手的旋转和样品台的旋转置为零，目的是设定固定点为零点，方便回零。
	ssp2_set_current_card(12);
	ssp2_set_command(2, 0);//样品台
	ssp2_set_command(3, 0);//点胶手

	SetTimer(1, 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPage2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		{
			int currCardIndex = ssp2_get_current_card();		
			if(0 == currCardIndex)
			{
				((CButton*)GetDlgItem(IDC_RADIO_CARD_ZERO))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_RADIO_CARD_FOUR))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_RADIO_CARD_TWELVE))->SetCheck(FALSE);
				GetAxisPosition(0, IDC_GLUE_ROBOT_X_POS2);
				GetAxisPosition(1, IDC_GLUE_ROBOT_Y_POS2);
				GetAxisPosition(2, IDC_GLUE_ROBOT_Z_POS2);
				GetAxisPosition(3, IDC_CCD_ROBOT_Z_POS2);
				GetDlgItem(IDC_CCD_ROBOT_X_POS2)->SetWindowText(_T("0"));
				GetDlgItem(IDC_CCD_ROBOT_U_POS2)->SetWindowText(_T("0"));
			}
			else if (4 == currCardIndex)
			{
				((CButton*)GetDlgItem(IDC_RADIO_CARD_ZERO))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_RADIO_CARD_FOUR))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_RADIO_CARD_TWELVE))->SetCheck(FALSE);
				GetAxisPosition(0, IDC_CLIP_ROBOT_X_POS2);
				GetAxisPosition(1, IDC_CLIP_ROBOT_Y_POS2);
				GetAxisPosition(2, IDC_CLIP_ROBOT_Z_POS2);
				GetAxisPosition(3, IDC_CCD_ROBOT_Y_POS2);
				GetDlgItem(IDC_CLIP_ROBOT_U_POS2)->SetWindowText(_T("0"));
			}
			else if (12 == currCardIndex)
			{
				((CButton*)GetDlgItem(IDC_RADIO_CARD_ZERO))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_RADIO_CARD_FOUR))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_RADIO_CARD_TWELVE))->SetCheck(TRUE);
				GetAxisPosition(0, IDC_SAMPLE_STAGE_X_POS2);
				GetAxisPosition(1, IDC_SAMPLE_STAGE_Y_POS2);
				GetDlgItem(IDC_SAMPLE_STAGE_Z_POS2)->SetWindowText(_T("0"));
				GetAxisPosition(2, IDC_SAMPLE_STAGE_U_POS2);
				GetAxisPosition(3, IDC_GLUE_ROBOT_U_POS2);
			}
		}
		break;
//	case 2:

	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

/////////////////////////////归零/////////////////////////////////////////
void CPage2::OnBnClickedClipHome()
{
	// TODO: Add your control notification handler code here
	AxisGoHome(4, 0);
	Sleep(1000);
	AxisGoHome(4, 1);
	Sleep(100);
	AxisGoHome(4, 2);
	Sleep(1000);
	AxisPosClear(4, 0);
	AxisPosClear(4, 1);
	AxisPosClear(4, 2);
}

void CPage2::OnBnClickedSampleStageHome()
{
	// TODO: Add your control notification handler code here
	CWinThread *pSampleStageHomeThread = NULL;
	pSampleStageHomeThread = AfxBeginThread(&SampleStageHomeThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
}

UINT CPage2::SampleStageHomeThreadFunc(LPVOID lpParam)
{
	CPage2 *pPage2 = (CPage2*)lpParam;
	pPage2->SampleStageHome();
	return 0;
}

void CPage2::SampleStageHome()
{
	ssp2_set_current_card(12);
	ssp2_set_home_mode(0, 3, 0, 1, 1, 0);
	ssp2_home_move(0, 1, 200, 2000, 400, 1, 2);
	Sleep(10);
	ssp2_set_home_mode(1, 3, 0, 1, 1, 0);
	ssp2_home_move(1, 1, 200, 2000, 400, 1, 2);
	Sleep(6000);
	AxisPosClear(12, 0);
	AxisPosClear(12, 1);
}

void CPage2::OnBnClickedGlueHome()
{
	// TODO: Add your control notification handler code here
	AxisGoHome(12, 3);
	Sleep(100);
	AxisGoHome(0, 0);
	Sleep(100);
	AxisGoHome(0, 1);
	Sleep(100);
	AxisGoHome(0, 2);
	Sleep(100);
	AxisPosClear(0, 0);
	AxisPosClear(0, 1);
	AxisPosClear(0, 2);
	AxisPosClear(12, 3);
}

void CPage2::OnBnClickedCcdHome()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i<4; i++)
	{
		AxisReset(4, i);
		Sleep(10);
	}
	AxisGoHome(4, 3);

	for(int i=0; i<4; i++)
	{
		AxisReset(0, i);
		Sleep(10);
	}
	AxisGoHome(0, 3);

	Sleep(1000);
	AxisPosClear(4, 3);
	AxisPosClear(0, 3);
}

void CPage2::OnBnClickedBtnGlueBackZero()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(12);
	ssp2_start_ps_move(3, 1, 0, 1000, 20000, 1, 2, 0.4, 0.5);
}

void CPage2::OnBnClickedBtnSampleStageBackZero()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(12);
	ssp2_start_ps_move(2, 1, 0, 1000, 20000, 1, 2, 0.4, 0.5);
}
/////////////////////////////归零/////////////////////////////////////////
void CPage2::OnRadioBtnSetCurrCard(UINT nCmdID)
{
	switch(nCmdID)
	{
	case IDC_RADIO_CARD_ZERO:
		{
			((CButton*)GetDlgItem(nCmdID))->SetCheck(TRUE);
			ssp2_set_current_card(0);
		}
		break;
	case IDC_RADIO_CARD_FOUR:
		{
			((CButton*)GetDlgItem(nCmdID))->SetCheck(TRUE);
			ssp2_set_current_card(4);
		}
		break;
	case IDC_RADIO_CARD_TWELVE:
		{
			((CButton*)GetDlgItem(nCmdID))->SetCheck(TRUE);
			ssp2_set_current_card(12);
		}	
		break;
	default:
		break;
	}
}
////////////////////////////初始化////////////////////////////////////////
void CPage2::OnBnClickedInitClip()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(4);
	//读配置文件
	CString		fileName(_T(".\\CONFIG\\CLIP\\CLIP_POS.ini"));
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	//zPOS
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);

	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);

	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
}

void CPage2::OnBnClickedInitGlue()
{
	// TODO: Add your control notification handler code here
	//读配置文件
	CString	fileName(_T(".\\CONFIG\\GLUE\\GluePinSafePos.ini"));
	char		cPos[20];
	CString	strPos(_T(""));	
	int		nPos(0);
	//uPos
	GetPrivateProfileString(_T("section4"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_set_current_card(12);
	ssp2_start_ps_move(3, 1, nPos, 1000, 20000, 1, 2, 0.5, 0.4);
	Sleep(1000);
	ssp2_set_current_card(0);
	//zPOS
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	//yPos
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);
	//xPos
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	
}

void CPage2::OnBnClickedInitGlueSucker()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(0);
	//读配置文件
	CString		fileName(_T(".\\CONFIG\\GLUE\\GlueSuck_POS.ini"));
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	//zPOS
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	//yPos
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);
	//xPos
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	//uPos
	GetPrivateProfileString(_T("section4"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_set_current_card(12);
	ssp2_start_ps_move(3, 1, nPos, 1000, 20000, 1, 2, 0.5, 0.4);
}

void CPage2::OnBnClickedInitSampleStage()
{
	// TODO: Add your control notification handler code here
	InitSampleStagePos();
}

void CPage2::InitSampleStagePos()
{
	ssp2_set_current_card(12);
	//读配置文件
	CString		fileName(_T(".\\CONFIG\\SampleStage_POS.ini"));
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	//xPos
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 2000, 1, 2, 0.4, 0.5);
	//yPos
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 2000, 1, 2, 0.4, 0.5);
	//uPos旋转
	GetPrivateProfileString(_T("section3"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 1000, 6000, 1, 2, 0.5, 0.5);
}

void CPage2::OnBnClickedInitCCDStage()
{
	// TODO: Add your control notification handler code here
	//读配置文件
	CString		fileName(_T(".\\CONFIG\\CCD\\CCD_POS.ini"));
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);

	GetPrivateProfileString(_T("section1"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_set_current_card(4);
	//y
	ssp2_start_ps_move(3, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);	//mode1：绝对位置
	Sleep(10);
	GetPrivateProfileString(_T("section2"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_set_current_card(0);
	//z
	ssp2_start_ps_move(3, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);	//mode1：绝对位置
}
////////////////////////////初始化////////////////////////////////////////
//////////////////////标定/////////////////////////////
//标定CCD位置
void CPage2::OnBnClickedDemarcateCCDPosV()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}

	CString		fileName(_T(".\\CONFIG\\CCD\\CCD_POS.ini"));
	CString		appName(_T("section2"));
	CString		keyName(_T("zPos"));
	CString		data(_T(""));
	GetDlgItem(IDC_CCD_ROBOT_Z_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
}

void CPage2::OnBnClickedDemarcateCCDPosH()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}

	CString		fileName(_T(".\\CONFIG\\CCD\\CCD_POS.ini"));
	CString		appName(_T("section1"));
	CString		keyName(_T("yPos"));
	CString		data(_T(""));
	GetDlgItem(IDC_CCD_ROBOT_Y_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
}
//标定样品台位置
void CPage2::OnBnClickedDemarcateSampleStagePos()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}

	CString		fileName(_T(".\\CONFIG\\SampleStage_POS.ini"));
	CString		appName(_T("section1"));
	CString		keyName(_T("xPos"));
	CString		data(_T(""));
	GetDlgItem(IDC_SAMPLE_STAGE_X_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	//YPOS
	appName = _T("section2");
	keyName = _T("yPos");
	GetDlgItem(IDC_SAMPLE_STAGE_Y_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	//UPOS
	appName = _T("section3");
	keyName = _T("uPos");
	GetDlgItem(IDC_SAMPLE_STAGE_U_POS2)->GetWindowText(data);
	if (data != "0")
	{
		MessageBox(_T("标定坐标不为0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
}
//标定夹具台位置
void CPage2::OnBnClickedDemarcateClipRobotPos()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}

	CString		fileName(_T(".\\CONFIG\\CLIP\\CLIP_POS.ini"));
	CString		appName(_T("section1"));
	CString		keyName(_T("xPos"));
	CString		data(_T(""));
	GetDlgItem(IDC_CLIP_ROBOT_X_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section2");
	keyName = _T("yPos");
	GetDlgItem(IDC_CLIP_ROBOT_Y_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section3");
	keyName = _T("zPos");
	GetDlgItem(IDC_CLIP_ROBOT_Z_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
}
//标定点胶头位置
void CPage2::OnBnClickedDemarcateGluePinPos()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}
	//先获取配置文件的值，貌似GluePin_POS的值永远不能变，只能作为参考值
	CString	fileName(_T(".\\CONFIG\\GLUE\\GluePin_POS.ini"));	
	char	cPos[20];
	CString	strPos(_T(""));	
	int xPos(0);
	int yPos(0);
	int zPos(0);

	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	xPos = atoi(strPos);
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	yPos = atoi(strPos);
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	zPos = atoi(strPos);

	int xCurrPos = GetDlgItemInt(IDC_GLUE_ROBOT_X_POS2);
	int yCurrPos = GetDlgItemInt(IDC_GLUE_ROBOT_Y_POS2);
	int zCurrPos = GetDlgItemInt(IDC_GLUE_ROBOT_Z_POS2);

	//求取偏差，将偏差写进INI文件即可，每次更新的是偏差
	m_offsetX = xCurrPos - xPos;
	m_offsetY = yCurrPos - yPos;
	m_offsetZ = zCurrPos - zPos;

	fileName = _T(".\\CONFIG\\GLUE\\GluePinOffSet.ini");
	CString		appName(_T("section1"));
	CString		keyName(_T("xPos"));
	CString		data(_T(""));
	data.Format("%d", m_offsetX); 
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section2");
	keyName = _T("yPos");
	data.Format("%d", m_offsetY);
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section3");
	keyName = _T("zPos");
	data.Format("%d", m_offsetZ);
	WritePrivateProfileString(appName, keyName, data, fileName);
}
//标定吸头位置
void CPage2::OnBnClickedDemarcateGlueSuckPos()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}

	//先获取配置文件的值，貌似GlueSuck_POS的值永远不能变，只能作为参考值
	CString		fileName(_T(".\\CONFIG\\GLUE\\GlueSuck_POS.ini"));	
	char		cPos[20];
	CString		strPos(_T(""));	
	int			xPos(0);
	int			yPos(0);
	int			zPos(0);

	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	xPos = atoi(strPos);
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	yPos = atoi(strPos);
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	zPos = atoi(strPos);

	int xCurrPos = GetDlgItemInt(IDC_GLUE_ROBOT_X_POS2);
	int yCurrPos = GetDlgItemInt(IDC_GLUE_ROBOT_Y_POS2);
	int zCurrPos = GetDlgItemInt(IDC_GLUE_ROBOT_Z_POS2);

	//求取偏差，将偏差写进INI文件即可，每次更新的是偏差
	m_offsetX = xCurrPos - xPos;
	m_offsetY = yCurrPos - yPos;
	m_offsetZ = zCurrPos - zPos;

	fileName = _T(".\\CONFIG\\GLUE\\GlueSuckOffSet.ini");
	CString		appName(_T("section1"));
	CString		keyName(_T("xPos"));
	CString		data(_T(""));
	data.Format("%d", m_offsetX); 
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section2");
	keyName = _T("yPos");
	data.Format("%d", m_offsetY);
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section3");
	keyName = _T("zPos");
	data.Format("%d", m_offsetZ);
	WritePrivateProfileString(appName, keyName, data, fileName);

/*	CString		fileName(_T(".\\CONFIG\\GLUE\\GlueSuck_POS.ini"));
	CString		appName(_T("section1"));
	CString		keyName(_T("xPos"));
	CString		data(_T(""));
	GetDlgItem(IDC_GLUE_ROBOT_X_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	//YPOS
	appName = _T("section2");
	keyName = _T("yPos");
	GetDlgItem(IDC_GLUE_ROBOT_Y_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	//ZPOS
	appName = _T("section3");
	keyName = _T("zPos");
	GetDlgItem(IDC_GLUE_ROBOT_Z_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标是0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	//UPOS
	appName = _T("section4");
	keyName = _T("uPos");
	GetDlgItem(IDC_GLUE_ROBOT_U_POS2)->GetWindowText(data);
	if (data == "0")
	{
		MessageBox(_T("标定坐标为0！请确定好标定位置！\n"));
		return;
	}
	WritePrivateProfileString(appName, keyName, data, fileName);
	*/
}
//////////////////////标定/////////////////////////////
//设置运作状态
void CPage2::SetSampleState(int rows, int cols, bool flagState)
{
	if ((rows == 1) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_ONE_AUTO);
	else if((rows == 2) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_TWO_AUTO);
	else if((rows == 3) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_THREE_AUTO);
	else if((rows == 4) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_FOUR_AUTO);
	else if((rows == 1) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_EIGHT_AUTO);
	else if((rows == 2) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_SEVEN_AUTO);
	else if((rows == 3) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_SIX_AUTO);
	else if((rows == 4) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_FIVE_AUTO);
}

void CPage2::SetSampleStateSemiMode(int rows, int cols, bool flagState)
{
	if ((rows == 1) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_ONE);
	else if((rows == 2) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_TWO);
	else if((rows == 3) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_THREE);
	else if((rows == 4) && (cols == 1))
		SetFlagState(flagState, IDC_SAMPLE_FOUR);
	else if((rows == 1) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_EIGHT);
	else if((rows == 2) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_SEVEN);
	else if((rows == 3) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_SIX);
	else if((rows == 4) && (cols == 2))
		SetFlagState(flagState, IDC_SAMPLE_FIVE);
}

void CPage2::SetFlagState(bool m_bSampleState, UINT nID)
{
	CStatic *pSample =(CStatic*)GetDlgItem(nID);
	pSample->ModifyStyle(0, SS_BITMAP | SS_CENTERIMAGE);
	if (m_bSampleState == FALSE)
		pSample->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN)));
	else
		pSample->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN_HL)));
}
//读取轴当前脉冲数，并显示在edit控件上
void CPage2::GetAxisPosition(int nCurrAxis, UINT nID)
{
	CString strPos(_T(""));
	//获取当前脉冲数目
	int pos = ssp2_get_command(nCurrAxis);
	strPos.Format(_T("%d"), pos);
	GetDlgItem(nID)->SetWindowText(strPos);
	UpdateData(FALSE);
}
//////////////////////操作//////////////////////////////
//到运动指定位置
void CPage2::MoveToAssignedPosSemi(int rows)
{
	CString	fileName(_T(""));	
	char		cPos[20];
	CString	strPos(_T(""));	
	int		nPos(0);

	if (rows == 1)
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");
	else if(rows == 2)
		fileName = _T(".\\CONFIG\\CLIP\\21P.ini");
	else if(rows == 3)
		fileName = _T(".\\CONFIG\\CLIP\\31P.ini");
	else if(rows == 4)
		fileName = _T(".\\CONFIG\\CLIP\\41P.ini");

	ssp2_set_current_card(4);
	//z+
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(800);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(500);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);	
}
//夹紧工件
void CPage2::ClipWorkpiece(int nColIndex)
{ 	
	//夹紧并开始工件之前确保点胶头在安全位置
	GlueMoveSafePos();
	CString	fileName(_T(""));	
	char		cPos[20];
	CString	strPos(_T(""));	
	int		nPos(0);
	ssp2_set_current_card(4);
	if (nColIndex == 1)
		fileName = _T(".\\CONFIG\\CLIP\\M1.ini"); 
	else if(nColIndex == 2)
		fileName = _T(".\\CONFIG\\CLIP\\M2.ini");

	//x向前，y方向没有移动
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	if (nColIndex == 1)
	{
		ssp2_start_ps_move(0, 1, nPos, 100, 400, 1 , 2, 0.5, 0.4);
		Sleep(500);
	}
	else if (nColIndex == 2)
	{
		ssp2_start_ps_move(0, 1, nPos, 200, 800, 1 , 2, 0.5, 0.4);
		Sleep(1200);
	}
	//z向下
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(2500);
	//夹住工件
	FrapClaw();
	Sleep(1000);
}
//搬运工件
void CPage2::CarryWorkpiece()
{
	CString		fileName(_T(""));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	fileName = _T(".\\CONFIG\\CLIP\\DstPos.ini");
	//z上升
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(1500);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(800);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);
	Sleep(2000);
	
}
//放下工件,夹头退至指定位置
void CPage2::DropWorkpiece()
{
	CString		fileName(_T(""));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);

	fileName = _T(".\\CONFIG\\CLIP\\DropPos.ini");
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	//z向下准备放下工件
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(2000);
	//张开
	PuffClaw();
	Sleep(1000);
	ClipMoveWaitPos();
 	//识别工件中心位置，并移至图像中心位置
	Sleep(2500);
}
//夹头移到等待位置
void CPage2::ClipMoveWaitPos()
{
	CString		fileName(_T(""));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	fileName = _T(".\\CONFIG\\CLIP\\ClipWaitPos.ini");
	ssp2_set_current_card(4);
	//z上升
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(500);
	//x后退
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
}
//点胶头到安全位置
void CPage2::GlueMoveSafePos()
{
	//读偏差配置文件
	GetGluePinPosErr();
	//读配置文件
	CString		fileName(_T(".\\CONFIG\\GLUE\\GluePinSafePos.ini"));
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	//u
	GetPrivateProfileString(_T("section4"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_set_current_card(12);
	ssp2_start_ps_move(3, 1, nPos, 1000, 20000, 1, 2, 0.5, 0.4);
	Sleep(800);
	ssp2_set_current_card(0);
	//z
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos+m_posErrZ, 100, 400, 1, 2, 0.5, 0.4);
	Sleep(100);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos+m_posErrX, 100, 400, 1, 2, 0.5, 0.4);
	Sleep(100);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos+m_posErrY, 100, 400, 1, 2, 0.5, 0.4);
}
//点胶工件
void CPage2::GlueWorkpiece()
{
	GetGluePinPosErr();
	//读配置文件
	CString		fileName(_T(".\\CONFIG\\GLUE\\GluePinMove.ini"));	
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	ssp2_set_current_card(0);
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	//x前移
	ssp2_start_ps_move(0, 1, nPos+m_posErrX, 200, 400, 1, 2, 0.5, 0.4);
	Sleep(500);
	//yMove
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos+m_posErrY, 200, 400, 1, 2, 0.5, 0.4);
	//z下降
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos+m_posErrZ, 200, 400, 1, 2, 0.5, 0.4);
	Sleep(1200);
	ssp2_set_current_card(12);
	//载物台旋转
	ssp2_start_tarc_move(3, 0, 3200, 3300, 0, 0, 500, 3000, 2.0, 0);
	Sleep(100);
	//点胶
	DropGlue();
	Sleep(12000);
	//点胶头移动到安全位置
	GlueMoveSafePos();

	//检查点胶后工件是否还在图像中央

	//如果不在图像中央，则移动样品台使工件中心移至图像中央
}
//吸头取玻璃环
void CPage2::AbsorbWorkpiece(int rows, int cols, int nCounts)
{
	GetGlueAbsorbPosErr();
	CString		fileName(_T(""));	
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);

	switch(nCounts)
	{
	case 1:
		fileName = _T(".\\CONFIG\\GCL\\11.ini");
		break;
	case 2:
		fileName = _T(".\\CONFIG\\GCL\\12.ini");
		break;
	case 3:
		fileName = _T(".\\CONFIG\\GCL\\21.ini");
		break;
	case 4:
		fileName = _T(".\\CONFIG\\GCL\\22.ini");
		break;
	case 5:
		fileName = _T(".\\CONFIG\\GCL\\31.ini");
		break;
	case 6:
		fileName = _T(".\\CONFIG\\GCL\\32.ini");
		break;
	default:
		break;
	}

	if (nCounts>6)
	{
		if ((rows == 3) && (cols == 2))
		{
			fileName = _T(".\\CONFIG\\GCL\\11.ini");
		}
		else if ((rows == 4) && (cols == 2))
		{
			fileName = _T(".\\CONFIG\\GCL\\12.ini");
		}
	}

	ssp2_set_current_card(0);
	//xMove
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos+m_posErrX, 200, 800, 1, 2, 0.5, 0.4);
	Sleep(100);
	//yMove
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos+m_posErrY, 200, 800, 1, 2, 0.5, 0.4);
	Sleep(2000);
	//u
	ssp2_set_current_card(12);
	GetPrivateProfileString(_T("section4"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(3, 1, nPos, 1000, 20000, 1, 2, 0.5, 0.4);
	Sleep(1000);
	//z
	ssp2_set_current_card(0);
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos+m_posErrZ, 200, 800, 1, 2, 0.5, 0.4);
	Sleep(2000);
	//吸取芯片,同时夹具张开,这是因为夹具和吸取芯片是共用一个气管
	InhaleChip();
	Sleep(1000);
}
//移动样品台和CCD，以便好识别
void CPage2::MoveStage_CCD()
{
	//移动微动台,样品台移动一定距离,仅有y方向的移动
	ssp2_set_current_card(12);
	ssp2_start_ps_move(1, 1, 14000, 1000, 5000, 1, 2, 0.5, 0.4);
	Sleep(100);
	//ccd上升，使图像变清晰些
	ssp2_set_current_card(0);
	ssp2_start_ps_move(3, 1, 1970, 200, 400, 1, 2, 0.5, 0.4);
	Sleep(100);
}
//搬运玻璃环
void CPage2::CarryGlassCircle()
{
	GetGlueAbsorbPosErr();
	CString		fileName(_T(".\\CONFIG\\GLUE\\CarryGC.ini"));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	ssp2_set_current_card(0);
	//z上升
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos+m_posErrZ, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(800);
	//y前进，点胶手移动至镜头下面来识别玻璃环中心点位置
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos+m_posErrY, 200, 800, 1, 2, 0.5, 0.4);
	Sleep(100);
	//x前进
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos+m_posErrX, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(2000);
}
//识别玻璃环中心位置，并作校正
void CPage2::Detect_AdjustGCPos()
{
	//识别玻璃环
//	DetectGlassCircleCenter(g_src2);
	Sleep(800);
	//控制点胶机械手将玻璃环移至图像中央位置
	int offsetX(0), offsetY(0);
	offsetX = (320 - m_GlassaCenter.x)*0.45;
	offsetY = (256 - m_GlassaCenter.y)*0.45;
	//
	if ((abs(offsetX)>5) || (abs(offsetY)>5))
	{
		ssp2_set_current_card(0);
//		ssp2_start_ps_move(0, 0, int(disX*0.45), 50, 100, 1 , 2, 0.5, 0.4);
		ssp2_start_ps_move(0, 0, offsetX, 50, 200, 1, 2, 0.5, 0.4);
		Sleep(10);
		ssp2_start_ps_move(1, 0, offsetY, 50, 200, 1, 2, 0.5, 0.4);
		Sleep(1000);
	}
}
//将样品台和CCD回归原位
void CPage2::MoveBackStage_CCD()
{
	//载物台回移
	ssp2_set_current_card(12);
	ssp2_start_ps_move(1, 1, 7000, 1000, 5000, 1, 2, 0.5, 0.4);
	Sleep(1000);
	//CCD镜头回到原位
	ssp2_set_current_card(0);
	ssp2_start_ps_move(3, 1, 1780, 200, 400, 1, 2, 0.5, 0.4);
	Sleep(2000);
}
//放下玻璃环
void CPage2::PutDownChip()
{
	//放置芯片，同时夹具成收缩夹紧状态
	CString		fileName(_T(".\\CONFIG\\GLUE\\GlueSuckDownPos.ini"));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	ssp2_set_current_card(0);
	//z下降
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	Sleep(500);
	ReleaseChip();
	Sleep(1000);
	//点胶手回到初始位置,这边不能回初始位置，夹具会撞到点交头的，应当设定一个安全位置
	GlueMoveSafePos();
	//张开夹具
	PuffClaw();
}
//移动到工件位置
void CPage2::MoveToWP()
{
	CarryWorkpiece();
	ssp2_set_current_card(4);
	CString		fileName(_T(".\\CONFIG\\CLIP\\DropPos.ini"));	
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	//z-
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 400, 1, 2, 0.5, 0.4);
	Sleep(2500);
	FrapClaw();
	Sleep(1000);
}
//将工件搬至原位
void CPage2::PutBackWP(int rows, int cols)
{
	CString		fileName(_T(""));	
	char		cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	if ((rows == 1) && (cols == 1))
		fileName = _T(".\\CONFIG\\CLIP\\11.ini");
	else if ((rows == 2) && (cols == 1))
		fileName = _T(".\\CONFIG\\CLIP\\21.ini");
	else if ((rows == 3) && (cols == 1))
		fileName = _T(".\\CONFIG\\CLIP\\31.ini");
	else if ((rows == 4) && (cols == 1))
		fileName = _T(".\\CONFIG\\CLIP\\41.ini");
	else if ((rows == 1) && (cols == 2))
		fileName = _T(".\\CONFIG\\CLIP\\12.ini");
	else if ((rows == 2) && (cols == 2))
		fileName = _T(".\\CONFIG\\CLIP\\22.ini");
	else if ((rows == 3) && (cols == 2))
		fileName = _T(".\\CONFIG\\CLIP\\32.ini");
	else if ((rows == 4) && (cols == 2))
		fileName = _T(".\\CONFIG\\CLIP\\42.ini");

	ssp2_set_current_card(4);
	//z+
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(1500);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(1000);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(2500);
	//z向下准备放下工件
	fileName = _T(".\\CONFIG\\CLIP\\M11.ini");
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);	
	ssp2_start_ps_move(2, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);
	Sleep(2000);
	//张开
	PuffClaw();
	Sleep(1500);	
}
//移动到准备位置
void CPage2::MoveToPreparePos(int rows, int cols, bool colChangeFlag)
{
	CString		fileName(_T(""));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	if ((rows == 1) && (cols == 1) && (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\21P.ini");
	else if ((rows == 1) && (cols == 1) && (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 2) && (cols == 1) && (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\31P.ini");
	else if ((rows == 2) && (cols == 1) && (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 3) && (cols == 1)&& (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\41P.ini");
	else if ((rows == 3) && (cols == 1)&& (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 4) && (cols == 1) && (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");
	else if ((rows == 4) && (cols == 1) && (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 1) && (cols == 2)&& (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\21P.ini");
	else if ((rows == 1) && (cols == 2)&& (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 2) && (cols == 2) && (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\31P.ini");
	else if ((rows == 2) && (cols == 2) && (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 3) && (cols == 2) && (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\41P.ini");
	else if ((rows == 3) && (cols == 2) && (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	else if ((rows == 4) && (cols == 2) && (!colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");
	else if ((rows == 4) && (cols == 2) && (colChangeFlag))
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");

	ssp2_set_current_card(4);
	//z+
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(800);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(500);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);

}

//////////////////////操作//////////////////////////////

//////////////////全自动运行模式//////////////////////

void CPage2::OnBnClickedAutoRun()
{
	// TODO: Add your control notification handler code here
	int nAutoSampleStateCount = 0;
	for (int i=1; i<3; i++)
	{
		for (int j=1; j<5; j++)
		{
			nAutoSampleStateCount++;
			m_bAutoSampleState[nAutoSampleStateCount] = false;
			SetSampleState(j, i, m_bAutoSampleState[nAutoSampleStateCount]);
		}
	}
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}
	m_bIsRunning = true;
//	CWinThread *pAutoRunThread = NULL;
	pAutoRunThread = AfxBeginThread(&AutoRunThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
	pAutoRunThread->m_bAutoDelete = FALSE;
}

UINT CPage2::AutoRunThreadFunc(LPVOID lpParam)
{
	CPage2 *pPage2 = (CPage2*)lpParam;
	pPage2->AutoRun();
	return 0;
}

void CPage2::AutoRun()
{
	bool colIndexChange(false);
	int rows = GetDlgItemInt(IDC_EDIT_ROW_AUTO);
	int cols = GetDlgItemInt(IDC_EDIT_COL_AUTO);
	if (rows>4 || rows<1 || cols>2 || cols <1)
	{
		MessageBox("行范围在1-4, 列范围在1-2。\n请重新输入！");
		return;
	}
	int nCount = 0;
	for (int i=1; i<=cols; i++)
	{
		for (int j=1; j<=rows; j++)
		{
			nCount++;
			m_bAutoSampleState[nCount-1] = true;
			ClipWorkpiece(i);
			CarryWorkpiece();
			DropWorkpiece();
			DetectWorkpieceCenter(g_src2);
			GlueWorkpiece();
			AbsorbWorkpiece(j, i, nCount);
			MoveStage_CCD();
			CarryGlassCircle();
			//识别玻璃环
			DetectGlassCircleCenter(g_src2);
			Detect_AdjustGCPos();
			MoveBackStage_CCD();
			PutDownChip();
			MoveToWP();
			PutBackWP(j, i);
			if (j == rows && cols>1)
				colIndexChange = true;
			else
				colIndexChange = false;
			MoveToPreparePos(j, i, colIndexChange);		
			SetSampleState(j, i, m_bAutoSampleState[nCount-1]);
		}	
	}
}

void CPage2::OnBnClickedPauseOperation()
{
	// TODO: Add your control notification handler code here
	
	DWORD dwCode;
	GetExitCodeThread(pAutoRunThread->m_hThread, &dwCode);
	TerminateThread(pAutoRunThread->m_hThread, dwCode);
	CloseHandle(pAutoRunThread->m_hThread);

	m_bIsRunning = false;

	int cardIndex = ssp2_get_current_card();
	for (int i=0; i<4; i++)
	{
		AxisStop(cardIndex, i);
	}
}

void CPage2::OnBnClickedResetAndResumeRun()
{
	// TODO: Add your control notification handler code here
	int rows = GetDlgItemInt(IDC_EDIT_ROW_AUTO);
	int cols = GetDlgItemInt(IDC_EDIT_COL_AUTO);

	CString		fileName(_T(""));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);

	if (rows == 1)
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");
	else if (rows == 2)
		fileName = _T(".\\CONFIG\\CLIP\\21P.ini");
	else if (rows == 3)
		fileName = _T(".\\CONFIG\\CLIP\\31P.ini");
	else if (rows == 4)
		fileName = _T(".\\CONFIG\\CLIP\\41P.ini");

	ssp2_set_current_card(4);
	//z+
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(800);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(500);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(1200);
	ssp2_set_current_card(0);
	GlueMoveSafePos();
	Sleep(1200);
	InitSampleStagePos();

}
//////////////////全自动运行模式//////////////////////


//大津法自适应阈值函数
int CPage2::OtsuThreshold(const Mat & img)  
{  
	int T = 0;			//阈值  
	int height = img.rows;  
	int width  = img.cols;  
	int step   = img.step;  
	int channels  = img.channels();  
	uchar* data  = /*(uchar*)*/img.data;  
	double gSum0;		//第一类灰度总值  
	double gSum1;		//第二类灰度总值  
	double N0 = 0;		//前景像素数  
	double N1 = 0;		//背景像素数  
	double u0 = 0;		//前景像素平均灰度  
	double u1 = 0;		//背景像素平均灰度  
	double w0 = 0;		//前景像素点数占整幅图像的比例为ω0  
	double w1 = 0;		//背景像素点数占整幅图像的比例为ω1  
	double u = 0;		//总平均灰度  
	double tempg = -1;	//临时类间方差  
	double g = -1;		//类间方差  
	double Histogram[256]={0};	// = new double[256];//灰度直方图  
	double N = width*height;	//总像素数  
	for(int i=0; i<height; i++)  
	{//计算直方图  
		for(int j=0; j<width; j++)  
		{  
			double temp = data[i*step + j*3]*0.114 + data[i*step + j*3 + 1]*0.587 + data[i*step + j*3 + 2]*0.299;  
			temp = temp<0 ? 0:temp;  
			temp = temp>255 ? 255:temp;  
			Histogram[(int)temp]++;  
		}   
	}  
	//计算阈值  
	for (int i=0; i<256; i++)  
	{  
		gSum0 = 0;  
		gSum1 = 0;  
		N0 += Histogram[i];           
		N1 = N-N0;  
		if(0==N1)break;		//当出现前景无像素点时，跳出循环  
		w0 = N0/N;  
		w1 = 1-w0;  
		for (int j = 0;j<=i;j++)  
		{  
			gSum0 += j*Histogram[j];  
		}  
		u0 = gSum0/N0;  
		for(int k = i+1;k<256;k++)  
		{  
			gSum1 += k*Histogram[k];  
		}  
		u1 = gSum1/N1;  
		u = w0*u0 + w1*u1;  
		g = w0*w1*(u0-u1)*(u0-u1);  
		if (tempg<g)  
		{  
			tempg = g;  
			T = i;  
		}  
	}  
	return T;   
}  
//大津法自适应阈值法
int CPage2::OtsuThreshold2(IplImage* pImg)
{
	int height=pImg->height;    
	int width=pImg->width;        
	long size = height * width;   
	//histogram    
	float histogram[256] = {0};    
	for(int m=0; m < height; m++)  
	{    
		unsigned char* p=(unsigned char*)pImg->imageData + pImg->widthStep * m;    
		for(int n = 0; n < width; n++)   
		{    
			histogram[int(*p++)]++;    
		}    
	}    

	int threshold;      
	long sum0 = 0, sum1 = 0; //存储前景的灰度总和和背景灰度总和  
	long cnt0 = 0, cnt1 = 0; //前景的总个数和背景的总个数  
	double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例  
	double u0 = 0, u1 = 0;  //前景和背景的平均灰度  
	double variance = 0; //最大类间方差  
	int i, j;  
	double u = 0;  
	double maxVariance = 0;  
	for(i = 1; i < 256; i++) //一次遍历每个像素  
	{    
		sum0 = 0;  
		sum1 = 0;   
		cnt0 = 0;  
		cnt1 = 0;  
		w0 = 0;  
		w1 = 0;  
		for(j = 0; j < i; j++)  
		{  
			cnt0 += (long)histogram[j];  
			sum0 += long(j * histogram[j]);  
		}  

		u0 = (double)sum0 /  cnt0;   
		w0 = (double)cnt0 / size;  

		for(j = i ; j <= 255; j++)  
		{  
			cnt1 += (long)histogram[j];  
			sum1 += long(j * histogram[j]);  
		}  

		u1 = (double)sum1 / cnt1;  
		w1 = 1 - w0; // (double)cnt1 / size;  

		u = u0 * w0 + u1 * w1; //图像的平均灰度  
		//		printf("u = %f\n", u);  
		//variance =  w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);  
		variance =  w0 * w1 *  (u0 - u1) * (u0 - u1);  
		if(variance > maxVariance)   
		{    
			maxVariance = variance;    
			threshold = i;    
		}   
	}    
	//	printf("threshold = %d\n", threshold);  
	return threshold;    
}

//检测工件中心轮廓位置
void CPage2::DetectWorkpieceCenter(Mat img)
{
	IplImage* pImg = &IplImage(img);
	IplImage * grayImg=cvCreateImage(cvGetSize(pImg), 8, 1);	
	cvCvtColor(pImg, grayImg, CV_BGR2GRAY);  //灰度化
	cvSmooth(grayImg, grayImg, CV_GAUSSIAN, 3, 3);
	cvErode(grayImg, grayImg, NULL, 1);
	cvDilate(grayImg, grayImg, NULL, 1);
	cvThreshold(grayImg, grayImg, 45, 255, CV_THRESH_BINARY_INV);  //二值化
	////////////////////////////////轮廓检测方法/////////////////////////////////////////////
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;	
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);
	int contourNum = 0;
	contourNum = cvFindContours(grayImg, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	//判断轮廓是否为空
	if (contour == NULL)
	{
		AfxMessageBox("Find no contours! Change your CCD parameters!");
		return;
	}
	CvSeq* maxCirleContour = NULL;
	int	   minContourArea = 500;   //这个作为筛选条件
	int    maxContourArea = 0;
	int    newContourNum = 0;	   //筛选后轮廓的个数
	for ( ; contour != 0; contour = contour->h_next)
	{		
		CvRect contourRect = cvBoundingRect(contour);
		double contArea= contourRect.width * contourRect.height;
		if (contArea > 0.3*640*512)
		{
			cvSeqRemove(contour, 0);
			continue;
		}
		newContourNum ++;
		//算法思想：在循环过程中，遇到更大的数时，先把最大值转存到第二大的值，然后最大值再用那个更大的值覆盖掉
		if ( contArea > maxContourArea)  //筛选两个轮廓中的最大轮廓
		{
			maxCirleContour = contour;
			maxContourArea = contArea;
		}
	}
	//画轮廓
//	cvDrawContours(pImg, maxCirleContour, CV_RGB(0,255,0), CV_RGB(255,0,0), -1, 2, 8, cvPoint(0,0));
	CvRect maxCircleContourRect = cvBoundingRect(maxCirleContour);
	//轮廓外接矩形框
	cvRectangle(pImg, cvPoint(maxCircleContourRect.x, maxCircleContourRect.y), 
		cvPoint(maxCircleContourRect.x+maxCircleContourRect.width,
		maxCircleContourRect.y+maxCircleContourRect.height), CV_RGB(0,255,0), 2, CV_AA);

	CvBox2D minBoxB;
	minBoxB = cvMinAreaRect2(maxCirleContour);
	double radiusB = 0.0;
	if (minBoxB.size.width > minBoxB.size.height)
	{
		radiusB = minBoxB.size.height/2;
	}
	else
	{
		radiusB  = minBoxB.size.width/2;
	}
	//圆心
	cvCircle(pImg, cvPoint((int)minBoxB.center.x, (int)minBoxB.center.y), 3, CV_RGB(0, 255, 0), -1, CV_AA);
	//轮廓圆
	cvCircle(pImg, cvPoint((int)minBoxB.center.x, (int)minBoxB.center.y), (int)radiusB, CV_RGB(255, 0, 0), 2, CV_AA);
	//保存工件识别的中心坐标位置
 	m_WorkPieceCenter.x = (int)minBoxB.center.x;
 	m_WorkPieceCenter.y = (int)minBoxB.center.y;

	Mat dstImg(pImg);
	dstImg.data = (uchar*)pImg->imageData;
	resize(dstImg, dstImg, Size(320, 256));
	ShowImage(dstImg, IDC_PREVIEW_PIC2);
	//最后释放相关内存
	cvReleaseImage(&grayImg);
	cvReleaseMemStorage(&storage);
	//	cvReleaseImage(&pImg);  //这一句不能加，pImg不能释放，因为pImg与m_src2共用一个数据头
}

void CPage2::DetectGlassCircleCenter(Mat img)
{
	IplImage* pImg = &IplImage(img);
	IplImage * grayImg=cvCreateImage(cvGetSize(pImg), 8, 1);	
	cvCvtColor(pImg, grayImg, CV_BGR2GRAY);  //灰度化
	cvSmooth(grayImg, grayImg, CV_GAUSSIAN, 3, 3);
	cvErode(grayImg, grayImg, NULL, 1);
	cvDilate(grayImg, grayImg, NULL, 1);
//	int thresholdValue = 45;
	cvThreshold(grayImg, grayImg, 45, 255, CV_THRESH_BINARY_INV);  //二值化
	////////////////////////////////轮廓检测方法/////////////////////////////////////////////
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;	
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);
	int contourNum = 0;
	contourNum = cvFindContours(grayImg, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	//判断轮廓是否为空
	if (contour == NULL)
	{
		AfxMessageBox("Find no contours! Change your CCD parameters!");
		return;
	}
	CvSeq* maxCirleContour = NULL;
	int	   minContourArea = 200;   //这个作为筛选条件
	double maxContourArea = 0.0;
	int    newContourNum = 0;	   //筛选后轮廓的个数
	for ( ; contour != 0; contour = contour->h_next)
	{		
		CvRect contourRect = cvBoundingRect(contour);
		double contArea= contourRect.width * contourRect.height;
		if (contArea > 0.2*640*512)
		{
			cvSeqRemove(contour, 0);
			continue;
		}
		newContourNum ++;
		//算法思想：在循环过程中，遇到更大的数时，先把最大值转存到第二大的值，然后最大值再用那个更大的值覆盖掉
		if ( contArea > maxContourArea)  //筛选两个轮廓中的最大轮廓
		{
			maxCirleContour = contour;
			maxContourArea = contArea;
		}
	}
	//画轮廓
//	cvDrawContours(pImg, maxCirleContour, CV_RGB(0,255,0), CV_RGB(255,0,0), -1, 2, 8, cvPoint(0,0));
	CvRect maxCircleContourRect = cvBoundingRect(maxCirleContour);
	//轮廓外接矩形框
	cvRectangle(pImg, cvPoint(maxCircleContourRect.x, maxCircleContourRect.y), 
		cvPoint(maxCircleContourRect.x+maxCircleContourRect.width,
		maxCircleContourRect.y+maxCircleContourRect.height), CV_RGB(0,255,0), 2, CV_AA);

	CvBox2D minBoxB;
	minBoxB = cvMinAreaRect2(maxCirleContour);
	double radiusB = 0.0;
	if (minBoxB.size.width > minBoxB.size.height)
	{
		radiusB = minBoxB.size.height/2;
	}
	else
	{
		radiusB  = minBoxB.size.width/2;
	}
	//圆心
	cvCircle(pImg, cvPoint((int)minBoxB.center.x, (int)minBoxB.center.y), 3, CV_RGB(0, 255, 0), -1, CV_AA);
	//轮廓圆
	cvCircle(pImg, cvPoint((int)minBoxB.center.x, (int)minBoxB.center.y), (int)radiusB, CV_RGB(255, 0, 0), 2, CV_AA);
	//保存识别的玻璃环的中心坐标位置
	m_GlassaCenter.x = (int)(minBoxB.center.x);
	m_GlassaCenter.y = (int)(minBoxB.center.y);

	Mat dstImg(pImg);
	dstImg.data = (uchar*)pImg->imageData;
	resize(dstImg, dstImg, Size(320, 256));
	ShowImage(dstImg, IDC_PREVIEW_PIC2);
	//最后释放相关内存
	cvReleaseImage(&grayImg);
	cvReleaseMemStorage(&storage);
	//	cvReleaseImage(&pImg);  //这一句不能加，pImg不能释放，因为pImg与m_src2共用一个数据头
}

void CPage2::ShowImage(Mat img, INT_PTR ID)
{
	CMDRSDlg *pMDRSDlg = (CMDRSDlg*)GetParent()->GetParent();
	CDC *pDC = (pMDRSDlg->GetDlgItem(ID))->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect(0,0,0,0);
	(pMDRSDlg->GetDlgItem(ID))->GetClientRect(&rect);
	int tx = (int)(rect.Width() - img.cols)/2;
	int ty = (int)(rect.Height() - img.rows)/2;
	SetRect(rect, tx, ty, tx+img.cols, ty+img.rows);
	CvvImage cimg;
	IplImage temp = img;
	cimg.CopyOf(&temp);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

//////////////////半自动运行模式///////////////////////

UINT CPage2::SemiAutoThreadFunc(LPVOID lpParam)
{
	CPage2 *pPage2 = (CPage2*)lpParam;
	pPage2->SemiRun();
	return 0;
}

void CPage2::SemiRun()
{
	int rows = GetDlgItemInt(IDC_EDIT_ROW);
	int cols = GetDlgItemInt(IDC_EDIT_COL);
	if (m_bCarry)
	{	
		MoveToAssignedPosSemi(rows);
		ClipWorkpiece(cols);
		CarryWorkpiece();
		DropWorkpiece();
		m_bCarry = false;
		SetSampleStateSemiMode(rows, cols, true);
	}
	else if (m_bDetectWpCenter)
	{
		DetectWorkpieceCenter(g_src2);
		m_bDetectWpCenter = false;
		SetSampleStateSemiMode(rows, cols, true);
	}
	else if (m_bGlueStart)
	{
		GlueWorkpiece();
		m_bGlueStart = false;
		SetSampleStateSemiMode(rows, cols, true);
	}
	else if (m_bMountGlassCircle)
	{
		AbsorbWorkpiece(rows, cols, 1);
		MoveStage_CCD();
		CarryGlassCircle();
		m_bMountGlassCircle = false;
		SetSampleStateSemiMode(rows, cols, true);
	}
	else if (m_bDetectGcCenter)
	{
		DetectGlassCircleCenter(g_src2);
		Detect_AdjustGCPos();
		MoveBackStage_CCD();
		PutDownChip();
		m_bDetectGcCenter = false;
		SetSampleStateSemiMode(rows, cols, true);
	}
	else if (m_bCarryBack)
	{
		MoveToWP();
		PutBackWP(rows, cols);
		MoveToPreparePos(rows, cols, false);
		SetSampleStateSemiMode(rows, cols, true);
		m_bCarryBack = false;
	}
}

void CPage2::OnBnClickedBtnCarry()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}	
	int rows = GetDlgItemInt(IDC_EDIT_ROW);
	int cols = GetDlgItemInt(IDC_EDIT_COL);
	if (rows>4 || rows<1 || cols>2 || cols <1)
	{
		MessageBox("行范围在1-4, 列范围在1-2。\n请重新输入！");
		return;
	}
	for (int i=1; i<3; i++)
	{
		for (int j=1; j<5; j++)
			SetSampleStateSemiMode(j, i, false);
	}
	m_bCarry = true;
	
	pSemiRunThread = AfxBeginThread(&SemiAutoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
}

void CPage2::OnBnClickedBtnDetectWpCenter()
{
	// TODO: Add your control notification handler code here
	m_bDetectWpCenter = true;
//	CWinThread *pSemiRunThread = NULL;
	pSemiRunThread = AfxBeginThread(&SemiAutoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);	
}

void CPage2::OnBnClickedBtnGlueStart()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}
	m_bGlueStart = true;
//	CWinThread *pSemiRunThread = NULL;
	pSemiRunThread = AfxBeginThread(&SemiAutoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);	
}

void CPage2::OnBnClickedBtnMountGlassCircle()
{
	// TODO: Add your control notification handler code here
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}
	int rows = GetDlgItemInt(IDC_EDIT_ROW);
	int cols = GetDlgItemInt(IDC_EDIT_COL);
	if (rows>4 || rows<1 || cols>2 || cols <1)
	{
		MessageBox("行范围在1-4, 列范围在1-2。\n请重新输入！");
		return;
	}
	m_bMountGlassCircle = true;
//	CWinThread *pSemiRunThread = NULL;
	pSemiRunThread = AfxBeginThread(&SemiAutoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
}

void CPage2::OnBnClickedBtnDetectGcCenter()
{
	// TODO: Add your control notification handler code here
	m_bDetectGcCenter = true;
//	CWinThread *pSemiRunThread = NULL;
	pSemiRunThread = AfxBeginThread(&SemiAutoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);	
}

void CPage2::OnBnClickedBtnCarryBack()
{
	// TODO: Add your control notification handler code here	
	if (!g_bIsInit)
	{
		AfxMessageBox(_T("请将系统归零并初始化！"));
		return;
	}
	int rows = GetDlgItemInt(IDC_EDIT_ROW);
	int cols = GetDlgItemInt(IDC_EDIT_COL);
	if (rows>4 || rows<1 || cols>2 || cols <1)
	{
		MessageBox("行范围在1-4, 列范围在1-2。\n请重新输入！");
		return;
	}
	m_bCarryBack = true;
//	CWinThread *pSemiRunThread = NULL;
	pSemiRunThread = AfxBeginThread(&SemiAutoThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);
}

//////////////////半自动运行模式//////////////////////

void CPage2::OnBnClickedBtnAllGoHome()
{
	// TODO: Add your control notification handler code here
	AllGoHome();
	Sleep(3000);
	AllClear();
	m_bIsAllHome = true;

}

void CPage2::OnBnClickedBtnAllInit()
{
	// TODO: Add your control notification handler code here
	if (!m_bIsAllHome)
	{
		AfxMessageBox(_T("请先将系统归零！"));
		return;
	}
	AllClear();
	Sleep(50);
	CWinThread *pInitAllThread = NULL;
	pInitAllThread = AfxBeginThread(&InitAllThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL);

}

UINT CPage2::InitAllThreadFunc(LPVOID lpParam)
{
	CPage2 *pPage2 = (CPage2*)lpParam;
	pPage2->InitAll();
	return 0;
}

void CPage2::InitAll()
{
	AllInit();
	g_bIsInit = true;
	Sleep(3000);
	ssp2_set_current_card(0);
	Sleep(1000);
	ssp2_set_current_card(4);
	Sleep(1000);
	ssp2_set_current_card(12);
}

void CPage2::OnBnClickedPauseSemi()
{
	// TODO: Add your control notification handler code here
	DWORD dwCode;
	GetExitCodeThread(pSemiRunThread->m_hThread, &dwCode);
	TerminateThread(pSemiRunThread->m_hThread, dwCode);
	CloseHandle(pSemiRunThread->m_hThread);
	int cardIndex = ssp2_get_current_card();
	for (int i=0; i<4; i++)
		AxisStop(cardIndex, i);
}

void CPage2::OnBnClickedResumePosSemi()
{
	// TODO: Add your control notification handler code here
	int rows = GetDlgItemInt(IDC_EDIT_ROW);
	int cols = GetDlgItemInt(IDC_EDIT_COL);

	CString		fileName(_T(""));	
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);

	if (rows == 1)
		fileName = _T(".\\CONFIG\\CLIP\\11P.ini");
	else if (rows == 2)
		fileName = _T(".\\CONFIG\\CLIP\\21P.ini");
	else if (rows == 3)
		fileName = _T(".\\CONFIG\\CLIP\\31P.ini");
	else if (rows == 4)
		fileName = _T(".\\CONFIG\\CLIP\\41P.ini");

	ssp2_set_current_card(4);
	//z+
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(800);
	//x
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(500);
	//y
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 100, 800, 1, 2, 0.5, 0.4);
	Sleep(1200);
	ssp2_set_current_card(0);
	GlueMoveSafePos();
	Sleep(1200);
	InitSampleStagePos();
}

void CPage2::GetGluePinPosErr()
{
	CString		strFileName(_T(".\\CONFIG\\GLUE\\GluePinOffSet.ini"));
	char			cPosErr[20];
	CString		strPosErr(_T(""));	
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPosErr, 20, strFileName);
	strPosErr = cPosErr;
	m_posErrX = atoi(strPosErr);
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPosErr, 20, strFileName);
	strPosErr = cPosErr;
	m_posErrY = atoi(strPosErr);
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPosErr, 20, strFileName);
	strPosErr = cPosErr;
	m_posErrZ = atoi(strPosErr);
}

void CPage2::GetGlueAbsorbPosErr()
{
	CString		strFileName(_T(".\\CONFIG\\GLUE\\GlueSuckOffSet.ini"));
	char			cPosErr[20];
	CString		strPosErr(_T(""));	
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPosErr, 20, strFileName);
	strPosErr = cPosErr;
	m_posErrX = atoi(strPosErr);
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPosErr, 20, strFileName);
	strPosErr = cPosErr;
	m_posErrY = atoi(strPosErr);
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPosErr, 20, strFileName);
	strPosErr = cPosErr;
	m_posErrZ = atoi(strPosErr);
}