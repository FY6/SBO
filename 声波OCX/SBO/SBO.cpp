// SBO.cpp : CSBOApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "SBO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSBOApp theApp;

const GUID CDECL _tlid = { 0x5231766C, 0x4B82, 0x40D7, { 0x8E, 0xA2, 0xFE, 0xDD, 0x9C, 0xFA, 0x56, 0x25 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CSBOApp::InitInstance - DLL ��ʼ��

BOOL CSBOApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// CSBOApp::ExitInstance - DLL ��ֹ

int CSBOApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
