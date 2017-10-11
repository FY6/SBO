#include "stdafx.h"
#include "testVoice.h"

#include "CmnHdr.h" 
#include <Windowsx.h>

#include "voicePlay.h"
#include "voiceRecog.h"
#include "audioRecorder.h"

#include <stdio.h>
#include <assert.h>

#pragma comment(lib, "cvoiceRecognizeLib.lib")

#define M_SEND_TEXT    (WM_USER + 100)

CString resultData;
HANDLE handle=CreateEvent(NULL, TRUE, FALSE, NULL);

CTestVoice::CTestVoice()
{
	player=NULL;
	recognizer=NULL;
	recorder=NULL;
}

CTestVoice::~CTestVoice()
{
	player=NULL;
	recognizer=NULL;
	recorder=NULL;
	SBOCtrl=NULL;
}

//ʶ��ʼ�Ļص�����
void recorderRecognizerStart(void *_listener, float _soundTime)
{
}

//ʶ������Ļص�����
void recorderRecognizerEnd(void *_listener, float _soundTime, int _recogStatus, char *_data, int _dataLen)
{
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
			resultData=result;
			SetEvent(handle);
		}
	}
	else
	{
		//printf("------------------recognize invalid data, errorCode:%d, error:%s\n", _recogStatus, recorderRecogErrorMsg(_recogStatus));
	}
}

//���ſ�ʼ�Ļص�
void onPlayStart(void *_listener)
{
}

//���Ž����Ļص�
void onPlayEnd(void *_listener)
{
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
void CTestVoice::init()
{


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
	_beginthread(WaitForVoiceData, 0, this);

	CString abc("recognize start");
	SetData(abc);

}

void WaitForVoiceData(LPVOID lpParam)
{
	CTestVoice* pParent = (CTestVoice*)lpParam;
 	if (pParent == NULL)
 	{
 		return;
 	}

	while(true)
	{
		ResetEvent(handle);
		WaitForSingleObject(handle, INFINITE);
		::PostMessage(pParent->SBOCtrl->m_hWnd,WM_GetVoiceData,NULL,(LPARAM)resultData.AllocSysString());
	}
}


void CTestVoice::destory()
{
	//ֹͣ¼��
	int r = stopRecord(recorder);
	if(r != 0)
	{
	}
	r = releaseRecorder(recorder);
	if(r != 0)
	{
	}

	//֪ͨʶ����ֹͣ�����ȴ�ʶ���������˳�
	vr_stopRecognize(recognizer);
	do 
	{	
		CString abc("recognizer is quiting");
		SetData(abc);
		Sleep(100);
	} while (!vr_isRecognizerStopped(recognizer));

	//����ʶ����
	vr_destroyVoiceRecognizer(recognizer);
}

void CTestVoice::SetData(CString data)
{
	SBOCtrl->OnGetLog(data.AllocSysString());
}

void CTestVoice::playVoice(char* hwnd)
{
	char msg[100];
	strcpy(msg,hwnd);
	msg[strlen(hwnd)]=0;
	vp_playString(player, msg, 1, 0);
}

void CTestVoice::OpenVoicePlayer()
{
	//��������������
	player = vp_createVoicePlayer();
	vp_setFreqs(player, playFreqs, sizeof(playFreqs)/sizeof(int));
	vp_setPlayerListener(player, NULL, onPlayStart, onPlayEnd);//���ò������ص�����
	CString abc("VoicePlayer Start");
	SetData(abc);
}

void CTestVoice::CloseVoicePlayer()
{
	vp_destoryVoicePlayer(player);
	CString abc("VoicePlayer is quiting");
	SetData(abc);
}