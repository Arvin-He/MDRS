#include "stdafx.h"
#include "MDRS.h"
#include "MAC2SSPv102.h"
#include "Global.h"

using namespace std;

//ͼ������λ��
CPoint  g_imgCenterPos(320, 256);
Mat g_src1( Size(640, 512), CV_8UC3);
Mat g_src2( Size(640, 512), CV_8UC3);
bool g_bIsInit(false);
//I/O�ڲ���
void ResetIOState()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x00);
}

//��ɫ��������
void WarningStateRed()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x02);
}
//��ɫ��λ����
void ResetStateYellow()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x08);
}
//��ɫ��ͣ����
void PaulStateYellow()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x08);
}
//�����̵���
void NormalStateGreen()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x20);	//����״̬���̵���
}
//��������
void BeepWarning()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x80);
}
//�ſ���צ
void PuffClaw()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x08);
}
//������צ
void FrapClaw()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x02);
}
//��ȡоƬ
void InhaleChip()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x08);
}
//�ͷ�оƬ
void ReleaseChip()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x02);
}
//�㽺
void DropGlue()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x8000);
}
//��״̬��λ
void AxisReset(int cardIndex, int nCurrAxis)
{
	if (0 == cardIndex)
	{
		ssp2_set_current_card(0);
		switch(nCurrAxis)
		{
		case 0:		//x�Ḵλ
			ssp2_set_general_io(0x04);
			break;
		case 1:		//y�Ḵλ
			ssp2_set_general_io(0x40);
			break;
		case 2:		//z�Ḵλ
			ssp2_set_general_io(0x400);
			break;
		case 3:		//CCDz�Ḵλ
			ssp2_set_general_io(0x4000);
			break;
		}
	}
	else if (4 == cardIndex)
	{
		ssp2_set_current_card(4);
		switch(nCurrAxis)
		{
		case 0:		//x�Ḵλ
			ssp2_set_general_io(0x04);
			break;
		case 1:		//y�Ḵλ
			ssp2_set_general_io(0x40);
			break;
		case 2:		//z�Ḵλ
			ssp2_set_general_io(0x400);
			break;
		case 3:		//CCDy�Ḵλ
			ssp2_set_general_io(0x4000);
			break;
		}
	}
}
//��λ�ù���
void AxisGoHome(int cardIndex, int nCurrAxis)
{
	if (0 == cardIndex)
	{
		ssp2_set_current_card(0);
		switch(nCurrAxis)
		{
		case 0:		//x���ԭ��
			ssp2_set_general_io(0x01);
			break;
		case 1:		//y���ԭ��
			ssp2_set_general_io(0x10);
			break;
		case 2:		//z���ԭ��
			ssp2_set_general_io(0x100);
			break;
		case 3:		//CCD-z���ԭ��
			ssp2_set_general_io(0x1000);
			break;
		}
	}
	else if (4 == cardIndex)
	{
		ssp2_set_current_card(4);
		switch(nCurrAxis)
		{
		case 0:		//x���ԭ��
			ssp2_set_general_io(0x01);
			break;
		case 1:		//y���ԭ��
			ssp2_set_general_io(0x10);
			break;
		case 2:		//z���ԭ��
			ssp2_set_general_io(0x100);
			break;
		case 3:		//CCD-y���ԭ��
			ssp2_set_general_io(0x1000);
			break;
		}
	}
	else if (12 == cardIndex)
	{
		ssp2_set_current_card(12);
		switch (nCurrAxis)
		{
		case 0://sample_stage-x
			{
				ssp2_set_home_mode(0, 3, 0, 1, 1, 0);
				ssp2_home_move(0, 1, 200, 3000, 400, 1, 2);
			}
			break;
		case 1://sample_stage-y
			{
				ssp2_set_home_mode(1, 3, 0, 1, 1, 0);
				ssp2_home_move(0, 1, 200, 3000, 400, 1, 2);
			}
			break;
		case 2://sample_stage_rotate
			{
				ssp2_set_home_mode(2, 3, 0, 1, 1, 0);
				ssp2_home_move(0, 1, 2000, 10000, 400, 1, 2);
			}
			break;
		case 3://glue_claw_rotate
			{
				ssp2_set_home_mode(3, 3, 0, 1, 1, 0);
				ssp2_home_move(0, 1, 2000, 10000, 400, 1, 2);
			}
			break;
		}
	}
}

void AxisPosClear(int cardIndex, int nCurrAxis)
{
	if (0 == cardIndex)
	{
		ssp2_set_current_card(0);
		switch(nCurrAxis)
		{
		case 0:		//x������
			ssp2_set_command(0 , 0);
			break;
		case 1:		//y������
			ssp2_set_command(1 , 0);
			break;
		case 2:		//z������
			ssp2_set_command(2 , 0);
			break;
		case 3:		//CCD-z������
			ssp2_set_command(3 , 0);
			break;
		}
	}
	else if (4 == cardIndex)
	{
		ssp2_set_current_card(4);
		switch(nCurrAxis)
		{
		case 0:		//x������
			ssp2_set_command(0 , 0);
			break;
		case 1:		//y������
			ssp2_set_command(1 , 0);
			break;
		case 2:		//z������
			ssp2_set_command(2 , 0);
			break;
		case 3:		//CCD-y������
			ssp2_set_command(3 , 0);
			break;
		}
	}
	else if (12 == cardIndex)
	{
		ssp2_set_current_card(12);
		switch(nCurrAxis)
		{
		case 0:		//x������
			ssp2_set_command(0 , 0);
			break;
		case 1:		//y������
			ssp2_set_command(1 , 0);
			break;
		case 2:		//��ת̨������
			ssp2_set_command(2 , 0);
			break;
		case 3:		//�㽺��������
			ssp2_set_command(3 , 0);
			break;
		}
	}
	Sleep(5);
}

void AxisStop(int cardIndex, int nCurrAxis)
{
	if (cardIndex == ssp2_get_current_card())
		ssp2_emg_stop(nCurrAxis);
	else
	{
		ssp2_set_current_card(cardIndex);
		ssp2_emg_stop(nCurrAxis);
	}
}

void AllGoHome()
{
	for (int i=0; i<4; i++)
	{
		AxisGoHome(0, i);
		Sleep(100);
	}
	Sleep(2000);
	for (int i=0; i<4; i++)
	{
		AxisGoHome(4, i);
		Sleep(100);
	}
	Sleep(2000);
// 	for (int i=0; i<4; i++)
// 	{
// 		AxisGoHome(12, i);
// 		Sleep(1000);
		ssp2_set_current_card(12);
		ssp2_set_home_mode(0, 3, 0, 1, 1, 0);
		ssp2_home_move(0, 1, 200, 2000, 400, 1, 2);
		Sleep(10);
		ssp2_set_home_mode(1, 3, 0, 1, 1, 0);
		ssp2_home_move(1, 1, 200, 2000, 400, 1, 2);
//	}
}

void AllClear()
{
	for (int i=0; i<4; i++)
	{
		AxisPosClear(0, i);
		Sleep(100);
		AxisPosClear(4, i);
		Sleep(100);
		AxisPosClear(12, i);
	}
}

void AllInit()
{
	//12�ſ�
	ssp2_set_current_card(12);
	CString		fileName(_T(".\\CONFIG\\SampleStage_POS.ini"));
	char			cPos[20];
	CString		strPos(_T(""));	
	int			nPos(0);
	//xPos
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 2000, 1, 2, 0.4, 0.5);
	//yPos
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 2000, 1, 2, 0.4, 0.5);
	//uPos��ת
	GetPrivateProfileString(_T("section3"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 1000, 20000, 1, 2, 0.5, 0.5);
	//glue-rotate
	fileName = (_T(".\\CONFIG\\GLUE\\GluePin_POS.ini"));
	GetPrivateProfileString(_T("section4"), _T("uPos"), _T("uPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(3, 1, nPos, 2000, 20000, 1, 2, 0.5, 0.5);
	Sleep(2000);
	//4�ſ�
	ssp2_set_current_card(4);
	fileName = (_T(".\\CONFIG\\CLIP\\CLIP_POS.ini"));
	//zPOS
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);

	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);

	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);

	fileName = (_T(".\\CONFIG\\CCD\\CCD_POS.ini"));
	GetPrivateProfileString(_T("section2"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	//ccd-z
	ssp2_start_ps_move(3, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);	//mode1������λ��
	Sleep(2000);

	//0�ſ�
	ssp2_set_current_card(0);
	fileName = (_T(".\\CONFIG\\GLUE\\GluePinSafePOS.ini"));
	//zPOS
	GetPrivateProfileString(_T("section3"), _T("zPos"), _T("zPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(2, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	//yPos
	GetPrivateProfileString(_T("section2"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(1, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);
	//xPos
	GetPrivateProfileString(_T("section1"), _T("xPos"), _T("xPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	ssp2_start_ps_move(0, 1, nPos, 200, 600, 1, 2, 0.5, 0.4);
	fileName = (_T(".\\CONFIG\\CCD\\CCD_POS.ini"));
	GetPrivateProfileString(_T("section1"), _T("yPos"), _T("yPos"), cPos, 20, fileName);
	strPos = cPos;
	nPos = atoi(strPos);
	//ccd-z
	ssp2_start_ps_move(3, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);	//mode1������λ��
}
