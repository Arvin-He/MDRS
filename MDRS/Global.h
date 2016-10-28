#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
extern Mat g_src1;
extern Mat g_src2;
extern bool g_bIsInit;
//全局函数
//***************************I/O口的操作**************************************//
//***************************报警灯*******************************************//
//清除IO口当前状态
extern void ResetIOState();
//警告红色灯亮
extern void WarningStateRed();	
//IO复位黄灯亮
extern void ResetStateYellow();	
//暂停黄灯亮
extern void PaulStateYellow();	
//正常工作绿灯亮
extern void NormalStateGreen();
//蜂鸣警告	
extern void BeepWarning();
//***************************报警灯*******************************************//
//**************************气缸电磁阀IO操作*********************************//
//张开夹具爪
extern void PuffClaw();	
//收紧夹具爪
extern void FrapClaw();	
//吸取芯片
extern void InhaleChip();	
//放开芯片
extern void ReleaseChip();	
//点胶
extern void DropGlue();	
//**************************气缸电磁阀IO操作*********************************//
//复位
extern void AxisReset(int cardIndex, int nCurrAxis);			//轴IO复位
//归零
extern void AxisGoHome(int cardIndex, int nCurrAxis);
//位置清零
extern void AxisPosClear(int cardIndex, int nCurrAxis);
//停止运动
extern void AxisStop(int cardIndex, int nCurrAxis);
//***************************I/O口的操作**************************************//
extern void AllGoHome();

extern void AllClear();

extern void AllInit();

