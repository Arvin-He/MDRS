#include "DirectPort.h"
#pragma comment(lib, "DirectPort.lib")

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define	EXPORT __declspec(dllexport)
#endif
//设定中断号
EXPORT unsigned short CALLBACK    GetData(void);
//设定中断号
EXPORT  void    CALLBACK    SetData(unsigned short);
//加载系统并口驱动，需指定并口的控制口输入号(通常为并口的四字节输入地址那个)
EXPORT	BOOL	CALLBACK	InitParallel(unsigned short usData, unsigned short usCtrl);
//判断超时
EXPORT	BOOL	CALLBACK	GetTimeOut(unsigned short usData);
//清除超时
EXPORT	void	CALLBACK	ClearTimeOut(unsigned short usData);
//并口检测
//EXPORT  BOOL	CALLBACK	EppCheck(void);
//当系统读取EPP信息时，确定系统状态
EXPORT	BOOL	CALLBACK	ReadEppStatus(unsigned short usData);
//当系统向EPP写入信息时，确定系统状态
EXPORT	BOOL	CALLBACK	WriteEppStatus(unsigned short usData);
//获得系统EPP并口信息
EXPORT	BOOL	CALLBACK	GetEppInfo(unsigned short usData,int,int);
//设置系统EPP并口信息
EXPORT	BOOL	CALLBACK	SetEppInfo(unsigned short usData,int,int,float,float,float,float,float,float,float,float);
//上位机闭环控制
EXPORT	BOOL	CALLBACK	EpigynyClose(unsigned short usData,int,int);
//上位机开环控制
EXPORT	BOOL	CALLBACK	EpigynyOpen(unsigned short usData,int,int);
//下位机开环控制
EXPORT	BOOL	CALLBACK	HypogynyOpen(unsigned short usData,int,int);
//下位机闭环控制
EXPORT	BOOL	CALLBACK	HypogynyClose(unsigned short usData,int,int);
//模拟控制命令
EXPORT	BOOL	CALLBACK	SimulateCtrl(unsigned short usData,int,int);
//读取指定机箱、通道的位移
EXPORT	float	CALLBACK	ReadInterval(unsigned short usData,int,int,float,float,float,float);
//向指定机箱、通道发送位移指令
EXPORT	BOOL	CALLBACK	WriteInterval(unsigned short usData,int,int,float,float,float,float,float);
//向指定机箱、通道发送电压指令
EXPORT	BOOL	CALLBACK	WriteVoltage(unsigned short usData,int,int,float,float,float,float,float);

/********** 系统标定 **********/
//读取AD
EXPORT long CALLBACK ReadAD(unsigned short usData,int,int);
//读取DA
EXPORT long CALLBACK ReadDA(unsigned short usData,int,int);
//发送标定电压
EXPORT BOOL CALLBACK OutVoltage(unsigned short usData,int,int,float,float,float,float,float);


