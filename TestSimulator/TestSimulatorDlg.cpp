
// TestSimulatorDlg.cpp : ��@��
//


#include "stdafx.h"
#include "TestSimulator.h"
#include "TestSimulatorDlg.h"
#include "afxdialogex.h"

#define _USE_MATH_DEFINES
#include "math.h"

//#include <iostream>
//using namespace std;

#define WIN_WIDTH 1316					//��ܲ��Ȥ�ϼe��			
#define WIN_HEIGHT 559					//��ܲ��Ȥ�ϰ���
#define GRID_R 1						//��@���I�b�|�j�p
#define MAX_STR_LEN 100					//�̤j�r����� (�Ω�Ū��ini��)

#define WHITE RGB (255, 255, 255)		//�u�@�ϭI���w�]�C��
#define GRAY  RGB (240, 240, 240)		//���I�C��
#define LIGHT_BLUE RGB (176, 224, 230)	//���Block����C��
#define RED   RGB (255, 0, 0)			//����Block����C��		
#define BLACK RGB (0, 0, 0)				//�s�u�C��

#define	MARK_FONTSIZE 120				//�Ҧ�Mark�r��j�p		
#define MARK_WIDTH 90					//Mark�ϰ�e��
#define MARK_LOCK_WIDTH 60				//Lock Mark�ϰ�e��
#define MARK_HEIGHT 40					//Mark�ϰ찪��

#define NO_VALUE -10					//���s�b��J�T��
#define NO_OPER_FLAG -11				//���s�b�B�⤸
#define NO_INPUT_FLAG -12				//���s�b��J�i��
#define COMPUTE_ERROR -13				//�����\���B��
#define NO_BLOCK_HEAD -14				//���s�b�s��Block
#define NO_SEL_NUM -15					//���s�b���Block�s��

#define NO_BLK_TYPE 0					//Block���ݩ�{��type
#define BLK_IN		1					//Block��IN
#define BLK_OUT		2					//Block��OUT
#define BLK_AND		3					//Block��AND
#define BLK_OR		4					//Block��OR
#define BLK_NOT		5					//Block��NOT
#define BLK_FUN		6					//Block��FUN

#define ZERO 0							//FALSE flag
#define ONE  1							//TRUE flag
#define SIN  2							//Sin flag
#define COS  3							//Cos flag

#define MSEC 0.001						//millisecond
#define MAX_DATA_NUM 512				//�̤j�x�sdata�ƥ� (������2�������)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
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


// CTestSimulatorDlg ��ܤ��



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


// CTestSimulatorDlg �T���B�z�`��

BOOL CTestSimulatorDlg::OnInitDialog()
{
	this->SetWindowPos (NULL, 0, 0, WIN_WIDTH, WIN_HEIGHT, SWP_NOZORDER | SWP_NOMOVE);	//�]�w��ܲ��Ȥ�ϰ�
	m_rcWork.SetRect (m_iWorkLeft, m_iWorkTop, m_iWorkRight, m_iWorkBottom);			//�]�w�u�@�ϰ�
	this->GetClientRect (&m_rcClient);													//Ū����ܲ��Ȥ�ϰ�
	m_dcBgPic.CreateCompatibleDC (NULL);												//�إ߻P��ܲ��ۮe��dc�A�H���J�I���Ϥ�

	CString strIniPath (_T (".\\Initial.ini"));	//.ini���|
	CString strAppName (_T ("BgParameter"));	//.ini��[�`�W]

	CString strBgPicBOOL;	//�x�s�I���Ϥ����L�Ȫ��r��
	CString strBgPicPath;	//�x�s�I���Ϥ����|���r��
	CString strBgColor;		//�x�s�I���C��Ȫ��r��
	CString strGridBOOL;	//�x�s���I���L�Ȫ��r��
	CString strLockBOOL;	//�x�s��w���L�Ȫ��r��
	
	CString strKeyName (_T ("BgPicBOOL"));		//.ini���ܼƦW��
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strBgPicBOOL.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("BgPicPath"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strBgPicPath.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("BgColor"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strBgColor.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("GridBOOL"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strGridBOOL.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);
	strKeyName.Format (_T ("LockBOOL"));
	GetPrivateProfileString (strAppName, strKeyName, _T ("None"), strLockBOOL.GetBuffer (MAX_STR_LEN), MAX_STR_LEN, strIniPath);

	//���J�I���Ϥ�(or�C��)
	if (strBgPicBOOL == _T ("TRUE"))
	{
		m_strPicPath = strBgPicPath;

		//�NŪ����.bmp���w��HBITMAP
		HBITMAP hbBgPic;
		//�Q��HBITMAP�[����ϡALoadImage (handle, �Ϥ����|, ���J�˦�, �ϥܼe��(����), �ϥܪ���(����), LR_LOADFROMFILE: �ھڸ��|���ȸ˦b�Ϲ�)
		hbBgPic = (HBITMAP)::LoadImage (AfxGetInstanceHandle (), m_strPicPath, IMAGE_BITMAP, m_rcWork.Width (), m_rcWork.Height (), LR_LOADFROMFILE);
		m_bitmapBgPic.Attach (hbBgPic);
		m_pOldBitmap = m_dcBgPic.SelectObject (&m_bitmapBgPic);

		m_bBgPic = TRUE;
	}
	else if (strBgPicBOOL == _T ("FALSE"))
		m_clrBg = _ttoi (strBgColor);

	//�]�w���I��l���A
	if (strGridBOOL == _T ("TRUE"))
		m_bGrid = TRUE;

	//���J��w��l���A
	if (strLockBOOL == _T ("TRUE"))
		m_bLock = TRUE;

	
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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
	
	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	m_bInitDlg = TRUE;

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CTestSimulatorDlg::OnPaint ()
{
	//���w�īإ�
	CPaintDC dcSim (this);				//�D��ܲ�dc
	CDC dcMem;							//���sdc
	CBitmap bitmapMem;
	CBitmap * pOldBitmap;

	dcMem.CreateCompatibleDC (&dcSim);	//�إ߻P�D��ܲ��ۮe��dc
	bitmapMem.CreateCompatibleBitmap (&dcSim, m_iWorkRight, m_iWorkBottom);
	pOldBitmap = dcMem.SelectObject (&bitmapMem);

	//ø�s�I��
	dcMem.SetBkMode (TRANSPARENT);
	dcMem.FillSolidRect (m_rcWork, m_clrBg);

	//�Ҧ�Mark��rect
	CPoint ptMarkTL (m_iWorkRight - MARK_WIDTH, m_iWorkTop);
	CPoint ptMarkBR (m_iWorkRight, m_iWorkTop + MARK_HEIGHT);
	CRect rcModeMark (ptMarkTL, ptMarkBR);

	//�Ҧ�Mark�r��
	CFont fontModeMark, * pOldFont;	
	fontModeMark.CreatePointFont (MARK_FONTSIZE, _T ("Arial"), &dcMem);	

	//�Ѧs��I���Ϥ���dc�����ܤ��sdc
	if (m_bBgPic)
		dcMem.BitBlt (m_iWorkLeft, m_iWorkTop, m_rcWork.Width (), m_rcWork.Height (), &m_dcBgPic, 0, 0, SRCCOPY);

	//ø�s���I
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

	//ø�s��wMark
	if (m_bLock)
	{
		CPoint ptLockMarkTL (m_iWorkLeft, m_iWorkTop);
		CPoint ptLockMarkBR (m_iWorkLeft + MARK_LOCK_WIDTH, m_iWorkTop + MARK_HEIGHT);
		CRect rcLockMark (ptLockMarkTL, ptLockMarkBR);

		pOldFont = dcMem.SelectObject (&fontModeMark);
		dcMem.DrawText (_T ("Lock"), -1, &rcLockMark, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dcMem.SelectObject (pOldFont);
	}

	//ø�sBlock�s�u
	for (int i = 0; i < m_iContainerSize; i++)
		m_vcBlkContainer[i]->DrawLine (&dcMem);

	//ø�s���Block���
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

	//ø�s�����Ҧ�Mark�P����Block���
	if (m_bSimulate)
	{
		//ø�s�����Ҧ�Mark
		pOldFont = dcMem.SelectObject (&fontModeMark);
		dcMem.DrawText (_T ("Simulation"), -1, &rcModeMark, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dcMem.SelectObject (pOldFont);

		//ø�s����Block���
		CPen penRed (PS_SOLID, 10, RED);
		CPen* pOldPen;
		pOldPen = dcMem.SelectObject (&penRed);
		
		dcMem.Rectangle (m_vcBlkContainer[m_iSimBlkNum]->GetBlkRect ());
		dcMem.SelectObject (pOldPen);
	}

	//ø�sBlock
	for (int i = 0; i < m_iContainerSize; i++)
		m_vcBlkContainer[i]->Draw (&dcMem);

	//ø�s�s�u�Ҧ�Mark�P�s�u������u�u�q
	if (m_bLineMode)
	{
		//ø�s�s�u�Ҧ�Mar
		pOldFont = dcMem.SelectObject (&fontModeMark);
		dcMem.DrawText (_T ("LineMode"), -1, &rcModeMark, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dcMem.SelectObject (pOldFont);

		//ø�s�s�u������u�u�q
		if (m_bConnect)
		{
			CPen penBlack (PS_DASH, 1, BLACK);
			CPen* pOldPen;

			pOldPen = dcMem.SelectObject (&penBlack);

			CPoint ptLStart = m_vcBlkContainer[m_iHeadBlkNum]->GetPinOutRect ().CenterPoint ();			//�u�q�_�I��Block��XPin�������I
			int iCornerGridY = (int) (((ptLStart.y + m_ptLEnd.y) / 2. - m_iWorkTop) / m_iGridY + 0.5);	//�s�u���B�Z�u�@�ϤW��X�Ӯ��I
			int iCornerY = iCornerGridY * m_iGridY + m_iWorkTop;										//�s�u���BY�y��
			dcMem.MoveTo (ptLStart);
			dcMem.LineTo (CPoint (ptLStart.x, iCornerY));
			dcMem.MoveTo (CPoint (ptLStart.x, iCornerY));
			dcMem.LineTo (CPoint (m_ptLEnd.x, iCornerY));
			dcMem.MoveTo (CPoint (m_ptLEnd.x, iCornerY));
			dcMem.LineTo (m_ptLEnd);

			dcMem.SelectObject (pOldPen);
		}		
	}

	//�Ѥ��sdc�����ܥD��ܲ�dc
	dcSim.BitBlt (m_iWorkLeft, m_iWorkTop, m_rcWork.Width (), m_rcWork.Height (), &dcMem, m_iWorkLeft, m_iWorkTop, SRCCOPY);

	//�M�z
	dcMem.SelectObject (pOldBitmap);
	bitmapMem.DeleteObject ();
	dcMem.DeleteDC ();
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CTestSimulatorDlg::OnQueryDragIcon ()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���UIN���s
void CTestSimulatorDlg::OnBnClickedButtonIn ()
{
	m_bLineMode = FALSE;	//�����s�u�Ҧ�

	CBlockBasis* pBlk = new CBlockIn;																				//new�@��CBlockIn������
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//�]�wBlock In��rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//�]�wBlock�s��
	m_vcBlkContainer.push_back (pBlk);																				//�s�JBlock��vector��
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//����Block��vector size

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//���UOUT���s
void CTestSimulatorDlg::OnBnClickedButtonOut ()
{
	m_bLineMode = FALSE;	//�����s�u�Ҧ�

	CBlockBasis* pBlk = new CBlockOut;																				//new�@��CBlockOut������
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//�]�wBlock Out��rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//�]�wBlock�s��
	m_vcBlkContainer.push_back (pBlk);																				//�s�JBlock��vector��
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//����Block��vector size

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//���UAND���s
void CTestSimulatorDlg::OnBnClickedButtonAnd ()
{
	m_bLineMode = FALSE;	//�����s�u�Ҧ�

	CBlockBasis* pBlk = new CBlockAnd;																				//new�@��CBlockAnd������
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//�]�wBlock And��rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//�]�wBlock�s��
	m_vcBlkContainer.push_back (pBlk);																				//�s�JBlock��vector��
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//����Block��vector size

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//���UOR���s
void CTestSimulatorDlg::OnBnClickedButtonOr ()
{
	m_bLineMode = FALSE;	//�����s�u�Ҧ�

	CBlockBasis* pBlk = new CBlockOr;																				//new�@��CBlockOr������
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//�]�wBlock Or��rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//�]�wBlock�s��
	m_vcBlkContainer.push_back (pBlk);																				//�s�JBlock��vector��
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//����Block��vector size

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//���UNOT���s
void CTestSimulatorDlg::OnBnClickedButtonNot ()
{
	m_bLineMode = FALSE;	//�����s�u�Ҧ�

	CBlockBasis* pBlk = new CBlockNot;																				//new�@��CBlockNot������
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//�]�wBlock Not��rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//�]�wBlock�s��
	m_vcBlkContainer.push_back (pBlk);																				//�s�JBlock��vector��
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//����Block��vector size

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//���UFunction���s
void CTestSimulatorDlg::OnBnClickedButtonFunction ()
{
	m_bLineMode = FALSE;	//�����s�u�Ҧ�

	CBlockBasis* pBlk = new CBlockFun;																				//new�@��CBlockFun������
	pBlk->SetBlkRect (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y), m_iBlkWidth, m_iBlkHeight);	//�]�wBlock Fun��rect
	pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());																//�]�wBlock�s��
	m_vcBlkContainer.push_back (pBlk);																				//�s�JBlock��vector��
	m_iContainerSize = (int) m_vcBlkContainer.size ();																//����Block��vector size

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();	
}

//���ULine���s
void CTestSimulatorDlg::OnBnClickedButtonLine ()
{
	//�]�w�s�u�Ҧ��í�ø�u�@��
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

//���USimulate���s
void CTestSimulatorDlg::OnBnClickedButtonSimulate ()
{
	if (!m_bSimulate)
	{
		if (m_bSelectBlk && m_iSlctBlkNum >= 0 && m_iSlctBlkNum < m_iContainerSize &&
			m_vcBlkContainer[m_iSlctBlkNum]->BlkTypeIs () == BLK_OUT && m_vcBlkContainer[m_iSlctBlkNum]->GetBlkHeadSize () > 0)
		{
			m_iSimBlkNum = m_iSlctBlkNum;	//�]�w����Block�s��
			m_bSimulate = TRUE;				//�]�w�����Ҧ�
			m_bLineMode = FALSE;			//�����s�u�Ҧ�
			m_bDelBlk = FALSE;				//�T��R��Block

			//�Y�ܪi����ܲ����Х������w�O����A�hnew�@�ӥܪi����ܲ�
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

			//��ø�u�@��
			InvalidateRect (m_rcWork, FALSE);
			UpdateWindow ();
		}
		else 
			MessageBox (_T ("Please select output block and check if it is connected."));
	}	
}

//���UGrid���s
void CTestSimulatorDlg::OnBnClickedButtonGrid ()
{
	//�]�w���I�Ҧ�
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

//���ULock���s
void CTestSimulatorDlg::OnBnClickedButtonLock ()
{
	if (m_bGrid)
	{
		if (!m_bLock)
		{
			m_bLock = TRUE;	//�]�w��w�Ҧ�

			//�]�w�{��Block��w��̬��F�񪺮��I�W
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

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();
}

//���U�I���Ϥ����s
void CTestSimulatorDlg::OnBnClickedButtonBgpic ()
{
	//�إ�CFileDialog (TRUE�}���ɮ�, �w�]�����ɦW, ��l�ɮצW, OFN_HIDEREADONLY: ���, �z��)
	CFileDialog fileDlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T (" bitmap (*.bmp)|*.bmp||"));

	//�YFile��ܲ����U�T�w��
	if (fileDlg.DoModal () == IDOK)
	{
		m_strPicPath = fileDlg.GetPathName ();	//Ū���ɮ׸��|

		//�NŪ����.bmp���w��HBITMAP
		HBITMAP hbBgPic;
		hbBgPic = (HBITMAP)::LoadImage (AfxGetInstanceHandle (), m_strPicPath, IMAGE_BITMAP, m_rcWork.Width (), m_rcWork.Height (), LR_LOADFROMFILE);
		m_bitmapBgPic.Detach ();
		m_bitmapBgPic.Attach (hbBgPic);

		//�ȩ󭺦����J�Ϥ��ɬ�����lbitmap����
		if (!m_bBgPic)
			m_pOldBitmap = m_dcBgPic.SelectObject (&m_bitmapBgPic);
		else
			m_dcBgPic.SelectObject (&m_bitmapBgPic);

		m_bBgPic = TRUE;

		//��ø�u�@��
		InvalidateRect (m_rcWork, FALSE);
		UpdateWindow ();
	}
}

//���U�I���C����s
void CTestSimulatorDlg::OnBnClickedButtonBgcolor ()
{
	//�إߦ�m��ܲ�
	CColorDialog clrDlg (m_clrBg, CC_PREVENTFULLOPEN);

	//�Y��m��ܲ����U�T�w��
	if (clrDlg.DoModal () == IDOK)
	{
		if (m_bBgPic && m_pOldBitmap != NULL)
		{
			m_dcBgPic.SelectObject (m_pOldBitmap);
			m_bBgPic = FALSE;
		}

		m_clrBg = clrDlg.GetColor ();	//�]�w�I���C��

		//��ø�u�@��
		InvalidateRect (m_rcWork, FALSE);
		UpdateWindow ();
	}		
}

//���UOpen���s
void CTestSimulatorDlg::OnBnClickedButtonOpen ()
{
	//�����Ҧ��U���i�i��Ū��
	if (!m_bSimulate)
	{
		//�إ�CFileDialog (TRUE�}���ɮ�, �w�]�����ɦW, ��l�ɮצW, OFN_HIDEREADONLY: ���, �z��)
		CFileDialog fileDlg (TRUE, _T ("txt"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T (" txt (*.txt)|*.txt||"));

		//�إ�CStdioFile�HŪ���ɮ׸��
		CStdioFile fileSim;

		//�Y�ɮ׹�ܲ����U�T�w��
		if (fileDlg.DoModal () == IDOK)
		{
			//�}���ɮ� (�ɮ׸��|, CFile::modeNoTruncate: ���M�Ť�󤺮e | CFile::modeRead: ��Ū)
			fileSim.Open (fileDlg.GetPathName (), CFile::modeNoTruncate | CFile::modeRead);

			CString strData;		//�x�s�C�@�C�r����
			CStringArray strArray;	//�x�s�C�@Block�U�����
		
			fileSim.ReadString (strData);	//Ū���ɮ׸�ƶi�r��
			if (strData == "BlockData: ")	//��ƶ}�Y����BlockDate
			{
				//Ū���ɮ׮ɨ������Block
				m_bSelectBlk = FALSE;		
				m_iSlctBlkNum = NO_SEL_NUM;	

				//�M���ܪi����ܲ�����
				if (m_pOscDlg != NULL)
				{
					m_pOscDlg->DestroyWindow ();
					delete m_pOscDlg;
					m_pOscDlg = NULL;
				}			
				
				//�M���s��Block��vector	
				for (int i = 0; i < m_iContainerSize; i++)
					delete m_vcBlkContainer[i];
				std::vector <CBlockBasis*> ().swap (m_vcBlkContainer);

				//�C�����ɮ�Ū���@����
				while (fileSim.ReadString (strData))
				{
					SplitString (strData, ',', strArray);	//�NŪ���쪺�r����j��Block�U����ơA���x�s�ܰ}�C��

					if (strArray[0] == "IN")		//Block In��Ƥ��e�]�t�����I��m, �i��flag
					{
						CBlockBasis* pBlk = new CBlockIn;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetInputFlag (_ttoi (strArray[3]));
						pBlk->SetBlkValue ();
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "OUT")	//Block Out��Ƥ��e�]�t�����I��m, ��Jchannel�ƥ�, ��JBlock�s��
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
					else if (strArray[0] == "AND")	//Block And��Ƥ��e�]�t�����I��m, ��JBlock�s��
					{
						CBlockBasis* pBlk = new CBlockAnd;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkHead2Num (_ttoi (strArray[4]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "OR")	//Block Or��Ƥ��e�]�t�����I��m, ��JBlock�s��
					{
						CBlockBasis* pBlk = new CBlockOr;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkHead2Num (_ttoi (strArray[4]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "NOT")	//Block Not��Ƥ��e�]�t�����I��m, ��JBlock�s��
					{
						CBlockBasis* pBlk = new CBlockNot;
						pBlk->SetBlkRect (CPoint (_ttoi (strArray[1]), _ttoi (strArray[2])), m_iBlkWidth, m_iBlkHeight);
						pBlk->SetBlkHead1Num (_ttoi (strArray[3]));
						pBlk->SetBlkNum ((int) m_vcBlkContainer.size ());
						m_vcBlkContainer.push_back (pBlk);
						m_iContainerSize = (int) m_vcBlkContainer.size ();
					}
					else if (strArray[0] == "FUN")	//Block Fun��Ƥ��e�]�t�����I��m, ��JBlock�s��, �B�⤸flag
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

				//���Ū��������A�����ɮ�
				fileSim.Close ();

				//��Ū���쪺��JBlock�s���A���s�]�w�C�@Block��s��Block������
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

				//��ø�u�@��
				InvalidateRect (m_rcWork, FALSE);
				UpdateWindow ();	
			}
			else 
				fileSim.Close ();
		}
	}	
}

//���USave���s
void CTestSimulatorDlg::OnBnClickedButtonSave ()
{
	//�إ�CFileDialog (FALSE�x�s�ɮ�, �w�]�����ɦW, ��l�ɮצW, OFN_HIDEREADONLY: ���, �z��)
	CFileDialog fileDlg (FALSE, _T ("txt"), _T ("Simulator"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T (" txt (*.txt)|*.txt||"));
	
	//�إ�CStdioFile�H�x�s���
	CStdioFile fileSim;

	//�Y�ɮ׹�ܲ����U�T�w��
	if (fileDlg.DoModal () == IDOK)
	{
		// �}���ɮ� (�ɮ׸��|, CFile::modeCreate: �H�s�ؤ覡���} | CFile::modeWrite: �u�g)
		fileSim.Open (fileDlg.GetPathName (), CFile::modeCreate | CFile::modeWrite);

		CString strData;	//�ΥH�g�J�ɮת��r��
		strData.Format (_T ("BlockData: \n"));
		fileSim.WriteString (strData);

		for (int i = 0; i < m_iContainerSize; i++)
		{
			int iBlkFlag = m_vcBlkContainer[i]->BlkTypeIs ();
			switch (iBlkFlag)
			{
			case BLK_IN:	//Block In���x�s���e�]�t�����I��m, �i��flag
				strData.Format (_T ("%3s, %3d, %3d, %2d\n"), "IN", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetInputFlag ());
				break;
			case BLK_OUT:	//Block Out���x�s���e�]�t�����I��m, ��Jchannel�ƥ�, ��JBlock�s��
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
			case BLK_AND:	//Block And���x�s���e�]�t�����I��m, ��JBlock�s��
				strData.Format (_T ("%3s, %3d, %3d, %1d, %1d\n"), "AND", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num (), m_vcBlkContainer[i]->GetBlkHead2Num ());
				break;
			case BLK_OR:	//Block Or���x�s���e�]�t�����I��m, ��JBlock�s��
				strData.Format (_T ("%3s, %3d, %3d, %1d, %1d\n"), "OR", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num (), m_vcBlkContainer[i]->GetBlkHead2Num ());
				break;
			case BLK_NOT:	//Block Not���x�s���e�]�t�����I��m, ��JBlock�s��
				strData.Format (_T ("%3s, %3d, %3d, %1d\n"), "NOT", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num ());
				break;
			case BLK_FUN:	//Block Fun���x�s���e�]�t�����I��m, ��JBlock�s��, �B�⤸flag
				strData.Format (_T ("%3s, %3d, %3d, %1d, %1d, %2d\n"), "FUN", m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().x, m_vcBlkContainer[i]->GetBlkRect ().CenterPoint ().y,
								m_vcBlkContainer[i]->GetBlkHead1Num (), m_vcBlkContainer[i]->GetBlkHead2Num (), m_vcBlkContainer[i]->GetOperFlag ());
				break;
			default:
				break;
			}

			fileSim.WriteString (strData);	//�N�r�ꤺ�e�g�J�ɮ�
		}

		fileSim.Close ();	//�g�J������A�����ɮ�
	}
}

//�ƹ�������U
void CTestSimulatorDlg::OnLButtonDown (UINT nFlags, CPoint ptCursor)
{
	SetCapture ();
	
	m_bDelBlk = FALSE;	//�ƹ�������U�ɸT��R��Block

	//�P�_���ЬO�_���Block rect�d��
	for (int i = m_iContainerSize - 1; i >= 0; i--)
	{
		if (m_vcBlkContainer[i]->GetBlkRect ().PtInRect (ptCursor))
		{
			SetCursor (LoadCursor (NULL, IDC_SIZEALL));
			m_iSlctBlkNum = i;		//���Block�s��
			m_bSelectBlk = TRUE;
			m_bMoveBlk = TRUE;
			m_bLineMode = FALSE;	//�����s�u�Ҧ�
			break;
		}
		else	//�I��DBlock��m�ɡA��������аO
		{
			m_bSelectBlk = FALSE;
			m_iSlctBlkNum = NO_SEL_NUM;
		}			
	}

	//�Y�����Block�A�ߧY��ø�u�@��
	if (m_bSelectBlk)
	{
		InvalidateRect (m_rcWork, FALSE);
		UpdateWindow ();
	}

	//�s�u�Ҧ��ɡA�P�_���Ы��U��m�O�_����XPin�}�d��
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

//�ƹ�����
void CTestSimulatorDlg::OnMouseMove (UINT nFlags, CPoint ptCursor)
{
	//���ʿ��Block
	if (m_bMoveBlk)
	{
		//��w�Ҧ��U�p�⹫�Ц�m�̾F�񤧮��I�A�N���ʪ�Block���߳]�m��Ӯ��I��m
		if (m_bLock)
		{
			int iGridX = (int) ((ptCursor.x - m_iWorkLeft) / (double) m_iGridX + 0.5);
			int iGridY = (int) ((ptCursor.y - m_iWorkTop) / (double) m_iGridY + 0.5);

			m_vcBlkContainer[m_iSlctBlkNum]->Move (CPoint (iGridX * m_iGridX + m_iWorkLeft, iGridY * m_iGridY + m_iWorkTop));
		}
		else
			m_vcBlkContainer[m_iSlctBlkNum]->Move (ptCursor);
	}
		
	//�s�u�Ҧ�
	if (m_bLineMode)
	{
		for (int i = 0; i < m_iContainerSize ; i++)
		{
			//���оa��Block��XPin�}�ɡA�]�w���м˦����Q�r�b�Y
			if (m_vcBlkContainer[i]->GetPinOutRect ().PtInRect (ptCursor))
				SetCursor (LoadCursor (NULL, IDC_CROSS));

			//�s�u�ɡA�]�w���Ц�m����u�u�q����
			if (m_bConnect)
				m_ptLEnd = ptCursor;
		}
	}

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();

	CDialogEx::OnMouseMove (nFlags, ptCursor);
}

//�ƹ������}
void CTestSimulatorDlg::OnLButtonUp (UINT nFlags, CPoint ptCursor)
{
	//�����Block�ɡA�i�R����Block
	if (m_bSelectBlk)
		m_bDelBlk = TRUE;

	if (m_bMoveBlk)
	{
		//����Block���A�U�A��ƹ������}��m�W�X�u�@�ϡA�h�]�wBlock���^�u�@�Ϥ���
		if (!(m_rcWork.PtInRect (m_vcBlkContainer[m_iSlctBlkNum]->GetBlkRect ().TopLeft ()) && 
			  m_rcWork.PtInRect (m_vcBlkContainer[m_iSlctBlkNum]->GetBlkRect ().BottomRight ())))
			m_vcBlkContainer[m_iSlctBlkNum]->Move (CPoint (m_rcWork.CenterPoint ().x, m_rcWork.CenterPoint ().y));

		m_bMoveBlk = FALSE;	
	}
	
	//�s�u�Ҧ��U
	if (m_bLineMode && m_bConnect)
	{
		//�P�_�ƹ������}��m�O�_���Block��JPin�}�d��A�Y�O�A�h�]�w��Block�s���_�lBlock������
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

		m_bConnect = FALSE;	//�ƹ������}�ɡA�����s�u���u�q
	}	

	ReleaseCapture ();

	//��ø�u�@��
	InvalidateRect (m_rcWork, FALSE);
	UpdateWindow ();

	CDialogEx::OnLButtonUp(nFlags, ptCursor);
}

//�����ƹ�����
void CTestSimulatorDlg::OnLButtonDblClk (UINT nFlags, CPoint ptCursor)
{
	//�����Ҧ��U���i���Block flag
	if (!m_bSimulate)
	{
		for (int i = 0; i < m_iContainerSize; i++)
		{
			if (m_vcBlkContainer[i]->GetBlkRect ().PtInRect (ptCursor))
			{
				if (m_vcBlkContainer[i]->BlkTypeIs () == BLK_IN)		//���Block In��J�i��flag
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
				else if (m_vcBlkContainer[i]->BlkTypeIs () == BLK_FUN)	//���Block Fun�B�⤸flag
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
	
	int iTimeInterval = m_pOscDlg->GetTimeInterval ();	//���o�ϥΪ̳]�w���ɶ����j(���ms)
	double dTime = m_iStep * iTimeInterval * MSEC;		//�����i���`�ɶ�(���s)

	std::deque <double> deqRowValue;	//�ΥH�x�s�C�@�Ӯɶ��I�U�Uinput channel����

	//�����e�P�_input�O�_�X�z(�ȧP�_�@��)
	if (m_iStep == 0)
	{
		m_iOutSize = m_vcBlkContainer[m_iSimBlkNum]->GetBlkHeadSize ();	//Ū��channel�ƥ�

		for (int i = 0; i < m_iOutSize; i++)
		{
			int iValueFlag = m_vcBlkContainer[m_iSimBlkNum]->GetValueFlag (i);
			
			if (iValueFlag == NO_VALUE)				//���s�binput signal
			{
				MessageBox (_T ("Please check if the output block exists input signal."));
				m_pOscDlg->m_bSim = FALSE;
				m_pOscDlg->m_bModify = TRUE;
				break;
			}
			else if (iValueFlag == COMPUTE_ERROR)	//�s�b�����\���B��覡
			{
				MessageBox (_T ("Operation is not allowed."));
				m_pOscDlg->m_bSim = FALSE;
				m_pOscDlg->m_bModify = TRUE;
				break;
			}
			else if (iValueFlag == NO_OPER_FLAG)	//Block Fun���]�w�B�⤸
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

	//��input���X�z�B��A�����}�l
	if (m_pOscDlg->m_bSim)
	{	
		if (m_iStep == 0)
		{
			m_pOscDlg->SetOutputSize (m_iOutSize);	//�]�w�ܪi����ܲ���channel��
			m_pOscDlg->InitialData ();				//��l�ƥܪi����ܲ����(channel�ƥ�, �H���e��)
			m_deq2DOutputValue.clear ();			//�}�l�����e�M�ŤG���x�}
		}	

		//�]�w�Ӯɶ��U�A�C�@��Block In����
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

		//���o�������Block Out����(���h��channel)
		for (int i = 0; i < m_iOutSize; i++)	
		{
			double dValue =  m_vcBlkContainer[m_iSimBlkNum]->GetBlkValue (i);
			deqRowValue.push_back (dValue);	//�s�J�C�V�q(�����Ӯɶ��I�U�C�@��input channel����)
		}		

		int iDataSize = (int) m_deq2DOutputValue.size ();	//Ū���`data�ƥ�

		//�Y�`data�ƥضW�L�w�]�̤jdata�ơA�R���Ĥ@��data�A�åѥ��ݥ[�J�sdata
		if (iDataSize < MAX_DATA_NUM)
			m_deq2DOutputValue.push_back (deqRowValue);
		else
		{
			m_deq2DOutputValue.pop_front ();
			m_deq2DOutputValue.push_back (deqRowValue); 
		}

		//FFT�D�Y���W�v
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

		//�ǭȦܥܪi����ܲ���(��e�ɶ�, ���output value, �W�v)
		m_pOscDlg->SetData(dTime, deqRowValue[m_pOscDlg->m_iSlctOutputNum], dOutFreq);
		//m_pOscDlg->SetData (dTime, deqRowValue[m_pOscDlg->m_iSlctOutputNum]);

		m_iStep++;

		//��s�ܪi����ܲ�
		m_pOscDlg->UpdateData (FALSE);
		m_pOscDlg->InvalidateRect (m_pOscDlg->m_rcWork, FALSE);
		m_pOscDlg->UpdateWindow ();
	}	

	CDialogEx::OnTimer (nIDEvent);

	if (m_pOscDlg->m_bSim && nIDEvent == 0)
		SetTimer (0, 10, NULL);
}

//�D��ܲ������j�p����
void CTestSimulatorDlg::OnSize (UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//��ͦ��D��ܲ��ɤ��i�J
	if (m_bInitDlg)
	{	
		//�P�_�O�_�̤p��
		if (!IsIconic ())
		{
			//���o�D��ܲ��Ȥ��
			CRect rcCurClient;
			this->GetClientRect (&rcCurClient);

			//�p��x, y��V�Y����
			double dMultipleWid = rcCurClient.Width () / (double) m_rcClient.Width ();
			m_rcClient.right = rcCurClient.Width ();
			double dMultipleHeight = rcCurClient.Height () / (double) m_rcClient.Height ();
			m_rcClient.bottom = rcCurClient.Height ();

			//�]�w��s���u�@�ϰ�
			m_iWorkLeft = (int) (dMultipleWid * m_iWorkLeft + 0.5);
			m_iWorkTop = (int) (dMultipleHeight * m_iWorkTop + 0.5);
			m_iWorkRight = (int) (dMultipleWid * m_iWorkRight + 0.5);
			m_iWorkBottom = (int) (dMultipleHeight * m_iWorkBottom + 0.5);
			m_rcWork.SetRect (m_iWorkLeft, m_iWorkTop, m_iWorkRight, m_iWorkBottom);

			//�]�w��s�������m�P�j�p
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

			//���s���J�I���Ϥ�
			if (m_bBgPic)
			{
				HBITMAP hbBgPic;
				hbBgPic = (HBITMAP)::LoadImage (AfxGetInstanceHandle (), m_strPicPath, IMAGE_BITMAP, m_rcWork.Width (), m_rcWork.Height (), LR_LOADFROMFILE);
				m_bitmapBgPic.Detach ();
				m_bitmapBgPic.Attach (hbBgPic);
				m_dcBgPic.SelectObject (&m_bitmapBgPic);
			}
			
			//�]�w��s��Block��m�P�j�p
			m_iBlkWidth = (int) (m_iBlkWidth * dMultipleWid + 0.5);
			m_iBlkHeight = (int) (m_iBlkHeight * dMultipleHeight + 0.5);
			m_iGridX = (int) (m_iBlkWidth / 4. + 0.5);
			m_iGridY = (int) (m_iBlkHeight / 2. + 0.5);
			for (int i = 0; i < m_iContainerSize; i++)
				Resize (m_vcBlkContainer[i], dMultipleWid, dMultipleHeight);
			
			//��ø�u�@��
			Invalidate (TRUE);
			UpdateWindow ();	

			//�Y�s�b�ܪi����ܲ��A�h��ܵ���
			if (m_bSimulate && m_pOscDlg != NULL)
				m_pOscDlg->ShowWindow (SW_SHOW);
		}
	}	
}

BOOL CTestSimulatorDlg::PreTranslateMessage (MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//���U�R����
		if (pMsg->wParam == VK_DELETE && !m_vcBlkContainer.empty () && m_bDelBlk && !m_bSimulate)
		{
			//�P�_��LBlock�O�_���s���ܸӧR����Block�A�Y���A�h�M���s������
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

			//�M�����Block
			delete m_vcBlkContainer[m_iSlctBlkNum];

			m_vcBlkContainer.erase (m_vcBlkContainer.begin () + m_iSlctBlkNum);
			m_iContainerSize = (int) m_vcBlkContainer.size ();

			//���s�s��
			for (int i = 0; i < m_iContainerSize; i++)
				m_vcBlkContainer[i]->SetBlkNum (i);

			//���s�]�w�s���s��
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

			//�R����A�������
			m_bSelectBlk = FALSE;
			m_iSlctBlkNum = NO_SEL_NUM;
			m_bDelBlk = FALSE;

			//��ø�u�@��
			InvalidateRect (m_rcWork, FALSE);
			UpdateWindow ();
		}
		//���UESC��
		else if (pMsg->wParam == VK_ESCAPE)
		{
			m_bSelectBlk = FALSE;
			m_iSlctBlkNum = NO_SEL_NUM;
			m_bMoveBlk = FALSE;
			m_bLineMode = FALSE;
			m_bConnect = FALSE;
			m_bDelBlk = FALSE;

			//��ø�u�@��
			InvalidateRect (m_rcWork, FALSE);
			UpdateWindow ();

			return TRUE;
		}
		//���UENTER��
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestSimulatorDlg::OnDestroy ()
{
	CString strIniPath (_T (".\\Initial.ini"));	//.ini���|
	CString strAppName (_T ("BgParameter"));	//.ini��[�`�W]
	CString strColorRef;						//�C���ܼ�
	strColorRef.Format (_T ("%u"), m_clrBg);	//�N�C����ର�r��

	//�����I��������T
	CString strKeyName (_T ("BgPicBOOL"));		//.ini���ܼƦW��
	if (m_bBgPic)
	{
		WritePrivateProfileString (strAppName, strKeyName, _T ("TRUE"), strIniPath);	//���J�I���Ϥ�
		strKeyName.Format (_T ("BgPicPath"));
		WritePrivateProfileString (strAppName, strKeyName, m_strPicPath, strIniPath);	//�I���Ϥ����|
		strKeyName.Format (_T ("BgColor"));
		WritePrivateProfileString (strAppName, strKeyName, _T ("None"), strIniPath);	//�I���C�⬰None
	}
	else
	{
		WritePrivateProfileString (strAppName, strKeyName, _T ("FALSE"), strIniPath);	//�����J�I���Ϥ�
		strKeyName.Format (_T ("BgPicPath"));
		WritePrivateProfileString (strAppName, strKeyName, _T ("None"), strIniPath);	//�I���Ϥ����|��None
		strKeyName.Format (_T ("BgColor"));
		WritePrivateProfileString (strAppName, strKeyName, strColorRef, strIniPath);	//�����I���C���
	}

	//�������I�}�һP�_
	strKeyName.Format (_T ("GridBOOL"));
	if (m_bGrid)
		WritePrivateProfileString (strAppName, strKeyName, _T ("TRUE"), strIniPath);
	else
		WritePrivateProfileString (strAppName, strKeyName, _T ("FALSE"), strIniPath);

	//������w�}�һP�_
	strKeyName.Format (_T ("LockBOOL"));
	if (m_bLock)
		WritePrivateProfileString (strAppName, strKeyName, _T ("TRUE"), strIniPath);
	else
		WritePrivateProfileString (strAppName, strKeyName, _T ("FALSE"), strIniPath);

	//�M���s��I���Ϥ���dc
	m_dcBgPic.SelectObject (m_pOldBitmap);
	m_dcBgPic.DeleteDC ();

	//�M���ܪi����ܲ�����
	if (m_pOscDlg != NULL)
	{
		m_pOscDlg->DestroyWindow ();
		delete m_pOscDlg;
	}
			
	//�M��Block vector
	for (int i = 0; i < m_iContainerSize; i++)
		delete m_vcBlkContainer[i];

	CDialogEx::OnDestroy();
}

//�N�@�C�r��̾ڤ��j�Ÿ����ά��W�ߦr��
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

//���s�]�w�����m�Τj�p
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

//���s�]�wBlock��m�Τj�p
void CTestSimulatorDlg::Resize (CBlockBasis* pBlk, double dMulWid, double dMulHei)
{
	CRect rcItem = pBlk->GetBlkRect ();

	rcItem.TopLeft ().x = (int) (rcItem.TopLeft ().x * dMulWid + 0.5);
	rcItem.TopLeft ().y = (int) (rcItem.TopLeft ().y * dMulHei + 0.5);
	rcItem.BottomRight ().x = (int) (rcItem.BottomRight ().x * dMulWid + 0.5);
	rcItem.BottomRight ().y = (int) (rcItem.BottomRight ().y * dMulHei + 0.5);

	pBlk->SetBlkRect (rcItem.CenterPoint (), m_iBlkWidth, m_iBlkHeight);
}

//�p��data�h��(���Ω�FFT)
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

//FFT�D�Y���W�v
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

	//�p���v���ǦC
	for (int i = 0; i < iDeqLen / 2; i++)
	{
		double dAngle = i * dFixedFactors;
		deqWeights.push_back (CComplex (cos (dAngle), sin (dAngle)));
	}

	for (int i = 0; i < iDeqLen / 2; i++)
		deqWeights.push_back (CComplex (-deqWeights[i].m_dReal, -deqWeights[i].m_dImag));

	//�p��˧Ǧ�X
	for (int i = 0; i < iDeqLen; i++)
	{
		int iIndex = 0;
		for (int j = iLayers - 1; j >= 0; j--)
			iIndex += (1 && (i & (1 << j))) << (iLayers - j - 1);

		deqX.push_back (deqFData[iIndex]);
	}

	//�p��FFT
	for (int L = 1; L <= iLayers; L++)
	{
		int iDistance = 1 << (L - 1);
		int iW = 1 << (iLayers - L);	//�`��ƪ��׳Q���Ѫ�����

		int iB = iDeqLen >> L;			//�ݭp��X����ƪ��׬�iN���ť߸��ഫ
		int iN = iDeqLen / iB;			//�ݭp��ť߸��ഫ����ƪ���

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

	//�p�⮶�T
	for (int i = 0; i < iDeqLen; i++)
		deqAmplitude.push_back (deqFData[i].ComputeAmplitude ());

	double dMaxAmp = 0.;					//�ŧi�̤j���T�ܼ�
	double dCornerFreq = 1 / dUnitTime / 2;	//�p��I���W�v
	double dOutFreq = 0.;					//�����̤j���֥X�{���W�v

	//�����W�줤�X�{�̤j���T����m�A������W�v�Y����X�W�v
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