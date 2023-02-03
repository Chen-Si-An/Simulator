
// InputDlg.cpp : ��@��
//


#include "stdafx.h"
#include "TestSimulator.h"
#include "InputDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NO_INPUT_FLAG -12	//���s�b��J�i��

#define ZERO 0				//FALSE flag
#define ONE 1				//TRUE flag
#define SIN 2				//Sin flag
#define COS 3				//Cos flag


CInputDlg::CInputDlg (CWnd* pParent)
	: CDialogEx (CInputDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);
	m_iInputFlag = NO_INPUT_FLAG;	//��l�ƿ�J�i��flag
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

	//������l�Ʈ�Ū����J�i��flag�A�ñN����ܩ�Edit��
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
	m_iInputFlag = ONE;		//���UTrue��]�w��J�i��flag��ONE
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("True"));
}

void CInputDlg::OnBnClickedButtonFalse ()
{
	m_iInputFlag = ZERO;	//���UFalse��]�w��J�i��flag��ZERO
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("False"));
}

void CInputDlg::OnBnClickedButtonSin ()
{
	m_iInputFlag = SIN;		//���USin��]�w��J�i��flag��SIN
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Sin (t)"));
}

void CInputDlg::OnBnClickedButtonCos ()
{
	m_iInputFlag = COS;		//���UCos��]�w��J�i��flag��COS
	( (CEdit*) GetDlgItem (IDC_EDIT_INPUT_VALUE))->SetWindowTextA (_T ("Cos (t)"));
}