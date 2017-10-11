#pragma once

// SBOCtrl.h : CSBOCtrl ActiveX �ؼ����������

#include "testVoice.h"
// CSBOCtrl : �й�ʵ�ֵ���Ϣ������� SBOCtrl.cpp��
class CTestVoice;
class CSBOCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSBOCtrl)

// ���캯��
public:
	CSBOCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CSBOCtrl();

	DECLARE_OLECREATE_EX(CSBOCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CSBOCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSBOCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CSBOCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	CTestVoice *testVoice;
	int recoderFlag;
	int playerFlag;
public:
	void OpenRecorder(void);

	enum 
	{
		eventidOnGetLog = 2L,
		dispidCloseRecorder = 6L,
		eventidASDFSA = 2L,
		dispidCloseVoicePlayer = 5L,
		dispidOpenVoicePlayer = 4L,
		dispidPlayVoice = 3L,
		eventidOnGetVoiceData = 1L,
		dispidOpenRecorder = 1L
	};

	void OnGetVoiceData(BSTR data)
	{
		FireEvent(eventidOnGetVoiceData, EVENT_PARAM(VTS_BSTR), data);
	}
	void OnGetLog(BSTR data)
	{
		FireEvent(eventidOnGetLog, EVENT_PARAM(VTS_BSTR),data);
	}
	void PlayVoice(BSTR data);
	void OpenVoicePlayer(void);
	void CloseVoicePlayer(void);
protected:
	afx_msg LRESULT OnGetvoicedata(WPARAM wParam, LPARAM lParam);
	void CloseRecorder(void);
};

