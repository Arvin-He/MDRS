
// MDRSDlg.h : header file
//

#pragma once
#include <vfw.h>
#include "DHCamera/inc/HVDAILT.h"
#include "DHCamera/inc/Raw2Rgb.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"

using namespace cv;
using namespace std;

#pragma comment(lib, "Vfw32.lib")

//自定义消息ID号
#define WM_SNAP_CHANGE1		(WM_USER + 100)
#define WM_SNAP_STOP1		(WM_USER + 102)
#define WM_SNAP_CHANGE2		(WM_USER + 103)
#define WM_SNAP_STOP2		(WM_USER + 104)
#define WM_SNAP_ERROR		(WM_USER + 101)  
// CMDRSDlg dialog
class CMDRSDlg : public CDialogEx
{
// Construction
public:
	CMDRSDlg(CWnd* pParent = NULL);	// standard constructor
	~CMDRSDlg();
	BOOL InitialDHCamera1();
	BOOL InitialDHCamera2();
	BOOL DestroyDHCamera(HHV hDHCamera, BYTE *pRawBuffer, BYTE *pImageBuffer);
	BOOL SetGain(int ctrID);
	void ShowImage(Mat img, INT_PTR ID);
	void DrawCross(Mat img);
	void HuofuCircleDetection(Mat img);
	void SetImgScale(Mat img);
	void ContourDetect(Mat img);
	void ContourDetect2(Mat img);
	void GlassCircleContourDetect(Mat img);
	int	 OtsuThreshold(const Mat & img);
	int  OtsuThreshold2(IplImage* pImg);

	void WriteCCDConfig();
	void ReadCCDConfig();
// Dialog Data
	enum { IDD = IDD_MDRS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


public:
	//工件中心坐标位置
	CPoint m_workPieceCenterPt;
	//玻璃环中心位置
	CPoint m_glassCircleCenterPt;
///////大恒CCD1相关参数和函数///////////////////////////////
private:
	HHV	 m_hhv1;			///< 数字摄像机句柄	
	BOOL m_bOpen1;			///< 初始化标志
	BOOL m_bStart1;			///< 启动标志
	long m_lHBlanking1;		///< 水平消隐
	long m_lVBlanking1;		///< 垂直消隐
	HV_BAYER_LAYOUT m_Layout1; ///< bayer格式	
	BITMAPINFO *m_pBmpInfo1;	///< BITMAPINFO 结构指针，显示图像时使用
	BYTE *m_pRawBuffer1;		///< 采集图像原始数据缓冲区
	BYTE *m_pImageBuffer1;		///< Bayer转换后缓冲区
	char m_chBmpBuf1[2048];		///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	BYTE m_pLutR1[256] ;		///< 颜色查找表,R分量
	BYTE m_pLutG1[256] ;		///< 颜色查找表,G分量
	BYTE m_pLutB1[256] ;		///< 颜色查找表,B分量
	int  m_nError1;				///< 错误计数
	int  m_nLost1;				///< 丢帧计数
	int  dwLastStatus1;
	/// 采集回调函数，用户也可以定义为全局函数，如果作为类的成员函数，必须为静态成员函数。
	static int CALLBACK SnapThreadCallback1(HV_SNAP_INFO *pInfo); 
	/// 获取状态码
	HVSTATUS GetLastStatus1();
	///录取视频的线程
	static UINT StoreVideoThreadFunc1(LPVOID lpParam);
	void StoreVideo1();
	/// 获取bayer格式
	HV_BAYER_LAYOUT HVGetBayerType(HHV hhv);
	/// 设置曝光时间
	HVSTATUS SetExposureTime(HHV hhv, int nWindWidth, long lTintUpper, long lTintLower, long HBlanking, 
							 HV_SNAP_SPEED SnapSpeed, HV_RESOLUTION Resolution);
///////大恒CCD1相关参数/////////////////////////////////////
///////大恒CCD2相关参数和函数///////////////////////////////
private:
	HHV	 m_hhv2;			///< 数字摄像机句柄	
	BOOL m_bOpen2;			///< 初始化标志
	BOOL m_bStart2;			///< 启动标志
	long m_lHBlanking2;		///< 水平消隐
	long m_lVBlanking2;		///< 垂直消隐
	HV_BAYER_LAYOUT m_Layout2; ///< bayer格式	
	BITMAPINFO *m_pBmpInfo2;	///< BITMAPINFO 结构指针，显示图像时使用
	BYTE *m_pRawBuffer2;		///< 采集图像原始数据缓冲区
	BYTE *m_pImageBuffer2;		///< Bayer转换后缓冲区
	char m_chBmpBuf2[2048];		///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	BYTE m_pLutR2[256] ;		///< 颜色查找表,R分量
	BYTE m_pLutG2[256] ;		///< 颜色查找表,G分量
	BYTE m_pLutB2[256] ;		///< 颜色查找表,B分量
	int  m_nError2;				///< 错误计数
	int  m_nLost2;				///< 丢帧计数
	int  dwLastStatus2;
	/// 采集回调函数，用户也可以定义为全局函数，如果作为类的成员函数，必须为静态成员函数。
	static int CALLBACK SnapThreadCallback2(HV_SNAP_INFO *pInfo); 
// 	/// 获取状态码
 	HVSTATUS GetLastStatus2();
	///录取视频的线程
	static UINT StoreVideoThreadFunc2(LPVOID lpParam);
	void StoreVideo2();
	///////大恒CCD2相关参数///////////////////////////////

public:
//	Mat m_src1, m_src2;

private:
	BOOL m_bIsCapture1;		//视频1保存标识
	BOOL m_bIsCapture2; 
	BOOL m_bDrawCross1;		//1#摄像头十字标记
	BOOL m_bDrawCross2;
	BOOL m_bHuofuCircle;	//霍夫圆检测
	BOOL m_bSetImgScale;	//设置图像比例尺
	BOOL m_bSetImgScaleH;
	BOOL m_bMeasure;		//是否进行测量标志
	double m_dImgScale;

	//src1图像测量距离
	bool m_bMeasureSrc1;
	int m_MeasurePtNum1;
	Point m_MeasurePt1[2];
	void MeasureSrc1(Mat img);

	int m_MeasurePtNum;
	Point m_MeasurePt[2];
	void Measure(Mat img);

	BOOL m_bDrawLine;
	int  m_LinePtNum;
	CvPoint m_LinePt[2];
	void DrawLine(Mat img);
	BOOL m_bDrawRect;
	int  m_RectPtNum;
	CvPoint m_RectPt[3];
	void DrawRect(Mat img);
	BOOL m_bDrawCircle;
	int  m_CirclePtNum;
	CvPoint m_CirclePt[2];
	void DrawCircle(Mat img);

	BOOL m_bCalibrateRectH;
	Point m_CalibrateRectPt;
	BOOL m_bDrawCalibrateRectH;
	void DrawCalibrateRectH(Mat img);
	Point m_CalibrateCirclePt;
	BOOL m_bCalibrateCircleH;
	BOOL m_bDrawCalibrateCircleH;
	void DrawCalibrateCircleH(Mat img);

	BOOL m_bCalibrateRectV;
	Point m_CalibrateRectPtV;
	BOOL m_bDrawCalibrateRectV;
	void DrawCalibrateRectV(Mat img);
	Point m_CalibrateCirclePtV;
	BOOL m_bCalibrateCircleV;
	BOOL m_bDrawCalibrateCircleV;
	void DrawCalibrateCircleV(Mat img);




	BOOL m_bHuofuGlassCircle;
// Implementation
protected:
	HICON		m_hIcon;
	CMenu		m_menu;
	CToolBar		m_toolBar;
	CImageList	m_imageList;

	CSliderCtrl		m_sliderGain1;
	CSpinButtonCtrl	m_spinGain1;
	CSliderCtrl		m_sliderShutter1;
	CSpinButtonCtrl	m_spinShutter1;

	CSliderCtrl		m_sliderGain2;
	CSpinButtonCtrl	m_spinGain2;
	CSliderCtrl		m_sliderShutter2;
	CSpinButtonCtrl	m_spinShutter2;
		
	int		m_editShutter1;
	int		m_editShutter2;
	int		m_editGain1;
	int		m_editGain2;
	
	CTabCtrl		m_tab;
	CPage1		m_page1;
	CPage2		m_page2;
	CPage3       m_page3;
	CPage4		m_page4;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSnapChange1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapChange2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapStop1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapStop2(WPARAM wParam, LPARAM lParam);
 	afx_msg void OnCamera1_Open();
	afx_msg void OnCamera1_Stop();
 	afx_msg void OnCamera1_Close();
	afx_msg void OnCamera1_SavePic();
	afx_msg void OnCamera1_SaveVideo();
	afx_msg void OnCamera1_StopVideo();	
	afx_msg void OnCamera2_Open();
	afx_msg void OnCamera2_Stop();
	afx_msg void OnCamera2_Close();
	afx_msg void OnCamera2_SavePic();
	afx_msg void OnCamera2_SaveVideo();
	afx_msg void OnCamera2_StopVideo();
	afx_msg BOOL OnCameraAllOpen(UINT ID);
	afx_msg BOOL OnCameraAllStop(UINT ID);
	afx_msg BOOL OnCameraAllClose(UINT ID);
 	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowCross();
	afx_msg void OnImagePreview();
	afx_msg void OnShowImageScale();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawLineByMouse();
	afx_msg void OnDrawRectByMouse();
	afx_msg void OnDrawCircleByMouse();
	afx_msg void OnCCDWriteConfig();
	afx_msg void OnCCDReadConfig();		
	afx_msg void OnMeasureSrc1();
	afx_msg void OnCalibrateRectH();
	afx_msg void OnCalibrateCircleH();

	afx_msg void OnDetectWPCenter_V();
	afx_msg void OnDetectWPCenterHuogh_V();
	afx_msg void OnDetectGlassCenter_V();
	afx_msg void OnDetectGlassCenterHuogh_V();
	afx_msg void OnMeasureSrc2();
	afx_msg void OnCalibrateRectV();
	afx_msg void OnCalibrateCircleV();

	
	
};
