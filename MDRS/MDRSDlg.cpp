
// MDRSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MDRS.h"
#include "MDRSDlg.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include "ErrorBox.h"
#include "CvvImage.h"
#include <string>
#include <vector>
#include "Global.h"
#include <math.h>
#include "MAC2SSPv102.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef MAX
#define MAX(a,b)      (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)      (((a) < (b)) ? (a) : (b))
#endif
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMDRSDlg dialog
//定义自己的0
#define  MY_ZERO 0.000000001
//const
const HV_RESOLUTION Resolution          = RES_MODE0;
const HV_SNAP_MODE SnapMode             = CONTINUATION;
const HV_BAYER_CONVERT_TYPE ConvertType = BAYER2RGB_NEIGHBOUR1;
const long Gain               = 10;
const long ExposureTint_Upper = 60;
const long ExposureTint_Lower = 1000;
const long ADCLevel           = ADC_LEVEL2;
const int XStart              = 0;
const int YStart              = 0;
const int Width               = 640;
const int Height              = 512;
const HV_SNAP_SPEED SnapSpeed = HIGH_SPEED;


CMDRSDlg::CMDRSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMDRSDlg::IDD, pParent), m_pBmpInfo1(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	

	InitialDHCamera1();
	InitialDHCamera2();	

	m_editGain1 = 4;
	m_editGain2 = 6;
	m_editShutter1 = 80;
	m_editShutter2 = 80;

	m_bDrawCross1  = FALSE;
	m_bDrawCross2  = FALSE;
	m_bIsCapture1 = FALSE;
	m_bIsCapture2 = FALSE;
	m_bHuofuCircle = FALSE;
	m_bSetImgScale = FALSE;

	m_bSetImgScaleH = FALSE;

	m_bDrawLine = FALSE;
	m_bDrawRect = FALSE;
	m_bDrawCircle = FALSE;

	m_LinePtNum = 0;
	m_RectPtNum = 0;
	m_CirclePtNum = 0;

	
	m_bMeasure = FALSE;
	m_dImgScale = 6.4516129;
	m_MeasurePtNum = 0;
	m_MeasurePtNum1 = 0;
	m_bMeasureSrc1 = false;
	
	m_workPieceCenterPt = CPoint(0, 0);
	m_glassCircleCenterPt = CPoint(0, 0);

	m_bCalibrateRectH = FALSE;
	m_bCalibrateCircleH = FALSE;
	m_bDrawCalibrateRectH = FALSE;
	m_bDrawCalibrateCircleH = FALSE;

	m_bCalibrateRectV = FALSE;
	m_bDrawCalibrateRectV = FALSE;
	m_bCalibrateCircleV = FALSE;
	m_bDrawCalibrateCircleV = FALSE;

	m_bHuofuGlassCircle = FALSE;
}

CMDRSDlg::~CMDRSDlg()
{
	
}

void CMDRSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_GAIN1, m_sliderGain1);
	DDX_Control(pDX, IDC_SPIN_GAIN1, m_spinGain1);
	DDX_Control(pDX, IDC_SLIDER_GAIN2, m_sliderGain2);
	DDX_Control(pDX, IDC_SPIN_GAIN2, m_spinGain2);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_SLIDER_SHUTTER2, m_sliderShutter2);
	DDX_Control(pDX, IDC_SPIN_SHUTTER1, m_spinShutter1);
	DDX_Control(pDX, IDC_SPIN_SHUUTTER2, m_spinShutter2);
	DDX_Text(pDX, IDC_EDIT_SHUTTER1, m_editShutter1);
	DDV_MinMaxInt(pDX, m_editShutter1, 0, 100);
	DDX_Text(pDX, IDC_EDIT_SHUTTER2, m_editShutter2);
	DDV_MinMaxInt(pDX, m_editShutter2, 0, 100);
	DDX_Text(pDX, IDC_EDIT_GAIN1, m_editGain1);
	DDV_MinMaxInt(pDX, m_editGain1, 0, 63);
	DDX_Text(pDX, IDC_EDIT_GAIN2, m_editGain2);
	DDV_MinMaxInt(pDX, m_editGain2, 0, 63);
	DDX_Control(pDX, IDC_SLIDER_SHUTTER1, m_sliderShutter1);
	DDX_Control(pDX, IDC_SLIDER_SHUTTER2, m_sliderShutter2);
}

BEGIN_MESSAGE_MAP(CMDRSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SNAP_CHANGE1, OnSnapChange1)	
	ON_MESSAGE(WM_SNAP_CHANGE2, OnSnapChange2)
 	ON_MESSAGE(WM_SNAP_ERROR, OnSnapError)
	ON_MESSAGE(WM_SNAP_STOP1, OnSnapStop1)
	ON_MESSAGE(WM_SNAP_STOP2, OnSnapStop2)

 	ON_COMMAND(IDM_C1_OPEN, &CMDRSDlg::OnCamera1_Open)	
	ON_COMMAND(IDM_C1_STOP, &CMDRSDlg::OnCamera1_Stop)
 	ON_COMMAND(IDM_C1_CLOSE, &CMDRSDlg::OnCamera1_Close)
	ON_COMMAND(IDM_C1_SAVE_PIC, &CMDRSDlg::OnCamera1_SavePic)
	ON_COMMAND(IDM_C1_SAVE_VIDEO, &CMDRSDlg::OnCamera1_SaveVideo)
	ON_COMMAND(IDM_C1_STOP_VIDEO, &CMDRSDlg::OnCamera1_StopVideo)	
	ON_COMMAND(IDM_C2_OPEN, &CMDRSDlg::OnCamera2_Open)
	ON_COMMAND(IDM_C2_STOP, &CMDRSDlg::OnCamera2_Stop)
	ON_COMMAND(IDM_C2_CLOSE, &CMDRSDlg::OnCamera2_Close)
	ON_COMMAND(IDM_C2_SAVE_PIC, &CMDRSDlg::OnCamera2_SavePic)
	ON_COMMAND(IDM_C2_SAVE_VIDEO, &CMDRSDlg::OnCamera2_SaveVideo)
	ON_COMMAND(IDM_C2_STOP_VIDEO, &CMDRSDlg::OnCamera2_StopVideo)
	ON_COMMAND_EX(IDT_CAMERA_ALL_OPEN, &CMDRSDlg::OnCameraAllOpen)
	ON_COMMAND_EX(IDT_CAMERA_ALL_STOP, &CMDRSDlg::OnCameraAllStop)
	ON_COMMAND_EX(IDT_CAMERA_ALL_CLOSE, &CMDRSDlg::OnCameraAllClose)

	ON_MESSAGE(WM_SNAP_STOP1, OnSnapStop1)
	ON_MESSAGE(WM_SNAP_STOP2, OnSnapStop2)

 	ON_WM_CLOSE()
	ON_WM_HSCROLL()

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMDRSDlg::OnTcnSelchangeTab1)
	ON_COMMAND(IDT_SHOW_CROSS, &CMDRSDlg::OnShowCross)
	ON_COMMAND(IDM_IMAGE_PREVIEW, &CMDRSDlg::OnImagePreview)
	ON_COMMAND(IDM_SHOW_IMAGE_SCALE, &CMDRSDlg::OnShowImageScale)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_DRAW_LINE, &CMDRSDlg::OnDrawLineByMouse)
	ON_COMMAND(IDM_DRAW_RECT, &CMDRSDlg::OnDrawRectByMouse)
	ON_COMMAND(IDM_DRAW_CIRCLE, &CMDRSDlg::OnDrawCircleByMouse)
	ON_COMMAND(IDM_WRITE_CONFIG, &CMDRSDlg::OnCCDWriteConfig)
	ON_COMMAND(IDM_READ_CONFIG, &CMDRSDlg::OnCCDReadConfig)
	
	ON_COMMAND(IDM_MEASURE_SRC1, &CMDRSDlg::OnMeasureSrc1)
	ON_COMMAND(IDM_CALIBRATE_RECT_H, &CMDRSDlg::OnCalibrateRectH)
	ON_COMMAND(IDM_CALIBRATE_CIRCLE_H, &CMDRSDlg::OnCalibrateCircleH)

	ON_COMMAND(IDM_DETECT_WP_CENTER, &CMDRSDlg::OnDetectWPCenter_V)
	ON_COMMAND(IDM_DETECT_WP_CENTER_HUOGH, &CMDRSDlg::OnDetectWPCenterHuogh_V)
	ON_COMMAND(IDM_DETECT_GLASS_CENTER, &CMDRSDlg::OnDetectGlassCenter_V)
	ON_COMMAND(IDM_DETECT_GLASS_CENTER_HUOGH, &CMDRSDlg::OnDetectGlassCenterHuogh_V)
	ON_COMMAND(IDM_MEASURE_SRC2, &CMDRSDlg::OnMeasureSrc2)
	ON_COMMAND(IDM_CALIBRATE_RECT_V, &CMDRSDlg::OnCalibrateRectV)
	ON_COMMAND(IDM_CALIBRATE_CIRCLE_V, &CMDRSDlg::OnCalibrateCircleV)
		
END_MESSAGE_MAP()


// CMDRSDlg message handlers

BOOL CMDRSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	ShowWindow(SW_MAXIMIZE);
	// TODO: Add extra initialization here
	SetWindowText(_T("宏微双驱机器人系统"));
	//add menu
	if (!m_menu.LoadMenu(IDR_MENU))
	{
		TRACE0("菜单加载失败！");
		return -1;
	}
	SetMenu(&m_menu);
	//add toolBar
	if (!m_toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP, CRect(4,4,0,0))
		|| !m_toolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("工具栏加载失败！");
		return -1;
	}
	//add toolbar icon
	m_imageList.Create(32, 32, ILC_MASK|ILC_COLOR24, 1, 1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_OPEN));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_STOP));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_CLOSE));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_CENTER));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_PREVIEW));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_SCALE));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_WRITE_CONFIG));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_READ_CONFIG));
//	m_imageList.Add(AfxGetApp()->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GREEN))));
	m_toolBar.GetToolBarCtrl().SetImageList(&m_imageList);
	m_toolBar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_imageList.Detach();
	//set picture control size
	GetDlgItem(IDC_SHOW_PIC_1)->SetWindowPos(NULL, 7, 78, 640, 512, SWP_NOZORDER);
	GetDlgItem(IDC_SHOW_PIC_2)->SetWindowPos(NULL, 655, 78, 640, 512, SWP_NOZORDER);
	GetDlgItem(IDC_PREVIEW_PIC2)->SetWindowPos(NULL, 1315, 78, 320, 256, SWP_NOZORDER);
	GetDlgItem(IDC_PREVIEW_PIC1)->SetWindowPos(NULL, 1315, 360, 320, 256, SWP_NOZORDER);
	m_tab.InsertItem(0, _T("手动控制"));
	m_tab.InsertItem(1, _T("自动控制"));
	m_tab.InsertItem(2, _T("待扩展1"));
	m_tab.InsertItem(3, _T("待扩展2"));

	m_page1.Create(IDD_PAGE1, GetDlgItem(IDC_TAB1));
	m_page2.Create(IDD_PAGE2, GetDlgItem(IDC_TAB1));
 	m_page3.Create(IDD_PAGE3, GetDlgItem(IDC_TAB1));
 	m_page4.Create(IDD_PAGE4, GetDlgItem(IDC_TAB1));
	
	m_page1.EnableWindow(TRUE);
	m_page2.EnableWindow(TRUE);
	m_page3.EnableWindow(TRUE);
	m_page4.EnableWindow(TRUE);

	CRect mainDlgRect(0,0,0,0);
	GetClientRect(&mainDlgRect);
	
	mainDlgRect.top += 600;
	mainDlgRect.left += 10;
	mainDlgRect.bottom -= 10;
	mainDlgRect.right -= 10;
	m_tab.MoveWindow(&mainDlgRect);

	CRect tabRect(0,0,0,0);
	m_tab.GetClientRect(&tabRect);

	tabRect.top += 20;
	tabRect.bottom -= 10;
	tabRect.left += 10;
	tabRect.right -= 10;

	m_page1.MoveWindow(&tabRect);
	m_page2.MoveWindow(&tabRect);
	m_page3.MoveWindow(&tabRect);
	m_page4.MoveWindow(&tabRect);

	m_page1.ShowWindow(SW_SHOW);
	m_page2.ShowWindow(SW_HIDE);
	m_page3.ShowWindow(SW_HIDE);
	m_page4.ShowWindow(SW_HIDE);
	m_tab.SetCurSel(0);

	m_spinGain1.SetRange(0,63);
	m_spinGain1.SetPos(m_editGain1);
	m_spinGain1.SetBuddy(GetDlgItem(IDC_EDIT_GAIN1));
	m_sliderGain1.SetRange(0,63);
	m_sliderGain1.SetPos(m_editGain1);
	m_sliderGain1.SetTicFreq(6);

	m_spinShutter1.SetRange(0,100);
	m_spinShutter1.SetPos(m_editShutter1);
	m_spinShutter1.SetBuddy(GetDlgItem(IDC_EDIT_SHUTTER1));
	m_sliderShutter1.SetRange(0,100);
	m_sliderShutter1.SetPos(m_editShutter1);
	m_sliderShutter1.SetTicFreq(10);

	m_spinGain2.SetRange(0,63);
	m_spinGain2.SetPos(m_editGain2);
	m_spinGain2.SetBuddy(GetDlgItem(IDC_EDIT_GAIN2));
	m_sliderGain2.SetRange(0,63);
	m_sliderGain2.SetPos(m_editGain2);
	m_sliderGain2.SetTicFreq(6);

	m_spinShutter2.SetRange(0,100);
	m_spinShutter2.SetPos(m_editShutter2);
	m_spinShutter2.SetBuddy(GetDlgItem(IDC_EDIT_SHUTTER2));
	m_sliderShutter2.SetRange(0,100);
	m_sliderShutter2.SetPos(m_editShutter2);
	m_sliderShutter2.SetTicFreq(10);
//	UpdateData(FALSE);
//	m_page2.SetMainDlgPtr(this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMDRSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMDRSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMDRSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//////////////////////////CCD公共函数///////////////////////////
//获取bayer格式
HV_BAYER_LAYOUT CMDRSDlg::HVGetBayerType(HHV hhv) 
{
	int nSize = 0; 
	HVSTATUS status = STATUS_OK;;
	HV_BAYER_LAYOUT Layout;
	status = HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, NULL,&nSize);	          
	if(STATUS_OK != status)
		Layout = BAYER_GR;
	else
	{
		BYTE *pbBayerLayout = NULL;
		pbBayerLayout = new BYTE[nSize];
		status = HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, pbBayerLayout,&nSize);
		Layout = (HV_BAYER_LAYOUT)*pbBayerLayout;
		delete []pbBayerLayout; 
		pbBayerLayout=NULL;
	}
	return Layout;
}

/*	函数:SetExposureTime
	输入参数:int nWindWidth			当前图像宽度		
			 int lTintUpper			曝光时间的分子，shutter的取值
			 int lTintLower			曝光时间的分母，与shutter的单位相关	（ms:1000；s:1）
	输出参数:无						
	说明:设置曝光时间（其他的参数如摄像机时钟频率，消隐值都取默认值）*/
HVSTATUS CMDRSDlg::SetExposureTime(HHV hhv, int nWindWidth, long lTintUpper, long lTintLower, 
								   long HBlanking, HV_SNAP_SPEED SnapSpeed, HV_RESOLUTION Resolution)
{
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(hhv, DESC_DEVICE_TYPE, &type, &size);	
	int nOutputWid = nWindWidth;
	double dExposure = 0.0;
	double dTint = MAX((double)lTintUpper/(double)lTintLower,MY_ZERO);
	double lClockFreq = 0.0;  
		
	lClockFreq = (SnapSpeed == HIGH_SPEED)? 24000000:12000000;
	long lTb = HBlanking;
	lTb += 9;
	lTb -= 19;	
	if(lTb <= 0) lTb =0;
	if(((double)nOutputWid + 244.0 + lTb ) > 552)	
		dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 244.0 + lTb);
	else
		dExposure = ((dTint * lClockFreq)+ 180.0) / 552 ;
	if((dExposure-(int)dExposure) >0.5)
		dExposure += 1.0;
	if(dExposure <= 0)
		dExposure = 1;
	else if(dExposure > 16383) 
		dExposure = 16383;

	return HVAECControl(hhv, AEC_EXPOSURE_TIME, (long)dExposure);
}

/*	函数:OnSnapError；
	输入参数:WPARAM wParam 没有使用	,LPARAM lParam 异常状态码；
    输出参数:LRESULT；说明:摄像机采集异常错误报告*/
LRESULT CMDRSDlg::OnSnapError(WPARAM wParam, LPARAM lParam)
{	
	CErrorBox ErrDlg;
	ErrDlg.m_dwStatus = lParam;
	if (ErrDlg.m_dwStatus == dwLastStatus1)
	{
		if (ErrDlg.DoModal()==IDOK)
		{
			OnCamera1_Stop();
			OnCamera1_Open();  
		}
	}
	else if (ErrDlg.m_dwStatus == dwLastStatus2)
	{
		if (ErrDlg.DoModal()==IDOK)
		{
			OnCamera2_Stop();
			OnCamera2_Open();  
		}
	}
 	return 1;
}

LRESULT CMDRSDlg::OnSnapStop1(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status =STATUS_OK;

	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart1 = FALSE;
	return TRUE;
}

LRESULT CMDRSDlg::OnSnapStop2(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv2);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart2 = FALSE;
	return TRUE;
}

BOOL CMDRSDlg::DestroyDHCamera(HHV hDHCamera, BYTE *pRawBuffer, BYTE *pImageBuffer)
{
	HVSTATUS status = STATUS_OK;
	//	关闭数字摄像机，释放数字摄像机内部资源
	status = EndHVDevice(hDHCamera);
	HV_VERIFY(status);
	//	回收图像缓冲区
	delete []pRawBuffer;
	delete []pImageBuffer;
	return TRUE;
}

void CMDRSDlg::ShowImage(Mat img, INT_PTR ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect(0,0,0,0);
	GetDlgItem(ID)->GetClientRect(&rect);
	int tx = (int)(rect.Width() - img.cols)/2;
	int ty = (int)(rect.Height() - img.rows)/2;
	SetRect(rect, tx, ty, tx+img.cols, ty+img.rows);
	CvvImage cimg;
	IplImage temp = img;
	cimg.CopyOf(&temp);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}
////////////////////////////CCD公共函数//////////////////////////////////////////////
////////////////////////////CCD1的函数//////////////////////////////////////////////
HVSTATUS CMDRSDlg::GetLastStatus1()
{
	HV_ARG_GET_LAST_STATUS ArgFeature;
	ArgFeature.type = HV_LAST_STATUS_TRANSFER;	
	HV_RES_GET_LAST_STATUS ResFeature;	
	HVAPI_CONTROL_PARAMETER  p;
	p.pInBuf		 = &ArgFeature;
	p.dwInBufSize	 = sizeof(ArgFeature);	
	p.pOutBuf		 = &ResFeature;
	p.dwOutBufSize	 = sizeof(ResFeature);
	p.pBytesRet		 = NULL;		
	p.code			 = ORD_GET_LAST_STATUS;
	int	dwSize = sizeof(p);
	HVSTATUS status = HVCommand(m_hhv1, CMD_HVAPI_CONTROL, &p, &dwSize);
	if(status != STATUS_OK)
	{
		TRACE("HVCommand return status is %d", status);
	}
	dwLastStatus1 = 0;
	if (HV_SUCCESS(status))
		dwLastStatus1 = ResFeature.status;

	return status;
}

/*	函数:SnapThreadCallback
	输入参数:SNAP_INFO *pInfo；SNAP_INFO结构包括当前数字摄像机SNAP执行状态；输出参数:int						
	说明:数字摄像机采集到内存回调函数，但用户一般不用调用，由用户提供给SDK使用，
	用户在回调函数内实现对采集数据的处理和显示即可*/
int CALLBACK CMDRSDlg::SnapThreadCallback1(HV_SNAP_INFO *pInfo)
{
	CMDRSDlg *This = (CMDRSDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;
	HVSTATUS status = This->GetLastStatus1();	// 读取摄像机图像采集工作状态	
	if(!HV_SUCCESS(status))
	{	//如果函数调用不成功，摄像机停止图像采集。
		::PostMessage(hwnd, WM_SNAP_STOP1, 0, 0);
		return 1;
	}
	TRACE("LastStatus is %X", This->dwLastStatus1);
	if(This->dwLastStatus1 == 0)	// 当状态值等于0时，表示摄像机图像采集正常，则调用SendMessage显示图像；
	{
	   /*发送自定义消息WM_SNAP_EX_CHANGE到主窗口，同时传入当前可以处理的图像序号
		*注意：用SendMessage发送消息，必须等待消息处理完毕后，才能退出整个SendMessage函数*/
		::PostMessage(hwnd, WM_SNAP_CHANGE1, 0, 0);
		return 1;	
	}
	else if(This->dwLastStatus1 == STATUS_FILE_INVALID)// 当状态值不等于-18时，表示摄像机发生1次丢帧行为,在连续高速采集时，1次丢帧行为可能包括1帧以上图像。
	{
		This->m_nLost1++;	//丢帧计数
	}
	else if (This->dwLastStatus1 == 0xc0000012 || This->dwLastStatus1 == 0xc0000011)
	{
		This->m_nError1++;	//错误计数
	}
	else // 当状态值为其他非0值时，表示摄像机图像采集异常
	{
	   //发送自定义消息 WM_SNAP_ERROR 到主窗口，注意：用PostMessage发送消息，不必等待消息处理完，就可以返回。
		::PostMessage(hwnd, WM_SNAP_ERROR, 0, This->dwLastStatus1);
	}	
	return 1;
}

/*	函数:OnSnapChange1
	输入参数:WPARAM wParam	字参数，在消息中为当前可以处理的图像序号; LPARAM lParam	没有使用；
	输出参数:LRESULT；说明:实现对采集数据的处理和显示*/
LRESULT CMDRSDlg::OnSnapChange1(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status = STATUS_OK;	
	//	将原始图像数据进行Bayer转换，转换后为24位。同时将原始数据进行上下翻转
	ConvertBayer2Rgb(m_pImageBuffer1, m_pRawBuffer1, Width, Height, ConvertType, m_pLutR1, m_pLutG1, m_pLutB1, false, m_Layout1);	
	g_src1.data = (uchar*)m_pImageBuffer1;
//	flip(m_src1, m_src1, -1);
	if (m_bDrawCross1)
		DrawCross(g_src1);
	if (m_bCalibrateRectH && m_bDrawCalibrateRectH)
		DrawCalibrateRectH(g_src1);
	if (m_bMeasureSrc1)
		MeasureSrc1(g_src1);
	if (m_bSetImgScaleH)
		SetImgScale(g_src1);

	ShowImage(g_src1, IDC_SHOW_PIC_1);	
	return 1;
}

/*	初始化数字摄像机硬件状态，用户也可以在其他位置初始化数字摄像机，但应保证数字摄像机已经打开，
	建议用户在应用程序初始化时，同时初始化数字摄像机硬件。 */
BOOL CMDRSDlg::InitialDHCamera1()
{
	//	初始化所有成员变量，同时打开数字摄像机
	HVSTATUS status = STATUS_OK;	
	m_bOpen1		= FALSE;
	m_bStart1		= FALSE;
	m_pBmpInfo1		= NULL;
	m_pRawBuffer1	= NULL;
	m_pImageBuffer1	= NULL;
	m_lHBlanking1	= 0;
	m_lVBlanking1	= 0;
	m_nError1		= 0;
	m_nLost1			= 0;
	for(int i=0; i<256; i++)
	{
		m_pLutR1[i] = i;
		m_pLutG1[i] = i;
		m_pLutB1[i] = i;
	}
	//	打开数字摄像机 1
	status = BeginHVDevice(1, &m_hhv1);
	//	检验函数执行状态，如果失败，则返回错误状态消息框
	HV_VERIFY(status);
	m_Layout1 = HVGetBayerType(m_hhv1);
	//	设置数字摄像机分辨率
	HVSetResolution(m_hhv1, Resolution);		
	//	采集模式，包括 CONTINUATION(连续)、TRIGGER(外触发)
	HVSetSnapMode(m_hhv1, SnapMode);	
	//  设置各个分量的增益
	for (int i = 0; i < 4; i++)
	{
		HVAGCControl(m_hhv1, RED_CHANNEL + i, Gain);
	}
	//  设置ADC的级别
	HVADCControl(m_hhv1, ADC_BITS, ADCLevel);
	//  获取设备类型
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv1, DESC_DEVICE_TYPE, &type, &size);	
	//设置消隐。
	HVSetBlanking(m_hhv1, m_lHBlanking1, m_lVBlanking1);
	/*视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内，视频窗口左上角X坐标和窗口宽度应为4的倍数，
	左上角Y坐标和窗口高度应为2的倍数，输出窗口的起始位置一般设置为(0, 0)即可。*/
	HVSetOutputWindow(m_hhv1, XStart, YStart, Width, Height);	
	//设置采集速度
	HVSetSnapSpeed(m_hhv1, SnapSpeed);
	//设置曝光时间
	SetExposureTime(m_hhv1, Width, ExposureTint_Upper, ExposureTint_Lower, m_lHBlanking1, SnapSpeed, Resolution);	
	//	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	m_pBmpInfo1						= (BITMAPINFO *)m_chBmpBuf1;
	//	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo1->bmiHeader.biSize	= sizeof(BITMAPINFOHEADER);
	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo1->bmiHeader.biWidth	= Width;
	//	图像宽度，一般为输出窗口高度
	m_pBmpInfo1->bmiHeader.biHeight	= Height;	
	/*	以下设置一般相同，对于低于8位的位图，还应设置相应的位图调色板*/
	m_pBmpInfo1->bmiHeader.biPlanes			= 1;
	m_pBmpInfo1->bmiHeader.biBitCount		= 24;
	m_pBmpInfo1->bmiHeader.biCompression	= BI_RGB;
	m_pBmpInfo1->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo1->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo1->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo1->bmiHeader.biClrUsed		= 0;
	m_pBmpInfo1->bmiHeader.biClrImportant	= 0;	
	/*	分配原始图像缓冲区，一般用来存储采集图像原始数据
	*  一般图像缓冲区大小由输出窗口大小和视频格式确定。*/
	m_pRawBuffer1 = new BYTE[Width * Height];
	ASSERT(m_pRawBuffer1);
	//分配Bayer转换后图像数据缓冲
	m_pImageBuffer1 = new BYTE[Width * Height * 3];
	ASSERT(m_pImageBuffer1);

	return TRUE;
}
////////////////////////////CCD1的函数//////////////////////////////////////////////
///////////////////////////	CCD2的函数///////////////////////////////////////////////
/*	函数:SnapThreadCallback2
	输入参数:SNAP_INFO *pInfo		SNAP_INFO结构包括当前数字摄像机SNAP执行状态
	输出参数:int						
	说明:数字摄像机采集到内存回调函数，但用户一般不用调用，由用户提供给SDK使用，用户在回调函数内实现对采集数据的处理和显示即可*/
int CALLBACK CMDRSDlg::SnapThreadCallback2(HV_SNAP_INFO *pInfo)
{
	CMDRSDlg *This = (CMDRSDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;
	HVSTATUS status = This->GetLastStatus2();	// 读取摄像机图像采集工作状态	
	if(!HV_SUCCESS(status))
	{	//如果函数调用不成功，摄像机停止图像采集。
		::PostMessage(hwnd, WM_SNAP_STOP2, 0, 0);
		return 1;
	}
	TRACE("LastStatus is %X", This->dwLastStatus2);
	if(This->dwLastStatus2 == 0)	// 当状态值等于0时，表示摄像机图像采集正常，则调用SendMessage显示图像；
	{
	   /*发送自定义消息WM_SNAP_EX_CHANGE到主窗口，同时传入当前可以处理的图像序号
		*注意：用SendMessage发送消息，必须等待消息处理完毕后，才能退出整个SendMessage函数*/
		::PostMessage(hwnd, WM_SNAP_CHANGE2, 0, 0);
		return 1;	
	}
	else if(This->dwLastStatus2 == STATUS_FILE_INVALID)// 当状态值不等于-18时，表示摄像机发生1次丢帧行为,在连续高速采集时，1次丢帧行为可能包括1帧以上图像。
	{
		This->m_nLost2++;	//丢帧计数
	}
	else if (This->dwLastStatus2 == 0xc0000012 || This->dwLastStatus2 == 0xc0000011)
	{
		This->m_nError2++;	//错误计数
	}
	else // 当状态值为其他非0值时，表示摄像机图像采集异常
	{
	   //发送自定义消息 WM_SNAP_ERROR 到主窗口，注意：用PostMessage发送消息，不必等待消息处理完，就可以返回。
		::PostMessage(hwnd, WM_SNAP_ERROR, 0, This->dwLastStatus2);
	}	
	return 1;
}

/*  函数:OnSnapChange2
	输入参数:WPARAM wParam	字参数，在消息中为当前可以处理的图像序号; LPARAM lParam		没有使用
	输出参数:LRESULT		说明:实现对采集数据的处理和显示*/
LRESULT CMDRSDlg::OnSnapChange2(WPARAM wParam, LPARAM lParam)
{
	HVSTATUS status = STATUS_OK;	
	//	将原始图像数据进行Bayer转换，转换后为24位。同时将原始数据进行上下翻转
	ConvertBayer2Rgb(m_pImageBuffer2, m_pRawBuffer2, Width, Height, ConvertType, m_pLutR2, m_pLutG2, m_pLutB2, true, m_Layout2);	
	g_src2.data = (uchar*)m_pImageBuffer2;
	if (m_bDrawCross2)
		DrawCross(g_src2);
	if (m_bSetImgScale)
		SetImgScale(g_src2);
	if (m_bDrawLine)
		DrawLine(g_src2);
	if (m_bDrawRect)
		DrawRect(g_src2);
	if (m_bDrawCircle)
		DrawCircle(g_src2);
	if (m_bMeasure)
		Measure(g_src2);
	ShowImage(g_src2, IDC_SHOW_PIC_2);
	return 1;
}
/*	初始化数字摄像机硬件状态，用户也可以在其他位置初始化数字摄像机，
 *	但应保证数字摄像机已经打开，建议用户在应用程序初始化时，同时初始化数字摄像机硬件。*/
BOOL CMDRSDlg::InitialDHCamera2()
{
	//	初始化所有成员变量，同时打开数字摄像机
	HVSTATUS status = STATUS_OK;	
	m_bOpen2		= FALSE;
	m_bStart2		= FALSE;
	m_pBmpInfo2		= NULL;
	m_pRawBuffer2	= NULL;
	m_pImageBuffer2	= NULL;
	m_lHBlanking2	= 0;
	m_lVBlanking2	= 0;
	m_nError2		= 0;
	m_nLost2		= 0;
	for(int i=0;i<256;i++)
	{
		m_pLutR2[i] = i;
		m_pLutG2[i] = i;
		m_pLutB2[i] = i;
	}
	//	打开数字摄像机 2
	status = BeginHVDevice(2, &m_hhv2);
	//	检验函数执行状态，如果失败，则返回错误状态消息框
	HV_VERIFY(status);
	m_Layout2 = HVGetBayerType(m_hhv2);
	//	设置数字摄像机分辨率
	HVSetResolution(m_hhv2, Resolution);		
	//	采集模式，包括 CONTINUATION(连续)、TRIGGER(外触发)
	HVSetSnapMode(m_hhv2, SnapMode);	
	//  设置各个分量的增益
	for (int i = 0; i < 4; i++)
	{
		HVAGCControl(m_hhv2, RED_CHANNEL + i, Gain);
	}
	//  设置ADC的级别
	HVADCControl(m_hhv2, ADC_BITS, ADCLevel);
	//  获取设备类型
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv2, DESC_DEVICE_TYPE, &type, &size);	
	//设置消隐。
	HVSetBlanking(m_hhv2, m_lHBlanking2, m_lVBlanking2);
	/* 视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内，
	*  视频窗口左上角X坐标和窗口宽度应为4的倍数，左上角Y坐标和窗口高度应为2的倍数
	*  输出窗口的起始位置一般设置为(0, 0)即可。*/
	HVSetOutputWindow(m_hhv2, XStart, YStart, Width, Height);	
	//设置采集速度
	HVSetSnapSpeed(m_hhv2, SnapSpeed);
	//设置曝光时间
	SetExposureTime(m_hhv2, Width, ExposureTint_Upper, ExposureTint_Lower, m_lHBlanking2, SnapSpeed, Resolution);	
	//	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	m_pBmpInfo2						= (BITMAPINFO *)m_chBmpBuf2;
	//	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo2->bmiHeader.biSize	= sizeof(BITMAPINFOHEADER);
	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo2->bmiHeader.biWidth	= Width;
	//	图像宽度，一般为输出窗口高度
	m_pBmpInfo2->bmiHeader.biHeight	= Height;	
	//以下设置一般相同，对于低于8位的位图，还应设置相应的位图调色板
	m_pBmpInfo2->bmiHeader.biPlanes			= 1;
	m_pBmpInfo2->bmiHeader.biBitCount		= 24;
	m_pBmpInfo2->bmiHeader.biCompression	= BI_RGB;
	m_pBmpInfo2->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo2->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo2->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo2->bmiHeader.biClrUsed		= 0;
	m_pBmpInfo2->bmiHeader.biClrImportant	= 0;	
	/*	分配原始图像缓冲区，一般用来存储采集图像原始数据,一般图像缓冲区大小由输出窗口大小和视频格式确定。*/
	m_pRawBuffer2 = new BYTE[Width * Height];
	ASSERT(m_pRawBuffer2);
	//分配Bayer转换后图像数据缓冲
	m_pImageBuffer2 = new BYTE[Width * Height * 3];
	ASSERT(m_pImageBuffer2);

	return TRUE;
}

HVSTATUS CMDRSDlg::GetLastStatus2()
{
	HV_ARG_GET_LAST_STATUS ArgFeature;
	ArgFeature.type = HV_LAST_STATUS_TRANSFER;
	HV_RES_GET_LAST_STATUS ResFeature;
	HVAPI_CONTROL_PARAMETER  p;
	p.pInBuf		 = &ArgFeature;
	p.dwInBufSize	 = sizeof(ArgFeature);	
	p.pOutBuf		 = &ResFeature;
	p.dwOutBufSize	 = sizeof(ResFeature);
	p.pBytesRet		 = NULL;		
	p.code			 = ORD_GET_LAST_STATUS;
	int	dwSize = sizeof(p);
	HVSTATUS status = HVCommand(m_hhv2, CMD_HVAPI_CONTROL, &p, &dwSize);
	if(status != STATUS_OK)
	{
		TRACE("HVCommand return status is %d", status);
	}
	dwLastStatus2 = 0;
	if (HV_SUCCESS(status)) {
		dwLastStatus2 = ResFeature.status;
	}
	return status;
}
///////////////////////////////CCD2的函数///////////////////////////////////////////

///////////////////////////////Menu操作命令/////////////////////////////////////////
///////////////////////////////#1摄像头的菜单操作命令///////////////////////////////
//打开1#摄像头
void CMDRSDlg::OnCamera1_Open()
{
	// TODO: Add your command handler code here
	HVSTATUS status = STATUS_OK;
	//初始化数字摄像机采集图像到内存的控制，指定回调函数SnapThreadCallback和this指针
	status = HVOpenSnap(m_hhv1, SnapThreadCallback1, this);					
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bOpen1 = TRUE;		//标志已经打开Snap环境
	//启动数字摄像机采集图像到内存
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer1;
	status = HVStartSnap(m_hhv1, ppBuf,1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart1 = TRUE;
}
//暂停1#摄像头
void CMDRSDlg::OnCamera1_Stop()
{
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
		m_bStart1 = FALSE;
}
//关闭1#摄像头
void CMDRSDlg::OnCamera1_Close()
{
	// TODO: Add your command handler code here
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status))
		m_bStart1 = FALSE;
	status = HVCloseSnap(m_hhv1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
	{
		m_bOpen1		= FALSE;
		m_bStart1	= FALSE;
		m_nError1 = 0;
		m_nLost1  = 0;
	}
}
//保存1#摄像头图片
void CMDRSDlg::OnCamera1_SavePic()
{
	// TODO: Add your command handler code here
	//	以下保存BMP文件设置基本相同
	Mat saveImg = g_src1.clone();
	CFileDialog dlg(FALSE , "*.bmp", _T("C1img") ,OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, "Bitmap Files(*.bmp)|*.bmp", this);
	if (dlg.DoModal() == IDOK) 
	{
		CString imgPath = dlg.GetPathName();
		if (!imgPath.IsEmpty())
		{
			USES_CONVERSION;
			string imgPathAndName = T2CA(imgPath);
			imwrite(imgPathAndName, saveImg);
		}
		else
		{
			MessageBox(_T("文件路径为空！\n"));
			return;
		}	
	}
	else
		return;
}
//录取1#摄像头的视频，这里录取视频要开线程，否则界面其他按钮不能响应消息，直到该线程执行完毕
void CMDRSDlg::OnCamera1_SaveVideo()
{
	// TODO: Add your command handler code here
	CWinThread *pStoreVideoThread = NULL;
	pStoreVideoThread = AfxBeginThread(&StoreVideoThreadFunc1, (LPVOID)this, THREAD_PRIORITY_NORMAL);

	return;
}
UINT CMDRSDlg::StoreVideoThreadFunc1(LPVOID lpParam)
{
	CMDRSDlg *pMDRSDlg = (CMDRSDlg*)lpParam;
	pMDRSDlg->StoreVideo1();
	return 0;
}
void CMDRSDlg::StoreVideo1()
{
	m_bIsCapture1 = TRUE;
	CString strAVIFileName = _T("");
	CFileDialog dlg(FALSE, "avi", _T("C1-Video"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "AVI Files (*.avi)|*.avi;All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK)
	{
		strAVIFileName = dlg.GetPathName();
		if (!strAVIFileName.IsEmpty())
		{
			USES_CONVERSION;
			string videoPathAndName = T2CA(strAVIFileName);
			VideoWriter camera1_Writer(videoPathAndName, CV_FOURCC('M', 'J', 'P', 'G'), 60.0, Size(640, 512));
			while(m_bIsCapture1)
			{
				Mat frame = g_src1.clone();	
				camera1_Writer << frame;
				waitKey(27);
				if (m_bIsCapture1 == FALSE )
					break;
			}
		}
		else
		{
			MessageBox(_T("文件路径为空！\n"));
			return;
		}	
	}
	else
		return;
}
//停止1#摄像头视频录取
void CMDRSDlg::OnCamera1_StopVideo()
{
	// TODO: Add your command handler code here
	m_bIsCapture1 = FALSE;
}
/////////////////////////////#2摄像头的菜单操作命令//////////////////////////////////
//打开2#摄像头
void CMDRSDlg::OnCamera2_Open()
{
	// TODO: Add your command handler code here
	HVSTATUS status = STATUS_OK;
	//初始化数字摄像机采集图像到内存的控制，指定回调函数SnapThreadCallback和this指针
	status = HVOpenSnap(m_hhv2, SnapThreadCallback2, this);					
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bOpen2 = TRUE;		//标志已经打开Snap环境
	//启动数字摄像机采集图像到内存
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer2;
	status = HVStartSnap(m_hhv2, ppBuf, 1);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart2 = TRUE;
	return;
}
//暂停2#摄像头
void CMDRSDlg::OnCamera2_Stop()
{
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv2);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart2 = FALSE;
}
//关闭2#摄像头
void CMDRSDlg::OnCamera2_Close()
{
	// TODO: Add your command handler code here
	HVSTATUS status =STATUS_OK;
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv2);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
		m_bStart2 = FALSE;
	status = HVCloseSnap(m_hhv2);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) 
	{
		m_bOpen2	= FALSE;
		m_bStart2	= FALSE;
		m_nError2   = 0;
		m_nLost2    = 0;
	}
}
//保存2#摄像头图片
void CMDRSDlg::OnCamera2_SavePic()
{
	// TODO: Add your command handler code here
	Mat saveImg = g_src2.clone();
	CFileDialog dlg(FALSE , "*.bmp", _T("C2img") ,OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, "Bitmap Files(*.bmp)|*.bmp", this);
	if (dlg.DoModal() == IDOK) 
	{
		CString imgPath = dlg.GetPathName();
		if (!imgPath.IsEmpty())
		{
			USES_CONVERSION;
			string imgPathAndName = T2CA(imgPath);
			imwrite(imgPathAndName, saveImg);
		}
		else
		{
			MessageBox(_T("文件路径为空！\n"));
			return;
		}	
	}
	else
		return;
}
//录取2#摄像头的视频，这里录取视频要开线程，
void CMDRSDlg::OnCamera2_SaveVideo()
{
	// TODO: Add your command handler code here
	CWinThread *pStoreVideoThread = NULL;
	pStoreVideoThread = AfxBeginThread(&StoreVideoThreadFunc2, (LPVOID)this, THREAD_PRIORITY_NORMAL); 
}
UINT CMDRSDlg::StoreVideoThreadFunc2(LPVOID lpParam)
{
	CMDRSDlg *pMDRSDlg = (CMDRSDlg*)lpParam;
	pMDRSDlg->StoreVideo2();
	return 0;
}
void CMDRSDlg::StoreVideo2()
{
	m_bIsCapture2 = TRUE;
	CString strAVIFileName = _T("");
	CFileDialog dlg(FALSE, "avi", _T("C2-Video"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "AVI Files (*.avi)|*.avi;All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK)
	{
		strAVIFileName = dlg.GetPathName();
		if (!strAVIFileName.IsEmpty())
		{
			USES_CONVERSION;
			string videoPathAndName = T2CA(strAVIFileName);
			VideoWriter camera2_Writer(videoPathAndName, CV_FOURCC('M', 'J', 'P', 'G'), 60.0, Size(640, 512));
			while(m_bIsCapture2)
			{
				Mat frame = g_src2.clone();	
				camera2_Writer << frame;
				waitKey(27);
				if (m_bIsCapture2 == FALSE )
					break;
			}
		}
		else 
		{
			MessageBox(_T("文件路径为空！\n"));
			return;
		}	
	}
	else
		return;
}
//停止2#摄像头视频录取
void CMDRSDlg::OnCamera2_StopVideo()
{
	// TODO: Add your command handler code here
	m_bIsCapture2 = FALSE;
}

////////////////////////窗口关闭按钮消息响应//////////////////////////////////////////////////
void CMDRSDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);	//	Page1的定时器
	if (m_bOpen1 || m_bOpen2)
	{
		AfxMessageBox(_T("请先关闭CCD摄像头！\n"));
		return;
	}
	DestroyDHCamera(m_hhv2, m_pRawBuffer2, m_pImageBuffer2);
	DestroyDHCamera(m_hhv1, m_pRawBuffer1, m_pImageBuffer1);
	CDialogEx::OnClose();
}
////////////////////////窗口关闭按钮消息响应//////////////////////////////////////////////////
void CMDRSDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	int ctrlID=pScrollBar->GetDlgCtrlID();
	int nPosition(0);
	switch(ctrlID)
	{
	case IDC_SLIDER_GAIN1:
		nPosition=m_sliderGain1.GetPos();
		m_spinGain1.SetPos(nPosition);
		m_editGain1 = nPosition;
		SetGain(IDC_SLIDER_GAIN1);
		break;
	case IDC_SPIN_GAIN1:
		nPosition=m_spinGain1.GetPos();
		m_sliderGain1.SetPos(nPosition);
		m_editGain1 = nPosition;
		SetGain(IDC_SPIN_GAIN1);
		break;
	case IDC_SLIDER_GAIN2:
		nPosition=m_sliderGain2.GetPos();
		m_spinGain2.SetPos(nPosition);
		m_editGain2 = nPosition;
		SetGain(IDC_SLIDER_GAIN2);
		break;
	case IDC_SPIN_GAIN2:
		nPosition=m_spinGain2.GetPos();
		m_sliderGain2.SetPos(nPosition);
		m_editGain2 = nPosition;
		SetGain(IDC_SPIN_GAIN2);
		break;	
	case IDC_SLIDER_SHUTTER1:
		nPosition=m_sliderShutter1.GetPos();
		m_spinShutter1.SetPos(nPosition);
		m_editShutter1 = nPosition;
		SetExposureTime(m_hhv1, Width, nPosition, ExposureTint_Lower, m_lHBlanking1, SnapSpeed, Resolution);
		break;
	case IDC_SPIN_SHUTTER1:
		nPosition=m_spinShutter1.GetPos();
		m_sliderShutter1.SetPos(nPosition);
		m_editShutter1 = nPosition;
		SetExposureTime(m_hhv1, Width, nPosition, ExposureTint_Lower, m_lHBlanking1, SnapSpeed,Resolution);
		break;
	case IDC_SLIDER_SHUTTER2:
		nPosition=m_sliderShutter2.GetPos();
		m_spinShutter2.SetPos(nPosition);
		m_editShutter2 = nPosition;
		SetExposureTime(m_hhv2, Width, nPosition, ExposureTint_Lower, m_lHBlanking2, SnapSpeed, Resolution);
		break;
	case IDC_SPIN_SHUUTTER2:
		nPosition=m_spinShutter2.GetPos();
		m_sliderShutter2.SetPos(nPosition);
		m_editGain1 = nPosition;
		SetExposureTime(m_hhv2, Width, nPosition, ExposureTint_Lower, m_lHBlanking2, SnapSpeed, Resolution);
		break;
	}
	UpdateData(FALSE);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CMDRSDlg::SetGain(int ctrID)
{
	HVSTATUS status=STATUS_OK;
	long nGain;
	switch(ctrID)
	{
	case IDC_SLIDER_GAIN1:
		nGain = m_sliderGain1.GetPos();
		for (int nChannel=RED_CHANNEL;nChannel<=BLUE_CHANNEL;nChannel++)
		{
			status=HVAGCControl(m_hhv1, nChannel,nGain);
			HV_VERIFY(status);
		}
	case IDC_SPIN_GAIN1:
		nGain = m_spinGain1.GetPos();
		for (int nChannel=RED_CHANNEL;nChannel<=BLUE_CHANNEL;nChannel++)
		{
			status=HVAGCControl(m_hhv1, nChannel, nGain);
			HV_VERIFY(status);
		}
	case IDC_SLIDER_GAIN2:
		nGain = m_sliderGain2.GetPos();
		for (int nChannel=RED_CHANNEL; nChannel<=BLUE_CHANNEL; nChannel++)
		{
			status=HVAGCControl(m_hhv2, nChannel, nGain);
			HV_VERIFY(status);
		}
	case IDC_SPIN_GAIN2:
		nGain = m_spinGain2.GetPos();
		for (int nChannel=RED_CHANNEL;nChannel<=BLUE_CHANNEL;nChannel++)
		{
			status=HVAGCControl(m_hhv2, nChannel, nGain);
			HV_VERIFY(status);
		}
	}	
	return TRUE;
}

void CMDRSDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int curSel = m_tab.GetCurSel();
	switch(curSel)
	{
	case 0:
		m_page1.ShowWindow(TRUE);
		m_page2.ShowWindow(FALSE);
 		m_page3.ShowWindow(FALSE);
 		m_page4.ShowWindow(FALSE);
		break;
	case 1:
		m_page1.ShowWindow(FALSE);
		m_page2.ShowWindow(TRUE);
 		m_page3.ShowWindow(FALSE);
 		m_page4.ShowWindow(FALSE);
		break;
	case 2:
		m_page1.ShowWindow(FALSE);
		m_page2.ShowWindow(FALSE);
 		m_page3.ShowWindow(TRUE);
 		m_page4.ShowWindow(FALSE);
		break;
	case 3:
		m_page1.ShowWindow(FALSE);
		m_page2.ShowWindow(FALSE);
 		m_page3.ShowWindow(FALSE);
 		m_page4.ShowWindow(TRUE);
		break;
	default:
		break;
	}
	*pResult = 0;
}

//<------------图像辅助------------->
////1#和2#摄像头图像中心十字开和关
void CMDRSDlg::OnShowCross()
{
	// TODO: Add your command handler code here
	if (m_bDrawCross1)
		m_bDrawCross1 = FALSE;
	else
		m_bDrawCross1 = TRUE;

	if (m_bDrawCross2)
		m_bDrawCross2 = FALSE;
	else
		m_bDrawCross2 = TRUE;
}
void CMDRSDlg::DrawCross(Mat img)
{
	int crossLen = 20;
	Point centerPt(320, 256);
	circle(img, centerPt, 15, Scalar(0,255,0), 1, CV_AA);
	line(img, Point(centerPt.x-20, centerPt.y), Point(centerPt.x+20, centerPt.y), Scalar(0,255,0), 1, CV_AA);
	line(img, Point(centerPt.x, centerPt.y-20), Point(centerPt.x, centerPt.y+20), Scalar(0,255,0), 1, CV_AA);
}

void CMDRSDlg::OnImagePreview()
{
	// TODO: Add your command handler code here
	Mat src1, src2;
	src1 = g_src1.clone();
	src2 = g_src2.clone();
	resize(src1, src1, Size(320, 256));
	resize(src2, src2, Size(320, 256));
	ShowImage(src2, IDC_PREVIEW_PIC2);
	ShowImage(src1, IDC_PREVIEW_PIC1);
}

void CMDRSDlg::OnShowImageScale()
{
	// TODO: Add your command handler code here
	if (!m_bSetImgScale)
		m_bSetImgScale = TRUE;
	else
		m_bSetImgScale = FALSE;

	if (m_bSetImgScaleH)
		m_bSetImgScaleH = FALSE;
	else
		m_bSetImgScaleH = TRUE;
}

void CMDRSDlg::SetImgScale(Mat img)
{
	rectangle(img, Point(568, 490), Point(630, 493), Scalar(0,255,0), -1, CV_AA);
	putText(img, "400um", Point(574, 485), FONT_HERSHEY_COMPLEX_SMALL, 0.6, Scalar(0,255,0), 1, CV_AA);
}
//<--------------------------图像辅助---------------------------->

//<--------------------------鼠标操作---------------------------->
void CMDRSDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDrawLine)
	{	
		m_LinePt[m_LinePtNum].x = point.x-655;
		m_LinePt[m_LinePtNum].y = point.y-78;
		m_LinePtNum ++;
	}
	if (m_bDrawRect)
	{
		m_RectPt[m_RectPtNum].x = point.x-655;
		m_RectPt[m_RectPtNum].y = point.y-78;
		m_RectPtNum++;
	}
	if (m_bDrawCircle)
	{
		m_CirclePt[m_CirclePtNum].x = point.x-655;
		m_CirclePt[m_CirclePtNum].y = point.y-78;
		m_CirclePtNum++;
	}
	if (m_bMeasure)
	{
		m_MeasurePt[m_MeasurePtNum].x = point.x - 655;
		m_MeasurePt[m_MeasurePtNum].y = point.y - 78;
		m_MeasurePtNum++;
	}
	if (m_bMeasureSrc1)
	{
		m_MeasurePt1[m_MeasurePtNum1].x = point.x - 7;
		m_MeasurePt1[m_MeasurePtNum1].y = point.y - 78;
		m_MeasurePtNum1++;
	}
	if (m_bCalibrateRectH)
	{
		m_CalibrateRectPt.x = point.x - 7;
		m_CalibrateRectPt.y = point.y - 78;
		m_bDrawCalibrateRectH = TRUE;
		int disX = m_CalibrateRectPt.x - 320;
		int disZ = m_CalibrateRectPt.y - 256;
		//确定x和z方向坐标标定位置
		ssp2_set_current_card(0);
		ssp2_start_ps_move(0, 0, int(disX*0.45), 50, 100, 1 , 2, 0.5, 0.4);
		Sleep(10);
		ssp2_start_ps_move(2, 0, int(disZ*0.45), 50, 100, 1 , 2, 0.5, 0.4);
	}
	if (m_bCalibrateCircleH)
	{
		m_CalibrateCirclePt.x = point.x - 7;
		m_CalibrateCirclePt.y = point.y - 78;
		m_bDrawCalibrateCircleH = TRUE;
		int disX = m_CalibrateCirclePt.x - 320;
		int disZ = m_CalibrateCirclePt.y - 256;
		//确定x和z方向坐标标定位置
		ssp2_set_current_card(0);
		ssp2_start_ps_move(0, 0, int(disX*0.45), 50, 100, 1 , 2, 0.5, 0.4);
		Sleep(10);
		ssp2_start_ps_move(2, 0, int(disZ*0.45), 50, 100, 1 , 2, 0.5, 0.4);
	}

	if (m_bCalibrateRectV)
	{
		m_CalibrateRectPtV.x = point.x - 655;
		m_CalibrateRectPtV.y = point.y - 78;
		m_bDrawCalibrateRectV = TRUE;
		int disX = m_CalibrateRectPtV.x - 320;
		int disZ = m_CalibrateRectPtV.y - 256;
		//确定x和Y方向坐标标定位置
		ssp2_set_current_card(12);
		ssp2_start_ps_move(0, 0, int(disX*0.45), 50, 100, 1 , 2, 0.5, 0.4);//X
		Sleep(10);
		ssp2_start_ps_move(1, 0, int(disZ*0.45), 50, 100, 1 , 2, 0.5, 0.4);//Y
	}
	if (m_bCalibrateCircleV)
	{
		m_CalibrateCirclePtV.x = point.x - 655;
		m_CalibrateCirclePtV.y = point.y - 78;
		m_bDrawCalibrateCircleV = TRUE;
		int disX = m_CalibrateCirclePtV.x - 320;
		int disZ = m_CalibrateCirclePtV.y - 256;
		//确定x和Y方向坐标标定位置
		ssp2_set_current_card(12);
		ssp2_start_ps_move(0, 0, int(disX*0.45), 50, 100, 1 , 2, 0.5, 0.4);
		Sleep(10);
		ssp2_start_ps_move(1, 0, int(disZ*0.45), 50, 100, 1 , 2, 0.5, 0.4);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMDRSDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDrawLine)
	{
		m_bDrawLine = FALSE;
		m_LinePtNum = 0;
	}
	if (m_bDrawRect)
	{
		m_bDrawRect = FALSE;
		m_RectPtNum = 0;
	}
	if (m_bDrawCircle)
	{
		m_bDrawCircle = FALSE;
		m_CirclePtNum = 0;
	}
	if (m_bMeasure)
	{
		m_bMeasure = FALSE;
		m_MeasurePtNum = 0;
	}
	if (m_bMeasureSrc1)
	{
		m_bMeasureSrc1 = false;
		m_MeasurePtNum1 = 0;
	}
	if (m_bDrawCalibrateRectH && m_bCalibrateRectH)
	{
		m_bCalibrateRectH = FALSE;
		m_bDrawCalibrateRectH = FALSE;
	}
	
	CDialogEx::OnRButtonDown(nFlags, point);
}

void CMDRSDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// 	if (m_bMeasure)
	// 	{
	// 		
	// 	}
	CDialogEx::OnMouseMove(nFlags, point);
}
//<--------------------------鼠标操作---------------------------->

//<--------------------menu：垂直CCD图像辅助--------------------->
//霍夫圆检测，但是这个算法很不稳定，有的时候能够检测到很好的圆，
//有的时候效果很差，而且参数需要自己调，不建议使用
void CMDRSDlg::OnDetectWPCenterHuogh_V()
{
	// TODO: Add your command handler code here
	m_bHuofuCircle = TRUE;
	if (m_bHuofuCircle)
	{
		m_bHuofuCircle = FALSE;
		HuofuCircleDetection(g_src2);
	}	
}
void CMDRSDlg::HuofuCircleDetection(Mat img)
{
	Mat srcCopy, midImg;
	srcCopy = img.clone();
	cvtColor(srcCopy, midImg, CV_BGR2GRAY);
	GaussianBlur(midImg, midImg, Size(3,3), 2, 2);
	vector<Vec3f> circles;
	HoughCircles(midImg, circles, CV_HOUGH_GRADIENT, 2, 150, 240, 80, 50, 300);
	if (circles.size() != 0)
	{
		for (size_t i=0; i<circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(srcCopy, center, 3, Scalar(0,255,0), -1, 8, 0);//绘制圆心
			circle(srcCopy, center, radius, Scalar(0,0,255), 2, 8, 0);
		}
	}
	else
		MessageBox(_T("霍夫圆检测失败！\n"));
	resize(srcCopy, srcCopy, Size(320, 256));
	ShowImage(srcCopy, IDC_PREVIEW_PIC2);
}
//检测工件中心轮廓位置，这里用的是自适应阈值的方法，而page2中用的是固定阈值的方法
void CMDRSDlg::ContourDetect2(Mat img)
{
	IplImage* pImg = &IplImage(img);
	IplImage * grayImg=cvCreateImage(cvGetSize(pImg), 8, 1);	
	cvCvtColor(pImg, grayImg, CV_BGR2GRAY);  //灰度化
	cvSmooth(grayImg, grayImg, CV_GAUSSIAN, 3, 3);
	cvErode(grayImg, grayImg, NULL, 3);
	cvDilate(grayImg, grayImg, NULL, 3);
	int thresholdValue = OtsuThreshold2(grayImg);
	cvThreshold(grayImg, grayImg, thresholdValue, 255, CV_THRESH_BINARY_INV);  //二值化
	//轮廓检测方法//
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
		if (contArea > 0.5*640*512)
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
	m_workPieceCenterPt.x = (LONG)minBoxB.center.x;
	m_workPieceCenterPt.y = (LONG)minBoxB.center.y;

	Mat dstImg(pImg);
	dstImg.data = (uchar*)pImg->imageData;
	resize(dstImg, dstImg, Size(320, 256));

	ShowImage(dstImg, IDC_PREVIEW_PIC2);
	//最后释放相关内存
	cvReleaseImage(&grayImg);
	cvReleaseMemStorage(&storage);
	//	cvReleaseImage(&pImg);  //这一句不能加，pImg不能释放，因为pImg与m_src2共用一个数据头
}
void CMDRSDlg::ContourDetect(Mat img)
{
	int cMin = 100;
	int cMax = 512;
	Mat srcCopy = img.clone();
	Mat midImg, binImg, contourImg;
	vector<vector<Point>> contours;
	cvtColor(srcCopy, midImg, CV_BGR2GRAY);
	GaussianBlur(midImg, midImg, Size(3,3), 0, 0);
	int threshValue = OtsuThreshold(midImg);
	threshold(midImg, binImg, threshValue, 255, CV_THRESH_BINARY_INV);
	imshow("BIN", binImg);	
	findContours(binImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0,0));
	drawContours(srcCopy, contours, -1, Scalar(0,255,0), 1, CV_AA);
	imshow("CON", srcCopy);
	//	waitKey(0);
	//筛选轮廓
	Mat dstContImg = srcCopy.clone();
	vector<vector<Point>> ::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < (size_t)cMin || itc->size() > (size_t)cMax )
			itc = contours.erase(itc);
		else
			++itc;
	}
	drawContours(dstContImg, contours, -1, Scalar(0,255,255), 1, CV_AA);
	imshow("CON2", dstContImg);
	resize(dstContImg, dstContImg, Size(320, 256));
	ShowImage(dstContImg, IDC_PREVIEW_PIC2);	  
}
void CMDRSDlg::OnDetectWPCenter_V()
{
	// TODO: Add your command handler code here
	//ContourDetect(m_src2);
	ContourDetect2(g_src2);
}
//玻璃环轮廓中心位置检测
void CMDRSDlg::OnDetectGlassCenter_V()
{
	// TODO: Add your command handler code here
	GlassCircleContourDetect(g_src2);
}
void CMDRSDlg::GlassCircleContourDetect(Mat img)
{
	IplImage* pImg = &IplImage(img);
	IplImage * grayImg=cvCreateImage(cvGetSize(pImg), 8, 1);	
	cvCvtColor(pImg, grayImg, CV_BGR2GRAY);  //灰度化
	cvSmooth(grayImg, grayImg, CV_GAUSSIAN, 3, 3);
	cvErode(grayImg, grayImg, NULL, 3);
	cvDilate(grayImg, grayImg, NULL, 3);
	int thresholdValue = OtsuThreshold2(grayImg);
	cvThreshold(grayImg, grayImg, thresholdValue, 255, CV_THRESH_BINARY_INV);  //二值化
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
		if (contArea > 0.4*640*512)
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
	//保存玻璃环中心坐标位置
	m_glassCircleCenterPt.x = (LONG)minBoxB.center.x;
	m_glassCircleCenterPt.y = (LONG)minBoxB.center.y;

	Mat dstImg(pImg);
	dstImg.data = (uchar*)pImg->imageData;
	resize(dstImg, dstImg, Size(320, 256));
	ShowImage(dstImg, IDC_PREVIEW_PIC2);
	//最后释放相关内存
	cvReleaseImage(&grayImg);
	cvReleaseMemStorage(&storage);
	//	cvReleaseImage(&pImg);  //这一句不能加，pImg不能释放，因为pImg与m_src2共用一个数据头
}
//大津法自适应阈值函数
int CMDRSDlg::OtsuThreshold(const Mat & img)  
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
int CMDRSDlg::OtsuThreshold2(IplImage* pImg)
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

void CMDRSDlg::OnDetectGlassCenterHuogh_V()
{
	// TODO: Add your command handler code here
	m_bHuofuGlassCircle = TRUE;
	if (m_bHuofuGlassCircle)
	{
		m_bHuofuGlassCircle = FALSE;
		HuofuCircleDetection(g_src2);
	}	
}

//鼠标两点距离测量
void CMDRSDlg::OnMeasureSrc2()
{
	// TODO: Add your command handler code here
	m_bMeasure = TRUE;
}
void CMDRSDlg::Measure(Mat img)
{
	if (m_bMeasure && m_MeasurePtNum == 2)
	{
		int disX = abs(m_MeasurePt[1].x-m_MeasurePt[0].x);
		int disY = abs(m_MeasurePt[1].y-m_MeasurePt[0].y);
		double dis = sqrt((double)(disX*disX) + (double)(disY*disY));
		line(img, m_MeasurePt[0], m_MeasurePt[1], Scalar(0, 255, 0), 2, CV_AA);
		CString strDis(_T(""));
		strDis.Format("%.3f", dis);
		strDis = strDis+_T("pixs");
		int flagX(1), flagY(1);
		if (m_MeasurePt[1].x > m_MeasurePt[0].x)
			flagX = 1;
		else
			flagX = -1;
		if (m_MeasurePt[1].y > m_MeasurePt[0].y)
			flagY = 1;
		else
			flagY = -1;
		putText(img, (string)strDis, Point(m_MeasurePt[0].x+(disX/2)*flagX, m_MeasurePt[0].y+(disY/2)*flagY),
			FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 255, 255), 1, CV_AA);
	}
}

void CMDRSDlg::OnDrawLineByMouse()
{
	// TODO: Add your command handler code here
	m_bDrawLine = TRUE;
	m_bDrawRect = FALSE;
	m_bDrawCircle = FALSE;
	m_LinePtNum = 0;
	m_RectPtNum = 0;
	m_CirclePtNum = 0;
}
void CMDRSDlg::DrawLine(Mat img)
{
	if (m_bDrawLine && (m_LinePtNum == 2))
		line(img, m_LinePt[0], m_LinePt[1], Scalar(255, 255, 0), 2, CV_AA);
}

void CMDRSDlg::OnDrawRectByMouse()
{
	// TODO: Add your command handler code here
	m_bDrawRect = TRUE;
	m_bDrawLine = FALSE;
	m_bDrawCircle = FALSE;
	m_LinePtNum = 0;
	m_RectPtNum = 0;
	m_CirclePtNum = 0;
}
void CMDRSDlg::DrawRect(Mat img)
{
	if (m_bDrawRect && m_RectPtNum == 3)
	{
		line(img, m_RectPt[0], m_RectPt[1], Scalar(255, 255, 0), 2, CV_AA);
		line(img, m_RectPt[1], m_RectPt[2], Scalar(255, 255, 0), 2, CV_AA);
		line(img, m_RectPt[2], Point(m_RectPt[0].x+(m_RectPt[2].x-m_RectPt[1].x), 
			m_RectPt[2].y+(m_RectPt[0].y-m_RectPt[1].y)), Scalar(255, 255, 0), 2, CV_AA);
		line(img, Point(m_RectPt[0].x+(m_RectPt[2].x-m_RectPt[1].x), 
			m_RectPt[2].y+(m_RectPt[0].y-m_RectPt[1].y)), m_RectPt[0], Scalar(255, 255, 0), 2, CV_AA);
	}
}

void CMDRSDlg::OnDrawCircleByMouse()
{
	// TODO: Add your command handler code here
	m_bDrawCircle = TRUE;
	m_bDrawLine = FALSE;
	m_bDrawRect = FALSE;
	m_LinePtNum = 0;
	m_RectPtNum = 0;
	m_CirclePtNum = 0;
}
void CMDRSDlg::DrawCircle(Mat img)
{
	if (m_bDrawCircle && m_CirclePtNum == 2)
	{
		int disX = abs(m_CirclePt[1].x-m_CirclePt[0].x);
		int disY = abs(m_CirclePt[1].y-m_CirclePt[0].y);
		double radius = sqrt((double)(disX*disX) + (double)(disY*disY));
		circle(img, m_CirclePt[0], (int)radius, Scalar(0, 255, 255), 2, CV_AA);
	}
}

void CMDRSDlg::OnCalibrateRectV()
{
	// TODO: Add your command handler code here
	m_bCalibrateRectV = TRUE;
}

void CMDRSDlg::DrawCalibrateRectV(Mat img)
{
	line(img, Point(0, m_CalibrateRectPtV.y), Point(640, m_CalibrateRectPtV.y), Scalar(0, 0, 255), 1, CV_AA);
	line(img, Point(m_CalibrateRectPtV.x, 0), Point(m_CalibrateRectPtV.x, 512), Scalar(0, 0, 255), 1, CV_AA);
	rectangle(img, Point(m_CalibrateRectPtV.x-40, m_CalibrateRectPtV.y-40), 
		Point(m_CalibrateRectPtV.x+40, m_CalibrateRectPtV.y+40), Scalar(0, 0, 255), 1, CV_AA);
}

void CMDRSDlg::OnCalibrateCircleV()
{
	// TODO: Add your command handler code here
	m_bCalibrateCircleV = TRUE;
}

void CMDRSDlg::DrawCalibrateCircleV(Mat img)
{
	line(img, Point(0, m_CalibrateCirclePtV.y), Point(640, m_CalibrateCirclePtV.y), Scalar(0, 0, 255), 1, CV_AA);
	line(img, Point(m_CalibrateCirclePtV.x, 0), Point(m_CalibrateCirclePtV.x, 512), Scalar(0, 0, 255), 1, CV_AA);
	circle(img, Point(m_CalibrateCirclePtV.x, m_CalibrateCirclePtV.y), 40, Scalar(0, 0, 255), 1, CV_AA);
}

//<----------------------Menu:水平CCD图像辅助---------------------------->
void CMDRSDlg::OnMeasureSrc1()
{
	// TODO: Add your command handler code here
	m_bMeasureSrc1 = true;
}
void CMDRSDlg::MeasureSrc1(Mat img)
{
	if (m_bMeasureSrc1 && m_MeasurePtNum1 == 2)
	{
		int disX = abs(m_MeasurePt1[1].x-m_MeasurePt1[0].x);
		int disY = abs(m_MeasurePt1[1].y-m_MeasurePt1[0].y);
		double dis = sqrt((double)(disX*disX) + (double)(disY*disY));
		line(img, m_MeasurePt1[0], m_MeasurePt1[1], Scalar(0, 255, 0), 2, CV_AA);
		CString strDis(_T(""));
		strDis.Format("%.3f", dis);
		strDis = strDis+_T("pixs");
		int flagX(1), flagY(1);
		if (m_MeasurePt1[1].x > m_MeasurePt1[0].x)
			flagX = 1;
		else
			flagX = -1;
		if (m_MeasurePt1[1].y > m_MeasurePt1[0].y)
			flagY = 1;
		else
			flagY = -1;
		putText(img, (string)strDis, Point(m_MeasurePt1[0].x+(disX/2)*flagX, m_MeasurePt1[0].y+(disY/2)*flagY),
			FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 255, 255), 1, CV_AA);
	}
}

void CMDRSDlg::OnCalibrateRectH()
{
	// TODO: Add your command handler code here
	m_bCalibrateRectH = TRUE;
}

void CMDRSDlg::DrawCalibrateRectH(Mat img)
{
	line(img, Point(0, m_CalibrateRectPt.y), Point(640, m_CalibrateRectPt.y), Scalar(0, 0, 255), 1, CV_AA);
	line(img, Point(m_CalibrateRectPt.x, 0), Point(m_CalibrateRectPt.x, 512), Scalar(0, 0, 255), 1, CV_AA);
	rectangle(img, Point(m_CalibrateRectPt.x-40, m_CalibrateRectPt.y-40), 
		Point(m_CalibrateRectPt.x+40, m_CalibrateRectPt.y+40), Scalar(0, 0, 255), 1, CV_AA);
}

void CMDRSDlg::OnCalibrateCircleH()
{
	// TODO: Add your command handler code here
	m_bDrawCalibrateCircleH = TRUE;
}

void CMDRSDlg::DrawCalibrateCircleH(Mat img)
{
	line(img, Point(0, m_CalibrateCirclePt.y), Point(640, m_CalibrateCirclePt.y), Scalar(0, 0, 255), 1, CV_AA);
	line(img, Point(m_CalibrateCirclePt.x, 0), Point(m_CalibrateCirclePt.x, 512), Scalar(0, 0, 255), 1, CV_AA);
	circle(img, Point(m_CalibrateCirclePt.x, m_CalibrateCirclePt.y), 40, Scalar(0, 0, 255), 1, CV_AA);
}

////<---------Menu:水平CCD图像辅助------------->//////////////


//<---------menu：CCD参数读写------------->
void CMDRSDlg::OnCCDWriteConfig()
{
	// TODO: Add your command handler code here
	WriteCCDConfig();
}

void CMDRSDlg::OnCCDReadConfig()
{
	// TODO: Add your command handler code here
	ReadCCDConfig();
}

void CMDRSDlg::WriteCCDConfig()
{
	CString		fileName(_T(".\\CONFIG\\CCD\\CCD.ini"));
	CString		appName(_T("section1"));
	CString		keyName(_T("CCD1增益"));
	CString		data(_T(""));
	GetDlgItem(IDC_EDIT_GAIN1)->GetWindowText(data);
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section2");
	keyName = _T("CCD1曝光度");
	GetDlgItem(IDC_EDIT_SHUTTER1)->GetWindowText(data);
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section3");
	keyName = _T("CCD2增益");
	GetDlgItem(IDC_EDIT_GAIN2)->GetWindowText(data);
	WritePrivateProfileString(appName, keyName, data, fileName);
	appName = _T("section4");
	keyName = _T("CCD2曝光度");
	GetDlgItem(IDC_EDIT_SHUTTER2)->GetWindowText(data);
	WritePrivateProfileString(appName, keyName, data, fileName);
}

void CMDRSDlg::ReadCCDConfig()
{
	CString		fileName(_T(".\\CONFIG\\CCD\\CCD.ini"));
	char gain[20];
	CString gainValue(_T(""));
	char shutter[20];
	CString shutterValue(_T(""));
	int nGainPosition(0);
	int nShutterPosition(0);
	//CCD1
	GetPrivateProfileString(_T("section1"), _T("CCD1增益"), _T("gain"), gain, 20, fileName);
	gainValue = gain;
	nGainPosition = atoi(gainValue);
	m_sliderGain1.SetPos(nGainPosition);    //设置滑块控件到指定位置
	m_spinGain1.SetPos(nGainPosition);
	SetGain(IDC_SLIDER_GAIN1);

	GetPrivateProfileString(_T("section2"), _T("CCD1曝光度"), _T("shutter"), shutter, 20, fileName);
	shutterValue = shutter;
	nShutterPosition = atoi(shutterValue);
	m_sliderShutter1.SetPos(nShutterPosition);    //设置滑块控件到指定位置
	m_spinShutter1.SetPos(nShutterPosition);
	SetExposureTime(m_hhv1, Width, nShutterPosition, ExposureTint_Lower, m_lHBlanking1, SnapSpeed, Resolution);
	//CCD2
	GetPrivateProfileString(_T("section3"), _T("CCD2增益"), _T("gain"), gain, 20, fileName);
	gainValue = gain;
	nGainPosition = atoi(gainValue);
	m_sliderGain2.SetPos(nGainPosition);    //设置滑块控件到指定位置
	m_spinGain2.SetPos(nGainPosition);
	SetGain(IDC_SLIDER_GAIN2);

	GetPrivateProfileString(_T("section4"), _T("CCD2曝光度"), _T("shutter"), shutter, 20, fileName);
	shutterValue = shutter;
	nShutterPosition = atoi(shutterValue);
	m_sliderShutter2.SetPos(nShutterPosition);    //设置滑块控件到指定位置
	m_spinShutter2.SetPos(nShutterPosition);
	SetExposureTime(m_hhv2, Width, nShutterPosition, ExposureTint_Lower, m_lHBlanking1, SnapSpeed, Resolution);

}
//<---------menu：CCD参数读写------------->

////////////////////////////工具栏按钮消息响应-控制2个CCD的消息响应//////////////
//1#和2#摄像头同时开启
BOOL CMDRSDlg::OnCameraAllOpen(UINT ID)
{
	//	MessageBox(_T("open all camera!"));
	OnCamera1_Open();
	OnCamera2_Open();
	return TRUE;
}
//1#和2#摄像头同时暂停
BOOL CMDRSDlg::OnCameraAllStop(UINT ID)
{
	OnCamera1_Stop();
	OnCamera2_Stop();
	return TRUE;
}
//1#和2#摄像头同时关闭
BOOL CMDRSDlg::OnCameraAllClose(UINT ID)
{
	OnCamera1_Close();
	OnCamera2_Close();
	return TRUE;
}

///////////////////////工具栏按钮消息响应-控制2个CCD的消息响应//////////////////




