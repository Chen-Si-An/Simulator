#include "stdafx.h"

#include "BlockNot.h"

#define PIN_RADIUS 8			//Pin�b�|

#define GOLD RGB (255, 215, 0)	//Pin�}�C��
#define BLACK RGB (0, 0, 0)		//�s�u�C��

#define NO_VALUE -10			//���s�b��J�T��
#define NO_OPER_FLAG -11		//���s�b�B�⤸
#define COMPUTE_ERROR -13		//�����\���B��
#define NO_BLOCK_HEAD -14		//���s�b�s��Block

#define TOLERANCE 0.001			//�e�Ի~�t��

#define DIGITAL_VALUE 4			//�Ʀ�T��flag
#define ANALOG_VALUE 5			//����T��flag

#define BLK_NOT 5				//Block�����s��

CBlockNot::CBlockNot ()
{
	m_rcPinIn = CRect ();
	m_rcPinOut = CRect ();

	m_pBlkHead = NULL;

	m_iBlkHeadNum = NO_BLOCK_HEAD;
}

CBlockNot::~CBlockNot ()
{
}

void CBlockNot::SetBlkRect (CPoint ptCenter, int iWidth, int iHeight)
{
	CBlockBasis::SetBlkRect (ptCenter, iWidth, iHeight);

	m_rcPinIn.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinIn.top = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinIn.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinIn.bottom = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) + PIN_RADIUS;

	m_rcPinOut.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinOut.top = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinOut.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinOut.bottom = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) + PIN_RADIUS;
}

CRect CBlockNot::GetPinIn1Rect () const 
{
	return 	m_rcPinIn;
}

CRect CBlockNot::GetPinOutRect () const 
{
	return 	m_rcPinOut;
}

CBlockBasis* CBlockNot::GetBlkHead1Ptr () const
{
	return m_pBlkHead;
}

void CBlockNot::SetBlkHead1Ptr (CBlockBasis* pBlkHead)
{
	m_pBlkHead = pBlkHead;
}

int CBlockNot::GetBlkHead1Num () const 
{
	return m_iBlkHeadNum;
}

void CBlockNot::SetBlkHead1Num (int iHeadBlkNum)  
{
	m_iBlkHeadNum = iHeadBlkNum;
}

double CBlockNot::GetBlkValue () const 
{
	//Not�޿� (�Y��J��0�A�h�^��1�A�Ϥ��^��0)
	if (m_pBlkHead->GetBlkValue () < TOLERANCE)	
		return 1.;
	else								
		return 0.;
}

int CBlockNot::GetValueFlag () const
{
	if (m_pBlkHead == NULL)									//�Y��J���I���s���A�^��"�L��"								
		return NO_VALUE;
	else if (m_pBlkHead->GetValueFlag () == NO_VALUE)		//�Y��J���L�ȡA�^��"�L��"
		return NO_VALUE;	
	else if (m_pBlkHead->GetValueFlag () == COMPUTE_ERROR)	//�Y��J���p����~�A�^��"�p����~"	
		return COMPUTE_ERROR;
	else if (m_pBlkHead->GetValueFlag () == ANALOG_VALUE)	//�����\�����J
		return COMPUTE_ERROR;
	else if (m_pBlkHead->GetValueFlag () == NO_OPER_FLAG)	//���]�w�B�⤸
		return NO_OPER_FLAG;
	else if (m_pBlkHead->GetValueFlag () == DIGITAL_VALUE)	//�Y��J���Ʀ�T���A�h�^�ǼƦ�T��																											                                                                                       
		return DIGITAL_VALUE;
	else
		return NO_VALUE;
}

void CBlockNot::Move (CPoint ptCursor)
{
	CBlockBasis::Move (ptCursor);

	m_rcPinIn.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinIn.top = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinIn.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinIn.bottom = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) + PIN_RADIUS;

	m_rcPinOut.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinOut.top = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinOut.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinOut.bottom = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) + PIN_RADIUS;
}


void CBlockNot::Draw (CDC* pDC)
{
	CBrush brushPin (GOLD), * pbrushOld;
	pbrushOld = pDC->SelectObject (&brushPin);
	pDC->Ellipse (m_rcPinIn);
	pDC->Ellipse (m_rcPinOut);
	pDC->SelectObject (pbrushOld);
	
	CBlockBasis::Draw (pDC);
	pDC->DrawTextA (_T ("NOT"), -1, &m_rcBlk,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CBlockNot::DrawLine (CDC* pDC)
{
	if (m_pBlkHead != NULL)
	{
		CPen penLine (PS_SOLID, 5, BLACK);
		CPen* pOldPen;

		pOldPen = pDC->SelectObject (&penLine);

		CPoint ptStart = m_pBlkHead->GetPinOutRect ().CenterPoint ();						//�s�u�_�I����JBlock����XPin�}�����I
		CPoint ptEnd = m_rcPinIn.CenterPoint ();											//�s�u���I����Block����JPin�}�����I
		int iGrid = (int) (m_rcBlk.Height () / 2. + 0.5);									//�������I���j
		int iCornerGridY = (int) (((ptStart.y + ptEnd.y) / 2. - ptStart.y) / iGrid + 0.5);	//�p��s�u���B�Z����JBlock�X�ӹj�I
		int iCornerY = iCornerGridY * iGrid + ptStart.y;									//�s�u���BY�y�Ц�m
		pDC->MoveTo (ptStart);
		pDC->LineTo (CPoint (ptStart.x, iCornerY));
		pDC->MoveTo (CPoint (ptStart.x, iCornerY));
		pDC->LineTo (CPoint (ptEnd.x, iCornerY));
		pDC->MoveTo (CPoint (ptEnd.x, iCornerY));
		pDC->LineTo (ptEnd);

		pDC->SelectObject (pOldPen);
	}	
}

int CBlockNot::BlkTypeIs () const
{
	return BLK_NOT;
}