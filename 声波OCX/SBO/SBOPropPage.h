#pragma once

// SBOPropPage.h : CSBOPropPage ����ҳ���������


// CSBOPropPage : �й�ʵ�ֵ���Ϣ������� SBOPropPage.cpp��

class CSBOPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSBOPropPage)
	DECLARE_OLECREATE_EX(CSBOPropPage)

// ���캯��
public:
	CSBOPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_SBO };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

