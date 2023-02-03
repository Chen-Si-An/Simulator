
// TestSimulatorDlg.h : ���Y��
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


// CTestSimulatorDlg ��ܤ��
class CTestSimulatorDlg : public CDialogEx
{
// �غc
public:
	CTestSimulatorDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_TESTSIMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩

private:
	std::vector <CBlockBasis*> m_vcBlkContainer;	//�s�񤣦PBlock��vector

	CRect m_rcWork;									//�u�@��
	CRect m_rcClient;								//��ܲ��Ȥ��
	CPoint m_ptLEnd;								//�s�u���u�q���I
	COscDlg* m_pOscDlg;								//�ܪi����ܲ�����
	CDC m_dcBgPic;									//�s��I���Ϥ���dc
	CBitmap m_bitmapBgPic;							//�s��I���Ϥ���bitmap
	CBitmap* m_pOldBitmap;							//���V��bitmap������
	CString m_strPicPath;							//�I���Ϥ����|
	COLORREF m_clrBg;								//�I���C��

	BOOL m_bInitDlg;								//�P�_��ܵ���l��
	BOOL m_bSelectBlk;								//�P�_Block���
	BOOL m_bMoveBlk;								//�P�_Block����
	BOOL m_bLineMode;								//�P�_�e�u�Ҧ�
	BOOL m_bConnect;								//�P�_�s�u
	BOOL m_bDelBlk;									//�P�_Block�R��
	BOOL m_bBgPic;									//�P�_���J�I���Ϥ�
	BOOL m_bGrid;									//�P�_���I���
	BOOL m_bLock;									//�P�_��w���I	

	int m_iWorkLeft;								//�u�@�ϥ����y��
	int m_iWorkTop;									//�u�@�ϤW��y��
	int m_iWorkRight;								//�u�@�ϥk���y��
	int m_iWorkBottom;								//�u�@�ϤU��y��
	int m_iBlkWidth;								//Block�e��
	int m_iBlkHeight;								//Block����
	int m_iGridX;									//X��V���I���j
	int m_iGridY;									//Y��V���I���j
	int m_iContainerSize;							//�s��Block��vector�j�p
	int m_iSlctBlkNum;								//���Block�s��
	int m_iHeadBlkNum;								//�s��Block�s��
	int m_iSimBlkNum;								//����Block�s��
	int m_iOutSize;									//����Block��channel�ƥ�

// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog ();
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam);
	afx_msg void OnPaint ();
	afx_msg HCURSOR OnQueryDragIcon ();
	DECLARE_MESSAGE_MAP ()
public:
	std::deque <std::deque <double> > m_deq2DOutputValue;	//�s��output value��deque
	BOOL m_bSimulate;										//�P�_�����Ҧ�
	int m_iStep;											//�����i��B��

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
