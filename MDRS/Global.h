#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
extern Mat g_src1;
extern Mat g_src2;
extern bool g_bIsInit;
//ȫ�ֺ���
//***************************I/O�ڵĲ���**************************************//
//***************************������*******************************************//
//���IO�ڵ�ǰ״̬
extern void ResetIOState();
//�����ɫ����
extern void WarningStateRed();	
//IO��λ�Ƶ���
extern void ResetStateYellow();	
//��ͣ�Ƶ���
extern void PaulStateYellow();	
//���������̵���
extern void NormalStateGreen();
//��������	
extern void BeepWarning();
//***************************������*******************************************//
//**************************���׵�ŷ�IO����*********************************//
//�ſ��о�צ
extern void PuffClaw();	
//�ս��о�צ
extern void FrapClaw();	
//��ȡоƬ
extern void InhaleChip();	
//�ſ�оƬ
extern void ReleaseChip();	
//�㽺
extern void DropGlue();	
//**************************���׵�ŷ�IO����*********************************//
//��λ
extern void AxisReset(int cardIndex, int nCurrAxis);			//��IO��λ
//����
extern void AxisGoHome(int cardIndex, int nCurrAxis);
//λ������
extern void AxisPosClear(int cardIndex, int nCurrAxis);
//ֹͣ�˶�
extern void AxisStop(int cardIndex, int nCurrAxis);
//***************************I/O�ڵĲ���**************************************//
extern void AllGoHome();

extern void AllClear();

extern void AllInit();

