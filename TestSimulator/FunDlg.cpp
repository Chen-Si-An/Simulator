
// FunDlg.cpp : ��@��
//


#include "stdafx.h"
#include "TestSimulator.h"
#include "FunDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NO_OPER_FLAG -11	//���s�b�B�⤸

#define PLUS 1				// + flag
#define MINUS 2				// - flag
#define MULTI 3				// * flag
#define DIV 4				// / flag


CFunDlg::CFunDlg (CWnd* pParent)
	: CDialogEx (CFunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);
	m_iOperFlag = NO_OPER_FLAG;
}

CFunDlg::~CFunDlg ()
{
}


BEGIN_MESSAGE_MAP (CFunDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PLUS, &CFunDlg::OnBnClickedButtonPlus)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CFunDlg::OnBnClickedButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_MULTI, &CFunDlg::OnBnClickedButtonMulti)
	ON_BN_CLICKED(IDC_BUTTON_DIV, &CFunDlg::OnBnClickedButtonDiv)
END_MESSAGE_MAP ()


void CFunDlg::DoDataExchange (CDataExchange* pDX)
{
	CDialogEx::DoDataExchange (pDX);
}

BOOL CFunDlg::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetReadOnly ();

	//������l�Ʈ�Ū���B�⤸flag�A�ñN����ܩ�Edit��
	switch (m_iOperFlag)
	{
	case PLUS:
		( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("+"));
		break;
	case MINUS:
		( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("-"));
		break;
	case MULTI:
		( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("*"));
		break;
	case DIV:
		( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("/"));
		break;
	default:
		( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("Please select operator"));
		break;
	}

	return TRUE;
}

int CFunDlg::GetOperFlag () const
{
	return m_iOperFlag;
}

void CFunDlg::SetOperFlag (int iOperFlag)
{
	m_iOperFlag = iOperFlag;
}

void CFunDlg::OnBnClickedButtonPlus ()
{
	m_iOperFlag = PLUS;		//���U+��]�w��J�i��flag��PLUS
	( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("+"));
}


void CFunDlg::OnBnClickedButtonMinus ()
{
	m_iOperFlag = MINUS;	//���U-��]�w��J�i��flag��MINUS
	( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("-"));
}


void CFunDlg::OnBnClickedButtonMulti ()
{
	m_iOperFlag = MULTI;	//���U*��]�w��J�i��flag��MULTI
	( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("*"));
}


void CFunDlg::OnBnClickedButtonDiv ()
{
	m_iOperFlag = DIV;		//���U/��]�w��J�i��flag��DIV
	( (CEdit* ) GetDlgItem (IDC_EDIT_OPERATOR))->SetWindowTextA (_T ("/"));
}