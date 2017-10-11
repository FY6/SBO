#pragma once

#include "resource.h"
#include "SBOCtrl.h"

void WaitForVoiceData(LPVOID lpParam);
class CSBOCtrl;
class CTestVoice
{
	// ���캯��
public:
	CTestVoice();
	// ʵ��
protected:
	~CTestVoice();
public:
	void init();
	void destory();
	void playVoice(char* hwnd);
	void OpenVoicePlayer();
	void CloseVoicePlayer();
	void SetData(CString data);
public:
	CSBOCtrl* SBOCtrl;
	void *player ;
	void *recognizer ;
	void *recorder ;
};