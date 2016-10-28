// PPCv100.h : main header file for the PPCV100 DLL
//

#if !defined(AFX_PPCV100_H__65DF4DC4_134B_4F94_B26B_0F0E5044438F__INCLUDED_)
#define AFX_PPCV100_H__65DF4DC4_134B_4F94_B26B_0F0E5044438F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"		// main symbols
/*
* 文件名称：PPCv100.h
*
* 当前版本：PPCv1.01
* 作    者：李寒冰
* 完成日期：2005年6月27日
*
* 文件名称：PPCv100.h
*
* 当前版本：PPCv1.00
* 作    者：李长峰
* 完成日期：2005年1月31日
*
* 取代版本：无
* 原作者  ：孟凡伟
* 完成日期：2004年1月
*/
/////////////////////////////////////////////////////////////////////////////
// CPPCv100App
// See PPCv100.cpp for the implementation of this class
//
//加载系统并口驱动
	BOOL	_stdcall	InitParallel(void);
//判断超时
	BOOL	_stdcall	GetTimeOut(void);
//清除超时
	void	_stdcall	ClearTimeOut(void);
//并口检测
//  BOOL	_stdcall	EppCheck(void);
//当系统读取EPP信息时，确定系统状态
	BOOL	_stdcall	ReadEppStatus(void);
//当系统向EPP写入信息时，确定系统状态
	BOOL	_stdcall	WriteEppStatus(void);
//获得系统EPP并口信息
	BOOL	_stdcall	GetEppInfo(int iMac,int iCh);
//设置系统EPP并口信息
	BOOL	_stdcall	SetEppInfo(int iMac,int iCh,float maxVoltage,float minVoltage,float maxInterval,
								float minInterval,float maxAD,float minAD,float maxDA,float minDA);
//上位机闭环控制05年06月24日 李寒冰修改，由于上位机闭环速度太慢，该功能取消，请在发行中清除此段
//	BOOL	_stdcall	EpigynyClose(int iMac,int iCh);
//上位机开环控制
	BOOL	_stdcall	EpigynyOpen(int iMac,int iCh);
//下位机开环控制
	BOOL	_stdcall	HypogynyOpen(int iMac,int iCh);
//下位机闭环控制
	BOOL	_stdcall	HypogynyClose(int iMac,int iCh);
//模拟控制命令
	BOOL	_stdcall	SimulateCtrl(int iMac,int iCh);
//读取指定机箱、通道的位移
	float	_stdcall	ReadInterval(int iMac,int iCh,
		float maxInterval,float minInterval,
		float maxAD,float minAD);
//向指定机箱、通道发送位移指令
	BOOL	_stdcall	WriteInterval(int iMac,int iCh,float value,
		float maxInterval,float minInterval,
		float maxAD,float minAD);
//向指定机箱、通道发送电压指令
	BOOL	_stdcall	WriteVoltage(int iMac,int iCh,float value,
		float maxVoltage,float minVoltage,
		float maxDA,float minDA);

/********** 系统标定 **********/
//读取AD
 long _stdcall ReadAD(int iMac,int iCh);
//读取DA
 long _stdcall ReadDA(int iMac,int iCh);
//发送标定电压
 BOOL _stdcall OutVoltage(int iMac,int iCh,float value,float max,float min,float maxDA,float minDA);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPCV100_H__65DF4DC4_134B_4F94_B26B_0F0E5044438F__INCLUDED_)
