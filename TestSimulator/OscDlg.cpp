
// OscDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "TestSimulator.h"
#include "OscDlg.h"
#include "afxdialogex.h"

#include "TestSimulatorDlg.h"

#include <cstdlib>
#include <ctime> 

//#include <iostream>
//using namespace std;

//工作區域參數
#define WORK_LEFT	0
#define WORK_TOP	0
#define WORK_RIGHT	500
#define WORK_BOTTOM	500

#define BLACK RGB (0, 0, 0)			//背景顏色
#define BLUE RGB (0, 0, 255)		//網格顏色
#define WHITE RGB (255, 255, 255)	//y = 0中線顏色
#define YELLOW RGB (255, 255, 0)	//距離中線一個單位的線條顏色
#define RED RGB (255, 0, 0)			//距離中線兩個單位的線條顏色
#define GOLD RGB (255, 215, 0)		//選取output波形顏色

#define GRID 10
#define UNITY 100

#define MAX_PLOT_NUM 501			//最大繪製data數
#define OFFSET_Y 250				//對話盒座標左上角為原點，為了使繪圖繪製於示波器中央，故繪圖時需增加一個偏移量使(0, 0)位於示波圖左中點


// COscDlg 對話方塊

COscDlg::COscDlg (CWnd* pParent /*=NULL*/)
	: CDialogEx(COscDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);

	m_pSimulatorDlg = (CTestSimulatorDlg*) pParent;

	m_rcWork.SetRect (WORK_LEFT, WORK_TOP, WORK_RIGHT, WORK_BOTTOM);

	m_dTime = 0.;
	m_dSlctOutputValue = 0.;
	m_dMaxDataValue = 0.;
	m_dFreq = 0.;

	m_iOutputSize = 0;
	m_iSlctOutputNum = 0;
	m_dYInterval = 0;
	m_iXInterval = 0;
	m_iYPos = 0;
	m_iXPos = 0;

	m_bSim = FALSE;
	m_bModify = TRUE;
	m_bFFT = TRUE;
}

COscDlg::~COscDlg ()
{
	if (!m_deqPenPtr.empty ())
	{
		int iSize = (int) m_deqPenPtr.size ();
		for (int i = 0; i < iSize; i++)
			delete m_deqPenPtr[i];
	}
}

void COscDlg::DoDataExchange (CDataExchange* pDX)
{
	CDialogEx::DoDataExchange (pDX);
	DDX_Text (pDX, IDC_EDIT_TIME, m_dTime);
	DDX_Text (pDX, IDC_EDIT_VALUE, m_dSlctOutputValue);
	DDX_Control (pDX, IDC_COMBO_OUTPUT, m_ctrlComboOutput);
	DDX_Control (pDX, IDC_COMBO_VALUE_INTERVAL, m_ctrlComboYInterval);
	DDX_Control (pDX, IDC_COMBO_TIME_INTERVAL, m_ctrlComboXInterval);
	DDX_Text (pDX, IDC_EDIT_FREQUENCY, m_dFreq);
}


BEGIN_MESSAGE_MAP (COscDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_START, &COscDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &COscDlg::OnBnClickedButtonStop)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT, &COscDlg::OnCbnSelchangeComboOutput)
	ON_CBN_SELCHANGE(IDC_COMBO_VALUE_INTERVAL, &COscDlg::OnCbnSelchangeComboValueInterval)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_INTERVAL, &COscDlg::OnCbnSelchangeComboTimeInterval)
END_MESSAGE_MAP ()


// COscDlg 訊息處理常式

BOOL COscDlg::OnInitDialog ()
{
	CDialogEx::OnInitDialog();

	//設定亂數種子
	srand ((unsigned int) time (NULL));

	//設定Y軸間隔Combo box內容
	for (int i = 0; i < 4; i++)
	{
		CString strYInterval;
		double dInterval = 0.5;
		for (int j = 0 ; j < i; j++)
			dInterval *= 2;
		strYInterval.Format (_T ("%g"), dInterval);
		m_ctrlComboYInterval.AddString (strYInterval);
	}

	//初始化Y軸間隔
	m_ctrlComboYInterval.SetCurSel (0);
	m_dYInterval = 0.5;
	m_iYPos = 0;
	m_dMaxDataValue = m_dYInterval * 2.5;


	//設定X軸間隔Combo box內容
	m_ctrlComboXInterval.AddString (_T ("5"));
	m_ctrlComboXInterval.AddString (_T ("10"));
	m_ctrlComboXInterval.AddString (_T ("50"));
	m_ctrlComboXInterval.AddString (_T ("100"));
	m_ctrlComboXInterval.AddString (_T ("500"));

	//初始化X軸間隔
	m_ctrlComboXInterval.SetCurSel (0);
	m_iXInterval = 5;
	m_iXPos = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}


void COscDlg::OnPaint ()
{
	//雙緩衝建立
	CPaintDC dcOsc (this);				//示波器對話盒dc
	CDC dcMem;							//內存dc
	CBitmap bitmapMem;
	CBitmap* pOldBitmap;

	dcMem.CreateCompatibleDC (&dcOsc);	//建立與示波器對話盒相容的dc
	bitmapMem.CreateCompatibleBitmap (&dcOsc, WORK_RIGHT, WORK_BOTTOM);
	pOldBitmap = dcMem.SelectObject (&bitmapMem);

	//繪製背景
	dcMem.SetBkMode (TRANSPARENT);
	dcMem.FillSolidRect (m_rcWork, BLACK);

	//藍色網格，每格邊長為 10 pixels
	CPen penBlue (PS_SOLID, 1, BLUE);
	CPen* pOldPen = dcMem.SelectObject (&penBlue);

	for (int i = 0; i < m_rcWork.Width (); i += GRID)
	{
		dcMem.MoveTo (i, 0);					//垂直線
		dcMem.LineTo (i, m_rcWork.Height ());
		dcMem.MoveTo (0, i);					//水平線
		dcMem.LineTo (m_rcWork.Width (), i);
	}

	//繪製示波器中線(X軸)
	CPen penWhite (PS_SOLID, 2, WHITE);
	CString strValueZero ("Value = 0");
	dcMem.SelectObject (&penWhite);
	SetTextColor (dcMem, WHITE);

	dcMem.MoveTo (0, m_rcWork.Height () / 2);
	dcMem.LineTo (m_rcWork.Width (), m_rcWork.Height () / 2);								//畫線
	dcMem.TextOutA (0, m_rcWork.Height () / 2, strValueZero, strValueZero.GetLength ());	//標記文字


	//距離中線一個UNITY (一個UNITY為 100 piexls)
	CPen penYellow (PS_SOLID, 1, YELLOW);
	CString strValueNOne;
	strValueNOne.Format (_T ("Value = %g"), -m_dYInterval);
	CString strValuePOne;
	strValuePOne.Format (_T ("Value = %g"), m_dYInterval);
	dcMem.SelectObject (&penYellow);
	SetTextColor (dcMem, YELLOW);

	dcMem.MoveTo (0, m_rcWork.Height () / 2 + UNITY);
	dcMem.LineTo (m_rcWork.Width (), m_rcWork.Height () / 2 + UNITY);
	dcMem.TextOutA (0, m_rcWork.Height () / 2 + UNITY, strValueNOne, strValueNOne.GetLength ());

	dcMem.MoveTo (0, m_rcWork.Height () / 2 - UNITY);
	dcMem.LineTo (m_rcWork.Width (), m_rcWork.Height () / 2 - UNITY);
	dcMem.TextOutA (0, m_rcWork.Height () / 2 - UNITY, strValuePOne, strValuePOne.GetLength ());


	//距離中線兩個UNITY (一個UNITY為 100 piexls)
	CPen penRed (PS_SOLID, 2, RED);
	CString strValueNTwo;
	strValueNTwo.Format (_T ("Value = %g"), -2 * m_dYInterval);
	CString strValuePTwo;
	strValuePTwo.Format (_T ("Value = %g"), 2 * m_dYInterval);
	dcMem.SelectObject (&penRed);
	SetTextColor (dcMem, RED);

	dcMem.MoveTo (0, m_rcWork.Height () / 2 + (2 * UNITY));
	dcMem.LineTo (m_rcWork.Width (), m_rcWork.Height () / 2 + (2 * UNITY));
	dcMem.TextOutA (0, m_rcWork.Height () / 2 + (2 * UNITY), strValueNTwo, strValueNTwo.GetLength ());

	dcMem.MoveTo (0, m_rcWork.Height () / 2 - (2 * UNITY));
	dcMem.LineTo (m_rcWork.Width (), m_rcWork.Height () / 2 - (2 * UNITY));
	dcMem.TextOutA (0, m_rcWork.Height () / 2 - (2 * UNITY), strValuePTwo, strValuePTwo.GetLength ());


	//模擬開始
	if (m_bSim)
	{
		CPen penGold (PS_SOLID, 6, GOLD);

		int iDataSize = (int) m_pSimulatorDlg->m_deq2DOutputValue.size ();	//取得data數目

		//當data數大於1，開始繪製output波形
		if (iDataSize > 1)
		{
			int iPlotStart = 0;	//繪製data起點
			if (iDataSize > MAX_PLOT_NUM)
				iPlotStart = iDataSize - MAX_PLOT_NUM;

			for (int i = 0; i < m_iOutputSize; i++)
			{
				//設定選取output的畫筆為金色較粗的畫筆
				if (m_iSlctOutputNum >= 0 && i == m_iSlctOutputNum)
					dcMem.SelectObject (&penGold);
				else	
					dcMem.SelectObject (m_deqPenPtr[i]);

				for (int j = 1; j < iDataSize; j++)
				{
					//超出可繪製data數，則立即跳出迴圈
					if (j >= MAX_PLOT_NUM)
						break;

					double dForwardValue = m_pSimulatorDlg->m_deq2DOutputValue[j - 1 + iPlotStart][i];
					double dPostValue = m_pSimulatorDlg->m_deq2DOutputValue[j + iPlotStart][i];

					//判斷data值是否超出示波器可顯示之最大值
					if (dForwardValue > m_dMaxDataValue)
						dForwardValue = m_dMaxDataValue;
					else if (dForwardValue < -m_dMaxDataValue)
						dForwardValue = -m_dMaxDataValue;

					if (dPostValue > m_dMaxDataValue)
						dPostValue = m_dMaxDataValue;
					else if (dPostValue < -m_dMaxDataValue)
						dPostValue = -m_dMaxDataValue;

					dcMem.MoveTo (j - 1, -(int) (dForwardValue / m_dYInterval * UNITY) + OFFSET_Y);
					dcMem.LineTo (j, -(int) (dPostValue / m_dYInterval * UNITY) + OFFSET_Y);
				}			
			}
		}				
	}

	//由內存dc拷貝至示波器對話盒dc
	dcOsc.BitBlt (WORK_LEFT, WORK_TOP, m_rcWork.Width (), m_rcWork.Height (), &dcMem, WORK_LEFT, WORK_TOP, SRCCOPY);

	//清理
	dcMem.SelectObject (pOldPen);
	dcMem.SelectObject (pOldBitmap);
	bitmapMem.DeleteObject ();
	dcMem.DeleteDC ();
}

//由主對話盒傳值(當前時間, 選取output value, 頻率)
void COscDlg::SetData (double dTime, double dValue, double dFreq)
{
	m_dTime = dTime;
	m_dSlctOutputValue = dValue;
	m_dFreq = dFreq;
}

// 由主對話盒傳值(當前時間, 選取output value)
void COscDlg::SetData (double dTime, double dValue)
{
	m_dTime = dTime;
	m_dSlctOutputValue = dValue;
}

//按下Start鍵
void COscDlg::OnBnClickedButtonStart ()
{
	if (!m_bSim)
	{
		m_bModify = FALSE;	//開始模擬不可更改X軸間隔Combo box
		m_bSim = TRUE;
		m_pSimulatorDlg->SetTimer (0, 10, NULL);
	}
}

//按下Stop鍵
void COscDlg::OnBnClickedButtonStop ()
{
	if (m_bSim)
	{
		m_bSim = FALSE;
		m_pSimulatorDlg->KillTimer (0);
	}
}

//改變output combo box
void COscDlg::OnCbnSelchangeComboOutput ()
{
	m_iSlctOutputNum = m_ctrlComboOutput.GetCurSel ();
	m_bFFT = TRUE;
}

//改變Y軸間隔 combo box
void COscDlg::OnCbnSelchangeComboValueInterval ()
{
	m_iYPos = m_ctrlComboYInterval.GetCurSel ();
	double dIni = 0.5;
	for (int i = 0; i < m_iYPos; i++)
		dIni *= 2;
	m_dYInterval = dIni;
	m_dMaxDataValue = 2.5 * m_dYInterval;

	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//改變X軸間隔 combo box
void COscDlg::OnCbnSelchangeComboTimeInterval ()
{
	if (m_bModify && (!m_bSim))
	{
		m_iXPos = m_ctrlComboXInterval.GetCurSel ();
		switch (m_iXPos)
		{
		case 0:
			m_iXInterval = 5;
			break;
		case 1:
			m_iXInterval = 10;
			break;
		case 2:
			m_iXInterval = 50;
			break;
		case 3:
			m_iXInterval = 100;
			break;
		case 4:
			m_iXInterval = 500;
			break;
		default:
			break;
		}
	}
	else
		m_ctrlComboXInterval.SetCurSel (m_iXPos);
}

BOOL COscDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//關閉視窗
void COscDlg::OnClose ()
{
	//清除隨機畫筆deque
	if (!m_deqPenPtr.empty ())
	{
		for (int i = 0; i < m_iOutputSize; i++)
			delete m_deqPenPtr[i];
	}
	
	m_pSimulatorDlg->m_bSimulate = FALSE;	//停止模擬
	m_deqPenPtr.clear ();
	m_ctrlComboOutput.ResetContent ();		//清空output combo box內容
	m_bSim = FALSE;							//停止模擬
	m_bModify = TRUE;						//停止模擬後可更改combo box
	m_bFFT = TRUE;
	m_iSlctOutputNum = 0;
	m_dTime = 0.;
	m_dSlctOutputValue = 0.;
	m_dFreq = 0.;
	m_pSimulatorDlg->m_iStep = 0;
	m_pSimulatorDlg->KillTimer (0);

	CDialogEx::OnClose ();
}

//設定channel數目
void COscDlg::SetOutputSize (int iOutSize)
{
	m_iOutputSize = iOutSize;
}

//初始化示波器對話盒資料(channel數目, 隨機畫筆)
void COscDlg::InitialData ()
{
	//設定output combo box內容
	for (int i = 0; i < m_iOutputSize; i++)
	{
		CString strOutput;
		strOutput.Format (_T ("Output %d"), i + 1);
		m_ctrlComboOutput.AddString (strOutput);
	}

	//建立隨機畫筆
	for (int i = 0; i < m_iOutputSize; i++)
	{
		CPen* pRandPen = new CPen (PS_SOLID, 3, RGB (rand () % 256, rand () % 256, rand () % 256));
		m_deqPenPtr.push_back (pRandPen);
	}

	m_ctrlComboOutput.SetCurSel (0);
}	

//取得時間間隔
int COscDlg::GetTimeInterval () const
{
	return m_iXInterval;
}