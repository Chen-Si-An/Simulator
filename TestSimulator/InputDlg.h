
// InputDlg.h : ���Y��
//

#pragma once


// InputDlg ��ܤ��
class CInputDlg : public CDialogEx
{	
	// �غc
public:
	CInputDlg (CWnd* pParent = NULL);	// �зǫغc�禡
	virtual ~CInputDlg ();

// ��ܤ�����
	enum { IDD = IDD_DIALOG_INPUT };

private:
	int m_iInputFlag;

protected:
	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV �䴩

	// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog ();
	DECLARE_MESSAGE_MAP ()

public:
	//���o�B�]�w��J�i��flag
	int GetInputFlag () const;
	void SetInputFlag (int);

	afx_msg void OnBnClickedButtonTrue();
	afx_msg void OnBnClickedButtonFalse();
	afx_msg void OnBnClickedButtonSin();
	afx_msg void OnBnClickedButtonCos();
};
