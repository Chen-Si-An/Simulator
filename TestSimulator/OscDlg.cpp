
// OscDlg.cpp : ��@��
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

//�u�@�ϰ�Ѽ�
#define WORK_LEFT	0
#define WORK_TOP	0
#define WORK_RIGHT	500
#define WORK_BOTTOM	500

#define BLACK RGB (0, 0, 0)			//�I���C��
#define BLUE RGB (0, 0, 255)		//�����C��
#define WHITE RGB (255, 255, 255)	//y = 0���u�C��
#define YELLOW RGB (255, 255, 0)	//�Z�����u�@�ӳ�쪺�u���C��
#define RED RGB (255, 0, 0)			//�Z�����u��ӳ�쪺�u���C��
#define GOLD RGB (255, 215, 0)		//���output�i���C��

#define GRID 10
#define UNITY 100

#define MAX_PLOT_NUM 501			//�̤jø�sdata��
#define OFFSET_Y 250				//��ܲ��y�Х��W�������I�A���F��ø��ø�s��ܪi�������A�Gø�ϮɻݼW�[�@�Ӱ����q��(0, 0)���ܪi�ϥ����I


// COscDlg ��ܤ��

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


// COscDlg �T���B�z�`��

BOOL COscDlg::OnInitDialog ()
{
	CDialogEx::OnInitDialog();

	//�]�w�üƺؤl
	srand ((unsigned int) time (NULL));

	//�]�wY�b���jCombo box���e
	for (int i = 0; i < 4; i++)
	{
		CString strYInterval;
		double dInterval = 0.5;
		for (int j = 0 ; j < i; j++)
			dInterval *= 2;
		strYInterval.Format (_T ("%g"), dInterval);
		m_ctrlComboYInterval.AddString (strYInterval);
	}

	//��l��Y�b���j
	m_ctrlComboYInterval.SetCurSel (0);
	m_dYInterval = 0.5;
	m_iYPos = 0;
	m_dMaxDataValue = m_dYInterval * 2.5;


	//�]�wX�b���jCombo box���e
	m_ctrlComboXInterval.AddString (_T ("5"));
	m_ctrlComboXInterval.AddString (_T ("10"));
	m_ctrlComboXInterval.AddString (_T ("50"));
	m_ctrlComboXInterval.AddString (_T ("100"));
	m_ctrlComboXInterval.AddString (_T ("500"));

	//��l��X�b���j
	m_ctrlComboXInterval.SetCurSel (0);
	m_iXInterval = 5;
	m_iXPos = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}


void COscDlg::OnPaint ()
{
	//���w�īإ�
	CPaintDC dcOsc (this);				//�ܪi����ܲ�dc
	CDC dcMem;							//���sdc
	CBitmap bitmapMem;
	CBitmap* pOldBitmap;

	dcMem.CreateCompatibleDC (&dcOsc);	//�إ߻P�ܪi����ܲ��ۮe��dc
	bitmapMem.CreateCompatibleBitmap (&dcOsc, WORK_RIGHT, WORK_BOTTOM);
	pOldBitmap = dcMem.SelectObject (&bitmapMem);

	//ø�s�I��
	dcMem.SetBkMode (TRANSPARENT);
	dcMem.FillSolidRect (m_rcWork, BLACK);

	//�Ŧ����A�C������� 10 pixels
	CPen penBlue (PS_SOLID, 1, BLUE);
	CPen* pOldPen = dcMem.SelectObject (&penBlue);

	for (int i = 0; i < m_rcWork.Width (); i += GRID)
	{
		dcMem.MoveTo (i, 0);					//�����u
		dcMem.LineTo (i, m_rcWork.Height ());
		dcMem.MoveTo (0, i);					//�����u
		dcMem.LineTo (m_rcWork.Width (), i);
	}

	//ø�s�ܪi�����u(X�b)
	CPen penWhite (PS_SOLID, 2, WHITE);
	CString strValueZero ("Value = 0");
	dcMem.SelectObject (&penWhite);
	SetTextColor (dcMem, WHITE);

	dcMem.MoveTo (0, m_rcWork.Height () / 2);
	dcMem.LineTo (m_rcWork.Width (), m_rcWork.Height () / 2);								//�e�u
	dcMem.TextOutA (0, m_rcWork.Height () / 2, strValueZero, strValueZero.GetLength ());	//�аO��r


	//�Z�����u�@��UNITY (�@��UNITY�� 100 piexls)
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


	//�Z�����u���UNITY (�@��UNITY�� 100 piexls)
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


	//�����}�l
	if (m_bSim)
	{
		CPen penGold (PS_SOLID, 6, GOLD);

		int iDataSize = (int) m_pSimulatorDlg->m_deq2DOutputValue.size ();	//���odata�ƥ�

		//��data�Ƥj��1�A�}�lø�soutput�i��
		if (iDataSize > 1)
		{
			int iPlotStart = 0;	//ø�sdata�_�I
			if (iDataSize > MAX_PLOT_NUM)
				iPlotStart = iDataSize - MAX_PLOT_NUM;

			for (int i = 0; i < m_iOutputSize; i++)
			{
				//�]�w���output���e����������ʪ��e��
				if (m_iSlctOutputNum >= 0 && i == m_iSlctOutputNum)
					dcMem.SelectObject (&penGold);
				else	
					dcMem.SelectObject (m_deqPenPtr[i]);

				for (int j = 1; j < iDataSize; j++)
				{
					//�W�X�iø�sdata�ơA�h�ߧY���X�j��
					if (j >= MAX_PLOT_NUM)
						break;

					double dForwardValue = m_pSimulatorDlg->m_deq2DOutputValue[j - 1 + iPlotStart][i];
					double dPostValue = m_pSimulatorDlg->m_deq2DOutputValue[j + iPlotStart][i];

					//�P�_data�ȬO�_�W�X�ܪi���i��ܤ��̤j��
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

	//�Ѥ��sdc�����ܥܪi����ܲ�dc
	dcOsc.BitBlt (WORK_LEFT, WORK_TOP, m_rcWork.Width (), m_rcWork.Height (), &dcMem, WORK_LEFT, WORK_TOP, SRCCOPY);

	//�M�z
	dcMem.SelectObject (pOldPen);
	dcMem.SelectObject (pOldBitmap);
	bitmapMem.DeleteObject ();
	dcMem.DeleteDC ();
}

//�ѥD��ܲ��ǭ�(��e�ɶ�, ���output value, �W�v)
void COscDlg::SetData (double dTime, double dValue, double dFreq)
{
	m_dTime = dTime;
	m_dSlctOutputValue = dValue;
	m_dFreq = dFreq;
}

// �ѥD��ܲ��ǭ�(��e�ɶ�, ���output value)
void COscDlg::SetData (double dTime, double dValue)
{
	m_dTime = dTime;
	m_dSlctOutputValue = dValue;
}

//���UStart��
void COscDlg::OnBnClickedButtonStart ()
{
	if (!m_bSim)
	{
		m_bModify = FALSE;	//�}�l�������i���X�b���jCombo box
		m_bSim = TRUE;
		m_pSimulatorDlg->SetTimer (0, 10, NULL);
	}
}

//���UStop��
void COscDlg::OnBnClickedButtonStop ()
{
	if (m_bSim)
	{
		m_bSim = FALSE;
		m_pSimulatorDlg->KillTimer (0);
	}
}

//����output combo box
void COscDlg::OnCbnSelchangeComboOutput ()
{
	m_iSlctOutputNum = m_ctrlComboOutput.GetCurSel ();
	m_bFFT = TRUE;
}

//����Y�b���j combo box
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

//����X�b���j combo box
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


//��������
void COscDlg::OnClose ()
{
	//�M���H���e��deque
	if (!m_deqPenPtr.empty ())
	{
		for (int i = 0; i < m_iOutputSize; i++)
			delete m_deqPenPtr[i];
	}
	
	m_pSimulatorDlg->m_bSimulate = FALSE;	//�������
	m_deqPenPtr.clear ();
	m_ctrlComboOutput.ResetContent ();		//�M��output combo box���e
	m_bSim = FALSE;							//�������
	m_bModify = TRUE;						//���������i���combo box
	m_bFFT = TRUE;
	m_iSlctOutputNum = 0;
	m_dTime = 0.;
	m_dSlctOutputValue = 0.;
	m_dFreq = 0.;
	m_pSimulatorDlg->m_iStep = 0;
	m_pSimulatorDlg->KillTimer (0);

	CDialogEx::OnClose ();
}

//�]�wchannel�ƥ�
void COscDlg::SetOutputSize (int iOutSize)
{
	m_iOutputSize = iOutSize;
}

//��l�ƥܪi����ܲ����(channel�ƥ�, �H���e��)
void COscDlg::InitialData ()
{
	//�]�woutput combo box���e
	for (int i = 0; i < m_iOutputSize; i++)
	{
		CString strOutput;
		strOutput.Format (_T ("Output %d"), i + 1);
		m_ctrlComboOutput.AddString (strOutput);
	}

	//�إ��H���e��
	for (int i = 0; i < m_iOutputSize; i++)
	{
		CPen* pRandPen = new CPen (PS_SOLID, 3, RGB (rand () % 256, rand () % 256, rand () % 256));
		m_deqPenPtr.push_back (pRandPen);
	}

	m_ctrlComboOutput.SetCurSel (0);
}	

//���o�ɶ����j
int COscDlg::GetTimeInterval () const
{
	return m_iXInterval;
}