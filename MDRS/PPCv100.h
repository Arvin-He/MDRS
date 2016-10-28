// PPCv100.h : main header file for the PPCV100 DLL
//

#if !defined(AFX_PPCV100_H__65DF4DC4_134B_4F94_B26B_0F0E5044438F__INCLUDED_)
#define AFX_PPCV100_H__65DF4DC4_134B_4F94_B26B_0F0E5044438F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"		// main symbols
/*
* �ļ����ƣ�PPCv100.h
*
* ��ǰ�汾��PPCv1.01
* ��    �ߣ����
* ������ڣ�2005��6��27��
*
* �ļ����ƣ�PPCv100.h
*
* ��ǰ�汾��PPCv1.00
* ��    �ߣ����
* ������ڣ�2005��1��31��
*
* ȡ���汾����
* ԭ����  ���Ϸ�ΰ
* ������ڣ�2004��1��
*/
/////////////////////////////////////////////////////////////////////////////
// CPPCv100App
// See PPCv100.cpp for the implementation of this class
//
//����ϵͳ��������
	BOOL	_stdcall	InitParallel(void);
//�жϳ�ʱ
	BOOL	_stdcall	GetTimeOut(void);
//�����ʱ
	void	_stdcall	ClearTimeOut(void);
//���ڼ��
//  BOOL	_stdcall	EppCheck(void);
//��ϵͳ��ȡEPP��Ϣʱ��ȷ��ϵͳ״̬
	BOOL	_stdcall	ReadEppStatus(void);
//��ϵͳ��EPPд����Ϣʱ��ȷ��ϵͳ״̬
	BOOL	_stdcall	WriteEppStatus(void);
//���ϵͳEPP������Ϣ
	BOOL	_stdcall	GetEppInfo(int iMac,int iCh);
//����ϵͳEPP������Ϣ
	BOOL	_stdcall	SetEppInfo(int iMac,int iCh,float maxVoltage,float minVoltage,float maxInterval,
								float minInterval,float maxAD,float minAD,float maxDA,float minDA);
//��λ���ջ�����05��06��24�� ����޸ģ�������λ���ջ��ٶ�̫�����ù���ȡ�������ڷ���������˶�
//	BOOL	_stdcall	EpigynyClose(int iMac,int iCh);
//��λ����������
	BOOL	_stdcall	EpigynyOpen(int iMac,int iCh);
//��λ����������
	BOOL	_stdcall	HypogynyOpen(int iMac,int iCh);
//��λ���ջ�����
	BOOL	_stdcall	HypogynyClose(int iMac,int iCh);
//ģ���������
	BOOL	_stdcall	SimulateCtrl(int iMac,int iCh);
//��ȡָ�����䡢ͨ����λ��
	float	_stdcall	ReadInterval(int iMac,int iCh,
		float maxInterval,float minInterval,
		float maxAD,float minAD);
//��ָ�����䡢ͨ������λ��ָ��
	BOOL	_stdcall	WriteInterval(int iMac,int iCh,float value,
		float maxInterval,float minInterval,
		float maxAD,float minAD);
//��ָ�����䡢ͨ�����͵�ѹָ��
	BOOL	_stdcall	WriteVoltage(int iMac,int iCh,float value,
		float maxVoltage,float minVoltage,
		float maxDA,float minDA);

/********** ϵͳ�궨 **********/
//��ȡAD
 long _stdcall ReadAD(int iMac,int iCh);
//��ȡDA
 long _stdcall ReadDA(int iMac,int iCh);
//���ͱ궨��ѹ
 BOOL _stdcall OutVoltage(int iMac,int iCh,float value,float max,float min,float maxDA,float minDA);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPCV100_H__65DF4DC4_134B_4F94_B26B_0F0E5044438F__INCLUDED_)
