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
'��ʼ������
Declare Function ssp2_enum_cards Lib "mac2sspv102.dll" (ByVal pcard_array As Long) As Long 'ö��ϵͳ�����еĿ�
Declare Function ssp2_initial Lib "mac2sspv102.dll" () As Long '�����ʼ��
Declare Function ssp2_close Lib "mac2sspv102.dll" () As Long '����ر�
Declare Function ssp2_reset Lib "mac2sspv102.dll" () As Long '���õ�ǰ��
Declare Function ssp2_set_current_card Lib "mac2sspv102.dll" (ByVal card_number As Long) As Long '���õ�ǰ��
Declare Function ssp2_get_current_card Lib "mac2sspv102.dll" () As Long '�õ���ǰ��
'�������ú���
Declare Function ssp2_set_out_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '�����������ģʽ
Declare Function ssp2_set_encoder_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal enable As Long, ByVal inverse As Long, ByVal filter As Long) As Long '���ñ���(EA/EB)����ģʽ
Declare Function ssp2_set_feedback_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '���û�������������Դ(0�������,1��������)
Declare Function ssp2_set_general_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '����ͨ�ü���������Դ(0�������,1��������,2PA/PB����,3ʱ�Ӽ�����1/2)
Declare Function ssp2_set_error_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '����ƫ�����������Դ(0�����EA/EB,1�����PA/PB,3EA/EB ��PA/PB)
Declare Function ssp2_set_hand_puls_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal input_mode As Long, ByVal enable As Long, ByVal inverse As Long, ByVal filter As Long) As Long '�����ֶ�����(PA/PB)����ģʽ
Declare Function ssp2_set_home_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal home_mode As Long, ByVal org_logic As Long, ByVal ez_logic As Long, ByVal ez_count As Long, ByVal erc_out As Long) As Long '���㷽ʽ����
Declare Function ssp2_set_alm Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal mode As Long) As Long '����Alm�߼��Ͳ���ģʽ
Declare Function ssp2_set_inp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal enabled As Long) As Long '����INP�߼����Ƿ���Ч
Declare Function ssp2_set_erc Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal Timer As Long) As Long '����ERC�߼���ʱ��
Declare Function ssp2_snd_erc Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '����ERC�ź�
Declare Function ssp2_clr_erc Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '���ERC��ƽ�ź�
Declare Function ssp2_set_sd Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long, ByVal mode As Long, ByVal enabled As Long) As Long '����SD�߼�������ģʽ���Ƿ���Ч
Declare Function ssp2_set_el Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long '����EL����ģʽ
Declare Function ssp2_set_backlash_comp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal bcomp As Long, ByVal speed As Long) As Long '���ü�϶����
Declare Function ssp2_set_suppress_vibra Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal tu1 As Long, ByVal tu2 As Long) As Long '���������ƶ�ʱ
Declare Function ssp2_set_move_ratio Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal ratio As Double) As Long '�����ٶȱ���
Declare Function ssp2_set_idle_pls Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal pulse As Long) As Long '���ÿ�ת������
Declare Function ssp2_set_irq_enable Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal enabled As Long) As Long 'ʹ��/��ֹ�жϹ���
Declare Function ssp2_set_irq_event Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal IrqEvent As Long) As Long '�����жϷ����¼�
Declare Function ssp2_set_svon Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Logic As Long) As Long '����SVON
Declare Function ssp2_get_srdy Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡSRDY
Declare Function ssp2_set_general_io Lib "mac2sspv102.dll" (ByVal value As Long) As Long '����ͨ��I/O
Declare Function ssp2_get_general_io Lib "mac2sspv102.dll" () As Long '��ȡͨ��I/O
Declare Function ssp2_set_fltr_mode Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal filter As Long) As Long '��EL,SD,ORG,ALM,INP�ź�Ӧ���˲���,����4΢����������
Declare Function ssp2_set_hand_ratio Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal spd As Long, ByVal ratio As Long) As Long '����PA/PB����ָ��ʺ����屶��
'�����ٶȷ�ʽ�˶�
Declare Function ssp2_start_vt_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double) As Long '��ʼ����һ���ᵽһ���ٶ�(����)
Declare Function ssp2_start_vs_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal Tsacc As Double) As Long '��ʼ����һ���ᵽһ���ٶ�(S��)
Declare Function ssp2_change_speed Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal new_speed As Long, ByVal Tacc As Double) As Long '�����ڸı��ٶ�
Declare Function ssp2_get_current_speed Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��õ�ǰ�ٶ�
Declare Function ssp2_sd_stop Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal tdec As Double) As Long '����ֹͣ
Declare Function ssp2_emg_stop Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '����ֹͣ
'����λ��ģʽ�˶�
Declare Function ssp2_start_pt_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal position As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal tdec As Double) As Long '��ʼ�����˶�(���/����)
Declare Function ssp2_start_ps_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long, ByVal position As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal tdec As Double, ByVal Tsacc As Double, ByVal Tsdec As Double) As Long '��ʼS���˶�(���/����)
Declare Function ssp2_resume_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal resume_mode As Long, ByVal stop_mode As Long) As Long '������;ֹͣ���˶�
'ֱ�߲岹
Declare Function ssp2_start_tline_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal position_array As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double) As Long '�������Բ岹(2,3,4�᣻����/���)
Declare Function ssp2_start_sline_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal position_array As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal Tsacc As Double) As Long 'S�����Բ岹(2,3,4��;����/���)
'Բ���岹
Declare Function ssp2_start_tarc_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal c1_offset As Long, ByVal c2_offset As Long, ByVal e1_ffset As Long, ByVal e2_offset As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal direct As Long) As Long '���α���Բ���岹(2�᣻����/���)
Declare Function ssp2_start_sarc_move Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long, ByVal c1_offset As Long, ByVal c2_offset As Long, ByVal e1_ffset As Long, ByVal e2_offset As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal Tsacc As Double, ByVal direct As Long) As Long 'S�α���Բ���岹(2�᣻����/���)
'����
Declare Function ssp2_home_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal direct As Long, ByVal fl As Long, ByVal fh As Long, ByVal fa As Long, ByVal Tacc As Double, ByVal tdec As Double) As Long '����
'�ֶ������˶�
Declare Function ssp2_start_vpls_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal speed_limit As Long) As Long '����������������Ƶ��ٶ�ģʽ�ƶ�
Declare Function ssp2_start_ppls_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal position As Long, ByVal speed_limit As Long) As Long '����������������Ƶ�λ��ģʽ�ƶ�
Declare Function ssp2_start_pls_home_move Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal home_type As Long, ByVal speed_limit As Long) As Long '����������������ƵĻ���
'״̬����
Declare Function ssp2_get_motion_status Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '�õ��˶�״̬(���㡢���١�ֹͣ���ȴ�������)
Declare Function ssp2_get_io_status Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '�õ�����������״̬
Declare Function ssp2_get_irq_status Lib "mac2sspv102.dll" (int_card As Long, int_axes As Long, ByVal err_sts As Long, ByVal int_sts As Long, ByVal wait As Boolean) As Long '�õ��ж�״̬
'λ�ÿ��ƺͼ�����
Declare Function ssp2_get_position Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡ����������
Declare Function ssp2_set_position Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '���û���������
Declare Function ssp2_get_command Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡ���������
Declare Function ssp2_set_command Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '�������������
Declare Function ssp2_get_error_counter Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡƫ�������
Declare Function ssp2_set_error_counter Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '����ƫ�������
Declare Function ssp2_get_general_counter Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡͨ�ü�����
Declare Function ssp2_set_general_counter Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal number As Long) As Long '����ͨ�ü�����
Declare Function ssp2_get_target_position Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡĿ��λ��
Declare Function ssp2_set_target_position Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal new_position As Long) As Long '�����趨Ŀ��λ��
Declare Function ssp2_get_rest_pulse Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡʣ��������
Declare Function ssp2_get_rampdown_position Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��ȡ���ٵ�
'��λ��λ�ñȽ�
Declare Function ssp2_set_soft_limit Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal p_limit As Long, ByVal n_limit As Long) As Long '��������λ
Declare Function ssp2_enable_softlimit Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal mode As Long) As Long 'ʹ������λ
Declare Function ssp2_disable_softlimit Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '��������λ
Declare Function ssp2_set_err_count_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal Tolerance As Long, ByVal enabled As Long) As Long '����ƫ��Ƚ�
Declare Function ssp2_set_general_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '����ͨ�ñȽ���
Declare Function ssp2_get_cmp_data Lib "mac2sspv102.dll" (ByVal Axis As Long, cmp_num As Long) As Long '�쿴�Ƚ���������
Declare Function ssp2_get_cmp_status Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '�쿴�Ƚ�����״̬(�õ���Щ�Ƚ������ڱȽ���)
Declare Function ssp2_set_psl_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '��������λ����ͨ�ñȽ���
Declare Function ssp2_set_nsl_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '���ø���λ����ͨ�ñȽ���
Declare Function ssp2_set_err_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '����ƫ��Ƚ�������ͨ�ñȽ���
Declare Function ssp2_set_continu_cmp Lib "mac2sspv102.dll" (ByVal Axis As Long, ByVal cmp_src As Long, ByVal cmp_method As Long, ByVal mode As Long, ByVal Data As Long, ByVal enabled As Long) As Long '���������Ƚ���
Declare Function ssp2_check_continucmp_buffer Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '���Ԥ�������Ƚ����Ƿ�Ϊ��
'�����˶�
Declare Function ssp2_check_continuous_buffer Lib "mac2sspv102.dll" (ByVal Axis As Long) As Long '���Ԥ�üĴ����Ƿ�Ϊ��
'ͬʱ�˶�
Declare Function ssp2_set_tr_move_all Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal pos As Long, ByVal fl As Long, ByVal fh As Long, ByVal Tacc As Double, ByVal tdec As Double) As Long '���ö���ͬʱ�˶�����
Declare Function ssp2_start_move_all Lib "mac2sspv102.dll" (ByVal Axes As Long) As Long '����ͬʱ�˶�
Declare Function ssp2_stop_move_all Lib "mac2sspv102.dll" (ByVal Axes As Long, ByVal mode As Long) As Long '����ͬʱֹͣ�˶�
