
// OscDlg.h : 標頭檔
//

#pragma once

#include "afxwin.h"
#include <deque>

class CTestSimulatorDlg;

// COscDlg 對話方塊

class COscDlg : public CDialogEx
{
public:
	COscDlg (CWnd* pParent = NULL);   // 標準建構函式
	virtual ~COscDlg ();

// 對話方塊資料
	enum { IDD = IDD_DIALOG_OSC };

private:
	CTestSimulatorDlg* m_pSimulatorDlg;	//主對話盒指標

	CComboBox m_ctrlComboOutput;		//Output combo box變數
	CComboBox m_ctrlComboYInterval;		//Y軸間隔 combo box變數
	CComboBox m_ctrlComboXInterval;		//X軸間隔 combo box變數

	std::deque <CPen*> m_deqPenPtr;		//隨機畫筆deque

	double m_dTime;						//模擬時間
	double m_dSlctOutputValue;			//選取output value
	double m_dMaxDataValue;				//示波器顯示最大值
	double m_dFreq;						//頻率

	int m_iOutputSize;					//channel數目
	double m_dYInterval;				//Y軸間隔
	int m_iXInterval;					//X軸間隔
	int m_iYPos;						//Y軸間隔 combo box位置
	int m_iXPos;						//X軸間隔 combo box位置

protected:
	HICON m_hIcon;

	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP ()
public:
	CRect m_rcWork;						//示波器對話盒工作區

	int m_iSlctOutputNum;				//選取output編號

	BOOL m_bSim;						//判斷模擬
	BOOL m_bModify;						//判斷combo box更改
	BOOL m_bFFT;						//判斷進行FFT

	virtual BOOL OnInitDialog ();
	afx_msg void OnPaint ();
	afx_msg void OnBnClickedButtonStart ();
	afx_msg void OnBnClickedButtonStop ();
	afx_msg void OnCbnSelchangeComboOutput ();
	afx_msg void OnCbnSelchangeComboValueInterval ();
	afx_msg void OnCbnSelchangeComboTimeInterval ();
	virtual BOOL PreTranslateMessage (MSG* pMsg);
	afx_msg void OnClose ();
	void SetData (double, double, double);
	void SetData (double, double);
	void SetOutputSize (int);
	void InitialData ();	
	int GetTimeInterval () const;
};
