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
	//��ʼ������
	__declspec(dllimport) int __stdcall ssp2_enum_cards(DWORD* card_array);//ö��ϵͳ�����еĿ�
	__declspec(dllimport) int __stdcall ssp2_initial();//�����ʼ��
	__declspec(dllimport) int __stdcall ssp2_close();//����ر�
	__declspec(dllimport) int __stdcall ssp2_set_current_card(int card_number); //���õ�ǰ��
	__declspec(dllimport) int __stdcall ssp2_get_current_card(); //�õ���ǰ��
	__declspec(dllimport) int __stdcall	ssp2_reset();//���õ�ǰ��
	//�������ú���
	__declspec(dllimport) int __stdcall ssp2_set_out_mode (int Axis,int mode);//�����������ģʽ
	__declspec(dllimport) int __stdcall ssp2_set_encoder_mode(int Axis,int mode,int enable,int inverse,int filter);//���ñ���(EA/EB)����ģʽ
	__declspec(dllimport) int __stdcall ssp2_set_feedback_mode(int Axis,int mode);//���û�������������Դ(0�������,1��������)
	__declspec(dllimport) int __stdcall ssp2_set_hand_puls_mode(int Axis,int input_mode,int enable,int inverse,int filter);//�����ֶ�����(PA/PB)����ģʽ
	__declspec(dllimport) int __stdcall ssp2_set_general_mode(int Axis,int mode);//����ͨ�ü���������Դ(0�������,1��������,2PA/PB����,3ʱ�Ӽ�����1/2)
	__declspec(dllimport) int __stdcall ssp2_set_error_mode(int Axis,int mode);//����ƫ�����������Դ(0�����EA/EB,1�����PA/PB,3EA/EB ��PA/PB)
	__declspec(dllimport) int __stdcall ssp2_set_home_mode(int Axis, int home_mode,int org_logic,int ez_logic, int ez_count, int erc_out);//���㷽ʽ����
	__declspec(dllimport) int __stdcall ssp2_set_alm(int Axis,int Logic,int mode);//����Alm�߼��Ͳ���ģʽ
	__declspec(dllimport) int __stdcall ssp2_set_inp(int Axis,int Logic, int enabled);//����INP�߼����Ƿ���Ч
	__declspec(dllimport) int __stdcall ssp2_set_erc(int Axis,int Logic, int Timer);//����ERC�߼���ʱ��
	__declspec(dllimport) int __stdcall ssp2_snd_erc(int Axis);//����erc�ź�
	__declspec(dllimport) int __stdcall ssp2_clr_erc(int Axis);//���erc��ƽ�ź�
	__declspec(dllimport) int __stdcall ssp2_set_sd(int Axis,int Logic, int mode, int enabled);//����SD�߼�������ģʽ���Ƿ���Ч
	__declspec(dllimport) int __stdcall ssp2_set_el(int Axis,int mode);//����EL����ģʽ
	__declspec(dllimport) int __stdcall ssp2_set_backlash_comp(int Axis,int mode,int bcomp,int speed);//���ü�϶����
	__declspec(dllimport) int __stdcall ssp2_set_suppress_vibra (int Axis,int tu1,int tu2);//���������ƶ�ʱ
	__declspec(dllimport) int __stdcall ssp2_set_move_ratio(int Axis,double ratio);//�����ٶȱ���
	__declspec(dllimport) int __stdcall ssp2_set_idle_pls(int Axis,int pulse);//���ÿ�ת������
	__declspec(dllimport) int __stdcall ssp2_set_irq_enable(int Axis,int enabled);//ʹ��/��ֹ�жϹ���
	__declspec(dllimport) int __stdcall ssp2_set_irq_event(int Axis,int IrqEvent);//�����жϷ����¼�
	__declspec(dllimport) int __stdcall ssp2_set_svon(int Axis,int Logic);//����SVON
	__declspec(dllimport) int __stdcall ssp2_get_srdy(int Axis);//��ȡSRDY
	__declspec(dllimport) int __stdcall ssp2_set_general_io(DWORD value);//����ͨ��I/O
	__declspec(dllimport) DWORD __stdcall ssp2_get_general_io();//��ȡͨ��I/O
	__declspec(dllimport) int __stdcall ssp2_set_fltr_mode(int Axis,int mode);//��EL,SD,ORG,ALM,INP�ź�Ӧ���˲���,����4΢����������
	__declspec(dllimport) int __stdcall ssp2_set_hand_ratio(int Axis, int spd , int ratio);//����PA/PB����ָ��ʺ����屶��
	//�����ٶȷ�ʽ�˶�
	__declspec(dllimport) int __stdcall	ssp2_start_vt_move(int Axis,int FL,int FH,double Tacc);//��ʼ����һ���ᵽһ���ٶ�(����)
	__declspec(dllimport) int __stdcall	ssp2_start_vs_move(int Axis,int FL,int FH,double Tacc ,double Tsacc);//��ʼ����һ���ᵽһ���ٶ�(S��)
	__declspec(dllimport) int __stdcall	ssp2_change_speed (int Axis, int new_speed,double Tacc);//�����ڸı��ٶ�
	__declspec(dllimport) int __stdcall	ssp2_get_current_speed(int Axis);//��õ�ǰ�ٶ�
	__declspec(dllimport) int __stdcall	ssp2_sd_stop(int Axis,double Tdec);//����ֹͣ
	__declspec(dllimport) int __stdcall	ssp2_emg_stop(int Axis);//����ֹͣ
	//����λ��ģʽ�˶�
	__declspec(dllimport) int __stdcall	ssp2_start_pt_move(int Axis,int mode,int position,int FL,int FH,double Tacc,double Tdec);//��ʼ�����˶�(���/����) 
	__declspec(dllimport) int __stdcall	ssp2_start_ps_move(int Axis,int mode,int position,int FL,int FH,double Tacc,double Tdec,double Tsacc, double Tsdec);//��ʼS���˶�(���/����)
	__declspec(dllimport) int __stdcall	ssp2_resume_move(int Axis,int resume_mode,int stop_mode);//������;ֹͣ���˶�
	//ֱ�߲岹
	__declspec(dllimport) int __stdcall	ssp2_start_tline_move(int Axes,int mode,int* position_array,int FL,int FH,double Tacc);//�������Բ岹(2,3,4�᣻����/���) 
	__declspec(dllimport) int __stdcall	ssp2_start_sline_move(int Axes,int mode,int* position_array,int FL,int FH,double Tacc,double Tsacc);//S�����Բ岹(2,3,4��;����/���)
	//Բ���岹
	__declspec(dllimport) int __stdcall	ssp2_start_tarc_move(int Axes,int mode ,int c1_offset,int c2_offset,int e1_ffset,int e2_offset,int FL ,int FH,double Tacc,int direct);//Բ���岹(2�᣻����/���)
	__declspec(dllimport) int __stdcall	ssp2_start_sarc_move(int Axes,int mode ,int c1_offset,int c2_offset,int e1_ffset,int e2_offset,int FL ,int FH,double Tacc,double Tsacc,int direct);//Բ���岹(2�᣻����/���)
	//����
	__declspec(dllimport) int __stdcall	ssp2_home_move(int Axis,int direct, int FL,int FH,int FA,double Tacc,double Tdec);//����
	//�ֶ������˶�
	__declspec(dllimport) int __stdcall	ssp2_start_vpls_move(int Axis,int speed_limit);//����������������Ƶ��ٶ�ģʽ�ƶ�
	__declspec(dllimport) int __stdcall	ssp2_start_ppls_move(int Axis,int mode,int position,int speed_limit);//����������������Ƶ�λ��ģʽ�ƶ�
	__declspec(dllimport) int __stdcall	ssp2_start_pls_home_move(int Axis,int home_type,int speed_limit);//����������������ƵĻ���
	//״̬����
	__declspec(dllimport) int __stdcall	ssp2_get_motion_status(int Axis);//�õ��˶�״̬(���㡢���١�ֹͣ���ȴ�������)
	__declspec(dllimport) DWORD __stdcall	ssp2_get_io_status(int Axis);//�õ�����������״̬
	__declspec(dllimport) int __stdcall	ssp2_get_irq_status(int* int_card , int* int_axes , DWORD* err_sts, DWORD* int_sts,bool wait=false);//�õ��ж�״̬
	//λ�ÿ��ƺͼ�����
	__declspec(dllimport) int __stdcall	ssp2_get_position(int Axis);//��ȡ����������
	__declspec(dllimport) int __stdcall	ssp2_set_position(int Axis,int number);//���û���������
	__declspec(dllimport) int __stdcall	ssp2_get_command(int Axis);//��ȡ���������
	__declspec(dllimport) int __stdcall	ssp2_set_command(int Axis,int number);//�������������
	__declspec(dllimport) int __stdcall	ssp2_get_error_counter(int Axis);//��ȡƫ�������
	__declspec(dllimport) int __stdcall	ssp2_set_error_counter(int Axis,int number);//����ƫ�������
	__declspec(dllimport) int __stdcall	ssp2_get_general_counter(int Axis);//��ȡͨ�ü�����
	__declspec(dllimport) int __stdcall	ssp2_set_general_counter(int Axis,int number);//����ͨ�ü�����
	__declspec(dllimport) int __stdcall	ssp2_get_target_position(int Axis);//��ȡĿ��λ��
	__declspec(dllimport) int __stdcall	ssp2_set_target_position(int Axis,int new_position);//�����趨Ŀ��λ��
	__declspec(dllimport) int __stdcall	ssp2_get_rest_pulse(int Axis);//��ȡʣ��������
	__declspec(dllimport) int __stdcall	ssp2_get_rampdown_position(int Axis);//��ȡ���ٵ�
	//��λ��λ�ñȽ�
	__declspec(dllimport) int __stdcall	ssp2_set_soft_limit(int Axis,int p_limit,int n_limit);//��������λ
	__declspec(dllimport) int __stdcall	ssp2_enable_softlimit(int Axis,int mode);//ʹ������λ
	__declspec(dllimport) int __stdcall	ssp2_disable_softlimit(int Axis);//��������λ
	__declspec(dllimport) int __stdcall	ssp2_set_err_count_cmp(int Axis ,int Tolerance, int enabled);//����ƫ��Ƚ�
	__declspec(dllimport) int __stdcall	ssp2_set_general_cmp (int Axis,int cmp_src,int cmp_method, int mode, int Data, int enabled);//����ͨ�ñȽ���
	__declspec(dllimport) int __stdcall	ssp2_get_cmp_data(int Axis,int* cmp_num);//�쿴�Ƚ���������
	__declspec(dllimport) DWORD __stdcall	ssp2_get_cmp_status(int Axis);//�쿴�Ƚ�����״̬(�õ���Щ�Ƚ������ڱȽ���)
	__declspec(dllimport) int __stdcall	ssp2_set_psl_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//��������λ����ͨ�ñȽ���
	__declspec(dllimport) int __stdcall	ssp2_set_nsl_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//���ø���λ����ͨ�ñȽ���
	__declspec(dllimport) int __stdcall	ssp2_set_err_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//����ƫ��Ƚ�������ͨ�ñȽ���
	__declspec(dllimport) int __stdcall	ssp2_set_continu_cmp(int Axis,int cmp_src,int cmp_method, int mode, int Data,int enabled);//���������Ƚ���
	__declspec(dllimport) int __stdcall	ssp2_check_continucmp_buffer(int Axis);//���Ԥ�������Ƚ����Ƿ�Ϊ��
	//�����˶�
	__declspec(dllimport) int __stdcall	ssp2_check_continuous_buffer(int Axis);//���Ԥ�üĴ����Ƿ�Ϊ��
	//ͬʱ�˶�
	__declspec(dllimport) int __stdcall	ssp2_set_tr_move_all(int Axes,int* pos,int* FL, int* FH,double* Tacc,double* Tdec);//���ö���ͬʱ�˶�����
	__declspec(dllimport) int __stdcall	ssp2_start_move_all(int Axes);//����ͬʱ�˶�
	__declspec(dllimport) int __stdcall	ssp2_stop_move_all(int Axes,int mode);//����ͬʱֹͣ�˶�
}
#endif // !defined(MAC2SSP4DLL)