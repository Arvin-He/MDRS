#include "stdafx.h"
#include "MDRS.h"
#include "MAC2SSPv102.h"
#include "Global.h"

using namespace std;

//图像中心位置
CPoint  g_imgCenterPos(320, 256);
Mat g_src1( Size(640, 512), CV_8UC3);
Mat g_src2( Size(640, 512), CV_8UC3);
bool g_bIsInit(false);
//I/O口操作
void ResetIOState()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x00);
}

//红色报警灯亮
void WarningStateRed()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x02);
}
//黄色复位灯亮
void ResetStateYellow()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x08);
}
//黄色暂停灯亮
void PaulStateYellow()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x08);
}
//正常绿灯亮
void NormalStateGreen()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x20);	//正常状态下绿灯亮
}
//蜂鸣报警
void BeepWarning()
{
	ssp2_set_current_card(0);
	ssp2_set_general_io(0x80);
}
//张开气爪
void PuffClaw()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x08);
}
//收缩气爪
void FrapClaw()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x02);
}
//吸取芯片
void InhaleChip()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x08);
}
//释放芯片
void ReleaseChip()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x02);
}
//点胶
void DropGlue()
{
	ssp2_set_current_card(4);
	ssp2_set_general_io(0x8000);
}
//轴状态复位
void AxisReset(int cardIndex, int nCurrAxis)
{
	if (0 == cardIndex)
	{
		ssp2_set_current_card(0);
		switch(nCurrAxis)
		{
		case 0:		//x轴复位
			ssp2_set_general_io(0x04);
			break;
		case 1:		//y轴复位
			ssp2_set_general_io(0x40);
			break;
		case 2:		//z轴复位
			ssp2_set_general_io(0x400);
			break;
		case 3:		//CCDz轴复位
			ssp2_set_general_io(0x4000);
			break;
		}
	}
	else if (4 == cardIndex)
	{
		ssp2_set_current_card(4);
		switch(nCurrAxis)
		{
		case 0:		//x轴复位
			ssp2_set_general_io(0x04);
			break;
		case 1:		//y轴复位
			ssp2_set_general_io(0x40);
			break;
		case 2:		//z轴复位
			ssp2_set_general_io(0x400);
			break;
		case 3:		//CCDy轴复位
			ssp2_set_general_io(0x4000);
			break;
		}
	}
}
//轴位置归零
void AxisGoHome(int cardIndex, int nCurrAxis)
{
	if (0 == cardIndex)
	{
		ssp2_set_current_card(0);
		switch(nCurrAxis)
		{
		case 0:		//x轴回原点
			ssp2_set_general_io(0x01);
			break;
		case 1:		//y轴回原点
			ssp2_set_general_io(0x10);
			break;
		case 2:		//z轴回原点
			ssp2_set_general_io(0x100);
			break;
		case 3:		//CCD-z轴回原点
			ssp2_set_general_io(0x1000);
			break;
		}
	}
	else if (4 == cardIndex)
	{
		ssp2_set_current_card(4);
		switch(nCurrAxis)
		{
		case 0:		//x轴回原点
			ssp2_set_general_io(0x01);
			break;
		case 1:		//y轴回原点
			ssp2_set_general_io(0x10);
			break;
		case 2:		//z轴回原点
			ssp2_set_general_io(0x100);
			break;
		case 3:		//CCD-y轴回原点
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
		case 0:		//x轴清零
			ssp2_set_command(0 , 0);
			break;
		case 1:		//y轴清零
			ssp2_set_command(1 , 0);
			break;
		case 2:		//z轴清零
			ssp2_set_command(2 , 0);
			break;
		case 3:		//CCD-z轴清零
			ssp2_set_command(3 , 0);
			break;
		}
	}
	else if (4 == cardIndex)
	{
		ssp2_set_current_card(4);
		switch(nCurrAxis)
		{
		case 0:		//x轴清零
			ssp2_set_command(0 , 0);
			break;
		case 1:		//y轴清零
			ssp2_set_command(1 , 0);
			break;
		case 2:		//z轴清零
			ssp2_set_command(2 , 0);
			break;
		case 3:		//CCD-y轴清零
			ssp2_set_command(3 , 0);
			break;
		}
	}
	else if (12 == cardIndex)
	{
		ssp2_set_current_card(12);
		switch(nCurrAxis)
		{
		case 0:		//x轴清零
			ssp2_set_command(0 , 0);
			break;
		case 1:		//y轴清零
			ssp2_set_command(1 , 0);
			break;
		case 2:		//旋转台轴清零
			ssp2_set_command(2 , 0);
			break;
		case 3:		//点胶手轴清零
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
	//12号卡
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
	//uPos旋转
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
	//4号卡
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
	ssp2_start_ps_move(3, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);	//mode1：绝对位置
	Sleep(2000);

	//0号卡
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
	ssp2_start_ps_move(3, 1, nPos, 200, 800, 1, 2, 0.5, 0.4);	//mode1：绝对位置
}
