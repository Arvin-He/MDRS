//文  件  名: DirectPort.h
//作      者: eBitWen
//e-Mail    : eBitWen@163.net
//发 布日 期: 2003-5-20 
//版      本: 1.0.0.0
//内核  版本: 1.0.0.0
//说      明: 这是一个提供在nt/win2000/xp下直接读写端口接口的库
//			  本软件是一个免费软件,无任何限制,允许在任何场合使用,但不保证后果.
//            发现Bug请mail至eBitWen@163.net
#ifndef _DIRECTPORT_1856D21A_B15D_4a69_86A5_FE6CD4D69E30_INCLUDE
#define _DIRECTPORT_1856D21A_B15D_4a69_86A5_FE6CD4D69E30_INCLUDE

#ifdef  DIRECTPORTDLL_EXPORTS  
#define DIRECTPORTAPI  __declspec(dllexport) 
#else
#define DIRECTPORTAPI  __declspec(dllimport) 
#endif

#ifndef WINAPI
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//功能: 
//		从端口读入一个字节
//描述:
//参数:
//		lPort: 端口号
//返回:
//      无
DWORD  DIRECTPORTAPI DP_InpB(long lPort);

//功能: 
//		从端口读入一个字
//描述:
//参数:
//		lPort: 端口号
//返回:
//      无
DWORD  DIRECTPORTAPI DP_InpW(long lPort);

//功能: 
//		从端口读入一个双字
//描述:
//参数:
//		lPort: 端口号
//返回:
//      无
DWORD  DIRECTPORTAPI DP_InpD(long lPort);

//功能: 
//		向端口输出一个字节
//描述:
//参数:
//		lPort: 端口号
//		cData: 要输出的字节
//返回:
//      无
void   DIRECTPORTAPI DP_OutpB(long lPort,BYTE cData);

//功能: 
//		向端口输出一个字
//描述:
//参数:
//		lPort: 端口号
//		cData: 要输出的字
//返回:
//      无
void DIRECTPORTAPI DP_OutpW(long lPort,WORD wData);

//功能: 
//		向端口输出一个双字
//描述:
//参数:
//		lPort: 端口号
//		cData: 要输出的双字
//返回:
//      无
void DIRECTPORTAPI DP_OutpD(long lPort,DWORD dData);


//功能: 
//		在 NT/Win2000/XP 下是否使用直接端口模式
//描述:
//		当不使用内核端口模式时:
//		可以在程序中直接使用__inp,__inpw,__inpd,__outp,__outw,__outd等 c 函数 ,
//		方便一些遗留程序的继续使用
//		上面六个函数亦直接使用端口输入输出指令,不用通过内核代码的转换,因而效率更高.
//		当使用内核端口模式时:
//		上面六个函数需要通过内核代码读写端口
//参数
//		bDirect: TRUE 不使用内核端口模式
//				FALSE 使用内核端口模式
//返回:
//     成功返回 TRUE
//     失败返回 FALSE     
BOOL DIRECTPORTAPI DP_UseDirectPortMode(BOOL bDirect);

//本机操作系统是否是 NT/Win2000/XP
//返回
//    TRUE: os 是 NT/Win2000/XP
//   FALSE: os 是 9x
BOOL DIRECTPORTAPI DP_IsNTOS();

//版本信息
//返回 
//     成功: 当前系统安装的DirectPort 内核版本 eg 1.0.0.0
//     失败: 0
//     如果当前系统不是NT/win2000/xp,总是返回 0
DWORD DIRECTPORTAPI DP_GetKernelVer();


//版本信息
//dwDllVer   : 当前系统安装的DirectPort  DLL版本
//返回 版本 eg 1.0.0.0
DWORD DIRECTPORTAPI DP_GetVer();


#ifdef __cplusplus
}
#endif

#endif //_DIRECTPORT_1856D21A_B15D_4a69_86A5_FE6CD4D69E30_INCLUDE