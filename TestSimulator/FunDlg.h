
// FunDlg.h : 標頭檔
//

#pragma once


// InputDlg 對話方塊
class CFunDlg: public CDialogEx
{	
	// 建構
public:
	CFunDlg (CWnd* pParent = NULL);	// 標準建構函式
	~CFunDlg ();

// 對話方塊資料
	enum { IDD = IDD_DIALOG_FUN };

private:
	int m_iOperFlag;
protected:
	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV 支援

	// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog ();
	DECLARE_MESSAGE_MAP ()

public:
	//取得、設定運算元flag
	int GetOperFlag () const;
	void SetOperFlag (int);

	afx_msg void OnBnClickedButtonPlus ();
	afx_msg void OnBnClickedButtonMinus ();
	afx_msg void OnBnClickedButtonMulti ();
	afx_msg void OnBnClickedButtonDiv ();
};
