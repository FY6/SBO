// SBOCtrl.cpp : CSBOCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "SBO.h"
#include "SBOCtrl.h"
#include "SBOPropPage.h"
#include "afxdialogex.h"
 
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")  
#pragma comment(lib,"comsuppwd.lib")  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSBOCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CSBOCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_GetVoiceData, &CSBOCtrl::OnGetvoicedata)
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CSBOCtrl, COleControl)
	DISP_FUNCTION_ID(CSBOCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSBOCtrl, "OpenRecorder", dispidOpenRecorder, OpenRecorder, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSBOCtrl, "PlayVoice", dispidPlayVoice, PlayVoice, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CSBOCtrl, "OpenVoicePlayer", dispidOpenVoicePlayer, OpenVoicePlayer, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSBOCtrl, "CloseVoicePlayer", dispidCloseVoicePlayer, CloseVoicePlayer, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSBOCtrl, "CloseRecorder", dispidCloseRecorder, CloseRecorder, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CSBOCtrl, COleControl)
	EVENT_CUSTOM_ID("OnGetVoiceData", eventidOnGetVoiceData, OnGetVoiceData, VTS_BSTR)
	EVENT_CUSTOM_ID("OnGetLog", eventidOnGetLog, OnGetLog, VTS_BSTR)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CSBOCtrl, 1)
	PROPPAGEID(CSBOPropPage::guid)
END_PROPPAGEIDS(CSBOCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CSBOCtrl, "SBO.SBOCtrl.1",
	0xb908d0e2, 0x38c8, 0x44ab, 0xa9, 0x34, 0xd5, 0x2b, 0xf4, 0x29, 0xd0, 0x75)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CSBOCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DSBO = { 0xFD73CF57, 0x9A76, 0x47AC, { 0xBE, 0x96, 0x7, 0xAC, 0xE7, 0x71, 0x45, 0xC } };
const IID IID_DSBOEvents = { 0x61789752, 0xB8A7, 0x4C1E, { 0x92, 0xBF, 0xC9, 0xD1, 0xC1, 0x23, 0x5B, 0xBA } };


// �ؼ�������Ϣ

static const DWORD _dwSBOOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSBOCtrl, IDS_SBO, _dwSBOOleMisc)



// CSBOCtrl::CSBOCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CSBOCtrl ��ϵͳע�����

BOOL CSBOCtrl::CSBOCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SBO,
			IDB_SBO,
			afxRegApartmentThreading,
			_dwSBOOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CSBOCtrl::CSBOCtrl - ���캯��

CSBOCtrl::CSBOCtrl()
{
	InitializeIIDs(&IID_DSBO, &IID_DSBOEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	testVoice=new CTestVoice();
	testVoice->SBOCtrl=this;
	recoderFlag=FALSE;
	playerFlag=FALSE;
}



// CSBOCtrl::~CSBOCtrl - ��������

CSBOCtrl::~CSBOCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
	if(testVoice!=NULL){
		testVoice=NULL;
	}
}



// CSBOCtrl::OnDraw - ��ͼ����

void CSBOCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CSBOCtrl::DoPropExchange - �־���֧��

void CSBOCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CSBOCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CSBOCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CSBOCtrl::AboutBox - ���û���ʾ�����ڡ���

void CSBOCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_SBO);
	dlgAbout.DoModal();
}




void CSBOCtrl::OpenRecorder(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(recoderFlag)
	{
		CString abc("�ѿ���¼����");
		OnGetLog(abc.AllocSysString());
		return;
	}
	else
	{
		recoderFlag=TRUE;
		testVoice->init();
	}
	// TODO: �ڴ���ӵ��ȴ���������
}



void CSBOCtrl::PlayVoice(BSTR data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	if(!playerFlag)
	{
		CString abc("δ����������");
		OnGetLog(abc.AllocSysString());
		return;
	}
	else
	{
		char* lpszText2 = _com_util::ConvertBSTRToString(data); 
		testVoice->playVoice(lpszText2);
		SysFreeString(data);
		delete[] lpszText2;
	}
}


void CSBOCtrl::OpenVoicePlayer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ���ӵ��ȴ���������
	if(playerFlag)
	{
		CString abc("�ѿ���������");
		OnGetLog(abc.AllocSysString());
		return;
	}
	else
	{
		playerFlag=TRUE;
		testVoice->OpenVoicePlayer();
	}
}


void CSBOCtrl::CloseVoicePlayer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	if(!playerFlag)
	{
		CString abc("δ����������");
		OnGetLog(abc.AllocSysString());
		return;
	}
	else
	{
		playerFlag=FALSE;
		testVoice->CloseVoicePlayer();
	}
}


afx_msg LRESULT CSBOCtrl::OnGetvoicedata(WPARAM wParam, LPARAM lParam)
{
	BSTR data=(BSTR)lParam;
	OnGetVoiceData(data);
	SysFreeString(data);
	return 0;
}


void CSBOCtrl::CloseRecorder(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(!recoderFlag)
	{
		CString abc("δ����¼����");
		OnGetLog(abc.AllocSysString());
		return;
	}
	else
	{
		testVoice->destory();
		recoderFlag=FALSE;
	}
	// TODO: �ڴ���ӵ��ȴ���������
}
