
// TestSimulator.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CTestSimulatorApp:
// �аѾ\��@�����O�� TestSimulator.cpp
//

class CTestSimulatorApp : public CWinApp
{
public:
	CTestSimulatorApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CTestSimulatorApp theApp;