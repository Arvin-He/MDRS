#include "DirectPort.h"
#pragma comment(lib, "DirectPort.lib")

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define	EXPORT __declspec(dllexport)
#endif
//�趨�жϺ�
EXPORT unsigned short CALLBACK    GetData(void);
//�趨�жϺ�
EXPORT  void    CALLBACK    SetData(unsigned short);
//����ϵͳ������������ָ�����ڵĿ��ƿ������(ͨ��Ϊ���ڵ����ֽ������ַ�Ǹ�)
EXPORT	BOOL	CALLBACK	InitParallel(unsigned short usData, unsigned short usCtrl);
//�жϳ�ʱ
EXPORT	BOOL	CALLBACK	GetTimeOut(unsigned short usData);
//�����ʱ
EXPORT	void	CALLBACK	ClearTimeOut(unsigned short usData);
//���ڼ��
//EXPORT  BOOL	CALLBACK	EppCheck(void);
//��ϵͳ��ȡEPP��Ϣʱ��ȷ��ϵͳ״̬
EXPORT	BOOL	CALLBACK	ReadEppStatus(unsigned short usData);
//��ϵͳ��EPPд����Ϣʱ��ȷ��ϵͳ״̬
EXPORT	BOOL	CALLBACK	WriteEppStatus(unsigned short usData);
//���ϵͳEPP������Ϣ
EXPORT	BOOL	CALLBACK	GetEppInfo(unsigned short usData,int,int);
//����ϵͳEPP������Ϣ
EXPORT	BOOL	CALLBACK	SetEppInfo(unsigned short usData,int,int,float,float,float,float,float,float,float,float);
//��λ���ջ�����
EXPORT	BOOL	CALLBACK	EpigynyClose(unsigned short usData,int,int);
//��λ����������
EXPORT	BOOL	CALLBACK	EpigynyOpen(unsigned short usData,int,int);
//��λ����������
EXPORT	BOOL	CALLBACK	HypogynyOpen(unsigned short usData,int,int);
//��λ���ջ�����
EXPORT	BOOL	CALLBACK	HypogynyClose(unsigned short usData,int,int);
//ģ���������
EXPORT	BOOL	CALLBACK	SimulateCtrl(unsigned short usData,int,int);
//��ȡָ�����䡢ͨ����λ��
EXPORT	float	CALLBACK	ReadInterval(unsigned short usData,int,int,float,float,float,float);
//��ָ�����䡢ͨ������λ��ָ��
EXPORT	BOOL	CALLBACK	WriteInterval(unsigned short usData,int,int,float,float,float,float,float);
//��ָ�����䡢ͨ�����͵�ѹָ��
EXPORT	BOOL	CALLBACK	WriteVoltage(unsigned short usData,int,int,float,float,float,float,float);

/********** ϵͳ�궨 **********/
//��ȡAD
EXPORT long CALLBACK ReadAD(unsigned short usData,int,int);
//��ȡDA
EXPORT long CALLBACK ReadDA(unsigned short usData,int,int);
//���ͱ궨��ѹ
EXPORT BOOL CALLBACK OutVoltage(unsigned short usData,int,int,float,float,float,float,float);


