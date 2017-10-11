// SBOPropPage.cpp : CSBOPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "SBO.h"
#include "SBOPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSBOPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CSBOPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CSBOPropPage, "SBO.SBOPropPage.1",
	0xc8ac0c45, 0xd542, 0x4e1d, 0x96, 0xbe, 0x7d, 0x76, 0x70, 0x4a, 0xea, 0xb5)



// CSBOPropPage::CSBOPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CSBOPropPage ��ϵͳע�����

BOOL CSBOPropPage::CSBOPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SBO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CSBOPropPage::CSBOPropPage - ���캯��

CSBOPropPage::CSBOPropPage() :
	COlePropertyPage(IDD, IDS_SBO_PPG_CAPTION)
{
}



// CSBOPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CSBOPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CSBOPropPage ��Ϣ�������
