
// FunDlg.h : ���Y��
//

#pragma once


// InputDlg ��ܤ��
class CFunDlg: public CDialogEx
{	
	// �غc
public:
	CFunDlg (CWnd* pParent = NULL);	// �зǫغc�禡
	~CFunDlg ();

// ��ܤ�����
	enum { IDD = IDD_DIALOG_FUN };

private:
	int m_iOperFlag;
protected:
	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV �䴩

	// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog ();
	DECLARE_MESSAGE_MAP ()

public:
	//���o�B�]�w�B�⤸flag
	int GetOperFlag () const;
	void SetOperFlag (int);

	afx_msg void OnBnClickedButtonPlus ();
	afx_msg void OnBnClickedButtonMinus ();
	afx_msg void OnBnClickedButtonMulti ();
	afx_msg void OnBnClickedButtonDiv ();
};
