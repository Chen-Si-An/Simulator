
// InputDlg.h : 標頭檔
//

#pragma once


// InputDlg 對話方塊
class CInputDlg : public CDialogEx
{	
	// 建構
public:
	CInputDlg (CWnd* pParent = NULL);	// 標準建構函式
	virtual ~CInputDlg ();

// 對話方塊資料
	enum { IDD = IDD_DIALOG_INPUT };

private:
	int m_iInputFlag;

protected:
	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV 支援

	// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog ();
	DECLARE_MESSAGE_MAP ()

public:
	//取得、設定輸入波形flag
	int GetInputFlag () const;
	void SetInputFlag (int);

	afx_msg void OnBnClickedButtonTrue();
	afx_msg void OnBnClickedButtonFalse();
	afx_msg void OnBnClickedButtonSin();
	afx_msg void OnBnClickedButtonCos();
};
