/************************************************************************ 
android/iphone/windows/linux����ͨѶ��
����ͨѶ�������� 
׼ȷ��95%���ϣ���ʵһ���ǲ������ġ� 
�ӿڷǳ��򵥣���������ʾ����3���ӾͿ��������Ӧ����������ͨѶ���� 
��������ǿ�����������������ô���ţ��źŶ���׼ȷ�� 
�����ı���Ϊ16���ƣ���ͨ������ɴ����κ��ַ� 
���ܷǳ�ǿ��û�����в��˵�ƽ̨������ͨ���ڴ���Ż�����ʱ����벻�ٷ������ڴ棬��7*24Сʱ���� 
��֧���κ�ƽ̨��������ƽ̨android, iphone, windows, linux, arm, mipsel����ʾ�� 
����ɲ鿴��http://blog.csdn.net/softlgh 
����: ҹ���� QQ:3116009971 �ʼ���3116009971@qq.com 
************************************************************************/  

#include "stdafx.h"
#include "testVoice.h"

#include "CmnHdr.h" 
#include <Windowsx.h>

#include "voicePlay.h"
#include "voiceRecog.h"
#include "audioRecorder.h"

#include <stdio.h>
#include <assert.h>

//#pragma comment(lib, "cvoiceRecognizeDLL.lib")
#pragma comment(lib, "cvoiceRecognizeLib.lib")

#define M_SEND_TEXT    (WM_USER + 100)

HWND thisHwnd = 0;
void *player = NULL;
void *recognizer = NULL;
void *recorder = NULL;
uintptr_t recorderThread;
bool receivedOwnSignal = true;
bool recognizerPaused = false;

//������Ϣ�������ú������Ǳ�Ҫ��
const char *recorderRecogErrorMsg(int _recogStatus)
{
	char *r = (char *)"unknow error";
	switch(_recogStatus)
	{
	case VR_ECCError:
		r = (char *)"ecc error";
		break;
	case VR_NotEnoughSignal:
		r = (char *)"not enough signal";
		break;
	case VR_NotHeaderOrTail:
		r = (char *)"signal no header or tail";
		break;
	case VR_RecogCountZero:
		r = (char *)"trial has expires, please try again";
		break;
	}
	return r;
}

//ʶ��ʼ�Ļص�����
void recorderRecognizerStart(void *_listener, float _soundTime)
{
	SetDlgItemTextA(thisHwnd, IDC_RECEIVE_STATUS, "����ʶ����......");
}

//ʶ������Ļص�����
void recorderRecognizerEnd(void *_listener, float _soundTime, int _recogStatus, char *_data, int _dataLen)
{
	SetDlgItemTextA(thisHwnd, IDC_RECEIVE_STATUS, "");

	if (_recogStatus == VR_SUCCESS)
	{				
		char result[100];
		int infoType = vr_decodeInfoType(_data, _dataLen);
		if(infoType == IT_STRING)
		{
			vr_decodeString(_recogStatus, _data, _dataLen, result, sizeof(result));
		}
		else if(infoType == IT_SSID_WIFI)//ssid+pwd��wifi
		{
			SSIDWiFiInfo wifi;
			vr_decodeSSIDWiFi(_recogStatus, _data, _dataLen, &wifi);
			sprintf_s(result, sizeof(result), "ssid:%s,pwd:%s", wifi.ssid, wifi.pwd);
		}
		else if(infoType == IT_WIFI)//mac+pwd��wifi
		{
			WiFiInfo macWifi;
			int i;
			vr_decodeWiFi(_recogStatus, _data, _dataLen, &macWifi);
			sprintf(result, "mac wifi:");
			for (i = 0; i < macWifi.macLen; i ++)
			{
				sprintf(result+ strlen(result), "0x%.2x ", macWifi.mac[i] & 0xff);
			}
			sprintf(result+ strlen(result), ", %s\n", macWifi.pwd);
		}
		if(infoType == IT_STRING || infoType == IT_SSID_WIFI || infoType == IT_WIFI)
		{
			HWND h_receivedText = GetDlgItem(thisHwnd, IDC_RECEIVED_TEXT);
			UINT oldStrLen = GetWindowTextLengthA(h_receivedText);
			UINT newStrLen = oldStrLen + strlen(result) + 3;
			char *newStr = (char *)malloc(newStrLen);
			memset(newStr, 0, newStrLen);
			GetDlgItemTextA(thisHwnd, IDC_RECEIVED_TEXT, newStr, oldStrLen+1);
			assert(strlen(newStr) + strlen(result) + 3 == newStrLen);
			if(oldStrLen > 0)strcat(newStr, "\r\n");
			strcat(newStr, result);
			SetDlgItemTextA(thisHwnd, IDC_RECEIVED_TEXT, newStr);
			free(newStr);
			SendMessage(h_receivedText, WM_VSCROLL, SB_BOTTOM, 0);
		}
	}
	else
	{
		printf("------------------recognize invalid data, errorCode:%d, error:%s\n", _recogStatus, recorderRecogErrorMsg(_recogStatus));
	}
}

//���ſ�ʼ�Ļص�
void onPlayStart(void *_listener)
{
	SetDlgItemTextA(thisHwnd, IDC_SEND_STATUS, "���ڲ�����......");
	if(!receivedOwnSignal)
	{
		vr_pauseRecognize(recognizer, 10 * 1000);
		recognizerPaused = true;
	}
}

//���Ž����Ļص�
void onPlayEnd(void *_listener)
{
	SetDlgItemTextA(thisHwnd, IDC_SEND_STATUS, "");
	if(recognizerPaused)
	{
		vr_pauseRecognize(recognizer, 0);
		recognizerPaused = false;
	}
}

//����ʶ����
void runRecorderVoiceRecognize( void * _recognizer)  
{
	vr_runRecognizer(_recognizer);
}

//¼�����ص�����
int recorderShortWrite(void *_writer, const void *_data, unsigned long _sampleCout)
{
	char *data = (char *)_data;
	void *recognizer = _writer;

	return vr_writeData(recognizer, data, ((int)_sampleCout) * 2);
}

//int playFreqs[] = {6500,6700,6900,7100,7300,7500,7700,7900,8100,8300,8500,8700,8900,9100,9300,9500,9700,9900,10100};
int playFreqs[] = {4000,4150,4300,4450,4600,4750,4900,5050,5200,5350,5500,5650,5800,5950,6100,6250,6400,6550,6700};
void init()
{
	//��������������
	player = vp_createVoicePlayer();
	vp_setFreqs(player, playFreqs, sizeof(playFreqs)/sizeof(int));
	vp_setPlayerListener(player, NULL, onPlayStart, onPlayEnd);//���ò������ص�����

	//����ʶ�����������ü�����
	recognizer = vr_createVoiceRecognizer(MemoryUsePriority);
	int r;

	vr_setRecognizeFreqs(recognizer, playFreqs, sizeof(playFreqs)/sizeof(int));
	vr_setRecognizerListener(recognizer, NULL, recorderRecognizerStart, recorderRecognizerEnd);//����ʶ�����ص�����
	//����¼����	
	r = initRecorder(44100, 1, 16, 512, &recorder);//Ҫ��¼ȡshort����
	if(r != 0)
	{
		printf("recorder init error:%d", r);
		return;
	}
	//��ʼ¼��
	r = startRecord(recorder, recognizer, recorderShortWrite);//short����
	if(r != 0)
	{
		printf("recorder record error:%d", r);
		return;
	}
	//��ʼʶ��
	_beginthread(runRecorderVoiceRecognize, 0, recognizer);
	printf("recognize start ������\n");	
}

void destory()
{
	vp_destoryVoicePlayer(player);

	//ֹͣ¼��
	int r = stopRecord(recorder);
	if(r != 0)
	{
		printf("recorder stop record error:%d", r);
	}
	r = releaseRecorder(recorder);
	if(r != 0)
	{
		printf("recorder release error:%d", r);
	}

	//֪ͨʶ����ֹͣ�����ȴ�ʶ���������˳�
	vr_stopRecognize(recognizer);
	do 
	{		
		printf("recognizer is quiting\n");
		Sleep(100);
	} while (!vr_isRecognizerStopped(recognizer));

	//����ʶ����
	vr_destroyVoiceRecognizer(recognizer);
}


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

	chSETDLGICONS(hwnd, IDI_TESTWINDOW2);	

	SetDlgItemTextA(hwnd, IDC_HELP_URL, "http://blog.csdn.net/softlgh/article/details/40507623");
	SetDlgItemTextA(hwnd, IDC_HELP_QQ, "3116009971");
	Edit_LimitText(GetDlgItem(hwnd, IDC_SEND_TEXT), 50);

	thisHwnd = hwnd;
	init();


	return(TRUE);
}


void playVoice(HWND hwnd)
{
	char msg[100];
	UINT msgLen = GetDlgItemTextA(hwnd, IDC_SEND_TEXT, msg, sizeof(msg)-1);
	msg[msgLen] = 0;
	vp_playString(player, msg, 1, 0);
	SetDlgItemTextA(hwnd, IDC_SEND_TEXT, NULL);
}


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

	switch (id) {

	case IDCANCEL:
		destory();
		EndDialog(hwnd, id);
		break;

	case IDC_IS_RECEIVED_OWN:
		receivedOwnSignal = !IsDlgButtonChecked(hwnd, IDC_IS_RECEIVED_OWN);
		break;

	case ID_SEND: 
		playVoice(hwnd);
		break;
	}
}


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
		chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
		chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
	}

	return(FALSE);
}


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

	DialogBoxParam(hinstExe, MAKEINTRESOURCE(IDD_VOICE_RECOG), 
		NULL, Dlg_Proc, _ttoi(pszCmdLine));
	return(0);
}
