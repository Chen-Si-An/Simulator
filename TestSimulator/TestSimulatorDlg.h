
// TestSimulatorDlg.h : 標頭檔
//

#pragma once

#include "BlockBasis.h"
#include "BlockIn.h"
#include "BlockOut.h"
#include "BlockAnd.h"
#include "BlockOr.h"
#include "BlockNot.h"
#include "BlockFun.h"
#include "InputDlg.h"
#include "FunDlg.h"
#include "OscDlg.h"
#include "Complex.h"

#include <vector>
#include <deque>


// CTestSimulatorDlg 對話方塊
class CTestSimulatorDlg : public CDialogEx
{
// 建構
public:
	CTestSimulatorDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_TESTSIMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援

private:
	std::vector <CBlockBasis*> m_vcBlkContainer;	//存放不同Block的vector

	CRect m_rcWork;									//工作區
	CRect m_rcClient;								//對話盒客戶區
	CPoint m_ptLEnd;								//連線中線段終點
	COscDlg* m_pOscDlg;								//示波器對話盒指標
	CDC m_dcBgPic;									//存放背景圖片的dc
	CBitmap m_bitmapBgPic;							//存放背景圖片的bitmap
	CBitmap* m_pOldBitmap;							//指向原bitmap的指標
	CString m_strPicPath;							//背景圖片路徑
	COLORREF m_clrBg;								//背景顏色

	BOOL m_bInitDlg;								//判斷對話窗初始化
	BOOL m_bSelectBlk;								//判斷Block選取
	BOOL m_bMoveBlk;								//判斷Block移動
	BOOL m_bLineMode;								//判斷畫線模式
	BOOL m_bConnect;								//判斷連線
	BOOL m_bDelBlk;									//判斷Block刪除
	BOOL m_bBgPic;									//判斷載入背景圖片
	BOOL m_bGrid;									//判斷格點顯示
	BOOL m_bLock;									//判斷鎖定格點	

	int m_iWorkLeft;								//工作區左側座標
	int m_iWorkTop;									//工作區上方座標
	int m_iWorkRight;								//工作區右側座標
	int m_iWorkBottom;								//工作區下方座標
	int m_iBlkWidth;								//Block寬度
	int m_iBlkHeight;								//Block高度
	int m_iGridX;									//X方向格點間隔
	int m_iGridY;									//Y方向格點間隔
	int m_iContainerSize;							//存放Block的vector大小
	int m_iSlctBlkNum;								//選取Block編號
	int m_iHeadBlkNum;								//連接Block編號
	int m_iSimBlkNum;								//模擬Block編號
	int m_iOutSize;									//模擬Block其channel數目

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog ();
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam);
	afx_msg void OnPaint ();
	afx_msg HCURSOR OnQueryDragIcon ();
	DECLARE_MESSAGE_MAP ()
public:
	std::deque <std::deque <double> > m_deq2DOutputValue;	//存放output value的deque
	BOOL m_bSimulate;										//判斷模擬模式
	int m_iStep;											//模擬進行步數

	afx_msg void OnBnClickedButtonIn ();
	afx_msg void OnBnClickedButtonOut ();
	afx_msg void OnBnClickedButtonAnd ();
	afx_msg void OnBnClickedButtonOr ();
	afx_msg void OnBnClickedButtonNot ();
	afx_msg void OnBnClickedButtonFunction ();
	afx_msg void OnBnClickedButtonLine ();
	afx_msg void OnBnClickedButtonSimulate ();
	afx_msg void OnBnClickedButtonGrid ();
	afx_msg void OnBnClickedButtonLock ();
	afx_msg void OnBnClickedButtonBgpic ();
	afx_msg void OnBnClickedButtonBgcolor ();
	afx_msg void OnBnClickedButtonOpen ();
	afx_msg void OnBnClickedButtonSave ();
	afx_msg void OnLButtonDown (UINT nFlags, CPoint ptCursor);
	afx_msg void OnMouseMove (UINT nFlags, CPoint ptCursor);
	afx_msg void OnLButtonUp (UINT nFlags, CPoint ptCursor);
	afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
	afx_msg void OnTimer (UINT_PTR nIDEvent);
	afx_msg void OnSize (UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage (MSG* pMsg);
	afx_msg void OnDestroy ();

	void SplitString (CString, char, CStringArray&);
	void Resize (int nID, double dMulWid, double dMulHei);
	void Resize (CBlockBasis* pBlk, double dMulWid, double dMulHei);
	int GetComputationLayers (int iDataSize);
	double FFT (const std::deque <double> &, int, double);
};
