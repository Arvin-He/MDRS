// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "MDRS.h"
#include "Page1.h"
#include "afxdialogex.h"
#include "MAC2SSPv102.h"
#include "DirectPort.h"
#include "epplib.h"
#include "Global.h"
#include "EppInfoDlg.h"
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#define  BaseEpp 0xE010   //EPP�˿ڵĻ���ַ,8bit���ݼĴ����ĵ�ַ�� 0x379��״̬�Ĵ�����0x37A�ǿ��ƼĴ���


// CPage1 dialog

IMPLEMENT_DYNAMIC(CPage1, CDialogEx)

	CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage1::IDD, pParent), m_sampleStageDir(1)
{
	m_bLightGreen = false;
	m_bLightYellow = false;
	m_bLightRed = false;
	m_bBeepWarn = false;
//΢������ʼ������
	m_iMachine0=0;			//�����
	m_iChannel0=0;			//ͨ����
	m_MinVoltage0=-5.0;		//��С��ѹ
	m_MaxVoltage0=130.0;		//����ѹ
	m_MinInterval0=0.0;		//��Сλ��
	m_MaxInterval0=25.24;	//���λ��
	m_MinDA0=31676.0;		//��СDA
	m_MaxDA0=61165.0;		//���DA
	m_MinAD0=31862.0;		//��СAD
	m_MaxAD0=65535.0;		//���AD

//	pData = NULL;

	m_iChannel1=1;			//ͨ����
	m_MinVoltage1=-5.0;		//��С��ѹ
	m_MaxVoltage1=125.0;		//����ѹ
	m_MinInterval1=0.0;		//��Сλ��
	m_MaxInterval1=25.84;	//���λ��
	m_MinDA1=31676.0;		//��СDA
	m_MaxDA1=60073.0;	//���DA
	m_MinAD1=33683.0;	//��СAD
	m_MaxAD1=65535.0;		//���AD
}

CPage1::~CPage1()
{
	//	KillTimer(0);
	ssp2_close();
}

void CPage1::DoDataExchange(CDataExchange* pDX) 
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPage1, CDialogEx)
	ON_COMMAND_EX(IDC_CLIP_ROBOT_X_AHEAD, OnClipRobotOpera)
	ON_COMMAND_EX(IDC_CLIP_ROBOT_X_BACK, OnClipRobotOpera)
	ON_COMMAND_EX(IDC_CLIP_ROBOT_Y_AHEAD, OnClipRobotOpera)
	ON_COMMAND_EX(IDC_CLIP_ROBOT_Y_BACK, OnClipRobotOpera)
	ON_COMMAND_EX(IDC_CLIP_ROBOT_Z_AHEAD, OnClipRobotOpera)
	ON_COMMAND_EX(IDC_CLIP_ROBOT_Z_BACK, OnClipRobotOpera)

	ON_COMMAND_EX(IDC_GLUE_ROBOT_X_AHEAD, OnGlueRobotOpera)
	ON_COMMAND_EX(IDC_GLUE_ROBOT_X_BACK, OnGlueRobotOpera)
	ON_COMMAND_EX(IDC_GLUE_ROBOT_Y_AHEAD, OnGlueRobotOpera)
	ON_COMMAND_EX(IDC_GLUE_ROBOT_Y_BACK, OnGlueRobotOpera)
	ON_COMMAND_EX(IDC_GLUE_ROBOT_Z_AHEAD, OnGlueRobotOpera)
	ON_COMMAND_EX(IDC_GLUE_ROBOT_Z_BACK, OnGlueRobotOpera)

	ON_COMMAND_EX(IDC_CCD_CTRL_STAGE_Y_AHEAD, OnCCDStageOpera)
	ON_COMMAND_EX(IDC_CCD_CTRL_STAGE_Y_BACK, OnCCDStageOpera)
	ON_COMMAND_EX(IDC_CCD_CTRL_STAGE_Z_AHEAD, OnCCDStageOpera)
	ON_COMMAND_EX(IDC_CCD_CTRL_STAGE_Z_BACK, OnCCDStageOpera)

	ON_COMMAND_EX(IDC_SAMPLE_STAGE_X_AHEAD, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_SAMPLE_STAGE_X_BACK, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_SAMPLE_STAGE_Y_AHEAD, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_SAMPLE_STAGE_Y_BACK, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_SAMPLE_STAGE_ROTATE_POSITIVE, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_SAMPLE_STAGE_ROTATE_NEGATIVE, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_POSITIVE_ROTATE_CLAW, OnSampleStageOpera)
	ON_COMMAND_EX(IDC_NEGATIVE_ROTATE_CLAW, OnSampleStageOpera)

	ON_COMMAND_EX(IDC_MICRO_STAGE_X_AHEAD, OnMicroStageOpera)
	ON_COMMAND_EX(IDC_MICRO_STAGE_X_BACK, OnMicroStageOpera)
	ON_COMMAND_EX(IDC_MICRO_STAGE_Y_AHEAD, OnMicroStageOpera)
	ON_COMMAND_EX(IDC_MICRO_STAGE_Y_BACK, OnMicroStageOpera)

	ON_COMMAND_EX(IDC_CHECK_CLIP_X_AXIS, OnClipRobotGoHome)
	ON_COMMAND_EX(IDC_CHECK_CLIP_Y_AXIS, OnClipRobotGoHome)
	ON_COMMAND_EX(IDC_CHECK_CLIP_Z_AXIS, OnClipRobotGoHome)
	ON_COMMAND_EX(IDC_CHECK_GLUE_X_AXIS, OnGlueRobotGoHome)
	ON_COMMAND_EX(IDC_CHECK_GLUE_Y_AXIS, OnGlueRobotGoHome)
	ON_COMMAND_EX(IDC_CHECK_GLUE_Z_AXIS, OnGlueRobotGoHome)
	ON_COMMAND_EX(IDC_CHECK_CCD_Y_AXIS, OnCCDStageGoHome)
	ON_COMMAND_EX(IDC_CHECK_CCD_Z_AXIS, OnCCDStageGoHome)
	ON_COMMAND_EX(IDC_CHECK_SAMPLE_STAGE_X_AXIS, OnSampleStageGoHome)
	ON_COMMAND_EX(IDC_CHECK_SAMPLE_STAGE_Y_AXIS, OnSampleStageGoHome)
	ON_COMMAND_EX(IDC_CHECK_SAMPLE_STAGE_ROTATE_AXIS, OnRotateAxisGoHome)
	ON_COMMAND_EX(IDC_CHECK_GLUE_U_AXIS, OnRotateAxisGoHome)

	ON_BN_CLICKED(IDC_CLIP_ROBOT_ALL_CHECK, &CPage1::OnBnClickedClipRobotAllCheck)
	ON_BN_CLICKED(IDC_GLUE_ROBOT_ALL_CHECK, &CPage1::OnBnClickedGlueRobotAllCheck)
	ON_BN_CLICKED(IDC_CCD_ROBOT_ALL_CHECK, &CPage1::OnBnClickedCcdRobotAllCheck)
	ON_BN_CLICKED(IDC_SAMPLE_STAGE_ALL_CHECK, &CPage1::OnBnClickedSampleStageAllCheck)

	ON_COMMAND_EX(IDC_CHECK_CLIP_X_AXIS_RESET, OnClipRobotReset)
	ON_COMMAND_EX(IDC_CHECK_CLIP_Y_AXIS_RESET, OnClipRobotReset)
	ON_COMMAND_EX(IDC_CHECK_CLIP_Z_AXIS_RESET, OnClipRobotReset)

	ON_COMMAND_EX(IDC_CHECK_GLUE_X_AXIS_RESET, OnGlueRobotReset)
	ON_COMMAND_EX(IDC_CHECK_GLUE_Y_AXIS_RESET, OnGlueRobotReset)
	ON_COMMAND_EX(IDC_CHECK_GLUE_Z_AXIS_RESET, OnGlueRobotReset)

	ON_COMMAND_EX(IDC_CHECK_CCD_Y_AXIS_RESET, OnCCDRobotReset)
	ON_COMMAND_EX(IDC_CHECK_CCD_Z_AXIS_RESET, OnCCDRobotReset)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FRAP_CLAW, &CPage1::OnBnClickedFrapClaw)
	ON_BN_CLICKED(IDC_PUFF_CLAW, &CPage1::OnBnClickedPuffClaw)
	ON_BN_CLICKED(IDC_DROPPING_GLUE, &CPage1::OnBnClickedDroppingGlue)
	ON_BN_CLICKED(IDC_SUCK_CHIP, &CPage1::OnBnClickedSuckChip)
	ON_BN_CLICKED(IDC_CLIP_ROBOT_ALL_CHECK_RESET, &CPage1::OnBnClickedClipRobotAllCheckReset)
	ON_BN_CLICKED(IDC_GLUE_ROBOT_ALL_CHECK_RESET, &CPage1::OnBnClickedGlueRobotAllCheckReset)
	ON_BN_CLICKED(IDC_CCD_ROBOT_ALL_CHECK_RESET, &CPage1::OnBnClickedCcdRobotAllCheckReset)
	ON_BN_CLICKED(IDC_SAMPLE_STAGE_ALL_CHECK_RESET, &CPage1::OnBnClickedSampleStageAllCheckReset)
	ON_BN_CLICKED(IDC_ROTATE_AXIS_ALL_CHECK, &CPage1::OnBnClickedRotateAxisAllCheck)
	ON_BN_CLICKED(IDC_SAMPLE_STAGE_SET_ZERO, &CPage1::OnBnClickedSampleStageSetZero)
	ON_BN_CLICKED(IDC_ROTATE_AXIS_SET_ZERO, &CPage1::OnBnClickedRotateAxisSetZero)
	ON_BN_CLICKED(IDC_RELEASE_CHIP, &CPage1::OnBnClickedReleaseChip)
	ON_BN_CLICKED(IDC_GREEN_LIGHT, &CPage1::OnBnClickedGreenLight)
	ON_BN_CLICKED(IDC_YELLOW_LIGHT, &CPage1::OnBnClickedYellowLight)
	ON_BN_CLICKED(IDC_RED_LIGHT, &CPage1::OnBnClickedRedLight)
	ON_BN_CLICKED(IDC_WARNING, &CPage1::OnBnClickedWarning)

	ON_BN_CLICKED(IDC_BTN_MICRO_STAGE_INIT, &CPage1::OnBnClickedBtnMicroStageInit)
	ON_BN_CLICKED(IDC_BTN_CHECK_EPP_INFO, &CPage1::OnBnClickedBtnCheckEppInfo)
END_MESSAGE_MAP()


// CPage1 message handlers
BOOL CPage1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	//�о�̨����
	CComboBox* pClipRobotComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CLIP_ROBOT_STEPSIZE);
	pClipRobotComboBox->AddString(_T("1"));
	pClipRobotComboBox->AddString(_T("2"));
	pClipRobotComboBox->AddString(_T("5"));
	pClipRobotComboBox->AddString(_T("10"));
	pClipRobotComboBox->AddString(_T("20"));
	pClipRobotComboBox->AddString(_T("50"));
	pClipRobotComboBox->AddString(_T("100"));
	pClipRobotComboBox->AddString(_T("200"));
	pClipRobotComboBox->AddString(_T("500"));
	pClipRobotComboBox->AddString(_T("1000"));
	pClipRobotComboBox->AddString(_T("3000"));
	pClipRobotComboBox->AddString(_T("5000"));
	pClipRobotComboBox->SetCurSel(5);
	//��ӿ��ư�ťͼ��
	CButton *pClipRobotBtnXAhead=(CButton*)GetDlgItem(IDC_CLIP_ROBOT_X_AHEAD);
	pClipRobotBtnXAhead->ModifyStyle(0,BS_BITMAP);
	pClipRobotBtnXAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_AHEAD)));

	CButton *pClipRobotBtnXBack=(CButton*)GetDlgItem(IDC_CLIP_ROBOT_X_BACK);
	pClipRobotBtnXBack->ModifyStyle(0,BS_BITMAP);
	pClipRobotBtnXBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_BACK)));

	CButton *pClipRobotBtnYAhead=(CButton*)GetDlgItem(IDC_CLIP_ROBOT_Y_AHEAD);
	pClipRobotBtnYAhead->ModifyStyle(0,BS_BITMAP);
	pClipRobotBtnYAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_AHEAD)));

	CButton *pClipRobotBtnYBack=(CButton*)GetDlgItem(IDC_CLIP_ROBOT_Y_BACK);
	pClipRobotBtnYBack->ModifyStyle(0,BS_BITMAP);
	pClipRobotBtnYBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_BACK)));

	CButton *pClipRobotBtnZAhead=(CButton*)GetDlgItem(IDC_CLIP_ROBOT_Z_AHEAD);
	pClipRobotBtnZAhead->ModifyStyle(0,BS_BITMAP);
	pClipRobotBtnZAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Z_AHEAD)));

	CButton *pClipRobotBtnZBack=(CButton*)GetDlgItem(IDC_CLIP_ROBOT_Z_BACK);
	pClipRobotBtnZBack->ModifyStyle(0,BS_BITMAP);
	pClipRobotBtnZBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Z_BACK)));

	//�㽺��е�ֲ���
	CComboBox* pGlueRobotComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_GLUE_ROBOT_STEPSIZE);
	pGlueRobotComboBox->AddString(_T("1"));
	pGlueRobotComboBox->AddString(_T("2"));
	pGlueRobotComboBox->AddString(_T("5"));
	pGlueRobotComboBox->AddString(_T("10"));
	pGlueRobotComboBox->AddString(_T("20"));
	pGlueRobotComboBox->AddString(_T("50"));
	pGlueRobotComboBox->AddString(_T("100"));
	pGlueRobotComboBox->AddString(_T("200"));
	pGlueRobotComboBox->AddString(_T("500"));
	pGlueRobotComboBox->AddString(_T("1000"));
	pGlueRobotComboBox->AddString(_T("3000"));
	pGlueRobotComboBox->AddString(_T("5000"));
	pGlueRobotComboBox->SetCurSel(6);
	//��ӿ��ư�ťͼ��
	CButton *pGlueRobotBtnXAhead=(CButton*)GetDlgItem(IDC_GLUE_ROBOT_X_AHEAD);
	pGlueRobotBtnXAhead->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnXAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_AHEAD)));

	CButton *pGlueRobotBtnXBack=(CButton*)GetDlgItem(IDC_GLUE_ROBOT_X_BACK);
	pGlueRobotBtnXBack->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnXBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_BACK)));

	CButton *pGlueRobotBtnYAhead=(CButton*)GetDlgItem(IDC_GLUE_ROBOT_Y_AHEAD);
	pGlueRobotBtnYAhead->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnYAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_AHEAD)));

	CButton *pGlueRobotBtnYBack=(CButton*)GetDlgItem(IDC_GLUE_ROBOT_Y_BACK);
	pGlueRobotBtnYBack->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnYBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_BACK)));

	CButton *pGlueRobotBtnZAhead=(CButton*)GetDlgItem(IDC_GLUE_ROBOT_Z_AHEAD);
	pGlueRobotBtnZAhead->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnZAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Z_AHEAD)));

	CButton *pGlueRobotBtnZBack=(CButton*)GetDlgItem(IDC_GLUE_ROBOT_Z_BACK);
	pGlueRobotBtnZBack->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnZBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Z_BACK)));

	CButton *pGlueRobotBtnPositiveRotate=(CButton*)GetDlgItem(IDC_POSITIVE_ROTATE_CLAW);
	pGlueRobotBtnPositiveRotate->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnPositiveRotate->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_POSITIVE_ROTATE)));

	CButton *pGlueRobotBtnNegativeRotate=(CButton*)GetDlgItem(IDC_NEGATIVE_ROTATE_CLAW);
	pGlueRobotBtnNegativeRotate->ModifyStyle(0,BS_BITMAP);
	pGlueRobotBtnNegativeRotate->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_NEGATIVE_ROTATE)));

	//CCD����ͷ�ƶ�����
	CComboBox* pCCDRobotComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CCD_ROBOT_STEPSIZE);
	pCCDRobotComboBox->AddString(_T("1"));
	pCCDRobotComboBox->AddString(_T("2"));
	pCCDRobotComboBox->AddString(_T("5"));
	pCCDRobotComboBox->AddString(_T("10"));
	pCCDRobotComboBox->AddString(_T("20"));
	pCCDRobotComboBox->AddString(_T("50"));
	pCCDRobotComboBox->AddString(_T("100"));
	pCCDRobotComboBox->AddString(_T("200"));
	pCCDRobotComboBox->AddString(_T("500"));
	pCCDRobotComboBox->AddString(_T("1000"));
	pCCDRobotComboBox->AddString(_T("3000"));
	pCCDRobotComboBox->SetCurSel(4);
	//��ӿ��ư�ťͼ��
	CButton *pCCDRobotBtnYAhead=(CButton*)GetDlgItem(IDC_CCD_CTRL_STAGE_Y_AHEAD);
	pCCDRobotBtnYAhead->ModifyStyle(0,BS_BITMAP);
	pCCDRobotBtnYAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_AHEAD)));

	CButton *pCCDRobotBtnYBack=(CButton*)GetDlgItem(IDC_CCD_CTRL_STAGE_Y_BACK);
	pCCDRobotBtnYBack->ModifyStyle(0,BS_BITMAP);
	pCCDRobotBtnYBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_BACK)));

	CButton *pCCDRobotBtnZAhead=(CButton*)GetDlgItem(IDC_CCD_CTRL_STAGE_Z_AHEAD);
	pCCDRobotBtnZAhead->ModifyStyle(0,BS_BITMAP);
	pCCDRobotBtnZAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Z_AHEAD)));

	CButton *pCCDRobotBtnZBack=(CButton*)GetDlgItem(IDC_CCD_CTRL_STAGE_Z_BACK);
	pCCDRobotBtnZBack->ModifyStyle(0,BS_BITMAP);
	pCCDRobotBtnZBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Z_BACK)));

	//��Ʒ̨�˶�����
	CComboBox* pSampleStageComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SAMPLE_STAGE_STEPSIZE);
	pSampleStageComboBox->AddString(_T("2"));
	pSampleStageComboBox->AddString(_T("5"));
	pSampleStageComboBox->AddString(_T("10"));
	pSampleStageComboBox->AddString(_T("20"));
	pSampleStageComboBox->AddString(_T("50"));
	pSampleStageComboBox->AddString(_T("100"));
	pSampleStageComboBox->AddString(_T("200"));
	pSampleStageComboBox->AddString(_T("500"));
	pSampleStageComboBox->AddString(_T("1000"));
	pSampleStageComboBox->AddString(_T("3000"));
	pSampleStageComboBox->AddString(_T("5000"));
	pSampleStageComboBox->SetCurSel(6);
	//��ӿ��ư�ťͼ��
	CButton *pSampleStageBtnXAhead=(CButton*)GetDlgItem(IDC_SAMPLE_STAGE_X_AHEAD);
	pSampleStageBtnXAhead->ModifyStyle(0,BS_BITMAP);
	pSampleStageBtnXAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_AHEAD)));

	CButton *pSampleStageBtnXBack=(CButton*)GetDlgItem(IDC_SAMPLE_STAGE_X_BACK);
	pSampleStageBtnXBack->ModifyStyle(0,BS_BITMAP);
	pSampleStageBtnXBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_BACK)));

	CButton *pSampleStageBtnYAhead=(CButton*)GetDlgItem(IDC_SAMPLE_STAGE_Y_AHEAD);
	pSampleStageBtnYAhead->ModifyStyle(0,BS_BITMAP);
	pSampleStageBtnYAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_AHEAD)));

	CButton *pSampleStageBtnYBack=(CButton*)GetDlgItem(IDC_SAMPLE_STAGE_Y_BACK);
	pSampleStageBtnYBack->ModifyStyle(0,BS_BITMAP);
	pSampleStageBtnYBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_BACK)));

	CButton *pSampleStageBtnPositiveRotate=(CButton*)GetDlgItem(IDC_SAMPLE_STAGE_ROTATE_POSITIVE);
	pSampleStageBtnPositiveRotate->ModifyStyle(0,BS_BITMAP);
	pSampleStageBtnPositiveRotate->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_POSITIVE_ROTATE)));

	CButton *pSampleStageBtnNegativeRotate=(CButton*)GetDlgItem(IDC_SAMPLE_STAGE_ROTATE_NEGATIVE);
	pSampleStageBtnNegativeRotate->ModifyStyle(0,BS_BITMAP);
	pSampleStageBtnNegativeRotate->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_NEGATIVE_ROTATE)));

	//��תװ����ת����
	CComboBox* pRotateSetupComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_ROTATE_STEPSIZE);
	pRotateSetupComboBox->AddString(_T("5"));
	pRotateSetupComboBox->AddString(_T("10"));
	pRotateSetupComboBox->AddString(_T("50"));
	pRotateSetupComboBox->AddString(_T("100"));
	pRotateSetupComboBox->AddString(_T("500"));
	pRotateSetupComboBox->AddString(_T("1000"));
	pRotateSetupComboBox->AddString(_T("2000"));
	pRotateSetupComboBox->AddString(_T("5000"));
	pRotateSetupComboBox->AddString(_T("10000"));
	pRotateSetupComboBox->AddString(_T("20000"));
	pRotateSetupComboBox->AddString(_T("50000"));
	pRotateSetupComboBox->SetCurSel(6);

	//΢��̨�ƶ�����
	CComboBox* pMicroStageComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MICRO_STAGE_STEPSIZE);
	pMicroStageComboBox->AddString(_T("1"));
	pMicroStageComboBox->AddString(_T("2"));
	pMicroStageComboBox->AddString(_T("5"));
	pMicroStageComboBox->AddString(_T("10"));
	pMicroStageComboBox->AddString(_T("20"));
	pMicroStageComboBox->SetCurSel(2);

	CButton *pMicroStageBtnXAhead=(CButton*)GetDlgItem(IDC_MICRO_STAGE_X_AHEAD);
	pMicroStageBtnXAhead->ModifyStyle(0,BS_BITMAP);
	pMicroStageBtnXAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_AHEAD)));

	CButton *pMicroStageBtnXBack=(CButton*)GetDlgItem(IDC_MICRO_STAGE_X_BACK);
	pMicroStageBtnXBack->ModifyStyle(0,BS_BITMAP);
	pMicroStageBtnXBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_X_BACK)));

	CButton *pMicroStageBtnYAhead=(CButton*)GetDlgItem(IDC_MICRO_STAGE_Y_AHEAD);
	pMicroStageBtnYAhead->ModifyStyle(0,BS_BITMAP);
	pMicroStageBtnYAhead->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_AHEAD)));

	CButton *pMicroStageBtnYBack=(CButton*)GetDlgItem(IDC_MICRO_STAGE_Y_BACK);
	pMicroStageBtnYBack->ModifyStyle(0,BS_BITMAP);
	pMicroStageBtnYBack->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_Y_BACK)));

	//
	CButton *pClipBtn=(CButton*)GetDlgItem(IDC_FRAP_CLAW);
	pClipBtn->ModifyStyle(0,BS_BITMAP);
	pClipBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_CLIP)));
	CButton *pPuffBtn=(CButton*)GetDlgItem(IDC_PUFF_CLAW);
	pPuffBtn->ModifyStyle(0,BS_BITMAP);
	pPuffBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_PUFF)));
	CButton *pGlueBtn=(CButton*)GetDlgItem(IDC_DROPPING_GLUE);
	pGlueBtn->ModifyStyle(0,BS_BITMAP);
	pGlueBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GLUE)));
	CButton *pAbsorbBtn=(CButton*)GetDlgItem(IDC_SUCK_CHIP);
	pAbsorbBtn->ModifyStyle(0,BS_BITMAP);
	pAbsorbBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ABSORB)));
	CButton *pReleaseBtn=(CButton*)GetDlgItem(IDC_RELEASE_CHIP);
	pReleaseBtn->ModifyStyle(0,BS_BITMAP);
	pReleaseBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_RELEASE)));

	CButton *pGLBtn=(CButton*)GetDlgItem(IDC_GREEN_LIGHT);
	pGLBtn->ModifyStyle(0,BS_BITMAP);
	pGLBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN_LIGHT)));
	CButton *pYLBtn=(CButton*)GetDlgItem(IDC_YELLOW_LIGHT);
	pYLBtn->ModifyStyle(0,BS_BITMAP);
	pYLBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_YELLOW_LIGHT)));
	CButton *pRLBtn=(CButton*)GetDlgItem(IDC_RED_LIGHT);
	pRLBtn->ModifyStyle(0,BS_BITMAP);
	pRLBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_RED_LIGHT)));
	CButton *pBWBtn=(CButton*)GetDlgItem(IDC_WARNING);
	pBWBtn->ModifyStyle(0,BS_BITMAP);
	pBWBtn->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BEEP_WARN)));
	
	//�˶����ƿ���ʼ��
	int err = ssp2_initial(); //���ؿ��ĸ���
	int cardsNum = err;		  //��ȡ���ĸ���
	CString str(_T(""));
	if (0 == err)
	{
		str.Format("��ʼ��ʧ��,������:%d.", err);
		AfxMessageBox(str);
	}
	else
	{
		err = ssp2_enum_cards(m_cardArrary);  //ö�����еĿ��������ص�ǰ����
		if (-1 == err)
		{
			str.Format("Ѱ�ҿ�ʧ��,������:%d.", err); 
			AfxMessageBox(str);
		}
	}

	ssp2_set_current_card(0);
	for (int i = 0; i<4; i++)
		SetInitialCardsPara(0, i);
	ssp2_set_current_card(4);
	for (int i = 0; i<4; i++)
		SetInitialCardsPara(4, i);
	ssp2_set_current_card(12);
	for (int i = 0; i<4; i++)
		SetInitialCardsPara(12, i);
	//���������λ
	//�㽺̨�����λ
	ssp2_set_current_card(0);
	//x
	ssp2_set_soft_limit(0, 1970, 1);
	ssp2_enable_softlimit(0, 1);  //mode=1 ����ֹͣ�� 0=�޶���
	//y
	ssp2_set_soft_limit(1, 2000, 1);
	ssp2_enable_softlimit(1, 1);
	//z
	ssp2_set_soft_limit(2, 1970, 1);
	ssp2_enable_softlimit(2, 1);
	//CCD-y��λ
	ssp2_set_soft_limit(3, 1970, 1);
	ssp2_enable_softlimit(3, 1);

	//�о�̨�����λ
	ssp2_set_current_card(4);
	//x
	ssp2_set_soft_limit(0, 2000, 1);
	ssp2_enable_softlimit(0, 1);  //mode=1 ����ֹͣ�� 0=�޶���
	//y
	ssp2_set_soft_limit(1, 3000, 1);
	ssp2_enable_softlimit(1, 1);
	//z
	ssp2_set_soft_limit(2, 1970, 1);
	ssp2_enable_softlimit(2, 1);
	//CCD-z��λ
	ssp2_set_soft_limit(3, 1970, 1);
	ssp2_enable_softlimit(3, 1);

	//��ʼ��΢��̨
//	InitialMicroStage();
//	WriteInterval(0XE010,iMachine,iChannel, 10 ,MaxInterval,MinInterval,MaxAD,MinAD);

	SetTimer(0, 1000, NULL);

	pClipRobotComboBox = NULL;
	pGlueRobotComboBox = NULL;
	pCCDRobotComboBox = NULL;
	pSampleStageComboBox = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage1::SetInitialCardsPara(int cardIndex, int nCurrAxis)
{
	//0�ſ���4�ſ�alm�Ǹߵ�ƽ��Ч
	if (0 == cardIndex || 4 == cardIndex)
		ssp2_set_alm(nCurrAxis, 1, 0);//����alm���߼��Ͳ���ģʽ//logic:0�͵�ƽ��Ч��1�ߵ�ƽ��Ч
	//12�ſ�alm�ǵ͵�ƽ��Ч
	else if (12 == cardIndex)
		ssp2_set_alm(nCurrAxis, 0, 0);//����alm���߼��Ͳ���ģʽ//logic:0�͵�ƽ��Ч��1�ߵ�ƽ��Ч
	ssp2_set_out_mode(nCurrAxis, 0);//��������߼�
	ssp2_set_inp(nCurrAxis,0,0);//����inp���߼���ʹ��
	ssp2_set_svon(nCurrAxis, 0);

	ssp2_set_erc(nCurrAxis,0,0);//����ERC�߼���ʱ��
	ssp2_set_sd(nCurrAxis,0,0,1);//����SD�߼�������ģʽ���Ƿ���Ч
	ssp2_set_el(nCurrAxis,0);//����EL����ģʽ
	ssp2_set_backlash_comp(nCurrAxis,0,10,100);//���ü�϶����
	ssp2_set_suppress_vibra(nCurrAxis,10,10);//���û�������	
	ssp2_set_encoder_mode(nCurrAxis,0,1,0,0);//���ý������Ľ��뷽ʽ
	ssp2_set_feedback_mode(nCurrAxis,0);//���û����������Դ
	ssp2_set_move_ratio(nCurrAxis,1);//�����ٶȱ���

	return TRUE;
}     
//���������ʱû�õ�
void CPage1::CheckAxisStatus(int cardIndex, int nCurrAxis)
{
	ssp2_set_current_card(cardIndex);
	int err = ssp2_check_continuous_buffer(nCurrAxis);
	if (0 != err)
	{
		MessageBox(_T("�������˶���"));
		return;
	}
	else
	{
		ssp2_set_alm(nCurrAxis, 0, 0);
		ssp2_set_svon(nCurrAxis, 1);
		ssp2_set_inp(nCurrAxis, 1, 0);
	}
}
//��ȡ�ᵱǰ������������ʾ��edit�ؼ���
void CPage1::GetAxisPosition(int nCurrAxis, UINT nID)
{
	CString strPos(_T(""));
//	int pos = ssp2_get_general_counter(nCurrAxis);	//��ȡ��ǰ������Ŀ
	int pos = ssp2_get_command(nCurrAxis);
	strPos.Format(_T("%d"), pos);
	GetDlgItem(nID)->SetWindowText(strPos);
	UpdateData(FALSE);
}

BOOL CPage1::OnClipRobotOpera(UINT nID)
{
	CString strStepsize(_T(""));
	int stepSize(0);
	CComboBox *pClipRobotStepSize = (CComboBox*)GetDlgItem(IDC_COMBO_CLIP_ROBOT_STEPSIZE);
	pClipRobotStepSize->GetWindowText(strStepsize);
	stepSize = atoi(strStepsize);
	ssp2_set_current_card(4);
	switch(nID)
	{
	case IDC_CLIP_ROBOT_X_AHEAD:
		ssp2_start_ps_move(0, 0, stepSize, 100, 1000, 1 , 2, 0.5, 0.4);
		break;
	case  IDC_CLIP_ROBOT_X_BACK:
		ssp2_start_ps_move(0, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_CLIP_ROBOT_Y_AHEAD:
		ssp2_start_ps_move(1, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case  IDC_CLIP_ROBOT_Y_BACK:
		ssp2_start_ps_move(1, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_CLIP_ROBOT_Z_AHEAD:
		ssp2_start_ps_move(2, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case  IDC_CLIP_ROBOT_Z_BACK:
		ssp2_start_ps_move(2, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	}
	pClipRobotStepSize = NULL;
	return TRUE;
}

BOOL CPage1::OnGlueRobotOpera(UINT nID)
{
	CString strStepsize(_T(""));
	int stepSize(0);
	CComboBox *pGlueRobotStepSize = (CComboBox*)GetDlgItem(IDC_COMBO_GLUE_ROBOT_STEPSIZE);
	pGlueRobotStepSize->GetWindowText(strStepsize);
	stepSize = atoi(strStepsize);
	ssp2_set_current_card(0);
	switch(nID)
	{
	case IDC_GLUE_ROBOT_X_AHEAD:
		ssp2_start_ps_move(0, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_GLUE_ROBOT_X_BACK:
		ssp2_start_ps_move(0, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_GLUE_ROBOT_Y_AHEAD:
		ssp2_start_ps_move(1, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_GLUE_ROBOT_Y_BACK:
		ssp2_start_ps_move(1, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_GLUE_ROBOT_Z_AHEAD:
		ssp2_start_ps_move(2, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case  IDC_GLUE_ROBOT_Z_BACK:
		ssp2_start_ps_move(2, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;

	}
	pGlueRobotStepSize = NULL;
	return TRUE;
}

BOOL CPage1::OnCCDStageOpera(UINT nID)
{
	CString strStepsize(_T(""));
	int stepSize(0);
	CComboBox *pCCDStageStepSize = (CComboBox*)GetDlgItem(IDC_COMBO_CCD_ROBOT_STEPSIZE);
	pCCDStageStepSize->GetWindowText(strStepsize);
	stepSize = atoi(strStepsize);

	switch (nID)
	{
	case IDC_CCD_CTRL_STAGE_Y_AHEAD:
		ssp2_set_current_card(4);
		ssp2_start_ps_move(3, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_CCD_CTRL_STAGE_Y_BACK:
		ssp2_set_current_card(4);
		ssp2_start_ps_move(3, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_CCD_CTRL_STAGE_Z_AHEAD:
		ssp2_set_current_card(0);
		ssp2_start_ps_move(3, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_CCD_CTRL_STAGE_Z_BACK:
		ssp2_set_current_card(0);
		ssp2_start_ps_move(3, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	}
	pCCDStageStepSize = NULL;
	return TRUE;
}

BOOL CPage1::OnSampleStageOpera(UINT nID)
{
	CString strStepsize(_T("")),strRotateStepSize(_T(""));
	int stepSize(0), rotateStepSize(0);
	CComboBox *pSampleStageStepSize = (CComboBox*)GetDlgItem(IDC_COMBO_SAMPLE_STAGE_STEPSIZE);
	pSampleStageStepSize->GetWindowText(strStepsize);
	stepSize = atoi(strStepsize);

	CComboBox *pRotateStepSize = (CComboBox*)GetDlgItem(IDC_COMBO_ROTATE_STEPSIZE);
	pRotateStepSize->GetWindowText(strRotateStepSize);
	rotateStepSize = atoi(strRotateStepSize);

	ssp2_set_current_card(12);
	switch (nID)
	{
	case IDC_SAMPLE_STAGE_X_AHEAD:
		ssp2_start_ps_move(0, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_SAMPLE_STAGE_X_BACK:
		ssp2_start_ps_move(0, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_SAMPLE_STAGE_Y_AHEAD:
		ssp2_start_ps_move(1, 0, stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_SAMPLE_STAGE_Y_BACK:
		ssp2_start_ps_move(1, 0, -stepSize, 100, 1000, 1, 2, 0.5, 0.4);
		break;
	case IDC_SAMPLE_STAGE_ROTATE_POSITIVE:
		ssp2_start_ps_move(2, 0, rotateStepSize, 1000, 20000, 1, 2, 0.5, 0.4);
		break;
	case IDC_SAMPLE_STAGE_ROTATE_NEGATIVE:
		ssp2_start_ps_move(2, 0, -rotateStepSize, 1000, 20000, 1, 2, 0.5, 0.4);
		break;
	case IDC_POSITIVE_ROTATE_CLAW:
		ssp2_start_ps_move(3, 0, rotateStepSize, 1000, 20000, 1, 2, 0.5, 0.4);
		break;
	case IDC_NEGATIVE_ROTATE_CLAW:
		ssp2_start_ps_move(3, 0, -rotateStepSize, 1000, 20000, 1, 2, 0.5, 0.4);
		break;
	}

	pSampleStageStepSize = NULL;
	pRotateStepSize = NULL;
	return TRUE;
}

BOOL CPage1::OnMicroStageOpera(UINT nID)
{
	CString strMicroStageStepsize(_T(""));
	int stepSize(0);
	CComboBox *pMicroStageStepSize = (CComboBox*)GetDlgItem(IDC_COMBO_MICRO_STAGE_STEPSIZE);
	pMicroStageStepSize->GetWindowText(strMicroStageStepsize);
	stepSize = atoi(strMicroStageStepsize);

	switch(nID)
	{
	case IDC_MICRO_STAGE_X_AHEAD:
		{
			WriteInterval(0XE010, m_iMachine0, m_iChannel0, stepSize, m_MaxInterval0, m_MinInterval0, m_MaxAD0, m_MinAD0);
			Sleep(1000);
			float realDis = ReadInterval(0XE010, m_iMachine0, m_iChannel0, m_MaxInterval0, m_MinInterval0, m_MaxAD0, m_MinAD0);
			CString strRealDis(_T(""));
			strRealDis.format("%.4f", realDis);
			SetWindowText(strRealDis);
		}
		break;
	case IDC_MICRO_STAGE_X_BACK:
		{
			WriteInterval(0XE010, m_iMachine0, m_iChannel0, -stepSize, m_MaxInterval0, m_MinInterval0, m_MaxAD0, m_MinAD0);
			Sleep(1000);
			float realDis = ReadInterval(0XE010, m_iMachine0, m_iChannel0, m_MaxInterval0, m_MinInterval0, m_MaxAD0, m_MinAD0);
			CString strRealDis(_T(""));
			strRealDis.format("%.4f", realDis);
			SetWindowText(strRealDis);
		}
		
		break;
	case IDC_MICRO_STAGE_Y_AHEAD:
		{
			WriteInterval(0XE010, m_iMachine0, m_iChannel1, stepSize, m_MaxInterval1 ,m_MinInterval1 ,m_MaxAD1 ,m_MinAD1);
			Sleep(1000);
			float realDis = ReadInterval(0XE010, m_iMachine0, m_iChannel1, m_MaxInterval1, m_MinInterval1, m_MaxAD1, m_MinAD1);
			CString strRealDis(_T(""));
			strRealDis.format("%.4f", realDis);
			SetWindowText(strRealDis);
		}
		
		break;
	case IDC_MICRO_STAGE_Y_BACK:
		{
			WriteInterval(0XE010, m_iMachine0, m_iChannel1, -stepSize, m_MaxInterval1 ,m_MinInterval1 ,m_MaxAD1 ,m_MinAD1);
			Sleep(1000);
			float realDis = ReadInterval(0XE010, m_iMachine0, m_iChannel1, m_MaxInterval1, m_MinInterval1, m_MaxAD1, m_MinAD1);
			CString strRealDis(_T(""));
			strRealDis.format("%.4f", realDis);
			SetWindowText(strRealDis);
		}	
		break;
	}
	return TRUE;
}
//�������
BOOL CPage1::OnClipRobotGoHome(UINT nID)
{
	switch(nID)
	{
	case IDC_CHECK_CLIP_X_AXIS:
		{
			int xState = ((CButton*)GetDlgItem(IDC_CHECK_CLIP_X_AXIS))->GetCheck();
			if (xState == 1)
			{
				AxisGoHome(4, 0);//x���ԭ��
				Sleep(1000);
				ssp2_set_command(0, 0);
			}
		}	
		break;
	case IDC_CHECK_CLIP_Y_AXIS:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_CLIP_Y_AXIS))->GetCheck();
			if (yState == 1)
			{
				AxisGoHome(4, 1);//y���ԭ��
				Sleep(1500);
				ssp2_set_command(1, 0);
			}
		}	
		break;
	case IDC_CHECK_CLIP_Z_AXIS:
		{
			int zState = ((CButton*)GetDlgItem(IDC_CHECK_CLIP_Z_AXIS))->GetCheck();
			if (zState == 1)
			{
				AxisGoHome(4, 2);//z���ԭ��
				Sleep(1500);
				ssp2_set_command(2, 0);
			}
		}	
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CPage1::OnGlueRobotGoHome(UINT nID)
{
	switch(nID)
	{
	case IDC_CHECK_GLUE_X_AXIS:
		{
			int xState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_X_AXIS))->GetCheck();
			if (xState == 1)
			{
				AxisGoHome(0, 0);
				Sleep(1000);
				ssp2_set_command(0, 0);
			}
		}	
		break;
	case IDC_CHECK_GLUE_Y_AXIS:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_Y_AXIS))->GetCheck();
			if (yState == 1)
			{
				AxisGoHome(0, 1);
				Sleep(1500);
				ssp2_set_command(1, 0);
			}
		}	
		break;
	case IDC_CHECK_GLUE_Z_AXIS:
		{
			int zState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_Z_AXIS))->GetCheck();
			if (zState == 1)
			{
				AxisGoHome(0, 2);
				Sleep(1000);
				ssp2_set_command(2, 0);
			}
		}	
		break;
	}
	return TRUE;
}

BOOL CPage1::OnCCDStageGoHome(UINT nID)
{
	switch(nID)
	{
	case IDC_CHECK_CCD_Y_AXIS:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_CCD_Y_AXIS))->GetCheck();
			if (yState == 1)
			{
				AxisGoHome(4, 3);//y���ԭ��
				Sleep(500);
			}
		}	
		break;
	case IDC_CHECK_CCD_Z_AXIS:
		{
			int zState = ((CButton*)GetDlgItem(IDC_CHECK_CCD_Z_AXIS))->GetCheck();
			if (zState == 1)
			{
				AxisGoHome(0, 3);//z���ԭ��
				Sleep(500);
			}
		}	
		break;
	}
	return TRUE;
}

BOOL CPage1::OnSampleStageGoHome(UINT nID)
{
	ssp2_set_current_card(12);
	switch(nID)
	{
	case IDC_CHECK_SAMPLE_STAGE_X_AXIS:
		{
			int xState = ((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_X_AXIS))->GetCheck();
			if (xState == 1)
			{
				ssp2_set_home_mode(0, 3, 0, 1, 1, 0);
				ssp2_home_move(0, 1, 100, 2000, 400, 1, 2);
			}
		}	
		break;
	case IDC_CHECK_SAMPLE_STAGE_Y_AXIS:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_Y_AXIS))->GetCheck();
			if (yState == 1)
			{
				ssp2_set_home_mode(1, 3, 0, 1, 1, 0);
				ssp2_home_move(1, 1, 100, 2000, 400, 1, 2);
			}
		}	
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CPage1::OnRotateAxisGoHome(UINT nID)
{
	ssp2_set_current_card(12);
	switch(nID)
	{
	case IDC_CHECK_SAMPLE_STAGE_ROTATE_AXIS:
		{
			int sampleUAixsState = ((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_ROTATE_AXIS))->GetCheck();
			if (sampleUAixsState == 1)
			{
				//����̨��ת����
				ssp2_set_home_mode(2, 0, 0, 1, 1, 0);	//����Ĺ���ģʽ��0����ͬ��X��y�Ĺ���ģʽ
				ssp2_home_move(2, 0, 1000, 10000, 2000, 1, 2);
			}
		}	
		break;
	case IDC_CHECK_GLUE_U_AXIS:
		{
			int glueUAixsState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_U_AXIS))->GetCheck();
			if (glueUAixsState == 1)
			{
				//�㽺����ת����
				ssp2_set_home_mode(3, 0, 0, 1, 1, 0);	//����Ĺ���ģʽ��0����ͬ��X��y�Ĺ���ģʽ
				ssp2_home_move(3, 0, 1000, 10000, 2000, 1, 2);
			}
		}	
		break;
	default:
		break;
	}
	return TRUE;
}

void CPage1::OnBnClickedClipRobotAllCheck()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_CLIP_X_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_CLIP_Y_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_CLIP_Z_AXIS))->SetCheck(1);
	AxisGoHome(4, 0);//x���ԭ��
	Sleep(1000);
	AxisGoHome(4, 1);//y���ԭ��
	Sleep(100);
	AxisGoHome(4, 2);//z���ԭ��
	Sleep(1000);
	ssp2_set_command(0, 0);
	ssp2_set_command(1, 0);
	ssp2_set_command(2, 0);
}

void CPage1::OnBnClickedGlueRobotAllCheck()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_X_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_Y_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_Z_AXIS))->SetCheck(1);	
	AxisGoHome(0, 0);//x���ԭ��
	Sleep(1000);
	AxisGoHome(0, 1);//y���ԭ��
	Sleep(100);
	AxisGoHome(0, 2);//z���ԭ��
	Sleep(1000);
	ssp2_set_command(0, 0);
	ssp2_set_command(1, 0);
	ssp2_set_command(2, 0);
}

void CPage1::OnBnClickedCcdRobotAllCheck()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_CCD_Y_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_CCD_Z_AXIS))->SetCheck(1);
	AxisGoHome(4, 3);//Y���ԭ��
	Sleep(1000);
	ssp2_set_command(3, 0);
	Sleep(10);

	AxisGoHome(0, 3);//z���ԭ��
	Sleep(1000);
	ssp2_set_command(3, 0);
	Sleep(10);
}

void CPage1::OnBnClickedSampleStageAllCheck()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_X_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_Y_AXIS))->SetCheck(1);
	ssp2_set_current_card(12);
	ssp2_set_home_mode(0, 3, 0, 1, 1, 0);//ע�⣺����Ļ��㷽ʽѡ��3
	ssp2_home_move(0, 1, 200, 2000, 400, 1, 2);  //�ڶ���������0���������˶�(Զ��������)�� 1���������˶�
	Sleep(10);
	ssp2_set_home_mode(1, 3, 0, 1, 1, 0);
	ssp2_home_move(1, 1, 200, 2000, 400, 1, 2);
}

void CPage1::OnBnClickedRotateAxisAllCheck()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_ROTATE_AXIS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_U_AXIS))->SetCheck(1);
	ssp2_set_current_card(12);
	//����̨��ת����
	ssp2_set_home_mode(2, 0, 0, 1, 1, 0);	//����Ĺ���ģʽ��0����ͬ��X��y�Ĺ���ģʽ
	ssp2_home_move(2, 0, 1000, 10000, 2000, 1, 2);
	Sleep(100);
	//�㽺����ת����
	ssp2_set_home_mode(3, 0, 0, 1, 1, 0);	//����Ĺ���ģʽ��0����ͬ��X��y�Ĺ���ģʽ
	ssp2_home_move(3, 0, 1000, 10000, 2000, 1, 2);
}
//���㣬��Ʒ̨��CCD���棬Ҫʵʱ��ʾ�ƶ�״̬��Ҫ����Ʒ̨�˶���λ��ɺ�����ֶ����㣬�Զ�����ͼ�����ͺ�
void CPage1::OnBnClickedSampleStageSetZero()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(12);
	ssp2_set_command(0, 0);
	ssp2_set_command(1, 0);
}
//���㣬��ת�������ʱ��ϳ������ֶ�����
void CPage1::OnBnClickedRotateAxisSetZero()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(12);
	ssp2_set_command(2, 0);//��Ʒ̨
	ssp2_set_command(3, 0);//�㽺��
}
//��λ����
BOOL CPage1::OnClipRobotReset(UINT nID)
{
	switch(nID)
	{
	case IDC_CHECK_CLIP_X_AXIS_RESET:
		{
			int xState = ((CButton*)GetDlgItem(IDC_CHECK_CLIP_X_AXIS_RESET))->GetCheck();
			if (xState == 1)
				AxisReset(4, 0);	//x�Ḵλ
		}	
		break;
	case IDC_CHECK_CLIP_Y_AXIS_RESET:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_CLIP_Y_AXIS_RESET))->GetCheck();
			if (yState == 1)
				AxisReset(4, 1);	//y�Ḵλ
		}	
		break;
	case IDC_CHECK_CLIP_Z_AXIS_RESET:
		{
			int zState = ((CButton*)GetDlgItem(IDC_CHECK_CLIP_Z_AXIS_RESET))->GetCheck();
			if (zState == 1)
				AxisReset(4, 2);	//z�Ḵλ
		}	
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CPage1::OnGlueRobotReset(UINT nID)
{
	switch(nID)
	{
	case IDC_CHECK_GLUE_X_AXIS_RESET:
		{
			int xState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_X_AXIS_RESET))->GetCheck();
			if (xState == 1)
				AxisReset(0, 0);	//x�Ḵλ
		}	
		break;
	case IDC_CHECK_GLUE_Y_AXIS_RESET:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_Y_AXIS_RESET))->GetCheck();
			if (yState == 1)
				AxisReset(0, 1);	//y�Ḵλ
		}	
		break;
	case IDC_CHECK_GLUE_Z_AXIS_RESET:
		{
			int zState = ((CButton*)GetDlgItem(IDC_CHECK_GLUE_Z_AXIS_RESET))->GetCheck();
			if (zState == 1)
				AxisReset(0, 2);	//z�Ḵλ
		}	
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CPage1::OnCCDRobotReset(UINT nID)
{
	switch(nID)
	{
	case IDC_CHECK_CCD_Y_AXIS_RESET:
		{
			int yState = ((CButton*)GetDlgItem(IDC_CHECK_CCD_Y_AXIS_RESET))->GetCheck();
			if (yState == 1)
				AxisReset(4, 3);	//u�Ḵλ(CCD��y��)
		}	
		break;
	case IDC_CHECK_CCD_Z_AXIS_RESET:
		{
			int zState = ((CButton*)GetDlgItem(IDC_CHECK_CCD_Z_AXIS_RESET))->GetCheck();
			if (zState == 1)
				AxisReset(0, 3);	//u�Ḵλ(CCD��z��)
		}	
		break;
	default:
		break;
	}
	return TRUE;
}

void CPage1::OnBnClickedClipRobotAllCheckReset()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_CLIP_X_AXIS_RESET))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_CLIP_Y_AXIS_RESET))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_CLIP_Z_AXIS_RESET))->SetCheck(1);
	AxisReset(4, 0);	//x�Ḵλ
	AxisReset(4, 1);	//y�Ḵλ
	AxisReset(4, 2);	//z�Ḵλ
}

void CPage1::OnBnClickedGlueRobotAllCheckReset()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_X_AXIS_RESET))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_Y_AXIS_RESET))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_GLUE_Z_AXIS_RESET))->SetCheck(1);
	AxisReset(0, 0);	//x�Ḵλ
	AxisReset(0, 1);	//y�Ḵλ
	AxisReset(0, 2);	//z�Ḵλ
}

void CPage1::OnBnClickedCcdRobotAllCheckReset()
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_CHECK_CCD_Y_AXIS_RESET))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK_CCD_Z_AXIS_RESET))->SetCheck(1);
	AxisReset(4, 3);	//u�Ḵλ(CCD��y��)
	AxisReset(0, 3);	//u�Ḵλ(CCD��z��)
}

void CPage1::OnBnClickedSampleStageAllCheckReset()
{
	// TODO: Add your control notification handler code here
// 	((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_X_AXIS_RESET))->SetCheck(1);
// 	((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_Y_AXIS_RESET))->SetCheck(1);
// 	((CButton*)GetDlgItem(IDC_CHECK_SAMPLE_STAGE_ROTATE_AXIS_RESET))->SetCheck(1);
// 	ssp2_set_current_card(12);
// 	ssp2_reset();
// 	Sleep(10);
// 	ssp2_initial();
}

void CPage1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 0:
		{
			int currCardIndex = ssp2_get_current_card();		
			if(0 == currCardIndex)
			{
				GetAxisPosition(0, IDC_GLUE_ROBOT_X_POS);
				GetAxisPosition(1, IDC_GLUE_ROBOT_Y_POS);
				GetAxisPosition(2, IDC_GLUE_ROBOT_Z_POS);
				GetAxisPosition(3, IDC_CCD_ROBOT_Z_POS);
			}
			else if (4 == currCardIndex)
			{
				GetAxisPosition(0, IDC_CLIP_ROBOT_X_POS);
				GetAxisPosition(1, IDC_CLIP_ROBOT_Y_POS);
				GetAxisPosition(2, IDC_CLIP_ROBOT_Z_POS);
				GetAxisPosition(3, IDC_CCD_ROBOT_Y_POS);
			}
			else if (12 == currCardIndex)
			{
				GetAxisPosition(0, IDC_SAMPLE_STAGE_X_POS);
				GetAxisPosition(1, IDC_SAMPLE_STAGE_Y_POS);
				GetAxisPosition(2, IDC_SAMPLE_STAGE_U_POS);
				GetAxisPosition(3, IDC_GLUE_ROBOT_U_POS);
			}
		}
		break;
	case 1:
		{
			ReadInterval(m_iMachine0, m_iChannel0, m_currInternalPos, m_MaxInterval0, m_MinInterval0, m_MaxAD0, m_MinAD0);
			ReadInterval(m_iMachine0, m_iChannel1, m_currInternalPos, m_MaxInterval1, m_MinInterval1, m_MaxAD1, m_MinAD0);
		}
		
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPage1::OnBnClickedFrapClaw()
{
	// TODO: Add your control notification handler code here
	FrapClaw();
}

void CPage1::OnBnClickedPuffClaw()
{
	// TODO: Add your control notification handler code here
	PuffClaw();
}

void CPage1::OnBnClickedDroppingGlue()
{
	// TODO: Add your control notification handler code here
	ssp2_set_current_card(12);
	ssp2_start_tarc_move(3, 0, 3000, 3000, 0, 0, 200, 2000, 1.0, 0);
//	ssp2_start_sarc_move(3, 0, 100, 100, 0, 0, 10, 100, 1.0, 0.5, 0);
//	ssp2_start_ps_move(2, )
	Sleep(200);
	DropGlue();
}

void CPage1::OnBnClickedSuckChip()
{
	// TODO: Add your control notification handler code here
	InhaleChip();
}

void CPage1::OnBnClickedReleaseChip()
{
	// TODO: Add your control notification handler code here
	ReleaseChip();
}
/////////////////////******************΢��̨**********************/////////////////////
void CPage1::InitialMicroStage()
{
	InitParallel(0XE010, 0xE000);
	ClearTimeOut(0XE010);		//�����ʱλ
	_outp(0XE010 + 2,0x00);	//����ƼĴ���д��0x00
	Sleep(20);
	if (GetTimeOut(0XE010))
	{
		MessageBox("EPP���ڼ�ⳬʱ��\n ��ȷ�Ͽ������ͼ�����Ѿ���ȷ���ӣ�");
		return;
	}
	if (!EpigynyOpen(BaseEpp, m_iMachine0, m_iChannel0))
	{
		MessageBox("EPP���ڼ��ʧ�ܣ�\n ��ȷ�Ͽ������ͼ�����Ѿ���ȷ���ӣ�");
		return;
	}
	if (!EpigynyOpen(BaseEpp, m_iMachine0, m_iChannel1))
	{
		MessageBox("EPP���ڼ��ʧ�ܣ�\n ��ȷ�Ͽ������ͼ�����Ѿ���ȷ���ӣ�");
		return;
	}
	else
		MessageBox("���ڼ��ɹ���\n");

	CFile File;
	File.Open("database.txt", CFile:: modeCreate | CFile::modeWrite, NULL);
	File.Close();		//��䲻��©��������������
	if (!GetEppInfo(BaseEpp, m_iMachine0, m_iChannel0))
		MessageBox("EPP������Ϣ��ȡʧ�ܣ�\n ��ȷ�Ͽ������ͼ�����Ѿ���ȷ���ӣ�");
	if (!GetEppInfo(BaseEpp, m_iMachine0, m_iChannel1))
		MessageBox("EPP������Ϣ��ȡʧ�ܣ�\n ��ȷ�Ͽ������ͼ�����Ѿ���ȷ���ӣ�");
	EpigynyClose(0XE010,0,1);
	SetTimer(1, 500);
	Sleep(20);

// 	int lineNum = 0;
// 	ifstream fin("database.txt", ios::in);
// 	char line[1024]={0};
// 	while(fin.getline(line, sizeof(line)))
// 	{
// 		stringstream data(line);
// 		if (lineNum == 0)
// 		{
// 			for (int i=0; i<10; i++)
// 			{
// 				data >> eppDataBase[0][i];
// 			}	
// 		}
// 		else if (lineNum == 1)
// 		{
// 			for (int i=0; i<10; i++)
// 			{
// 				data >> eppDataBase[1][i];
// 			}
// 		}
// 		lineNum++;
// 	}
// 	fin.clear();
// 	fin.close();

// 	char eppData[20];
// 	
// 
// 	m_iMachine0=0;			//�����
// 	m_iChannel0=0;			//ͨ����
// 	strcpy(eppData, eppDataBase[0][2].c_str());
// 	m_MinInterval0 = atof(eppData);		//��Сλ��
// 	strcpy(eppData, eppDataBase[0][3].c_str());
// 	m_MaxInterval0 = atof(eppData);	//���λ��
// 	strcpy(eppData, eppDataBase[0][4].c_str());
// 	m_MinDA0 = atof(eppData);	//��СDA
// 	strcpy(eppData, eppDataBase[0][5].c_str());
// 	m_MaxDA0 = atof(eppData);		//���DA
// 	strcpy(eppData, eppDataBase[0][6].c_str());
// 	m_MinVoltage0 = atof(eppData);		//��С��ѹ
// 	strcpy(eppData, eppDataBase[0][7].c_str());
// 	m_MaxVoltage0 = atof(eppData);		//����ѹ
// 	strcpy(eppData, eppDataBase[0][8].c_str());
// 	m_MinAD0 = atof(eppData);		//��СAD
// 	strcpy(eppData, eppDataBase[0][9].c_str());
// 	m_MaxAD0 = atof(eppData);		//���AD
// 
// 
// 	strcpy(eppData, eppDataBase[1][2].c_str());
// 	m_MinInterval1 = atof(eppData);		//��Сλ��
// 	strcpy(eppData, eppDataBase[2][3].c_str());
// 	m_MaxInterval1 = atof(eppData);	//���λ��
// 	strcpy(eppData, eppDataBase[1][4].c_str());
// 	m_MinDA1 = atof(eppData);	//��СDA
// 	strcpy(eppData, eppDataBase[1][5].c_str());
// 	m_MaxDA1 = atof(eppData);		//���DA
// 	strcpy(eppData, eppDataBase[1][6].c_str());
// 	m_MinVoltage1 = atof(eppData);		//��С��ѹ
// 	strcpy(eppData, eppDataBase[1][7].c_str());
// 	m_MaxVoltage1 = atof(eppData);		//����ѹ
// 	strcpy(eppData, eppDataBase[1][8].c_str());
// 	m_MinAD1 = atof(eppData);		//��СAD
// 	strcpy(eppData, eppDataBase[1][9].c_str());
// 	m_MaxAD1 = atof(eppData);		//���AD

// 	m_iChannel1=1;			//ͨ����
// 	m_MinVoltage1=-5;		//��С��ѹ
// 	m_MaxVoltage1=125;		//����ѹ
// 	m_MinInterval1=0;		//��Сλ��
// 	m_MaxInterval1=25.84;	//���λ��
// 	m_MinDA1=31676.0;		//��СDA
// 	m_MaxDA1=60073.0;	//���DA
// 	m_MinAD1=33683.0;	//��СAD
// 	m_MaxAD1=65535.0;		//���AD
}

void CPage1::OnBnClickedGreenLight()
{
	// TODO: Add your control notification handler code here
	if (!m_bLightGreen)
	{
		m_bLightGreen = true;
		NormalStateGreen();
	} 
	else
	{
		m_bLightGreen = false;
		ResetIOState();
	}
	
}

void CPage1::OnBnClickedYellowLight()
{
	// TODO: Add your control notification handler code here
	if (!m_bLightYellow)
	{
		m_bLightYellow = true;
		ResetStateYellow();
	} 
	else
	{
		m_bLightYellow = false;
		ResetIOState();
	}
}

void CPage1::OnBnClickedRedLight()
{
	// TODO: Add your control notification handler code here
	if (!m_bLightRed)
	{
		m_bLightRed = true;
		WarningStateRed();
	} 
	else
	{
		m_bLightRed = false;
		ResetIOState();
	}
	
}

void CPage1::OnBnClickedWarning()
{
	// TODO: Add your control notification handler code here
	if (!m_bBeepWarn)
	{
		m_bBeepWarn = true;
		BeepWarning();
	}
	else
	{
		m_bBeepWarn = false;
		ResetIOState();
	}
}

void CPage1::OnBnClickedBtnMicroStageInit()
{
	// TODO: Add your control notification handler code here
	InitialMicroStage();
}
//дtxt
// BOOL CPage1::InitList()
// {
// 	float	temp;
// 	int		i = 0;
// 	TCHAR	szText[1024];
// 
// 	pData	=	new float [1000];
// 	streami	=	fopen("database.txt", "r");
// 	if(streami == NULL)
// 	{
// 		MessageBox(TEXT("�����������ͨ����Ϣ��"));
// 		return FALSE;
// 	}
// 	fseek(streami, 0L, SEEK_SET);
// 	while(!feof(streami))
// 	{
// 		fscanf(streami, "%f", &temp);
// 		pData[i]	=	temp;
// 		i ++;
// 	}
// 	fclose(streami);
// 
// 	iItemNum	=	(i - 1) / 10;
// 
// 	for(i = 0 ; i < iItemNum ; i ++)
// 	{
// 		wsprintf(szText,"  %i.   %d ����  %d ͨ��", i, (int)pData[i * 10], (int)pData[i * 10 + 1]);
// 	}
// 	return TRUE;
// }
//��txt
// BOOL CPage1::UpdateList(int iSel)
// {
// 	int	i,j;
// 	streami	=	fopen("database.txt","wt");
// 	for(i = 0 ; i < iSel ; i ++)
// 	{
// 		for(j = 0 ; j < 10 ; j ++)
// 		{
// 			fprintf(streami,"%12.4f",pData[j + i * 10]);
// 		}
// 		fprintf(streami,"\n");
// 	}
// 
// 	for(i = iSel + 1; i < iItemNum ; i ++)
// 	{
// 		for(j = 0 ; j < 10 ; j ++)
// 		{
// 			fprintf(streami,"%12.4f",pData[j + i * 10]);
// 		}
// 		fprintf(streami,"\n");
// 
// 	}
// 	fclose(streami);
// 
// 	return TRUE;
// }

void CPage1::OnBnClickedBtnCheckEppInfo()
{
	// TODO: Add your control notification handler code here
	CEppInfoDlg eppInfoDlg;
	eppInfoDlg.DoModal();
}
