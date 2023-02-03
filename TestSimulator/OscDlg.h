
// OscDlg.h : ���Y��
//

#pragma once

#include "afxwin.h"
#include <deque>

class CTestSimulatorDlg;

// COscDlg ��ܤ��

class COscDlg : public CDialogEx
{
public:
	COscDlg (CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~COscDlg ();

// ��ܤ�����
	enum { IDD = IDD_DIALOG_OSC };

private:
	CTestSimulatorDlg* m_pSimulatorDlg;	//�D��ܲ�����

	CComboBox m_ctrlComboOutput;		//Output combo box�ܼ�
	CComboBox m_ctrlComboYInterval;		//Y�b���j combo box�ܼ�
	CComboBox m_ctrlComboXInterval;		//X�b���j combo box�ܼ�

	std::deque <CPen*> m_deqPenPtr;		//�H���e��deque

	double m_dTime;						//�����ɶ�
	double m_dSlctOutputValue;			//���output value
	double m_dMaxDataValue;				//�ܪi����̤ܳj��
	double m_dFreq;						//�W�v

	int m_iOutputSize;					//channel�ƥ�
	double m_dYInterval;				//Y�b���j
	int m_iXInterval;					//X�b���j
	int m_iYPos;						//Y�b���j combo box��m
	int m_iXPos;						//X�b���j combo box��m

protected:
	HICON m_hIcon;

	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP ()
public:
	CRect m_rcWork;						//�ܪi����ܲ��u�@��

	int m_iSlctOutputNum;				//���output�s��

	BOOL m_bSim;						//�P�_����
	BOOL m_bModify;						//�P�_combo box���
	BOOL m_bFFT;						//�P�_�i��FFT

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
