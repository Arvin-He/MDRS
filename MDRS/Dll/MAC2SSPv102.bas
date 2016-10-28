Attribute VB_Name = "Module1"
Option Explicit
Type array16_struct
v(15) As Long
End Type
Type array4_struct
v(3) As Long
End Type
Type array4d_struct
v(3) As Double
End Type
'初始化函数
Declare Function ssp2_enum_cards Lib "mac2sspv102.dll" (ByVal pcard_array As Long) As Long '枚举系统中所有的卡
Declare Function ssp2_initial Lib "mac2sspv102.dll" () As Long '软件初始化
Declare Function ssp2_close Lib "mac2sspv102.dll" () As Long '软件关闭
Declare Function ssp2_reset Lib "mac2sspv102.dll" () As Long '重置当前卡
Declare Function ssp2_set_current_card Lib "mac2sspv102.dll" (ByVal card_number As Long) As Long '设置当前卡
Declare Function ssp2_get_current_card Lib "mac2sspv102.dll" () As Long '得到当前卡
'各类设置函数
Declare Function ssp2_set_out_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '设置脉冲输出模式
Declare Function ssp2_set_encoder_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal enable As Long, ByVal inverse As Long, ByVal filter As Long) As Long '设置编码(EA/EB)输入模式
Declare Function ssp2_set_feedback_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '设置回馈计数器输入源(0脉冲输出,1编码输入)
Declare Function ssp2_set_general_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '设置通用计数器输入源(0脉冲输出,1编码输入,2PA/PB输入,3时钟计数的1/2)
Declare Function ssp2_set_error_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '设置偏差计数器输入源(0输出与EA/EB,1输出与PA/PB,3EA/EB 与PA/PB)
Declare Function ssp2_set_hand_puls_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal input_mode As Long, ByVal enable As Long, ByVal inverse As Long, ByVal filter As Long) As Long '设置手动脉冲(PA/PB)输入模式
Declare Function ssp2_set_home_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal home_mode As Long, ByVal org_logic As Long, ByVal ez_logic As Long, ByVal ez_count As Long, ByVal erc_out As Long) As Long '回零方式设置
Declare Function ssp2_set_alm Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal mode As Long) As Long '设置Alm逻辑和操作模式
Declare Function ssp2_set_inp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal enabled As Long) As Long '设置INP逻辑和是否有效
Declare Function ssp2_set_erc Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal Timer As Long) As Long '设置ERC逻辑和时长
Declare Function ssp2_snd_erc Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '发出ERC信号
Declare Function ssp2_clr_erc Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '清除ERC电平信号
Declare Function ssp2_set_sd Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal mode As Long, ByVal enabled As Long) As Long '设置SD逻辑、操作模式和是否有效
Declare Function ssp2_set_el Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '设置EL操作模式
Declare Function ssp2_set_backlash_comp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal bcomp As Long, ByVal speed As Long) As Long '设置间隙补偿
Declare Function ssp2_set_suppress_vibra Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal tu1 As Long, ByVal tu2 As Long) As Long '设置震动抑制定时
Declare Function ssp2_set_move_ratio Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal ratio As Double) As Long '设置速度倍率
Declare Function ssp2_set_idle_pls Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal pulse As Long) As Long '设置空转脉冲数
Declare Function ssp2_set_irq_enable Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal enabled As Long) As Long '使能/禁止中断功能
Declare Function ssp2_set_irq_event Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal IrqEvent As Long) As Long '设置中断发生事件
Declare Function ssp2_set_svon Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long) As Long '设置SVON
Declare Function ssp2_get_srdy Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取SRDY
Declare Function ssp2_set_general_io Lib "mac2sspv102.dll" (ByVal value As Long) As Long '设置通用I/O
Declare Function ssp2_get_general_io Lib "mac2sspv102.dll" () As Long '读取通用I/O
Declare Function ssp2_set_fltr_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal filter As Long) As Long '对EL,SD,ORG,ALM,INP信号应用滤波器,忽略4微秒以下脉冲
Declare Function ssp2_set_hand_ratio Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal spd As Long, ByVal ratio As Long) As Long '设置PA/PB脉冲分割率和脉冲倍数
'单轴速度方式运动
Declare Function ssp2_start_vt_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double) As Long '开始加速一个轴到一定速度(梯形)
Declare Function ssp2_start_vs_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal Tsacc As Double) As Long '开始加速一个轴到一定速度(S型)
Declare Function ssp2_change_speed Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal new_speed As Long, ByVal Tacc As Double) As Long '运行期改变速度
Declare Function ssp2_get_current_speed Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '获得当前速度
Declare Function ssp2_sd_stop Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal tdec As Double) As Long '减速停止
Declare Function ssp2_emg_stop Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '立即停止
'单轴位置模式运动
Declare Function ssp2_start_pt_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal position As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal tdec As Double) As Long '开始梯形运动(相对/绝对)
Declare Function ssp2_start_ps_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal position As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal tdec As Double, ByVal Tsacc As Double, ByVal Tsdec As Double) As Long '开始S型运动(相对/绝对)
Declare Function ssp2_resume_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal resume_mode As Long, ByVal stop_mode As Long) As Long '继续中途停止的运动
'直线插补
Declare Function ssp2_start_tline_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal position_array As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double) As Long '梯形线性插补(2,3,4轴；绝对/相对)
Declare Function ssp2_start_sline_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal position_array As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal Tsacc As Double) As Long 'S型线性插补(2,3,4轴;绝对/相对)
'圆弧插补
Declare Function ssp2_start_tarc_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal c1_offset As Long, ByVal c2_offset As Long, ByVal e1_ffset As Long, ByVal e2_offset As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal direct As Long) As Long '梯形变速圆弧插补(2轴；绝对/相对)
Declare Function ssp2_start_sarc_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal c1_offset As Long, ByVal c2_offset As Long, ByVal e1_ffset As Long, ByVal e2_offset As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal Tsacc As Double, ByVal direct As Long) As Long 'S形变速圆弧插补(2轴；绝对/相对)
'回零
Declare Function ssp2_home_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal direct As Long, ByVal fl As Long, ByVal fh As Long, ByVal fa As Long, ByVal Tacc As Double, ByVal tdec As Double) As Long '回零
'手动脉冲运动
Declare Function ssp2_start_vpls_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal speed_limit As Long) As Long '进行由脉冲输入控制的速度模式移动
Declare Function ssp2_start_ppls_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal position As Long, ByVal speed_limit As Long) As Long '进行由脉冲输入控制的位置模式移动
Declare Function ssp2_start_pls_home_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal home_type As Long, ByVal speed_limit As Long) As Long '进行由脉冲输入控制的回零
'状态监视
Declare Function ssp2_get_motion_status Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '得到运动状态(回零、加速、停止、等待。。。)
Declare Function ssp2_get_io_status Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '得到轴的输入输出状态
Declare Function ssp2_get_irq_status Lib "mac2sspv102.dll" (int_card As Long, int_axes As Long, ByVal err_sts As Long, ByVal int_sts As Long, ByVal wait As Boolean) As Long '得到中断状态
'位置控制和计数器
Declare Function ssp2_get_position Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取回馈计数器
Declare Function ssp2_set_position Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '设置回馈计数器
Declare Function ssp2_get_command Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取命令计数器
Declare Function ssp2_set_command Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '设置命令计数器
Declare Function ssp2_get_error_counter Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取偏差计数器
Declare Function ssp2_set_error_counter Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '设置偏差计数器
Declare Function ssp2_get_general_counter Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取通用计数器
Declare Function ssp2_set_general_counter Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '设置通用计数器
Declare Function ssp2_get_target_position Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取目标位置
Declare Function ssp2_set_target_position Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal new_position As Long) As Long '重新设定目标位置
Declare Function ssp2_get_rest_pulse Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取剩余脉冲数
Declare Function ssp2_get_rampdown_position Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '读取减速点
'限位，位置比较
Declare Function ssp2_set_soft_limit Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal p_limit As Long, ByVal n_limit As Long) As Long '设置软限位
Declare Function ssp2_enable_softlimit Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '使能软限位
Declare Function ssp2_disable_softlimit Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '屏蔽软限位
Declare Function ssp2_set_err_count_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Tolerance As Long, ByVal enabled As Long) As Long '设置偏差比较
Declare Function ssp2_set_general_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '设置通用比较器
Declare Function ssp2_get_cmp_data Lib "mac2sspv102.dll" (ByVal Axis As Long, cmp_num As Long) As Long '察看比较器的数据
Declare Function ssp2_get_cmp_status Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '察看比较器的状态(得到哪些比较器处于比较中)
Declare Function ssp2_set_psl_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '设置正限位用作通用比较器
Declare Function ssp2_set_nsl_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '设置负限位用作通用比较器
Declare Function ssp2_set_err_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '设置偏差比较器用作通用比较器
Declare Function ssp2_set_continu_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '设置连续比较器
Declare Function ssp2_check_continucmp_buffer Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '检查预置连续比较器是否为空
'连续运动
Declare Function ssp2_check_continuous_buffer Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '检查预置寄存器是否为空
'同时运动
Declare Function ssp2_set_tr_move_all Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal pos As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal tdec As Double) As Long '设置多轴同时运动参数
Declare Function ssp2_start_move_all Lib "mac2sspv102.dll" (ByVal Axes As Long) As Long '多轴同时运动
Declare Function ssp2_stop_move_all Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long) As Long '多轴同时停止运动
