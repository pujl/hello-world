#pragma once

//�˵������
//������ͼ���ְ�ť���+��ť�����������
//��ť����Ϊ��ͨ��ť(1)�����ּ��̰�ť(2)�͸����������ְ�ť��3������
//��ť������Ϊ���굯���������ͣ�1���Ͳ������2����ť
//��ť������ӣ��ڹ����и���ʵ��ʹ���������Ӽ���

const UINT  BTNID_EXIT_SYS			= 0x01020012;
const UINT  BTNID_CONN_SYS			= 0x01020013;
//�˳���ť
const UINT  BTNID_EXIT_CONFIRM		= 0x01021000;
const UINT  BTNID_CANCER_CONFIRM	= 0x01022000;
const UINT  BTNID_ENSUER_CONFIRM	= 0x01023000;

//���붨��
const UINT BTN_CATEGORY_MASK			= 0xFF000000;
const UINT BTN_OPR_MASK					= 0x00FF0000;
const UINT BTN_CMD_MASK					= 0x0000FFFF;
const UINT BTN_GUIDETGT_NUMMASK			= 0x00FFFFFF;

//��������
const UINT BTNTY_NORMAL_STYLE			= 0x01000000;
const UINT BTNTY_NUM_KEYBOARD			= 0x02000000;
const UINT BTNTY_NUM_GUIDE				= 0x03000000;
const UINT BTNTY_SWITCH_VIEW			= 0x00010000;
const UINT BTNTY_SEND_ORDER				= 0x00020000;

const UINT INVALID_ID					= 0xFFFFFFFF;


