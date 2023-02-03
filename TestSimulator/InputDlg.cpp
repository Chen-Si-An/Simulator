
// InputDlg.cpp : 實作檔
//


#include "stdafx.h"
#include "TestSimulator.h"
#include "InputDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NO_INPUT_FLAG -12	//不存在輸入波形

#define ZERO 0				//FALSE flag
#define ONE 1				//TRUE flag
#define SIN 2				//Sin flag
#define COS 3				//Cos flag


CInputDlg::CInputDlg (CWnd* pParent)
	: CDialogEx (CInputDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);
	m_iInputFlag = NO_INPUT_FLAG;	//初始化輸入波形flag
}

CInputDlg::~CInputDlg ()
{
}

BEGIN_MESSAGE_MAP (CInputDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TRUE, &CInputDlg::OnBnClickedButtonTrue)
	ON_BN_CLICKED(IDC_BUTTON_FALSE, &CInputDlg::OnBnClickedButtonFalse)
	ON_BN_CLICKED(IDC_BUTTON_SIN, &CInputDlg::OnBnClickedButtonSin)
	ON_BN_CLICKED(IDC_BUTTON_COS, &CInputDlg::OnBnClickedButtonCos)
END_MESSAGE_MAP ()

void CInputDlg::DoDataExchange (CDataExchange* pDX)
{
	CDialogEx::DoDataExchange (pDX);
}

BOOL CInputDlg::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetReadOnly ();

	//視窗初始化時讀取輸入波形flag，並將其顯示於Edit中
	switch (m_iInputFlag)
	{
	case ZERO:
		( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("False"));
		break;
	case ONE:
		( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("True"));
		break;
	case SIN:
		( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Sin (t)"));
		break;
	case COS:
		( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Cos (t)"));
		break;
	default:
		( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Please select input"));
	}

	return TRUE;
}

int CInputDlg::GetInputFlag () const
{
	return m_iInputFlag;
}

void CInputDlg::SetInputFlag (int iInputFlag)
{
	m_iInputFlag = iInputFlag;
}

void CInputDlg::OnBnClickedButtonTrue ()
{
	m_iInputFlag = ONE;		//按下True鍵設定輸入波形flag為ONE
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("True"));
}

void CInputDlg::OnBnClickedButtonFalse ()
{
	m_iInputFlag = ZERO;	//按下False鍵設定輸入波形flag為ZERO
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("False"));
}

void CInputDlg::OnBnClickedButtonSin ()
{
	m_iInputFlag = SIN;		//按下Sin鍵設定輸入波形flag為SIN
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Sin (t)"));
}

void CInputDlg::OnBnClickedButtonCos ()
{
	m_iInputFlag = COS;		//按下Cos鍵設定輸入波形flag為COS
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Cos (t)"));
}