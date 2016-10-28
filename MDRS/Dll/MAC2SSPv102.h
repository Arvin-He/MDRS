//MAC2SSPv102.h
#if !defined(MAC2SSP4DLL)
#define MAC2SSP4DLL
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const AxisX=0;
const AxisY=1;
const AxisZ=2;
const AxisU=3;
extern "C"
{
	//初始化函数
	__declspec(dllimport) int __stdcall ssp2_enum_cards(DWORD* card_array);//枚举系统中所有的卡
	__declspec(dllimport) int __stdcall ssp2_initial();//软件初始化
	__declspec(dllimport) int __stdcall ssp2_close();//软件关闭
	__declspec(dllimport) int __stdcall ssp2_set_current_card(int card_number); //设置当前卡
	__declspec(dllimport) int __stdcall ssp2_get_current_card(); //得到当前卡
	__declspec(dllimport) int __stdcall	ssp2_reset();//重置当前卡
	//各类设置函数
	__declspec(dllimport) int __stdcall ssp2_set_out_mode (int Axis,int mode);//设置脉冲输出模式
	__declspec(dllimport) int __stdcall ssp2_set_encoder_mode(int Axis,int mode,int enable,int inverse,int filter);//设置编码(EA/EB)输入模式
	__declspec(dllimport) int __stdcall ssp2_set_feedback_mode(int Axis,int mode);//设置回馈计数器输入源(0脉冲输出,1编码输入)
	__declspec(dllimport) int __stdcall ssp2_set_hand_puls_mode(int Axis,int input_mode,int enable,int inverse,int filter);//设置手动脉冲(PA/PB)输入模式
	__declspec(dllimport) int __stdcall ssp2_set_general_mode(int Axis,int mode);//设置通用计数器输入源(0脉冲输出,1编码输入,2PA/PB输入,3时钟计数的1/2)
	__declspec(dllimport) int __stdcall ssp2_set_error_mode(int Axis,int mode);//设置偏差计数器输入源(0输出与EA/EB,1输出与PA/PB,3EA/EB 与PA/PB)
	__declspec(dllimport) int __stdcall ssp2_set_home_mode(int Axis, int home_mode,int org_logic,int ez_logic, int ez_count, int erc_out);//回零方式设置
	__declspec(dllimport) int __stdcall ssp2_set_alm(int Axis,int Logic,int mode);//设置Alm逻辑和操作模式
	__declspec(dllimport) int __stdcall ssp2_set_inp(int Axis,int Logic, int enabled);//设置INP逻辑和是否有效
	__declspec(dllimport) int __stdcall ssp2_set_erc(int Axis,int Logic, int Timer);//设置ERC逻辑和时长
	__declspec(dllimport) int __stdcall ssp2_snd_erc(int Axis);//发出erc信号
	__declspec(dllimport) int __stdcall ssp2_clr_erc(int Axis);//清除erc电平信号
	__declspec(dllimport) int __stdcall ssp2_set_sd(int Axis,int Logic, int mode, int enabled);//设置SD逻辑、操作模式和是否有效
	__declspec(dllimport) int __stdcall ssp2_set_el(int Axis,int mode);//设置EL操作模式
	__declspec(dllimport) int __stdcall ssp2_set_backlash_comp(int Axis,int mode,int bcomp,int speed);//设置间隙补偿
	__declspec(dllimport) int __stdcall ssp2_set_suppress_vibra (int Axis,int tu1,int tu2);//设置震动抑制定时
	__declspec(dllimport) int __stdcall ssp2_set_move_ratio(int Axis,double ratio);//设置速度倍率
	__declspec(dllimport) int __stdcall ssp2_set_idle_pls(int Axis,int pulse);//设置空转脉冲数
	__declspec(dllimport) int __stdcall ssp2_set_irq_enable(int Axis,int enabled);//使能/禁止中断功能
	__declspec(dllimport) int __stdcall ssp2_set_irq_event(int Axis,int IrqEvent);//设置中断发生事件
	__declspec(dllimport) int __stdcall ssp2_set_svon(int Axis,int Logic);//设置SVON
	__declspec(dllimport) int __stdcall ssp2_get_srdy(int Axis);//读取SRDY
	__declspec(dllimport) int __stdcall ssp2_set_general_io(DWORD value);//设置通用I/O
	__declspec(dllimport) DWORD __stdcall ssp2_get_general_io();//读取通用I/O
	__declspec(dllimport) int __stdcall ssp2_set_fltr_mode(int Axis,int mode);//对EL,SD,ORG,ALM,INP信号应用滤波器,忽略4微秒以下脉冲
	__declspec(dllimport) int __stdcall ssp2_set_hand_ratio(int Axis, int spd , int ratio);//设置PA/PB脉冲分割率和脉冲倍数
	//单轴速度方式运动
	__declspec(dllimport) int __stdcall	ssp2_start_vt_move(int Axis,int FL,int FH,double Tacc);//开始加速一个轴到一定速度(梯形)
	__declspec(dllimport) int __stdcall	ssp2_start_vs_move(int Axis,int FL,int FH,double Tacc ,double Tsacc);//开始加速一个轴到一定速度(S型)
	__declspec(dllimport) int __stdcall	ssp2_change_speed (int Axis, int new_speed,double Tacc);//运行期改变速度
	__declspec(dllimport) int __stdcall	ssp2_get_current_speed(int Axis);//获得当前速度
	__declspec(dllimport) int __stdcall	ssp2_sd_stop(int Axis,double Tdec);//减速停止
	__declspec(dllimport) int __stdcall	ssp2_emg_stop(int Axis);//立即停止
	//单轴位置模式运动
	__declspec(dllimport) int __stdcall	ssp2_start_pt_move(int Axis,int mode,int position,int FL,int FH,double Tacc,double Tdec);//开始梯形运动(相对/绝对) 
	__declspec(dllimport) int __stdcall	ssp2_start_ps_move(int Axis,int mode,int position,int FL,int FH,double Tacc,double Tdec,double Tsacc, double Tsdec);//开始S型运动(相对/绝对)
	__declspec(dllimport) int __stdcall	ssp2_resume_move(int Axis,int resume_mode,int stop_mode);//继续中途停止的运动
	//直线插补
	__declspec(dllimport) int __stdcall	ssp2_start_tline_move(int Axes,int mode,int* position_array,int FL,int FH,double Tacc);//梯形线性插补(2,3,4轴；绝对/相对) 
	__declspec(dllimport) int __stdcall	ssp2_start_sline_move(int Axes,int mode,int* position_array,int FL,int FH,double Tacc,double Tsacc);//S型线性插补(2,3,4轴;绝对/相对)
	//圆弧插补
	__declspec(dllimport) int __stdcall	ssp2_start_tarc_move(int Axes,int mode ,int c1_offset,int c2_offset,int e1_ffset,int e2_offset,int FL ,int FH,double Tacc,int direct);//圆弧插补(2轴；绝对/相对)
	__declspec(dllimport) int __stdcall	ssp2_start_sarc_move(int Axes,int mode ,int c1_offset,int c2_offset,int e1_ffset,int e2_offset,int FL ,int FH,double Tacc,double Tsacc,int direct);//圆弧插补(2轴；绝对/相对)
	//回零
	__declspec(dllimport) int __stdcall	ssp2_home_move(int Axis,int direct, int FL,int FH,int FA,double Tacc,double Tdec);//回零
	//手动脉冲运动
	__declspec(dllimport) int __stdcall	ssp2_start_vpls_move(int Axis,int speed_limit);//进行由脉冲输入控制的速度模式移动
	__declspec(dllimport) int __stdcall	ssp2_start_ppls_move(int Axis,int mode,int position,int speed_limit);//进行由脉冲输入控制的位置模式移动
	__declspec(dllimport) int __stdcall	ssp2_start_pls_home_move(int Axis,int home_type,int speed_limit);//进行由脉冲输入控制的回零
	//状态监视
	__declspec(dllimport) int __stdcall	ssp2_get_motion_status(int Axis);//得到运动状态(回零、加速、停止、等待。。。)
	__declspec(dllimport) DWORD __stdcall	ssp2_get_io_status(int Axis);//得到轴的输入输出状态
	__declspec(dllimport) int __stdcall	ssp2_get_irq_status(int* int_card , int* int_axes , DWORD* err_sts, DWORD* int_sts,bool wait=false);//得到中断状态
	//位置控制和计数器
	__declspec(dllimport) int __stdcall	ssp2_get_position(int Axis);//读取回馈计数器
	__declspec(dllimport) int __stdcall	ssp2_set_position(int Axis,int number);//设置回馈计数器
	__declspec(dllimport) int __stdcall	ssp2_get_command(int Axis);//读取命令计数器
	__declspec(dllimport) int __stdcall	ssp2_set_command(int Axis,int number);//设置命令计数器
	__declspec(dllimport) int __stdcall	ssp2_get_error_counter(int Axis);//读取偏差计数器
	__declspec(dllimport) int __stdcall	ssp2_set_error_counter(int Axis,int number);//设置偏差计数器
	__declspec(dllimport) int __stdcall	ssp2_get_general_counter(int Axis);//读取通用计数器
	__declspec(dllimport) int __stdcall	ssp2_set_general_counter(int Axis,int number);//设置通用计数器
	__declspec(dllimport) int __stdcall	ssp2_get_target_position(int Axis);//读取目标位置
	__declspec(dllimport) int __stdcall	ssp2_set_target_position(int Axis,int new_position);//重新设定目标位置
	__declspec(dllimport) int __stdcall	ssp2_get_rest_pulse(int Axis);//读取剩余脉冲数
	__declspec(dllimport) int __stdcall	ssp2_get_rampdown_position(int Axis);//读取减速点
	//限位，位置比较
	__declspec(dllimport) int __stdcall	ssp2_set_soft_limit(int Axis,int p_limit,int n_limit);//设置软限位
	__declspec(dllimport) int __stdcall	ssp2_enable_softlimit(int Axis,int mode);//使能软限位
	__declspec(dllimport) int __stdcall	ssp2_disable_softlimit(int Axis);//屏蔽软限位
	__declspec(dllimport) int __stdcall	ssp2_set_err_count_cmp(int Axis ,int Tolerance, int enabled);//设置偏差比较
	__declspec(dllimport) int __stdcall	ssp2_set_general_cmp (int Axis,int cmp_src,int cmp_method, int mode, int Data, int enabled);//设置通用比较器
	__declspec(dllimport) int __stdcall	ssp2_get_cmp_data(int Axis,int* cmp_num);//察看比较器的数据
	__declspec(dllimport) DWORD __stdcall	ssp2_get_cmp_status(int Axis);//察看比较器的状态(得到哪些比较器处于比较中)
	__declspec(dllimport) int __stdcall	ssp2_set_psl_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//设置正限位用作通用比较器
	__declspec(dllimport) int __stdcall	ssp2_set_nsl_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//设置负限位用作通用比较器
	__declspec(dllimport) int __stdcall	ssp2_set_err_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//设置偏差比较器用作通用比较器
	__declspec(dllimport) int __stdcall	ssp2_set_continu_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//设置连续比较器
	__declspec(dllimport) int __stdcall	ssp2_check_continucmp_buffer(int Axis);//检查预置连续比较器是否为空
	//连续运动
	__declspec(dllimport) int __stdcall	ssp2_check_continuous_buffer(int Axis);//检查预置寄存器是否为空
	//同时运动
	__declspec(dllimport) int __stdcall	ssp2_set_tr_move_all(int Axes,int* pos,int* FL, int* FH,double* Tacc,double* Tdec);//设置多轴同时运动参数
	__declspec(dllimport) int __stdcall	ssp2_start_move_all(int Axes);//多轴同时运动
	__declspec(dllimport) int __stdcall	ssp2_stop_move_all(int Axes,int mode);//多轴同时停止运动
}
#endif // !defined(MAC2SSP4DLL)