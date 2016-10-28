// PPCv100.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PPCv100.h"
#include "DirectPort.h"
#include <conio.h>

#define BaseEpp	0x378

FILE	* streamdll;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//CPPCv100App theApp;
BOOL _stdcall InitParallel()
{

	BOOL aa;
	aa=DP_UseDirectPortMode(true);
	return aa;
}


//�жϳ�ʱ
BOOL _stdcall GetTimeOut(void)
{
	BYTE	byStatus;
	
	byStatus	=	_inp(BaseEpp + 1);
	Sleep(10);
	byStatus	=	byStatus & 0x01;
	
	if(byStatus)
		return TRUE;
	else
		return FALSE;
}

//�����ʱ
 void	_stdcall ClearTimeOut(void)
{
	unsigned	iPort;
	
	iPort	=	_inp(BaseEpp + 1);
	Sleep(10);
	iPort	=	iPort & 0xfe;
	
	_outp(BaseEpp + 1,iPort);
	Sleep(10);

	iPort	=	_inp(BaseEpp + 1);
	Sleep(10);
	iPort	=	iPort | 0x01;
	
	_outp(BaseEpp + 1,iPort);
	Sleep(10);
}

//��ϵͳ��ȡEPP��Ϣʱ��ȷ��ϵͳ״̬
BOOL	_stdcall ReadEppStatus(void)
{
	BYTE	iPort,iStatus;
	
	iPort	=	_inp(BaseEpp + 3);
	iStatus	=	iPort & 0xc0;
	
	if(iStatus == 0xc0)
		return TRUE;
	else
		return FALSE;
}
/*
//���ڼ��
EXPORT BOOL	CALLBACK EppCheck(void)
{
	BYTE	iPort;

	ClearTimeOut();

	_outp(BaseEpp + 2,0x00);
	Sleep(20);

	if(GetTimeOut())
		return FALSE;
//	else
//		return TRUE;

	iPort = _inp(BaseEpp + 3);
	Sleep(20);
	
	iPort = iPort / 64;

	if(iPort != 8 || iPort != 9 || iPort != 10 || iPort != 11 || iPort != 12)
		return FALSE;
	else
		return TRUE;

}
//*/
//��ϵͳ��EPPд����Ϣʱ��ȷ��ϵͳ״̬
BOOL	_stdcall	WriteEppStatus(void)
{
	BYTE	iPort,iStatus;
	
	iPort	=	_inp(BaseEpp + 3);
	iStatus	=	iPort & 0x30;
	
	if(iStatus == 0x30)
		return TRUE;
	else
		return FALSE;
}	

//���ϵͳEPP������Ϣ
BOOL _stdcall GetEppInfo(int iMac,int iCh)
{
	int			i;
	unsigned	iPort;
	BYTE		byPackage[78];
	BYTE		byCheckout=0,byCheckout1,byCheckout2;
	BYTE		byPack1,byPack2;
	int			iPackageLength;
	BYTE		byCommand;
	float		pBuf[10];
	
	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 1 1 0 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����

	ClearTimeOut();

	iPort	=	(unsigned)(224 + 8 + (unsigned)iMac * 2 + (unsigned)iCh);//11101000
	
	_outp(BaseEpp + 3,iPort);
	Sleep(20);


	if(GetTimeOut())
	{
		MessageBox(NULL,TEXT("����ָ�ʱ��"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}

	//��ȡ���ݰ�
	for(i = 0 ; i <= 77 ; i ++)
	{
		byPackage[i]	=	_inp(BaseEpp + 4);
		Sleep(20);
	}
	//�жϳ�ʱ
	if(GetTimeOut())
	{
		MessageBox(NULL,TEXT("�����ݰ�ʱ��ʱ��"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}

    //�����ʼͬ���ַ�"%%"
	if ((byPackage[0] != 37) || (byPackage[1] != 37))
	{
		MessageBox(NULL,TEXT("������ͬ���ַ�����!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	
	//�����ݳ���iPackageLength
	byPack1	=	byPackage[2];
	byPack2	=	byPackage[3];
	iPackageLength	=	byPack1 * 256 + byPack2;
	byCheckout	=	byPack1 + byPack2;
	//������1�ֽ�byCommand
	byCommand	=	byPackage[4];
	byCheckout	+=	byCommand;

	//��ϵͳ��Ϣ
	for(i = 5 ; i < 15 ; i ++)
	{
		byCheckout	+=	byPackage[i];
	}
	//����β = У���� + �����ַ�13
	//����У����
	byCheckout1	=	byPackage[75];
	byCheckout2	=	byPackage[76];

	if(byCheckout1 != 0)
	{
		MessageBox(NULL,TEXT("У��1����!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	if(byCheckout2 != 0)
	{
		MessageBox(NULL,TEXT("У��2����!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	//�����ַ�13
	if(byPackage[77] != 13)
	{
		MessageBox(NULL,TEXT("�����ַ�����!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	//��������
//*/	
	if(iCh	==	0)
	{
		//��Сλ��
		pBuf[2]	=	(float)(byPackage[21] * 256 * 256 * 256 + byPackage[22] * 256 * 256 + byPackage[23] * 256 + byPackage[24]) / 1000.;
		//���λ��
		pBuf[3]	=	(float)(byPackage[27] * 256 * 256 * 256 + byPackage[28] * 256 * 256 + byPackage[29] * 256 + byPackage[30]) / 1000.;
		//��СAD
		pBuf[4]	=	byPackage[25] * 256 + byPackage[26];
		//���AD
		pBuf[5]	=	byPackage[31] * 256 + byPackage[32];
		//��С��ѹ
		pBuf[6]	=	(float)(byPackage[33] * 256 * 256 * 256 + byPackage[34] * 256 * 256 + byPackage[35] * 256 + byPackage[36]) / 1000.;
		//����ѹ
		pBuf[7]	=	(float)(byPackage[39] * 256 * 256 * 256 + byPackage[40] * 256 * 256 + byPackage[41] * 256 + byPackage[42]) / 1000.;
		//��СDA
		pBuf[8]	=	byPackage[37] * 256 + byPackage[38];
		//���DA
		pBuf[9]	=	byPackage[43] * 256 + byPackage[44];
		
	}
	if(iCh	==	1)
	{
		//��Сλ��
		pBuf[2]	=	(float)(byPackage[51] * 256 * 256 * 256 + byPackage[52] * 256 * 256 + byPackage[53] * 256 + byPackage[54]) / 1000.;
		//���λ��
		pBuf[3]	=	(float)(byPackage[57] * 256 * 256 * 256 + byPackage[58] * 256 * 256 + byPackage[59] * 256 + byPackage[60]) / 1000.;
		//��СAD
		pBuf[4]	=	byPackage[55] * 256 + byPackage[56];
		//���AD
		pBuf[5]	=	byPackage[61] * 256 + byPackage[62];
		//��С��ѹ
		pBuf[6]	=	(float)(byPackage[63] * 256 * 256 * 256 + byPackage[64] * 256 * 256 + byPackage[65] * 256 + byPackage[66]) / 1000.;
		//����ѹ
		pBuf[7]	=	(float)(byPackage[69] * 256 * 256 * 256 + byPackage[70] * 256 * 256 + byPackage[71] * 256 + byPackage[72]) / 1000.;
		//��СDA
		pBuf[8]	=	byPackage[67] * 256 + byPackage[68];
		//���DA
		pBuf[9]	=	byPackage[73] * 256 + byPackage[74];
		
	}
	
	pBuf[0]	=	(float)iMac;
	pBuf[1]	=	(float)iCh;

	streamdll	=	fopen("database.txt","at");
	//�����;ͨ����;��С��ѹ;����ѹ;��Сλ��;���λ��;��СDA;���DA;��СAD;���AD
	for(i = 0 ; i <= 9 ; i ++)
	{
		fprintf(streamdll,"%12.4f",pBuf[i]);
	}
	
	fprintf(streamdll,"\n");
	fclose(streamdll);
	
	return TRUE;
}

//��λ���ջ�����
 BOOL	_stdcall EpigynyClose(int iMac,int iCh)
{
	unsigned	iPort;
	
	ClearTimeOut();
	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 0 0 1 0 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ�򵥸�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)144 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;

	return TRUE;
}

//��λ����������
BOOL	_stdcall EpigynyOpen(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 0 1 1 0 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ�򵥸�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)176 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//��λ����������
 BOOL	_stdcall HypogynyOpen(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 0 1 0 0 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ�򵥸�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)160 + (unsigned)iMac * 2 + iCh;//160
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//��λ���ջ�����
BOOL	_stdcall HypogynyClose(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 0 0 0 0 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ�򵥸�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)128 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//ģ���������
BOOL	_stdcall SimulateCtrl(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 1 0 0 0 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ�򵥸�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)192 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//��ȡָ�����䡢ͨ����λ��
float _stdcall ReadInterval(int iMac,int iCh,
								   float maxInterval,float minInterval,
								   float maxAD,float minAD)
{
	float		Interval;
	long		Intervall;
	unsigned	hi,lo;
	unsigned	iPort;
	int			iTime;
	
	//����ָ�� 7 6 5 4 3 2 1 0
	//         0 0 1 1 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	
	iPort	=	(unsigned)56 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
//	Sleep(5);
	iTime	=	GetTickCount();
	while((int)GetTickCount() <= iTime + 5);
	
	hi	=	_inp(BaseEpp + 4);
//	Sleep(5);
	iTime	=	GetTickCount();
	while((int)GetTickCount() <= iTime + 5);

	lo	=	_inp(BaseEpp + 4);
//	Sleep(5);
	iTime	=	GetTickCount();
	while((int)GetTickCount() <= iTime + 5);
	
	Intervall	=	(long)hi * 256 + lo;
	
	Interval	=	(Intervall - minAD) * (maxInterval - minInterval) / (maxAD - minAD);

	return Interval;	
}

//��ָ�����䡢ͨ������λ��ָ��
BOOL	_stdcall WriteInterval(int iMac,int iCh,float value,
								   float maxInterval,float minInterval,
								   float maxAD,float minAD)
{
	unsigned	iPort;
	unsigned	hi,lo;
	long		val;
	
	//����ָ�� 7 6 5 4 3 2 1 0
	//         0 0 1 0 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)40 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(5);
	
	value	*=	1000.;
	val		=	(long)(value * (maxAD - minAD) / ((maxInterval-minInterval) * 1000) + minAD);
	hi		=	(unsigned)val / 256;
	lo		=	val	- hi * 256;
	
	_outp(BaseEpp + 4,hi);
	Sleep(5);
	
	_outp(BaseEpp + 4,lo);
	Sleep(5);

	if(GetTimeOut())
		return FALSE;

	
	return TRUE;
}

//��ָ�����䡢ͨ�����͵�ѹָ��
BOOL	_stdcall	WriteVoltage(int iMac,int iCh,float value,
										 float maxVoltage,float minVoltage,
										 float maxDA,float minDA)
{
	unsigned	iPort;
	unsigned	hi,lo;
	long		val=0;

	//����ָ�� 7 6 5 4 3 2 1 0
	//         0 0 0 0 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	iPort	=	(unsigned)8 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(5);
	
	if(val > maxVoltage)
		val = maxVoltage;
	if(val < minVoltage)
		val = minVoltage;
//2004.7.14�������޸ģ�ԭ�����㷨����
	value*=1000.;
	//val		=	(long)(value * 65535 / (maxVoltage * 2) + 32767);
	val		=	(long)(value - 0) * (maxDA - 32767) / (maxVoltage - 0)/1000. + 32767;
	//val		=	(long)(value - minVoltage) * (maxDA - minDA) / (2 * maxVoltage) + minDA;
	//val		=	(long)(value * (maxDA - minDA) / ((maxVoltage-minVoltage) * 1000) + minDA);
	hi		=	(unsigned)val / 256;
	lo		=	val	- hi * 256;
	
	_outp(BaseEpp + 4,hi);
	Sleep(5);
	
	_outp(BaseEpp + 4,lo);
	Sleep(5);

	if(GetTimeOut())
		return FALSE;

	return TRUE;
}
//ϵͳ�궨
//���ͱ궨��ѹ ***ʹ��
 BOOL _stdcall OutVoltage(int iMac,int iCh,float value,float max,float min,float maxDA,float minDA)
{
	unsigned	iPort;
	unsigned	hi,lo;
	long		val;
	static	int	iSetFlag;

	iSetFlag = 0;

	if(value > max)
	{
		if(MessageBox(NULL,TEXT("ϵͳҪ�궨�ĵ�ѹ���ֵ������ϵͳ�趨���ֵҪ������Ҫ�궨��"),TEXT("�궨��Ϣ"),MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			iSetFlag	=	1;
		}
		else
			return FALSE;
		
	}
	if(value < min)
	{
		if(MessageBox(NULL,TEXT("ϵͳҪ�궨�ĵ�ѹ��Сֵ������ϵͳ�趨��СֵҪС������Ҫ�궨��"),TEXT("�궨��Ϣ"),MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			iSetFlag	=	2;
		}
		else
			return FALSE;
	}

//	if(iSetFlag == 0)
//	{
		val		=	(long)(value - 0) * (maxDA - 32767) / (max - 0) + 32767;
//	}
	//���͵�ѹָ��
	//����ָ�� 7 6 5 4 3 2 1 0
	//         0 0 0 0 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	
	iPort	=	(unsigned)8 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(5);

	hi		=	(unsigned)val / 256;
	lo		=	val	- hi * 256;
	
	_outp(BaseEpp + 4,hi);
	Sleep(5);
	
	_outp(BaseEpp + 4,lo);
	Sleep(5);
	
	return TRUE;
}

//��ȡAD *** ʹ��
long _stdcall ReadAD(int iMac,int iCh)
{
	long		AD;
	unsigned	hi,lo;
	unsigned	iPort;
	//����ָ�� 7 6 5 4 3 2 1 0
	//         0 0 1 1 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����
	
	iPort	=	(unsigned)56 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(5);
	
	hi	=	_inp(BaseEpp + 4);
	Sleep(5);
	lo	=	_inp(BaseEpp + 4);
	Sleep(5);
	
	AD	=	(long)hi * 256 + lo;
	
	return AD;	
}
//��ȡDA ***δʹ��
long _stdcall ReadDA(int iMac,int iCh)
{
	long		DA;
	unsigned	hi,lo;
	unsigned	iPort;

	//����ָ�� 7 6 5 4 3 2 1 0
	//         0 0 0 1 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����

	iPort	=	(unsigned)24 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(5);
	
	hi	=	_inp(BaseEpp + 4);
	Sleep(5);
	lo	=	_inp(BaseEpp + 4);
	Sleep(5);
	
	DA	=	(long)hi * 256 + lo;
	
	return DA;	
}
//����ϵͳEPP������Ϣ
 BOOL _stdcall SetEppInfo(int iMac,int iCh,float maxVoltage,float minVoltage,float maxInterval,
								float minInterval,float maxAD,float minAD,float maxDA,float minDA)
{
	int			i;
	unsigned	iPort;
	BYTE		byPackage[78];
		
	BYTE		IntervalMin[4];
	BYTE		IntervalMax[4];
	BYTE		ADMin[2];
	BYTE		ADMax[2];
	BYTE		VoltageMin[4];
	BYTE		VoltageMax[4];
	BYTE		DAMin[2];
	BYTE		DAMax[2];

	ClearTimeOut();

	//����ָ�� 7 6 5 4 3 2 1 0
	//         1 1 0 1 1 x x x
	//����λ7 6 5 4Ϊ�����,λ3��ʾ���Ӹ�ʽ,λ2 1Ϊ�����,λ0Ϊͨ����

	iPort	=	(unsigned)(208 + 8 + (unsigned)iMac * 2 + iCh);//11011000
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	//�жϳ�ʱ
	if(GetTimeOut())
	{
		MessageBox(NULL,TEXT("�趨ϵͳEPP��ʱ��ʱ��"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	//�Ϸ��Լ��
    //��ʼͬ���ַ�"%%"
	byPackage[0] = 37;
	byPackage[1] = 37;

	//�����ݳ��� 78 byte
	byPackage[2] = 0;
	byPackage[3] = 78;
	
	//������1�ֽ�iPort
	byPackage[4] = iPort;
	
	//����ϵͳ��Ϣ
	byPackage[5] = 10; //ϵͳ��Ϣ�ֽ���
	byPackage[6] =  1; //�ܹ�ͨ����
	byPackage[7] = 30; //ͨ����Ϣ�ֽ���

	for (i=8;i<15;i++) 
		byPackage[i]=0;
	
	//��������
	//ͨ����������
	minInterval = minInterval * 1000;
	
	IntervalMin[0] = (BYTE)(minInterval / (256 * 256 * 256));
	IntervalMin[1] = (BYTE)(minInterval / (256 * 256));
	IntervalMin[2] = (BYTE)(minInterval / 256);
	IntervalMin[3] = (BYTE)minInterval;	
	
	maxInterval = maxInterval * 1000;

	IntervalMax[0] = (BYTE)(maxInterval / (256 * 256 * 256));
	IntervalMax[1] = (BYTE)(maxInterval / (256 * 256));
	IntervalMax[2] = (BYTE)(maxInterval /  256);
	IntervalMax[3] = (BYTE)maxInterval;
	
	//ADֵ
	ADMin[0] = (BYTE)(minAD / 256);
	ADMin[1] = (BYTE)minAD;

	ADMax[0] = (BYTE)(maxAD / 256);
	ADMax[1] = (BYTE)maxAD;
	
	//��ʼ��ѹ
	minVoltage = minVoltage * 1000;
	
	VoltageMin[0] = (BYTE)((unsigned)minVoltage / (256 * 256 * 256));
	VoltageMin[1] = (BYTE)((unsigned)minVoltage / (256 * 256));
	VoltageMin[2] = (BYTE)((unsigned)minVoltage / 256);
	VoltageMin[3] = (BYTE)(unsigned)minVoltage;
	
	//������ѹ
	maxVoltage = maxVoltage * 1000;
	
	VoltageMax[0] = (BYTE)(maxVoltage / (256. * 256. * 256.));
	VoltageMax[1] = (BYTE)(maxVoltage / (256. * 256.));
	VoltageMax[2] = (BYTE)(maxVoltage / 256.);
	VoltageMax[3] = (BYTE)maxVoltage;
	
	//DAֵ
	DAMin[0] = (BYTE)(minDA / 256);
	DAMin[1] = (BYTE)minDA;
	
	DAMax[0] = (BYTE)(maxDA / 256);
	DAMax[1] = (BYTE)maxDA;
	
	//ͨ��0 ��Ϣ30���ֽ�
	if(iCh == 0)
	{
		byPackage[15] = iMac;
		//byPackage[16] ����
		byPackage[17] = 255;
		byPackage[18] = 255;
		byPackage[19] = 0;
		byPackage[20] = 0;
		byPackage[21] = IntervalMin[0];
		byPackage[22] = IntervalMin[1];
		byPackage[23] = IntervalMin[2];
		byPackage[24] = IntervalMin[3];
		byPackage[25] = ADMin[0];
		byPackage[26] = ADMin[1];
		byPackage[27] = IntervalMax[0];
		byPackage[28] = IntervalMax[1];
		byPackage[29] = IntervalMax[2];
		byPackage[30] = IntervalMax[3];
		byPackage[31] = ADMax[0];
		byPackage[32] = ADMax[1];
		byPackage[33] = VoltageMin[0];
		byPackage[34] = VoltageMin[1];
		byPackage[35] = VoltageMin[2];
		byPackage[36] = VoltageMin[3];
		byPackage[37] = DAMin[0];
		byPackage[38] = DAMin[1];
		byPackage[39] = VoltageMax[0];
		byPackage[40] = VoltageMax[1];
		byPackage[41] = VoltageMax[2];
		byPackage[42] = VoltageMax[3];
		byPackage[43] = DAMax[0];
		byPackage[44] = DAMax[1];
		
	}
	else
	{
		byPackage[45] = iMac;
		//byPackage[46] ����
		byPackage[47] = 255;
		byPackage[48] = 255;
		byPackage[49] = 0;
		byPackage[50] = 0;
		byPackage[51] = IntervalMin[0];
		byPackage[52] = IntervalMin[1];
		byPackage[53] = IntervalMin[2];
		byPackage[54] = IntervalMin[3];
		byPackage[55] = ADMin[0];
		byPackage[56] = ADMin[1];
		byPackage[57] = IntervalMax[0];
		byPackage[58] = IntervalMax[1];
		byPackage[59] = IntervalMax[2];
		byPackage[60] = IntervalMax[3];
		byPackage[61] = ADMax[0];
		byPackage[62] = ADMax[1];
		byPackage[63] = VoltageMin[0];
		byPackage[64] = VoltageMin[1];
		byPackage[65] = VoltageMin[2];
		byPackage[66] = VoltageMin[3];
		byPackage[67] = DAMin[0];
		byPackage[68] = DAMin[1];
		byPackage[69] = VoltageMax[0];
		byPackage[70] = VoltageMax[1];
		byPackage[71] = VoltageMax[2];
		byPackage[72] = VoltageMax[3];
		byPackage[73] = DAMax[0];
		byPackage[74] = DAMax[1];
		
	}	
	//����β=У����+�����ַ�13
	//����У����
	byPackage[75] = 0;
	byPackage[76] = 0;    
	//�����ַ�13
	byPackage[77] = 13;

	//��EPP��д����Ϣ
	for(i = 0 ; i <= 77 ; i ++)
	{
		_outp(BaseEpp + 4,byPackage[i]);
		Sleep(20);
	}
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}