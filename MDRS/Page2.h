#pragma once
#include <opencv/cv.hpp>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "CvvImage.h"
using namespace cv;
// CPage2 dialog

class CPage2 : public CDialogEx
{
	DECLARE_DYNAMIC(CPage2)
public:
	CPage2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPage2();
	void SetSampleState(/*int nIndex,*/int rows, int cols, bool flagState);
	void SetSampleStateSemiMode(int rows, int cols, bool flagState);
	void SetFlagState(bool m_bSampleState, UINT nID);
	void GetAxisPosition(int nCurrAxis, UINT nID);
	void InitSampleStagePos();
// Dialog Data
	enum { IDD = IDD_PAGE2 };
	
public:
	void MoveToAssignedPosSemi(int rows);
	//夹紧工件
	void ClipWorkpiece(int nColIndex);
	//搬运工件
	void CarryWorkpiece(/*int rows, int cols*/);
	//放置工件
	void DropWorkpiece();
	void ClipMoveWaitPos();
	//点胶工件
	void GlueWorkpiece();
	//吸取圆片
	void AbsorbWorkpiece(int rows, int cols, int nCounts);
	void MoveStage_CCD();
	//搬运玻璃环
	void CarryGlassCircle();
	void MoveBackStage_CCD();
	//识别并修正玻璃环中心位置
	void Detect_AdjustGCPos();
	//放置玻璃环
	void PutDownChip();
	//按压圆片
//	void PressCirclePane();
	void MoveToWP();
	//将工件搬至原位
	void PutBackWP(int rows, int cols);
	//回到准备位置
	void MoveToPreparePos(int rows, int cols, bool colChangeFlag);
	//点胶头到安全位置
	void GlueMoveSafePos();
	//图像相关
	void DetectWorkpieceCenter(Mat img);
	void DetectGlassCircleCenter(Mat img);
	int  OtsuThreshold(const Mat & img);
	int  OtsuThreshold2(IplImage* pImg);
	void ShowImage(Mat img, INT_PTR ID);
	void GetGluePinPosErr();
	void GetGlueAbsorbPosErr();
private:
	bool m_bIsAllPosSetZero;
	bool m_bIsAllHome;

	//半自动线程执行标志
	bool m_bCarry;
	bool m_bDetectWpCenter;
	bool m_bGlueStart;
	bool m_bMountGlassCircle;
	bool m_bDetectGcCenter;
	bool m_bCarryBack;
protected:
	//一键初始化线程
	static UINT InitAllThreadFunc(LPVOID lpParam);
	void InitAll();
	///样品台归零的线程
	static UINT SampleStageHomeThreadFunc(LPVOID lpParam);
	void SampleStageHome();
	///自动运行的线程
	CWinThread *pAutoRunThread;
	static UINT AutoRunThreadFunc(LPVOID lpParam);
	void AutoRun();
	//半自动运行的线程
	CWinThread *pSemiRunThread;
	static UINT SemiAutoThreadFunc(LPVOID lpParam);
	void SemiRun();
private:
	bool m_bIsRunning;
	bool m_bAutoSampleState[8];
	//玻璃环中心坐标位置
	Point m_WorkPieceCenter;
	Point m_GlassaCenter;
	//标定偏移值
	int m_offsetX;
	int m_offsetY;
	int m_offsetZ;
	int m_posErrX;
	int m_posErrY;
	int m_posErrZ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


public:
	afx_msg void OnBnClickedClipHome();
	afx_msg void OnBnClickedSampleStageHome();
	afx_msg void OnBnClickedGlueHome();
	afx_msg void OnBnClickedCcdHome();

	afx_msg void OnRadioBtnSetCurrCard(UINT nCmdID);

	afx_msg void OnBnClickedInitClip();
	afx_msg void OnBnClickedInitGlue();
	afx_msg void OnBnClickedInitSampleStage();
	afx_msg void OnBnClickedInitCCDStage();

	afx_msg void OnBnClickedDemarcateCCDPosH();
	afx_msg void OnBnClickedDemarcateCCDPosV();
	afx_msg void OnBnClickedDemarcateSampleStagePos();
	afx_msg void OnBnClickedDemarcateClipRobotPos();
	afx_msg void OnBnClickedDemarcateGluePinPos();
	afx_msg void OnBnClickedDemarcateGlueSuckPos();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnBnClickedAutoRun();
	afx_msg void OnBnClickedPauseOperation();
	afx_msg void OnBnClickedResetAndResumeRun();
	
	afx_msg void OnBnClickedBtnGlueBackZero();
	afx_msg void OnBnClickedBtnSampleStageBackZero();
	afx_msg void OnBnClickedInitGlueSucker();
	afx_msg void OnBnClickedBtnCarry();
	afx_msg void OnBnClickedBtnDetectWpCenter();
	afx_msg void OnBnClickedBtnGlueStart();
	afx_msg void OnBnClickedBtnMountGlassCircle();
	afx_msg void OnBnClickedBtnDetectGcCenter();
	afx_msg void OnBnClickedBtnCarryBack();
	afx_msg void OnBnClickedBtnAllGoHome();
	afx_msg void OnBnClickedBtnAllInit();
	afx_msg void OnBnClickedPauseSemi();
	afx_msg void OnBnClickedResumePosSemi();
};
