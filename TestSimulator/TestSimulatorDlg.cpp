
// TestSimulatorDlg.cpp : 實作檔
//


#include "stdafx.h"
#include "TestSimulator.h"
#include "TestSimulatorDlg.h"
#include "afxdialogex.h"

#define _USE_MATH_DEFINES
#include "math.h"

//#include <iostream>
//using namespace std;

#define WIN_WIDTH 1316					//對話盒客戶區寬度			
#define WIN_HEIGHT 559					//對話盒客戶區高度
#define GRID_R 1						//單一格點半徑大小
#define MAX_STR_LEN 100					//最大字串長度 (用於讀取ini檔)

#define WHITE RGB (255, 255, 255)		//工作區背景預設顏色
#define GRAY  RGB (240, 240, 240)		//格點顏色
#define LIGHT_BLUE RGB (176, 224, 230)	//選取Block方框顏色
#define RED   RGB (255, 0, 0)			//模擬Block方框顏色		
#define BLACK RGB (0, 0, 0)				//連線顏色

#define	MARK_FONTSIZE 120				//模式Mark字體大小		
#define MARK_WIDTH 90					//Mark區域寬度
#define MARK_LOCK_WIDTH 60				//Lock Mark區域寬度
#define MARK_HEIGHT 40					//Mark區域高度

#define NO_VALUE -10					//不存在輸入訊號
#define NO_OPER_FLAG -11				//不存在運算元
#define NO_INPUT_FLAG -12				//不存在輸入波形
#define COMPUTE_ERROR -13				//不允許的運算
#define NO_BLOCK_HEAD -14				//不存在連接Block
#define NO_SEL_NUM -15					//不存在選取Block編號

#define NO_BLK_TYPE 0					//Block不屬於現有type
#define BLK_IN		1					//Block為IN
#define BLK_OUT		2					//Block為OUT
#define BLK_AND		3					//Block為AND
#define BLK_OR		4					//Block為OR
#define BLK_NOT		5					//Block為NOT
#define BLK_FUN		6					//Block為FUN

#define ZERO 0							//FALSE flag
#define ONE  1							//TRUE flag
#define SIN  2							//Sin flag
#define COS  3							//Cos flag

#define MSEC 0.001						//millisecond
#define MAX_DATA_NUM 512				//最大儲存data數目 (必須為2的次方數)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestSimulatorDlg 對話方塊



CTestSimulatorDlg::CTestSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestSimulatorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_rcWork.SetRect (0, 0, 0, 0);
	m_rcClient.SetRect(0, 0, 0, 0);
	m_ptLEnd.SetPoint (0, 0);
	m_pOscDlg = NULL;
	m_pOldBitmap = NULL;
	m_clrBg = WHITE;

	m_bInitDlg = FALSE;
	m_bSelectBlk = FALSE;
	m_bMoveBlk = FALSE;
	m_bLineMode = FALSE;
	m_bConnect = FALSE;
	m_bDelBlk = FALSE;
	m_bSimulate = FALSE;
	m_bBgPic = FALSE;
	m_bGrid = FALSE;
	m_bLock = FALSE;

	m_iWorkLeft = 150;
	m_iWorkTop = 10;
	m_iWorkRight = 1150;
	m_iWorkBottom = 510;
	m_iBlkWidth = 100;
	m_iBlkHeight = 50;
	m_iGridX = (int) (m_iBlkWidth / 4.);
	m_iGridY = (int) (m_iBlkHeight / 2.);
	m_iContainerSize = 0;
	m_iSlctBlkNum = 0;
	m_iHeadBlkNum = 0;
	m_iSimBlkNum = 0;
	m_iOutSize = 0;

	m_iStep = 0;
}

void CTestSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestSimulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_IN, &CTestSimulatorDlg::OnBnClickedButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &CTestSimulatorDlg::OnBnClickedButtonOut)
	ON_BN_CLICKED(IDC_BUTTON_LINE, &CTestSimulatorDlg::OnBnClickedButtonLine)
	ON_BN_CLICKED(IDC_BUTTON_AND, &CTestSimulatorDlg::OnBnClickedButtonAnd)
	ON_BN_CLICKED(IDC_BUTTON_OR, &CTestSimulatorDlg::OnBnClickedButtonOr)
	ON_BN_CLICKED(IDC_BUTTON_NOT, &CTestSimulatorDlg::OnBnClickedButtonNot)
	ON_BN_CLICKED(IDC_BUTTON_FUNCTION, &CTestSimulatorDlg::OnBnClickedButtonFunction)
	ON_BN_CLICKED(IDC_BUTTON_SIMULATE, &CTestSimulatorDlg::OnBnClickedButtonSimulate)
	ON_BN_CLICKED(IDC_BUTTON_BGCOLOR, &CTestSimulatorDlg::OnBnClickedButtonBgcolor)
	ON_BN_CLICKED(IDC_BUTTON_BGPIC, &CTestSimulatorDlg::OnBnClickedButtonBgpic)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTestSimulatorDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTestSimulatorDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_GRID, &CTestSimulatorDlg::OnBnClickedButtonGrid)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CTestSimulatorDlg::OnBnClickedButtonLock)
END_MESSAGE_MAP()


// CTestSimulatorDlg 訊息處理常式

BOOL CTestSimulatorDlg::OnInitDialog()
{
	this->SetWindowPos (NULL, 0, 0, WIN_WIDTH, WIN_HEIGHT, SWP_NOZORDER | SWP_NOMOVE);	//設定對話盒客戶區域
	m_rcWork.SetRect (m_iWorkLeft, m_iWorkTop, m_iWorkRight, m_iWorkBottom);			//設定工作區域
	this->GetClientRect (&m_rcClient);													//讀取對話盒客戶區域
	m_dcBgPic.CreateCompatibleDC (NULL);												//建立與對話盒相容的dc，以載入背景圖片

	CString strIniPath (_T (".\\Initial.ini"));	//.ini路徑
	CString strAppName (_T ("BgParameter"));	//.ini中[節名]

	CString strBgPicBOOL;	//儲存背景圖片布林值的字串
	CString strBgPicPath;	//儲存背景圖片路徑的字串
	CString strBgColor;		//儲存背景顏色值的字串
	CString strGridBOOL;	//儲存格點布林值的字串
	CString strLockBOOL;	//儲存鎖定布林值的字串
	
	CString strKeyName (_T ("BgPicBOOL"));		//.ini中變數名稱
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strBgPicBOOL.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("BgPicPath"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strBgPicPath.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("BgColor"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strBgColor.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("GridBOOL"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strGridBOOL.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("LockBOOL"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strLockBOOL.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);

	//載入背景圖片(or顏色)
	if (strBgPicBOOL == _T ("TRUE"))
	{
		m_strPicPath = strBgPicPath;

		//將讀取的.bmp指定給HBITMAP
		HBITMAP hbBgPic;
		//利用HBITMAP加載位圖，LoadImage (handle, 圖片路徑, 載入樣式, 圖示寬度(像素), 圖示長度(像素), LR_LOADFROMFILE: 根據路徑的值裝在圖像)
		hbBgPic = (HBITMAP)::LoadImage (AfxGetInstanceHandle (), m_strPicPath, IMAGE_BITMAP, m_rcWork.Width (), m_rcWork.Height (), LR_LOADFROMFILE);
		m_bitmapBgPic.Attach (hbBgPic);
		m_pOldBitmap = m_dcBgPic.SelectObject (&m_bitmapBgPic);

		m_bBgPic = TRUE;
	}
	else if (strBgPicBOOL == _T ("FALSE"))
		m_clrBg = _ttoi (strBgColor);

	//設定格點初始狀態
	if (strGridBOOL == _T ("TRUE"))
		m_bGrid = TRUE;

	//載入鎖定初始狀態
	if (strLockBOOL == _T ("TRUE"))
		m_bLock = TRUE;

	
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	m_bInitDlg = TRUE;

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CTestSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CTestSimulatorDlg::OnPaint ()
{
	//雙緩衝建立
	CPaintDC dcSim (this);				//主對話盒dc
	CDC dcMem;							//內存dc
	CBitmap bitmapMem;
	CBitmap * pOldBitmap;

	dcMem.CreateCompatibleDC (&dcSim);	//建立與主對話盒相容的dc
	bitmapMem.CreateCompatibleBitmap (&dcSim, m_iWorkRight, m_iWorkBottom);
	pOldBitmap = dcMem.SelectObject (&bitmapMem);

	//繪製背景
	dcMem.SetBkMode (TRANSPARENT);
	dcMem.FillSolidRect (m_rcWork, m_clrBg);

	//模式Mark的rect
	CPoint ptMarkTL (m_iWorkRight - MARK_WIDTH, m_iWorkTop);
	CPoint ptMarkBR (m_iWorkRight, m_iWorkTop + MARK_HEIGHT);
	CRect rcModeMark (ptMarkTL, ptMarkBR);

	//模式Mark字體
	CFont fontModeMark, * pOldFont;	
	fontModeMark.CreatePointFont (MARK_FONTSIZE, _T ("Arial"), &dcMem);	

	//由存放背景圖片的dc拷貝至內存dc
	if (m_bBgPic)
		dcMem.BitBlt (m_iWorkLeft, m_iWorkTop, m_rcWork.Width (), m_rcWork.Height (), &m_dcBgPic, 0, 0, SRCCOPY);

	//繪製格點
	if (m_bGrid)
	{
		CBrush brushGray (GRAY);
		CBrush* pOldBrush;
		pOldBrush = dcMem.SelectObject (&brushGray);

		for (int i = m_iWorkLeft; i < m_iWorkRight; i += m_iGridX)
		{
			for (int j = m_iWorkTop; j < m_iWorkBottom; j += m_iGridY)		
				dcMem.Ellipse (CRect (i - GRID_R, j - GRID_R, i + GRID_R, j + GRID_R));
		}

		dcMem.SelectObject (pOldBrush);
	}

	//繪製鎖定Mark
	if (m_bLock)
	{
		CPoint ptLockMarkTL (m_iWorkLeft, m_iWorkTop);
		CPoint ptLockMarkBR (m_iWorkLeft + MARK_LOCK_WIDTH, m_iWorkTop + MARK_HEIGHT);
		CRect rcLockMark (ptLockMarkTL, ptLockMarkBR);

		pOldFont = dcMem.SelectObject (&fontModeMark);
		dcMem.DrawText (_T ("Lock"), -1, &rcLockMark, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dcMem.SelectObject (pOldFont);
	}

	//繪製Block連線
	for (int i = 0; i < m_iContainerSize; i++)
		m_vcBlkContainer[i]->DrawLine (&dcMem);

	//繪製選取Block方框
	if (m_bSelectBlk)
	{
		if (m_iSlctBlkNum >= 0 && m_iSlctBlkNum < m_iContainerSize)
		{
			CPen penLightBlue (PS_SOLID, 10, LIGHT_BLUE);
			CPen* pOldPen;
			pOldPen = dcMem.SelectObject (&penLightBlue);
		
			dcMem.Rectangle (m_vcBlkContainer[m_iSlctBlkNum]->GetBlkRect ());
			dcMem.SelectObject (pOldPen);	
		}		
	}

	//繪製模擬模式Mark與模擬Block方框
	if (m_bSimulate)
	{
		//繪製模擬模式Mark
		pOldFont = dcMem.SelectObject (&fontModeMark);
		dcMem.DrawText (_T ("Simulation"), -1, &rcModeMark, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dcMem.SelectObject (pOldFont);

		//繪製模擬Block方框
		CPen penRed (PS_SOLID, 10, RED);
		CPen* pOldPen;
		pOldPen = dcMem.SelectObject (&penRed);
		
		dcMem.Rectangle (m_vcBlkContainer[m_iSimBlkNum]->GetBlkRect ());
		dcMem.SelectObject (pOldPen);
	}

	//繪製Block
	for (int i = 0; i < m_iContainerSize; i++)
		m_vcBlkContainer[i]->Draw (&dcMem);

	//繪製連線模式Mark與連線中之虛線線段
	if (m_bLineMode)
	{
		//繪製連線模式Mar
		pOldFont = dcMem.SelectObject (&fontModeMark);
		dcMem.DrawText (_T ("LineMode"), -1, &rcModeMark, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dcMem.SelectObject (pOldFont);

		//繪製連線中之虛線線段
		if (m_bConnect)
		{
			CPen penBlack (PS_DASH, 1, BLACK);
			CPen* pOldPen;

			pOldPen = dcMem.SelectObject (&penBlack);

			CPoint ptLStart = m_vcBlkContainer[m_iHeadBlkNum]->GetPinOutRect ().CenterPoint ();			//線段起點為Block輸出Pin的中心點
			int iCornerGridY = (int) (((ptLStart.y + m_ptLEnd.y) / 2. - m_iWorkTop) / m_iGridY + 0.5);	//連線轉折處距工作區上方幾個格點
			int iCornerY = iCornerGridY * m_iGridY + m_iWorkTop;										//連線轉折處Y座標
			dcMem.MoveTo (ptLStart);
			dcMem.LineTo (CPoint (ptLStart.x, iCornerY));
			dcMem.MoveTo (CPoint (ptLStart.x, iCornerY));
			dcMem.LineTo (CPoint (m_ptLEnd.x, iCornerY));
			dcMem.MoveTo (CPoint (m_ptLEnd.x, iCornerY));
			dcMem.LineTo (m_ptLEnd);

			dcMem.SelectObject (pOldPen);
		}		
	}

	//由內存dc拷貝至主對話盒dc
	dcSim.BitBlt (m_iWorkLeft, m_iWorkTop, m_rcWork.Width (), m_rcWork.Height (), &dcMem, m_iWorkLeft, m_iWorkTop, SRCCOPY);

	//清理
	dcMem.SelectObject (pOldBitmap);
	bitmapMem.DeleteObject ();
	dcMem.DeleteDC ();
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CTestSimulatorDlg::OnQueryDragIcon ()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//按下IN按鈕
void CTestSimulatorDlg::OnBnClickedButtonIn ()
{
	m_bLineMode = FALSE;	//取消連線模式

	CBlockBasis* pBlk = new CBlockIn;																				//new一個CBlockIn的物件
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//設定Block In的rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//設定Block編號
	m_vcBlkContainer.push_back (pBlk);																				//存入Block的vector中
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//紀錄Block的vector size

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下OUT按鈕
void CTestSimulatorDlg::OnBnClickedButtonOut ()
{
	m_bLineMode = FALSE;	//取消連線模式

	CBlockBasis* pBlk = new CBlockOut;																				//new一個CBlockOut的物件
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//設定Block Out的rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//設定Block編號
	m_vcBlkContainer.push_back (pBlk);																				//存入Block的vector中
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//紀錄Block的vector size

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下AND按鈕
void CTestSimulatorDlg::OnBnClickedButtonAnd ()
{
	m_bLineMode = FALSE;	//取消連線模式

	CBlockBasis* pBlk = new CBlockAnd;																				//new一個CBlockAnd的物件
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//設定Block And的rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//設定Block編號
	m_vcBlkContainer.push_back (pBlk);																				//存入Block的vector中
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//紀錄Block的vector size

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下OR按鈕
void CTestSimulatorDlg::OnBnClickedButtonOr ()
{
	m_bLineMode = FALSE;	//取消連線模式

	CBlockBasis* pBlk = new CBlockOr;																				//new一個CBlockOr的物件
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//設定Block Or的rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//設定Block編號
	m_vcBlkContainer.push_back (pBlk);																				//存入Block的vector中
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//紀錄Block的vector size

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下NOT按鈕
void CTestSimulatorDlg::OnBnClickedButtonNot ()
{
	m_bLineMode = FALSE;	//取消連線模式

	CBlockBasis* pBlk = new CBlockNot;																				//new一個CBlockNot的物件
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//設定Block Not的rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//設定Block編號
	m_vcBlkContainer.push_back (pBlk);																				//存入Block的vector中
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//紀錄Block的vector size

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下Function按鈕
void CTestSimulatorDlg::OnBnClickedButtonFunction ()
{
	m_bLineMode = FALSE;	//取消連線模式

	CBlockBasis* pBlk = new CBlockFun;																				//new一個CBlockFun的物件
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//設定Block Fun的rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//設定Block編號
	m_vcBlkContainer.push_back (pBlk);																				//存入Block的vector中
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//紀錄Block的vector size

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();	
}

//按下Line按鈕
void CTestSimulatorDlg::OnBnClickedButtonLine ()
{
	//設定連線模式並重繪工作區
	if (!m_bSimulate)
	{
		if (m_bLineMode)
			m_bLineMode = FALSE;
		else
		{
			if (!m_vcBlkContainer.empty ())
			{
				m_bLineMode = TRUE;
				InvalidateRect (m_rcWork, FALSE);
				UpdateWindow ();
			}			
		}			
	}
}

//按下Simulate按鈕
void CTestSimulatorDlg::OnBnClickedButtonSimulate ()
{
	if (!m_bSimulate)
	{
		if (m_bSelectBlk && m_iSlctBlkNum >= 0 && m_iSlctBlkNum < m_iContainerSize &&
			m_vcBlkContainer[m_iSlctBlkNum]->BlkTypeIs () == BLK_OUT && m_vcBlkContainer[m_iSlctBlkNum]->GetBlkHeadSize () > 0)
		{
			m_iSimBlkNum = m_iSlctBlkNum;	//設定模擬Block編號
			m_bSimulate = TRUE;				//設定模擬模式
			m_bLineMode = FALSE;			//取消連線模式
			m_bDelBlk = FALSE;				//禁止刪除Block

			//若示波器對話盒指標未有指定記憶體，則new一個示波器對話盒
			if (m_pOscDlg == NULL)
			{
				m_pOscDlg = new COscDlg (this);
				m_pOscDlg->Create (IDD_DIALOG_OSC, this);
				m_pOscDlg->SetWindowPos (this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
				m_pOscDlg->UpdateData (FALSE);
			}
			else
			{
				m_pOscDlg->ShowWindow (SW_SHOW);
				m_pOscDlg->UpdateData (FALSE);
			}

			//重繪工作區
			InvalidateRect (m_rcWork, FALSE);
			UpdateWindow ();
		}
		else 
			MessageBox (_T ("Please select output block and check if it is connected."));
	}	
}

//按下Grid按鈕
void CTestSimulatorDlg::OnBnClickedButtonGrid ()
{
	//設定格點模式
	if (!m_bGrid)
		m_bGrid = TRUE;
	else 
	{
		m_bGrid = FALSE;
		m_bLock = FALSE;
	}		

	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下Lock按鈕
void CTestSimulatorDlg::OnBnClickedButtonLock ()
{
	if (m_bGrid)
	{
		if (!m_bLock)
		{
			m_bLock = TRUE;	//設定鎖定模式

			//設定現有Block鎖定於最為鄰近的格點上
			for (int i = 0; i < m_iContainerSize; i++)
			{
				int GridX = (int) ((m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x - m_iWorkLeft) / (double) m_iGridX + 0.5);
				int GridY = (int) ((m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y - m_iWorkTop) / (double) m_iGridY + 0.5);
				m_vcBlkContainer[i]->SetBlkRect (CPoint (GridX * m_iGridX + m_iWorkLeft, GridY * m_iGridY + m_iWorkTop), m_iBlkWidth, m_iBlkHeight);
			}
		}		
		else 
			m_bLock = FALSE;
	}

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//按下背景圖片按鈕
void CTestSimulatorDlg::OnBnClickedButtonBgpic ()
{
	//建立CFileDialog (TRUE開啟檔案, 預設的副檔名, 初始檔案名, OFN_HIDEREADONLY: 單選, 篩選)
	CFileDialog fileDlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T (" bitmap (*.bmp)|*.bmp||"));

	//若File對話盒按下確定鍵
	if (fileDlg.DoModal () == IDOK)
	{
		m_strPicPath = fileDlg.GetPathName ();	//讀取檔案路徑

		//將讀取的.bmp指定給HBITMAP
		HBITMAP hbBgPic;
		hbBgPic = (HBITMAP)::LoadImage (AfxGetInstanceHandle (), m_strPicPath, IMAGE_BITMAP, m_rcWork.Width (), m_rcWork.Height (), LR_LOADFROMFILE);
		m_bitmapBgPic.Detach ();
		m_bitmapBgPic.Attach (hbBgPic);

		//僅於首次載入圖片時紀錄初始bitmap指標
		if (!m_bBgPic)
			m_pOldBitmap = m_dcBgPic.SelectObject (&m_bitmapBgPic);
		else
			m_dcBgPic.SelectObject (&m_bitmapBgPic);

		m_bBgPic = TRUE;

		//重繪工作區
		InvalidateRect (m_rcWork, FALSE);
		UpdateWindow ();
	}
}

//按下背景顏色按鈕
void CTestSimulatorDlg::OnBnClickedButtonBgcolor ()
{
	//建立色彩對話盒
	CColorDialog clrDlg (m_clrBg, CC_PREVENTFULLOPEN);

	//若色彩對話盒按下確定鍵
	if (clrDlg.DoModal () == IDOK)
	{
		if (m_bBgPic && m_pOldBitmap != NULL)
		{
			m_dcBgPic.SelectObject (m_pOldBitmap);
			m_bBgPic = FALSE;
		}

		m_clrBg = clrDlg.GetColor ();	//設定背景顏色

		//重繪工作區
		InvalidateRect (m_rcWork, FALSE);
		UpdateWindow ();
	}		
}

//按下Open按鈕
void CTestSimulatorDlg::OnBnClickedButtonOpen ()
{
	//模擬模式下不可進行讀檔
	if (!m_bSimulate)
	{
		//建立CFileDialog (TRUE開啟檔案, 預設的副檔名, 初始檔案名, OFN_HIDEREADONLY: 單選, 篩選)
		CFileDialog fileDlg (TRUE, _T ("txt"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T (" txt (*.txt)|*.txt||"));

		//建立CStdioFile以讀取檔案資料
		CStdioFile fileSim;

		//若檔案對話盒按下確定鍵
		if (fileDlg.DoModal () == IDOK)
		{
			//開啟檔案 (檔案路徑, CFile::modeNoTruncate: 不清空文件內容 | CFile::modeRead: 唯讀)
			fileSim.Open (fileDlg.GetPathName (), CFile::modeNoTruncate | CFile::modeRead);

			CString strData;		//儲存每一列字串資料
			CStringArray strArray;	//儲存每一Block各筆資料
		
			fileSim.ReadString (strData);	//讀取檔案資料進字串
			if (strData == "BlockData: ")	//資料開頭應為BlockDate
			{
				//讀取檔案時取消選取Block
				m_bSelectBlk = FALSE;		
				m_iSlctBlkNum = NO_SEL_NUM;	

				//清除示波器對話盒指標
				if (m_pOscDlg != NULL)
				{
					m_pOscDlg->DestroyWindow ();
					delete m_pOscDlg;
					m_pOscDlg = NULL;
				}			
				
				//清除存放Block的vector	
				for (int i = 0; i < m_iContainerSize; i++)
					delete m_vcBlkContainer[i];
				std::vector <CBlockBasis*> ().swap (m_vcBlkContainer);

				//每次由檔案讀取一行資料
				while (fileSim.ReadString (strData))
				{
					SplitString (strData, ',', strArray);	//將讀取到的字串分隔為Block各筆資料，並儲存至陣列中

					if (strArray[0] == "IN")		//Block In資料內容包含中心點位置, 波形flag
					{
						CBlockBasis* pBlk = new CBlockIn;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetInputFlag (_ttoi (strArray[3]));
						pBlk->SetBlkValue ();
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "OUT")	//Block Out資料內容包含中心點位置, 輸入channel數目, 輸入Block編號
					{
						CBlockBasis* pBlk = new CBlockOut;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						int iBlkHeadSize = _ttoi (strArray[3]);
						for (int i = 0; i < iBlkHeadSize; i++)
						{
							pBlk->SetBlkHead1Num (_ttoi (strArray[4 + i]));	
							pBlk->SetBlkHeadSize (FALSE);
						}
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "AND")	//Block And資料內容包含中心點位置, 輸入Block編號
					{
						CBlockBasis* pBlk = new CBlockAnd;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkHead2Num (_ttoi (strArray[4]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "OR")	//Block Or資料內容包含中心點位置, 輸入Block編號
					{
						CBlockBasis* pBlk = new CBlockOr;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkHead2Num (_ttoi (strArray[4]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "NOT")	//Block Not資料內容包含中心點位置, 輸入Block編號
					{
						CBlockBasis* pBlk = new CBlockNot;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "FUN")	//Block Fun資料內容包含中心點位置, 輸入Block編號, 運算元flag
					{
						CBlockBasis* pBlk = new CBlockFun;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkHead2Num (_ttoi (strArray[4]));
						pBlk->SetOperFlag (_ttoi (strArray[5]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
				}

				//資料讀取完成後，關閉檔案
				fileSim.Close ();

				//依讀取到的輸入Block編號，重新設定每一Block其連接Block的指標
				m_iContainerSize = (int) m_vcBlkContainer.size ();
				for (int i = 0; i < m_iContainerSize; i++)
				{
					if (!(m_vcBlkContainer[i]->BlkTypeIs () == BLK_OUT))
					{
						int iHeadBlk1Num = m_vcBlkContainer[i]->GetBlkHead1Num ();
						if (iHeadBlk1Num >= 0)
							m_vcBlkContainer[i]->SetBlkHead1Ptr (m_vcBlkContainer[iHeadBlk1Num]);
						int iHeadBlk2Num = m_vcBlkContainer[i]->GetBlkHead2Num ();
						if (iHeadBlk2Num >= 0)
							m_vcBlkContainer[i]->SetBlkHead2Ptr (m_vcBlkContainer[iHeadBlk2Num]);
					}
					else
					{
						int iBlkHeadSize = m_vcBlkContainer[i]->GetBlkHeadSize ();
						for (int j = 0; j < iBlkHeadSize; j++)
						{
							int iBlkHeadNum = m_vcBlkContainer[i]->GetBlkHead1Num (j);
							m_vcBlkContainer[i]->SetBlkHead1Ptr (m_vcBlkContainer[iBlkHeadNum]);
						}								
					}			
				}

				//重繪工作區
				InvalidateRect (m_rcWork, FALSE);
				UpdateWindow ();	
			}
			else 
				fileSim.Close ();
		}
	}	
}

//按下Save按鈕
void CTestSimulatorDlg::OnBnClickedButtonSave ()
{
	//建立CFileDialog (FALSE儲存檔案, 預設的副檔名, 初始檔案名, OFN_HIDEREADONLY: 單選, 篩選)
	CFileDialog fileDlg (FALSE, _T ("txt"), _T ("Simulator"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T (" txt (*.txt)|*.txt||"));
	
	//建立CStdioFile以儲存資料
	CStdioFile fileSim;

	//若檔案對話盒按下確定鍵
	if (fileDlg.DoModal () == IDOK)
	{
		// 開啟檔案 (檔案路徑, CFile::modeCreate: 以新建方式打開 | CFile::modeWrite: 只寫)
		fileSim.Open (fileDlg.GetPathName (), CFile::modeCreate | CFile::modeWrite);

		CString strData;	//用以寫入檔案的字串
		strData.Format (_T ("BlockData: \n"));
		fileSim.WriteString (strData);

		for (int i = 0; i < m_iContainerSize; i++)
		{
			int iBlkFlag = m_vcBlkContainer[i]->BlkTypeIs ();
			switch (iBlkFlag)
			{
			case BLK_IN:	//Block In需儲存內容包含中心點位置, 波形flag
				strData.Format (_T ("%3s, %3d, %3d, %2d\n"), "IN", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetInputFlag ());
				break;
			case BLK_OUT:	//Block Out需儲存內容包含中心點位置, 輸入channel數目, 輸入Block編號
				strData.Format (_T ("%3s, %3d, %3d, %1d"), "OUT", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y, 
								m_vcBlkContainer[i]->GetBlkHeadSize ());

				for (int j = 0; j < m_vcBlkContainer[i]->GetBlkHeadSize (); j++)
				{			
					CString strNum;
					strNum.Format (_T ("%1d"), m_vcBlkContainer[i]->GetBlkHead1Num (j));
					strData.Append (_T (", "));
					strData.Append (strNum);
				}
				strData.Append (_T ("\n"));
				break;
			case BLK_AND:	//Block And需儲存內容包含中心點位置, 輸入Block編號
				strData.Format (_T ("%3s, %3d, %3d, %1d, %1d\n"), "AND", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num (), m_vcBlkContainer[i]->GetBlkHead2Num ());
				break;
			case BLK_OR:	//Block Or需儲存內容包含中心點位置, 輸入Block編號
				strData.Format (_T ("%3s, %3d, %3d, %1d, %1d\n"), "OR", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num (), m_vcBlkContainer[i]->GetBlkHead2Num ());
				break;
			case BLK_NOT:	//Block Not需儲存內容包含中心點位置, 輸入Block編號
				strData.Format (_T ("%3s, %3d, %3d, %1d\n"), "NOT", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num ());
				break;
			case BLK_FUN:	//Block Fun需儲存內容包含中心點位置, 輸入Block編號, 運算元flag
				strData.Format (_T ("%3s, %3d, %3d, %1d, %1d, %2d\n"), "FUN", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num (), m_vcBlkContainer[i]->GetBlkHead2Num (), m_vcBlkContainer[i]->GetOperFlag ());
				break;
			default:
				break;
			}

			fileSim.WriteString (strData);	//將字串內容寫入檔案
		}

		fileSim.Close ();	//寫入完成後，關閉檔案
	}
}

//滑鼠左鍵按下
void CTestSimulatorDlg::OnLButtonDown (UINT nFlags, CPoint ptCursor)
{
	SetCapture ();
	
	m_bDelBlk = FALSE;	//滑鼠左鍵按下時禁止刪除Block

	//判斷鼠標是否位於Block rect範圍
	for (int i = m_iContainerSize - 1; i >= 0; i--)
	{
		if (m_vcBlkContainer[i]->GetBlkRect ().PtInRect (ptCursor))
		{
			SetCursor (LoadCursor (NULL, IDC_SIZEALL));
			m_iSlctBlkNum = i;		//選取Block編號
			m_bSelectBlk = TRUE;
			m_bMoveBlk = TRUE;
			m_bLineMode = FALSE;	//取消連線模式
			break;
		}
		else	//點選非Block位置時，取消選取標記
		{
			m_bSelectBlk = FALSE;
			m_iSlctBlkNum = NO_SEL_NUM;
		}			
	}

	//若有選取Block，立即重繪工作區
	if (m_bSelectBlk)
	{
		InvalidateRect (m_rcWork, FALSE);
		UpdateWindow ();
	}

	//連線模式時，判斷鼠標按下位置是否位於輸出Pin腳範圍
	if (m_bLineMode)
	{
		for (int i = 0; i < m_iContainerSize; i++)
		{
			if (m_vcBlkContainer[i]->GetPinOutRect ().PtInRect (ptCursor))
			{
				m_iHeadBlkNum = i;
				m_bConnect = TRUE;
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown (nFlags, ptCursor);
}

//滑鼠移動
void CTestSimulatorDlg::OnMouseMove (UINT nFlags, CPoint ptCursor)
{
	//移動選取Block
	if (m_bMoveBlk)
	{
		//鎖定模式下計算鼠標位置最鄰近之格點，將移動的Block中心設置於該格點位置
		if (m_bLock)
		{
			int iGridX = (int) ((ptCursor.x - m_iWorkLeft) / (double) m_iGridX + 0.5);
			int iGridY = (int) ((ptCursor.y - m_iWorkTop) / (double) m_iGridY + 0.5);

			m_vcBlkContainer[m_iSlctBlkNum]->Move (CPoint (iGridX * m_iGridX + m_iWorkLeft, iGridY * m_iGridY + m_iWorkTop));
		}
		else
			m_vcBlkContainer[m_iSlctBlkNum]->Move (ptCursor);
	}
		
	//連線模式
	if (m_bLineMode)
	{
		for (int i = 0; i < m_iContainerSize ; i++)
		{
			//當鼠標靠近Block輸出Pin腳時，設定鼠標樣式為十字箭頭
			if (m_vcBlkContainer[i]->GetPinOutRect ().PtInRect (ptCursor))
				SetCursor (LoadCursor (NULL, IDC_CROSS));

			//連線時，設定鼠標位置為虛線線段末端
			if (m_bConnect)
				m_ptLEnd = ptCursor;
		}
	}

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();

	CDialogEx::OnMouseMove (nFlags, ptCursor);
}

//滑鼠左鍵放開
void CTestSimulatorDlg::OnLButtonUp (UINT nFlags, CPoint ptCursor)
{
	//當有選取Block時，可刪除該Block
	if (m_bSelectBlk)
		m_bDelBlk = TRUE;

	if (m_bMoveBlk)
	{
		//移動Block狀態下，當滑鼠左鍵放開位置超出工作區，則設定Block跳回工作區中心
		if (!(m_rcWork.PtInRect (m_vcBlkContainer[m_iSlctBlkNum]->GetBlkRect ().TopLeft ()) && 
			  m_rcWork.PtInRect (m_vcBlkContainer[m_iSlctBlkNum]->GetBlkRect ().BottomRight ())))
			m_vcBlkContainer[m_iSlctBlkNum]->Move (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y));

		m_bMoveBlk = FALSE;	
	}
	
	//連線模式下
	if (m_bLineMode && m_bConnect)
	{
		//判斷滑鼠左鍵放開位置是否位於Block輸入Pin腳範圍，若是，則設定該Block連接起始Block的指標
		for (int i = 0; i < m_iContainerSize; i++)
		{
			if (m_vcBlkContainer[i]->GetPinIn1Rect ().PtInRect (ptCursor))
			{
				m_vcBlkContainer[i]->SetBlkHead1Ptr (m_vcBlkContainer[m_iHeadBlkNum]);
				m_vcBlkContainer[i]->SetBlkHead1Num (m_iHeadBlkNum);
				m_vcBlkContainer[i]->SetBlkHeadSize (FALSE);
				break;
			}

			if (m_vcBlkContainer[i]->GetPinIn2Rect ().PtInRect (ptCursor))
			{
				m_vcBlkContainer[i]->SetBlkHead2Ptr (m_vcBlkContainer[m_iHeadBlkNum]);
				m_vcBlkContainer[i]->SetBlkHead2Num (m_iHeadBlkNum);
				m_vcBlkContainer[i]->SetBlkHeadSize (FALSE);
				break;
			}		
		}

		m_bConnect = FALSE;	//滑鼠左鍵放開時，取消連線中線段
	}	

	ReleaseCapture ();

	//重繪工作區
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();

	CDialogEx::OnLButtonUp(nFlags, ptCursor);
}

//雙擊滑鼠左鍵
void CTestSimulatorDlg::OnLButtonDblClk (UINT nFlags, CPoint ptCursor)
{
	//模擬模式下不可更改Block flag
	if (!m_bSimulate)
	{
		for (int i = 0; i < m_iContainerSize; i++)
		{
			if (m_vcBlkContainer[i]->GetBlkRect ().PtInRect (ptCursor))
			{
				if (m_vcBlkContainer[i]->BlkTypeIs () == BLK_IN)		//更改Block In輸入波形flag
				{
					CInputDlg inputdlgSim;
					inputdlgSim.SetInputFlag (m_vcBlkContainer[i]->GetInputFlag ());

					if (inputdlgSim.DoModal () == IDOK)
					{
						m_vcBlkContainer[i]->SetInputFlag (inputdlgSim.GetInputFlag ());
						m_vcBlkContainer[i]->SetBlkValue ();
					}
						
					else
						MessageBox (_T ("The input has not been changed."));
				}
				else if (m_vcBlkContainer[i]->BlkTypeIs () == BLK_FUN)	//更改Block Fun運算元flag
				{
					CFunDlg fundlgSim;
					fundlgSim.SetOperFlag (m_vcBlkContainer[i]->GetOperFlag ());

					if (fundlgSim.DoModal () == IDOK)
						m_vcBlkContainer[i]->SetOperFlag (fundlgSim.GetOperFlag ());
					else
						MessageBox (_T ("The operator has not been changed."));
				}

				break;
			}
		}
	}	

	CDialogEx::OnLButtonDblClk (nFlags, ptCursor);
}

void CTestSimulatorDlg::OnTimer (UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
		KillTimer (0);
	
	int iTimeInterval = m_pOscDlg->GetTimeInterval ();	//取得使用者設定的時間間隔(單位ms)
	double dTime = m_iStep * iTimeInterval * MSEC;		//模擬進行總時間(單位s)

	std::deque <double> deqRowValue;	//用以儲存每一個時間點下各input channel的值

	//模擬前判斷input是否合理(僅判斷一次)
	if (m_iStep == 0)
	{
		m_iOutSize = m_vcBlkContainer[m_iSimBlkNum]->GetBlkHeadSize ();	//讀取channel數目

		for (int i = 0; i < m_iOutSize; i++)
		{
			int iValueFlag = m_vcBlkContainer[m_iSimBlkNum]->GetValueFlag (i);
			
			if (iValueFlag == NO_VALUE)				//不存在input signal
			{
				MessageBox (_T ("Please check if the output block exists input signal."));
				m_pOscDlg->m_bSim = FALSE;
				m_pOscDlg->m_bModify = TRUE;
				break;
			}
			else if (iValueFlag == COMPUTE_ERROR)	//存在不允許的運算方式
			{
				MessageBox (_T ("Operation is not allowed."));
				m_pOscDlg->m_bSim = FALSE;
				m_pOscDlg->m_bModify = TRUE;
				break;
			}
			else if (iValueFlag == NO_OPER_FLAG)	//Block Fun未設定運算元
			{
				MessageBox (_T ("Please check if the operator in function block has been set up."));
				m_pOscDlg->m_bSim = FALSE;
				m_pOscDlg->m_bModify = TRUE;
				break;
			}
			else
				m_pOscDlg->m_bSim = TRUE;	
		}	
	}	

	//當input為合理運算，模擬開始
	if (m_pOscDlg->m_bSim)
	{	
		if (m_iStep == 0)
		{
			m_pOscDlg->SetOutputSize (m_iOutSize);	//設定示波器對話盒的channel數
			m_pOscDlg->InitialData ();				//初始化示波器對話盒資料(channel數目, 隨機畫筆)
			m_deq2DOutputValue.clear ();			//開始模擬前清空二維矩陣
		}	

		//設定該時間下，每一個Block In的值
		for (int i = 0; i < m_iContainerSize; i++)
		{
			if (m_vcBlkContainer[i]->BlkTypeIs () == BLK_IN)
			{
				if (m_vcBlkContainer[i]->GetInputFlag () == SIN)
					m_vcBlkContainer[i]->SetBlkValue (dTime);
				else if (m_vcBlkContainer[i]->GetInputFlag () == COS)
					m_vcBlkContainer[i]->SetBlkValue (dTime);
			}
		}

		//取得選取模擬Block Out的值(有多個channel)
		for (int i = 0; i < m_iOutSize; i++)	
		{
			double dValue =  m_vcBlkContainer[m_iSimBlkNum]->GetBlkValue (i);
			deqRowValue.push_back (dValue);	//存入列向量(紀錄該時間點下每一個input channel的值)
		}		

		int iDataSize = (int) m_deq2DOutputValue.size ();	//讀取總data數目

		//若總data數目超過預設最大data數，刪除第一個data，並由末端加入新data
		if (iDataSize < MAX_DATA_NUM)
			m_deq2DOutputValue.push_back (deqRowValue);
		else
		{
			m_deq2DOutputValue.pop_front ();
			m_deq2DOutputValue.push_back (deqRowValue); 
		}

		//FFT求即時頻率
		double dOutFreq = 0.;
		if (iDataSize == MAX_DATA_NUM /*&& m_pOscDlg->m_bFFT*/)
		{
			std::deque <double> deqOutputData;
			for (int i = 0; i < MAX_DATA_NUM; i++)
			{
				double dValue = m_deq2DOutputValue[i][m_pOscDlg->m_iSlctOutputNum];
				deqOutputData.push_back (dValue);
			}			

			dOutFreq = FFT (deqOutputData, MAX_DATA_NUM, iTimeInterval * MSEC);
		}

		//傳值至示波器對話盒中(當前時間, 選取output value, 頻率)
		m_pOscDlg->SetData(dTime, deqRowValue[m_pOscDlg->m_iSlctOutputNum], dOutFreq);
		//m_pOscDlg->SetData (dTime, deqRowValue[m_pOscDlg->m_iSlctOutputNum]);

		m_iStep++;

		//更新示波器對話盒
		m_pOscDlg->UpdateData (FALSE);
		m_pOscDlg->InvalidateRect (m_pOscDlg->m_rcWork, FALSE);
		m_pOscDlg->UpdateWindow ();
	}	

	CDialogEx::OnTimer (nIDEvent);

	if (m_pOscDlg->m_bSim && nIDEvent == 0)
		SetTimer (0, 10, NULL);
}

//主對話盒視窗大小改變
void CTestSimulatorDlg::OnSize (UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//剛生成主對話盒時不進入
	if (m_bInitDlg)
	{	
		//判斷是否最小化
		if (!IsIconic ())
		{
			//取得主對話盒客戶區
			CRect rcCurClient;
			this->GetClientRect (&rcCurClient);

			//計算x, y方向縮放比例
			double dMultipleWid = rcCurClient.Width () / (double) m_rcClient.Width ();
			m_rcClient.right = rcCurClient.Width ();
			double dMultipleHeight = rcCurClient.Height () / (double) m_rcClient.Height ();
			m_rcClient.bottom = rcCurClient.Height ();

			//設定更新的工作區域
			m_iWorkLeft = (int) (dMultipleWid * m_iWorkLeft + 0.5);
			m_iWorkTop = (int) (dMultipleHeight * m_iWorkTop + 0.5);
			m_iWorkRight = (int) (dMultipleWid * m_iWorkRight + 0.5);
			m_iWorkBottom = (int) (dMultipleHeight * m_iWorkBottom + 0.5);
			m_rcWork.SetRect (m_iWorkLeft, m_iWorkTop, m_iWorkRight, m_iWorkBottom);

			//設定更新的控件位置與大小
			Resize (IDC_BUTTON_IN , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_OUT , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_AND , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_OR , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_NOT , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_FUNCTION , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_LINE , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_SIMULATE , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_GRID , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_LOCK , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_BGPIC , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_BGCOLOR , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_OPEN , dMultipleWid, dMultipleHeight);
			Resize (IDC_BUTTON_SAVE , dMultipleWid, dMultipleHeight);

			//重新載入背景圖片
			if (m_bBgPic)
			{
				HBITMAP hbBgPic;
				hbBgPic = (HBITMAP)::LoadImage (AfxGetInstanceHandle (), m_strPicPath, IMAGE_BITMAP, m_rcWork.Width (), m_rcWork.Height (), LR_LOADFROMFILE);
				m_bitmapBgPic.Detach ();
				m_bitmapBgPic.Attach (hbBgPic);
				m_dcBgPic.SelectObject (&m_bitmapBgPic);
			}
			
			//設定更新的Block位置與大小
			m_iBlkWidth = (int) (m_iBlkWidth * dMultipleWid + 0.5);
			m_iBlkHeight = (int) (m_iBlkHeight * dMultipleHeight + 0.5);
			m_iGridX = (int) (m_iBlkWidth / 4. + 0.5);
			m_iGridY = (int) (m_iBlkHeight / 2. + 0.5);
			for (int i = 0; i < m_iContainerSize; i++)
				Resize (m_vcBlkContainer[i], dMultipleWid, dMultipleHeight);
			
			//重繪工作區
			Invalidate (TRUE);
			UpdateWindow ();	

			//若存在示波器對話盒，則顯示視窗
			if (m_bSimulate && m_pOscDlg != NULL)
				m_pOscDlg->ShowWindow (SW_SHOW);
		}
	}	
}

BOOL CTestSimulatorDlg::PreTranslateMessage (MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//按下刪除鍵
		if (pMsg->wParam == VK_DELETE && !m_vcBlkContainer.empty () && m_bDelBlk && !m_bSimulate)
		{
			//判斷其他Block是否有連接至該刪除的Block，若有，則清除連接指標
			for (int i = 0; i < m_iContainerSize; i++)
			{
				if (!(m_vcBlkContainer[i]->BlkTypeIs () == BLK_OUT))
				{
					if (m_vcBlkContainer[i]->GetBlkHead1Ptr () == m_vcBlkContainer[m_iSlctBlkNum])
					{
						m_vcBlkContainer[i]->SetBlkHead1Ptr (NULL);
						m_vcBlkContainer[i]->SetBlkHead1Num (NO_BLOCK_HEAD);
					}		

					if (m_vcBlkContainer[i]->GetBlkHead2Ptr () == m_vcBlkContainer[m_iSlctBlkNum])
					{
						m_vcBlkContainer[i]->SetBlkHead2Ptr (NULL);
						m_vcBlkContainer[i]->SetBlkHead2Num (NO_BLOCK_HEAD);
					}
				}
				else
				{
					int iBlkHeadSize = m_vcBlkContainer[i]->GetBlkHeadSize ();
					for (int j = 0; j < iBlkHeadSize; j++)
					{
						if (m_vcBlkContainer[i]->GetBlkHead1Ptr (j) == m_vcBlkContainer[m_iSlctBlkNum])
						{
							m_vcBlkContainer[i]->DeleteBlkHead1Ptr (j);
							m_vcBlkContainer[i]->SetBlkHeadSize (TRUE);
						}
					}
				}					
			}

			//清除選取Block
			delete m_vcBlkContainer[m_iSlctBlkNum];

			m_vcBlkContainer.erase (m_vcBlkContainer.begin () + m_iSlctBlkNum);
			m_iContainerSize = (int) m_vcBlkContainer.size ();

			//重新編號
			for (int i = 0; i < m_iContainerSize; i++)
				m_vcBlkContainer[i]->SetBlkNum (i);

			//重新設定連接編號
			for (int i = 0; i < m_iContainerSize; i++)
			{
				if (!(m_vcBlkContainer[i]->BlkTypeIs () == BLK_OUT))
				{
					if (m_vcBlkContainer[i]->GetBlkHead1Ptr () != NULL)
					{
						int iBlkHead1Num = m_vcBlkContainer[i]->GetBlkHead1Ptr ()->GetBlkNum ();
						m_vcBlkContainer[i]->SetBlkHead1Num (iBlkHead1Num);
					}

					if (m_vcBlkContainer[i]->GetBlkHead2Ptr () != NULL)
					{
						int iBlkHead2Num = m_vcBlkContainer[i]->GetBlkHead2Ptr ()->GetBlkNum ();
						m_vcBlkContainer[i]->SetBlkHead2Num (iBlkHead2Num);
					}		
				}
				else
				{
					int iBlkHeadSize = m_vcBlkContainer[i]->GetBlkHeadSize ();
					m_vcBlkContainer[i]->ClearBlkHead1Num ();
					for (int j = 0; j < iBlkHeadSize; j++)
					{
						int iBlkHeadNum = m_vcBlkContainer[i]->GetBlkHead1Ptr (j)->GetBlkNum ();
						m_vcBlkContainer[i]->SetBlkHead1Num (iBlkHeadNum);
					}
				}
			}

			//刪除後，取消選取
			m_bSelectBlk = FALSE;
			m_iSlctBlkNum = NO_SEL_NUM;
			m_bDelBlk = FALSE;

			//重繪工作區
			InvalidateRect (m_rcWork, FALSE);
			UpdateWindow ();
		}
		//按下ESC鍵
		else if (pMsg->wParam == VK_ESCAPE)
		{
			m_bSelectBlk = FALSE;
			m_iSlctBlkNum = NO_SEL_NUM;
			m_bMoveBlk = FALSE;
			m_bLineMode = FALSE;
			m_bConnect = FALSE;
			m_bDelBlk = FALSE;

			//重繪工作區
			InvalidateRect (m_rcWork, FALSE);
			UpdateWindow ();

			return TRUE;
		}
		//按下ENTER鍵
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestSimulatorDlg::OnDestroy ()
{
	CString strIniPath (_T (".\\Initial.ini"));	//.ini路徑
	CString strAppName (_T ("BgParameter"));	//.ini中[節名]
	CString strColorRef;						//顏色變數
	strColorRef.Format (_T ("%u"), m_clrBg);	//將顏色值轉為字串

	//紀錄背景相關資訊
	CString strKeyName (_T ("BgPicBOOL"));		//.ini中變數名稱
	if (m_bBgPic)
	{
		WritePrivateProfileString (strAppName, strKeyName, _T ("TRUE"), strIniPath);	//載入背景圖片
		strKeyName.Format (_T ("BgPicPath"));
		WritePrivateProfileString (strAppName, strKeyName, m_strPicPath, strIniPath);	//背景圖片路徑
		strKeyName.Format (_T ("BgColor"));
		WritePrivateProfileString (strAppName, strKeyName, _T ("None"), strIniPath);	//背景顏色為None
	}
	else
	{
		WritePrivateProfileString (strAppName, strKeyName, _T ("FALSE"), strIniPath);	//不載入背景圖片
		strKeyName.Format (_T ("BgPicPath"));
		WritePrivateProfileString (strAppName, strKeyName, _T ("None"), strIniPath);	//背景圖片路徑為None
		strKeyName.Format (_T ("BgColor"));
		WritePrivateProfileString (strAppName, strKeyName, strColorRef, strIniPath);	//紀錄背景顏色值
	}

	//紀錄格點開啟與否
	strKeyName.Format (_T ("GridBOOL"));
	if (m_bGrid)
		WritePrivateProfileString (strAppName, strKeyName, _T ("TRUE"), strIniPath);
	else
		WritePrivateProfileString (strAppName, strKeyName, _T ("FALSE"), strIniPath);

	//紀錄鎖定開啟與否
	strKeyName.Format (_T ("LockBOOL"));
	if (m_bLock)
		WritePrivateProfileString (strAppName, strKeyName, _T ("TRUE"), strIniPath);
	else
		WritePrivateProfileString (strAppName, strKeyName, _T ("FALSE"), strIniPath);

	//清除存放背景圖片的dc
	m_dcBgPic.SelectObject (m_pOldBitmap);
	m_dcBgPic.DeleteDC ();

	//清除示波器對話盒指標
	if (m_pOscDlg != NULL)
	{
		m_pOscDlg->DestroyWindow ();
		delete m_pOscDlg;
	}
			
	//清除Block vector
	for (int i = 0; i < m_iContainerSize; i++)
		delete m_vcBlkContainer[i];

	CDialogEx::OnDestroy();
}

//將一列字串依據分隔符號分割為獨立字串
void CTestSimulatorDlg::SplitString (CString str, char cSplit, CStringArray& strarray)
{
	strarray.RemoveAll ();
	CString strTemp = str;	

	while (1)
	{
		int iIndex = strTemp.Find (cSplit);

		if (iIndex >= 0)
		{
			CString strLeft = strTemp.Left (iIndex);
			strLeft.Remove (' ');
			strarray.Add (strLeft);
			strTemp = strTemp.Right (strTemp.GetLength () - 1 - iIndex);
		}	
		else
			break;
	}

	strTemp.Remove (' ');
	strarray.Add (strTemp);
}

//重新設定控件位置及大小
void CTestSimulatorDlg::Resize (int nID, double dMulWid, double dMulHei)
{
	CRect rcItem;
	GetDlgItem (nID)->GetWindowRect (&rcItem);
	ScreenToClient (&rcItem);
	rcItem.TopLeft ().x = (int) (rcItem.TopLeft ().x * dMulWid + 0.5);
	rcItem.TopLeft ().y = (int) (rcItem.TopLeft ().y * dMulHei + 0.5);
	rcItem.BottomRight ().x = (int) (rcItem.BottomRight ().x * dMulWid + 0.5);
	rcItem.BottomRight ().y = (int) (rcItem.BottomRight ().y * dMulHei + 0.5);

	GetDlgItem (nID)->MoveWindow (rcItem);
}

//重新設定Block位置及大小
void CTestSimulatorDlg::Resize (CBlockBasis* pBlk, double dMulWid, double dMulHei)
{
	CRect rcItem = pBlk->GetBlkRect ();

	rcItem.TopLeft ().x = (int) (rcItem.TopLeft ().x * dMulWid + 0.5);
	rcItem.TopLeft ().y = (int) (rcItem.TopLeft ().y * dMulHei + 0.5);
	rcItem.BottomRight ().x = (int) (rcItem.BottomRight ().x * dMulWid + 0.5);
	rcItem.BottomRight ().y = (int) (rcItem.BottomRight ().y * dMulHei + 0.5);

	pBlk->SetBlkRect (rcItem.CenterPoint (), m_iBlkWidth, m_iBlkHeight);
}

//計算data層數(應用於FFT)
int CTestSimulatorDlg::GetComputationLayers(int iDataSize)
{
	int m_iLayers = 1;

	if (iDataSize == 2)
		return 1;
	while (1)
	{
		iDataSize = iDataSize / 2;
		m_iLayers++;

		if (iDataSize == 2)
			return m_iLayers;
		if (iDataSize <= 1)
			return -1;
	}
}

//FFT求即時頻率
double CTestSimulatorDlg::FFT (const std::deque <double> & deq2dData, int iSize, double dUnitTime)
{
	std::deque <CComplex> deqFData;
	std::deque <CComplex> deqWeights;
	std::deque <CComplex> deqX;
	std::deque <double> deqAmplitude;

	int iDeqLen = iSize;
	int iLayers = GetComputationLayers (iSize);

	for (int i = 0; i < iDeqLen; i++)
		deqFData.push_back (CComplex (deq2dData[i], 0));

	double dFixedFactors = (-2 * M_PI) / iDeqLen;

	//計算權重序列
	for (int i = 0; i < iDeqLen / 2; i++)
	{
		double dAngle = i * dFixedFactors;
		deqWeights.push_back (CComplex (cos (dAngle), sin (dAngle)));
	}

	for (int i = 0; i < iDeqLen / 2; i++)
		deqWeights.push_back (CComplex (-deqWeights[i].m_dReal, -deqWeights[i].m_dImag));

	//計算倒序位碼
	for (int i = 0; i < iDeqLen; i++)
	{
		int iIndex = 0;
		for (int j = iLayers - 1; j >= 0; j--)
			iIndex += (1 && (i & (1 << j))) << (iLayers - j - 1);

		deqX.push_back (deqFData[iIndex]);
	}

	//計算FFT
	for (int L = 1; L <= iLayers; L++)
	{
		int iDistance = 1 << (L - 1);
		int iW = 1 << (iLayers - L);	//總資料長度被分解的份數

		int iB = iDeqLen >> L;			//需計算幾次資料長度為iN的傅立葉轉換
		int iN = iDeqLen / iB;			//需計算傅立葉轉換的資料長度

		for (int b = 0; b < iB; b++)
		{
			int iMid = b * iN;
			for (int n = 0; n < iN / 2; n++)
			{
				int iIndex = n + iMid;
				deqFData[iIndex] = deqX[iIndex] + deqWeights[n * iW] * deqX[iIndex + iDistance];	//Fe + W*Fo
			}

			for (int n = iN / 2; n < iN; n++)
			{
				int iIndex = n + iMid;
				deqFData[iIndex] = deqX[iIndex - iDistance] + deqWeights[n * iW] * deqX[iIndex];	//Fe - W*Fo 
			}
		}

		deqX = deqFData;
	}

	//計算振幅
	for (int i = 0; i < iDeqLen; i++)
		deqAmplitude.push_back (deqFData[i].ComputeAmplitude ());

	double dMaxAmp = 0.;					//宣告最大振幅變數
	double dCornerFreq = 1 / dUnitTime / 2;	//計算截止頻率
	double dOutFreq = 0.;					//紀錄最大振福出現之頻率

	//紀錄頻域中出現最大振幅的位置，其對應頻率即為輸出頻率
	for (int i = 0; i < iDeqLen; i++)
	{
		double dFreq = i / (dUnitTime * (iDeqLen - 1));
		if (dFreq > dCornerFreq)
			break;

		if (deqAmplitude[i] > dMaxAmp)
		{
			dMaxAmp = deqAmplitude[i];
			dOutFreq = dFreq;
		}
	}

	return dOutFreq;
}