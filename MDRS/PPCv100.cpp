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


//判断超时
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

//清除超时
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

//当系统读取EPP信息时，确定系统状态
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
//并口检测
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
//当系统向EPP写入信息时，确定系统状态
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

//获得系统EPP并口信息
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
	
	//控制指令 7 6 5 4 3 2 1 0
	//         1 1 1 0 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号

	ClearTimeOut();

	iPort	=	(unsigned)(224 + 8 + (unsigned)iMac * 2 + (unsigned)iCh);//11101000
	
	_outp(BaseEpp + 3,iPort);
	Sleep(20);


	if(GetTimeOut())
	{
		MessageBox(NULL,TEXT("发送指令超时！"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}

	//读取数据包
	for(i = 0 ; i <= 77 ; i ++)
	{
		byPackage[i]	=	_inp(BaseEpp + 4);
		Sleep(20);
	}
	//判断超时
	if(GetTimeOut())
	{
		MessageBox(NULL,TEXT("读数据包时超时！"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}

    //检查起始同步字符"%%"
	if ((byPackage[0] != 37) || (byPackage[1] != 37))
	{
		MessageBox(NULL,TEXT("包数据同步字符错误!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	
	//包数据长度iPackageLength
	byPack1	=	byPackage[2];
	byPack2	=	byPackage[3];
	iPackageLength	=	byPack1 * 256 + byPack2;
	byCheckout	=	byPack1 + byPack2;
	//命令字1字节byCommand
	byCommand	=	byPackage[4];
	byCheckout	+=	byCommand;

	//读系统信息
	for(i = 5 ; i < 15 ; i ++)
	{
		byCheckout	+=	byPackage[i];
	}
	//读包尾 = 校验字 + 结束字符13
	//计算校验字
	byCheckout1	=	byPackage[75];
	byCheckout2	=	byPackage[76];

	if(byCheckout1 != 0)
	{
		MessageBox(NULL,TEXT("校验1错误!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	if(byCheckout2 != 0)
	{
		MessageBox(NULL,TEXT("校验2错误!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	//结束字符13
	if(byPackage[77] != 13)
	{
		MessageBox(NULL,TEXT("结束字符错误!"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	//处理数据
//*/	
	if(iCh	==	0)
	{
		//最小位移
		pBuf[2]	=	(float)(byPackage[21] * 256 * 256 * 256 + byPackage[22] * 256 * 256 + byPackage[23] * 256 + byPackage[24]) / 1000.;
		//最大位移
		pBuf[3]	=	(float)(byPackage[27] * 256 * 256 * 256 + byPackage[28] * 256 * 256 + byPackage[29] * 256 + byPackage[30]) / 1000.;
		//最小AD
		pBuf[4]	=	byPackage[25] * 256 + byPackage[26];
		//最大AD
		pBuf[5]	=	byPackage[31] * 256 + byPackage[32];
		//最小电压
		pBuf[6]	=	(float)(byPackage[33] * 256 * 256 * 256 + byPackage[34] * 256 * 256 + byPackage[35] * 256 + byPackage[36]) / 1000.;
		//最大电压
		pBuf[7]	=	(float)(byPackage[39] * 256 * 256 * 256 + byPackage[40] * 256 * 256 + byPackage[41] * 256 + byPackage[42]) / 1000.;
		//最小DA
		pBuf[8]	=	byPackage[37] * 256 + byPackage[38];
		//最大DA
		pBuf[9]	=	byPackage[43] * 256 + byPackage[44];
		
	}
	if(iCh	==	1)
	{
		//最小位移
		pBuf[2]	=	(float)(byPackage[51] * 256 * 256 * 256 + byPackage[52] * 256 * 256 + byPackage[53] * 256 + byPackage[54]) / 1000.;
		//最大位移
		pBuf[3]	=	(float)(byPackage[57] * 256 * 256 * 256 + byPackage[58] * 256 * 256 + byPackage[59] * 256 + byPackage[60]) / 1000.;
		//最小AD
		pBuf[4]	=	byPackage[55] * 256 + byPackage[56];
		//最大AD
		pBuf[5]	=	byPackage[61] * 256 + byPackage[62];
		//最小电压
		pBuf[6]	=	(float)(byPackage[63] * 256 * 256 * 256 + byPackage[64] * 256 * 256 + byPackage[65] * 256 + byPackage[66]) / 1000.;
		//最大电压
		pBuf[7]	=	(float)(byPackage[69] * 256 * 256 * 256 + byPackage[70] * 256 * 256 + byPackage[71] * 256 + byPackage[72]) / 1000.;
		//最小DA
		pBuf[8]	=	byPackage[67] * 256 + byPackage[68];
		//最大DA
		pBuf[9]	=	byPackage[73] * 256 + byPackage[74];
		
	}
	
	pBuf[0]	=	(float)iMac;
	pBuf[1]	=	(float)iCh;

	streamdll	=	fopen("database.txt","at");
	//机箱号;通道号;最小电压;最大电压;最小位移;最大位移;最小DA;最大DA;最小AD;最大AD
	for(i = 0 ; i <= 9 ; i ++)
	{
		fprintf(streamdll,"%12.4f",pBuf[i]);
	}
	
	fprintf(streamdll,"\n");
	fclose(streamdll);
	
	return TRUE;
}

//上位机闭环控制
 BOOL	_stdcall EpigynyClose(int iMac,int iCh)
{
	unsigned	iPort;
	
	ClearTimeOut();
	//控制指令 7 6 5 4 3 2 1 0
	//         1 0 0 1 0 x x x
	//其中位7 6 5 4为命令号,位3表示简单格式,位2 1为机箱号,位0为通道号
	iPort	=	(unsigned)144 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;

	return TRUE;
}

//上位机开环控制
BOOL	_stdcall EpigynyOpen(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//控制指令 7 6 5 4 3 2 1 0
	//         1 0 1 1 0 x x x
	//其中位7 6 5 4为命令号,位3表示简单格式,位2 1为机箱号,位0为通道号
	iPort	=	(unsigned)176 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//下位机开环控制
 BOOL	_stdcall HypogynyOpen(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//控制指令 7 6 5 4 3 2 1 0
	//         1 0 1 0 0 x x x
	//其中位7 6 5 4为命令号,位3表示简单格式,位2 1为机箱号,位0为通道号
	iPort	=	(unsigned)160 + (unsigned)iMac * 2 + iCh;//160
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//下位机闭环控制
BOOL	_stdcall HypogynyClose(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//控制指令 7 6 5 4 3 2 1 0
	//         1 0 0 0 0 x x x
	//其中位7 6 5 4为命令号,位3表示简单格式,位2 1为机箱号,位0为通道号
	iPort	=	(unsigned)128 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//模拟控制命令
BOOL	_stdcall SimulateCtrl(int iMac,int iCh)
{
	unsigned	iPort;

	ClearTimeOut();

	//控制指令 7 6 5 4 3 2 1 0
	//         1 1 0 0 0 x x x
	//其中位7 6 5 4为命令号,位3表示简单格式,位2 1为机箱号,位0为通道号
	iPort	=	(unsigned)192 + (unsigned)iMac * 2 + iCh;
	_outp(BaseEpp + 3,iPort);
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}

//读取指定机箱、通道的位移
float _stdcall ReadInterval(int iMac,int iCh,
								   float maxInterval,float minInterval,
								   float maxAD,float minAD)
{
	float		Interval;
	long		Intervall;
	unsigned	hi,lo;
	unsigned	iPort;
	int			iTime;
	
	//控制指令 7 6 5 4 3 2 1 0
	//         0 0 1 1 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号
	
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

//向指定机箱、通道发送位移指令
BOOL	_stdcall WriteInterval(int iMac,int iCh,float value,
								   float maxInterval,float minInterval,
								   float maxAD,float minAD)
{
	unsigned	iPort;
	unsigned	hi,lo;
	long		val;
	
	//控制指令 7 6 5 4 3 2 1 0
	//         0 0 1 0 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号
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

//向指定机箱、通道发送电压指令
BOOL	_stdcall	WriteVoltage(int iMac,int iCh,float value,
										 float maxVoltage,float minVoltage,
										 float maxDA,float minDA)
{
	unsigned	iPort;
	unsigned	hi,lo;
	long		val=0;

	//控制指令 7 6 5 4 3 2 1 0
	//         0 0 0 0 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号
	iPort	=	(unsigned)8 + (unsigned)iMac * 2 + (unsigned)iCh;
	_outp(BaseEpp + 3,iPort);
	Sleep(5);
	
	if(val > maxVoltage)
		val = maxVoltage;
	if(val < minVoltage)
		val = minVoltage;
//2004.7.14李天智修改，原来的算法不对
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
//系统标定
//发送标定电压 ***使用
 BOOL _stdcall OutVoltage(int iMac,int iCh,float value,float max,float min,float maxDA,float minDA)
{
	unsigned	iPort;
	unsigned	hi,lo;
	long		val;
	static	int	iSetFlag;

	iSetFlag = 0;

	if(value > max)
	{
		if(MessageBox(NULL,TEXT("系统要标定的电压最大值比现在系统设定最大值要大！您仍要标定吗？"),TEXT("标定信息"),MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			iSetFlag	=	1;
		}
		else
			return FALSE;
		
	}
	if(value < min)
	{
		if(MessageBox(NULL,TEXT("系统要标定的电压最小值比现在系统设定最小值要小！您仍要标定吗？"),TEXT("标定信息"),MB_YESNO | MB_ICONQUESTION) == IDYES)
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
	//发送电压指令
	//控制指令 7 6 5 4 3 2 1 0
	//         0 0 0 0 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号
	
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

//读取AD *** 使用
long _stdcall ReadAD(int iMac,int iCh)
{
	long		AD;
	unsigned	hi,lo;
	unsigned	iPort;
	//控制指令 7 6 5 4 3 2 1 0
	//         0 0 1 1 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号
	
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
//读取DA ***未使用
long _stdcall ReadDA(int iMac,int iCh)
{
	long		DA;
	unsigned	hi,lo;
	unsigned	iPort;

	//控制指令 7 6 5 4 3 2 1 0
	//         0 0 0 1 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号

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
//设置系统EPP并口信息
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

	//控制指令 7 6 5 4 3 2 1 0
	//         1 1 0 1 1 x x x
	//其中位7 6 5 4为命令号,位3表示复杂格式,位2 1为机箱号,位0为通道号

	iPort	=	(unsigned)(208 + 8 + (unsigned)iMac * 2 + iCh);//11011000
	_outp(BaseEpp + 3,iPort);
	Sleep(10);
	//判断超时
	if(GetTimeOut())
	{
		MessageBox(NULL,TEXT("设定系统EPP口时超时！"),TEXT("ERROR"),MB_ICONERROR);
		return FALSE;
	}
	//合法性检查
    //起始同步字符"%%"
	byPackage[0] = 37;
	byPackage[1] = 37;

	//包数据长度 78 byte
	byPackage[2] = 0;
	byPackage[3] = 78;
	
	//命令字1字节iPort
	byPackage[4] = iPort;
	
	//设置系统信息
	byPackage[5] = 10; //系统信息字节数
	byPackage[6] =  1; //总共通道数
	byPackage[7] = 30; //通道信息字节数

	for (i=8;i<15;i++) 
		byPackage[i]=0;
	
	//处理数据
	//通道零点和满度
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
	
	//AD值
	ADMin[0] = (BYTE)(minAD / 256);
	ADMin[1] = (BYTE)minAD;

	ADMax[0] = (BYTE)(maxAD / 256);
	ADMax[1] = (BYTE)maxAD;
	
	//起始电压
	minVoltage = minVoltage * 1000;
	
	VoltageMin[0] = (BYTE)((unsigned)minVoltage / (256 * 256 * 256));
	VoltageMin[1] = (BYTE)((unsigned)minVoltage / (256 * 256));
	VoltageMin[2] = (BYTE)((unsigned)minVoltage / 256);
	VoltageMin[3] = (BYTE)(unsigned)minVoltage;
	
	//结束电压
	maxVoltage = maxVoltage * 1000;
	
	VoltageMax[0] = (BYTE)(maxVoltage / (256. * 256. * 256.));
	VoltageMax[1] = (BYTE)(maxVoltage / (256. * 256.));
	VoltageMax[2] = (BYTE)(maxVoltage / 256.);
	VoltageMax[3] = (BYTE)maxVoltage;
	
	//DA值
	DAMin[0] = (BYTE)(minDA / 256);
	DAMin[1] = (BYTE)minDA;
	
	DAMax[0] = (BYTE)(maxDA / 256);
	DAMax[1] = (BYTE)maxDA;
	
	//通道0 信息30个字节
	if(iCh == 0)
	{
		byPackage[15] = iMac;
		//byPackage[16] 保留
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
		//byPackage[46] 保留
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
	//读包尾=校验字+结束字符13
	//计算校验字
	byPackage[75] = 0;
	byPackage[76] = 0;    
	//结束字符13
	byPackage[77] = 13;

	//向EPP口写入信息
	for(i = 0 ; i <= 77 ; i ++)
	{
		_outp(BaseEpp + 4,byPackage[i]);
		Sleep(20);
	}
	
	if(GetTimeOut())
		return FALSE;
	
	return TRUE;
}