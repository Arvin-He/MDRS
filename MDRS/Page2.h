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
	//�н�����
	void ClipWorkpiece(int nColIndex);
	//���˹���
	void CarryWorkpiece(/*int rows, int cols*/);
	//���ù���
	void DropWorkpiece();
	void ClipMoveWaitPos();
	//�㽺����
	void GlueWorkpiece();
	//��ȡԲƬ
	void AbsorbWorkpiece(int rows, int cols, int nCounts);
	void MoveStage_CCD();
	//���˲�����
	void CarryGlassCircle();
	void MoveBackStage_CCD();
	//ʶ����������������λ��
	void Detect_AdjustGCPos();
	//���ò�����
	void PutDownChip();
	//��ѹԲƬ
//	void PressCirclePane();
	void MoveToWP();
	//����������ԭλ
	void PutBackWP(int rows, int cols);
	//�ص�׼��λ��
	void MoveToPreparePos(int rows, int cols, bool colChangeFlag);
	//�㽺ͷ����ȫλ��
	void GlueMoveSafePos();
	//ͼ�����
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

	//���Զ��߳�ִ�б�־
	bool m_bCarry;
	bool m_bDetectWpCenter;
	bool m_bGlueStart;
	bool m_bMountGlassCircle;
	bool m_bDetectGcCenter;
	bool m_bCarryBack;
protected:
	//һ����ʼ���߳�
	static UINT InitAllThreadFunc(LPVOID lpParam);
	void InitAll();
	///��Ʒ̨������߳�
	static UINT SampleStageHomeThreadFunc(LPVOID lpParam);
	void SampleStageHome();
	///�Զ����е��߳�
	CWinThread *pAutoRunThread;
	static UINT AutoRunThreadFunc(LPVOID lpParam);
	void AutoRun();
	//���Զ����е��߳�
	CWinThread *pSemiRunThread;
	static UINT SemiAutoThreadFunc(LPVOID lpParam);
	void SemiRun();
private:
	bool m_bIsRunning;
	bool m_bAutoSampleState[8];
	//��������������λ��
	Point m_WorkPieceCenter;
	Point m_GlassaCenter;
	//�궨ƫ��ֵ
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
