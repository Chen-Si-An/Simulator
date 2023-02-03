#include "stdafx.h"

#include "BlockAnd.h"

#include "math.h"

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

#define BLK_AND 3				//Block�����s��

CBlockAnd::CBlockAnd ()
{
	m_rcPinIn1 = CRect ();
	m_rcPinIn2 = CRect ();
	m_rcPinOut = CRect ();

	m_pBlkHead1 = NULL;
	m_pBlkHead2 = NULL;

	m_iBlkHead1Num = NO_BLOCK_HEAD;
	m_iBlkHead2Num = NO_BLOCK_HEAD;
}

CBlockAnd::~CBlockAnd ()
{
}

void CBlockAnd::SetBlkRect (CPoint ptCenter, int iWidth, int iHeight)
{
	CBlockBasis::SetBlkRect (ptCenter, iWidth, iHeight);

	m_rcPinIn1.left = m_rcBlk.CenterPoint ().x - int (m_rcBlk.Width () / 4) - PIN_RADIUS;
	m_rcPinIn1.top = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinIn1.right = m_rcBlk.CenterPoint ().x - int (m_rcBlk.Width () / 4) + PIN_RADIUS;
	m_rcPinIn1.bottom = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) + PIN_RADIUS;

	m_rcPinIn2.left = m_rcBlk.CenterPoint ().x + int (m_rcBlk.Width () / 4) - PIN_RADIUS;
	m_rcPinIn2.top = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinIn2.right = m_rcBlk.CenterPoint ().x + int (m_rcBlk.Width () / 4) + PIN_RADIUS;
	m_rcPinIn2.bottom = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) + PIN_RADIUS;

	m_rcPinOut.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinOut.top = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinOut.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinOut.bottom = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) + PIN_RADIUS;
}

CRect CBlockAnd::GetPinIn1Rect () const 
{
	return 	m_rcPinIn1;
}

CRect CBlockAnd::GetPinIn2Rect () const 
{
	return 	m_rcPinIn2;
}

CRect CBlockAnd::GetPinOutRect () const 
{
	return 	m_rcPinOut;
}

CBlockBasis* CBlockAnd::GetBlkHead1Ptr () const
{
	return m_pBlkHead1;
}

CBlockBasis* CBlockAnd::GetBlkHead2Ptr () const
{
	return m_pBlkHead2;
}

void CBlockAnd::SetBlkHead1Ptr (CBlockBasis* pBlkHead)
{
	m_pBlkHead1 = pBlkHead;
}

void CBlockAnd::SetBlkHead2Ptr (CBlockBasis* pBlkHead)
{
	m_pBlkHead2 = pBlkHead;
}

int CBlockAnd::GetBlkHead1Num () const 
{
	return m_iBlkHead1Num;
}

int CBlockAnd::GetBlkHead2Num () const 
{
	return m_iBlkHead2Num;
}

void CBlockAnd::SetBlkHead1Num (int iHeadBlkNum)  
{
	m_iBlkHead1Num = iHeadBlkNum;
}

void CBlockAnd::SetBlkHead2Num (int iHeadBlkNum)  
{
	m_iBlkHead2Num = iHeadBlkNum;
}

double CBlockAnd::GetBlkValue () const
{
	//And�޿� (�Y���J�Ҭ�1�A�h�^��1�A�_�h�^��0)
	if (abs (m_pBlkHead1->GetBlkValue () - 1) < TOLERANCE && abs (m_pBlkHead2->GetBlkValue () - 1) < TOLERANCE)
		return 1.;	
	else
		return 0.;
}

int CBlockAnd::GetValueFlag () const
{
	if (m_pBlkHead1 == NULL || m_pBlkHead2 == NULL)																//�Y�s�b���@��J���I���s���A�^��"�L��"										
		return NO_VALUE;
	else if (m_pBlkHead1->GetValueFlag () == NO_VALUE || m_pBlkHead2->GetValueFlag () == NO_VALUE)				//�Y�s�b���@��J���L�ȡA�^��"�L��"
		return NO_VALUE;
	else if (m_pBlkHead1->GetValueFlag () == COMPUTE_ERROR || m_pBlkHead2->GetValueFlag () == COMPUTE_ERROR)	//�Y�s�b���@��J���p����~�A�^��"�p����~"	
		return COMPUTE_ERROR;
	else if (m_pBlkHead1->GetValueFlag () == ANALOG_VALUE || m_pBlkHead2->GetValueFlag () == ANALOG_VALUE)		//�����\�����J
		return COMPUTE_ERROR;
	else if (m_pBlkHead1->GetValueFlag () == NO_OPER_FLAG || m_pBlkHead2->GetValueFlag () == NO_OPER_FLAG)		//���]�w�B�⤸
		return NO_OPER_FLAG;
	else if (m_pBlkHead1->GetValueFlag () == DIGITAL_VALUE && m_pBlkHead2->GetValueFlag () == DIGITAL_VALUE)	//�Y��J�Ҭ��Ʀ�T���A�h�^�ǼƦ�T��
		return DIGITAL_VALUE;
	else 
		return NO_VALUE;
}

void CBlockAnd::Move (CPoint ptCursor)
{
	CBlockBasis::Move (ptCursor);

	m_rcPinIn1.left = m_rcBlk.CenterPoint ().x - int (m_rcBlk.Width () / 4) - PIN_RADIUS;
	m_rcPinIn1.top = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinIn1.right = m_rcBlk.CenterPoint ().x - int (m_rcBlk.Width () / 4) + PIN_RADIUS;
	m_rcPinIn1.bottom = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) + PIN_RADIUS;

	m_rcPinIn2.left = m_rcBlk.CenterPoint ().x + int (m_rcBlk.Width () / 4) - PIN_RADIUS;
	m_rcPinIn2.top = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinIn2.right = m_rcBlk.CenterPoint ().x + int (m_rcBlk.Width () / 4) + PIN_RADIUS;
	m_rcPinIn2.bottom = m_rcBlk.CenterPoint ().y - int (m_rcBlk.Height () / 2) + PIN_RADIUS;

	m_rcPinOut.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinOut.top = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinOut.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinOut.bottom = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) + PIN_RADIUS;
}

void CBlockAnd::Draw (CDC* pDC)
{
	CBrush brushPin (GOLD), * pbrushOld;
	pbrushOld = pDC->SelectObject (&brushPin);
	pDC->Ellipse (m_rcPinIn1);
	pDC->Ellipse (m_rcPinIn2);
	pDC->Ellipse (m_rcPinOut);
	pDC->SelectObject (pbrushOld);
	
	CBlockBasis::Draw (pDC);
	pDC->DrawTextA (_T ("AND"), -1, &m_rcBlk,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CBlockAnd::DrawLine (CDC* pDC)
{
	if (m_pBlkHead1 != NULL)
	{
		CPen penLine (PS_SOLID, 5, BLACK);
		CPen* pOldPen;

		pOldPen = pDC->SelectObject (&penLine);

		CPoint ptStart = m_pBlkHead1->GetPinOutRect ().CenterPoint ();						//�s�u�_�I����JBlock����XPin�}�����I
		CPoint ptEnd = m_rcPinIn1.CenterPoint ();											//�s�u���I����Block����JPin�}�����I
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

	if (m_pBlkHead2 != NULL)
	{
		CPen penLine (PS_SOLID, 5, BLACK);
		CPen* pOldPen;

		pOldPen = pDC->SelectObject (&penLine);
		
		CPoint ptStart = m_pBlkHead2->GetPinOutRect ().CenterPoint ();						//�s�u�_�I����JBlock����XPin�}�����I				
		CPoint ptEnd = m_rcPinIn2.CenterPoint ();											//�s�u���I����Block����JPin�}�����I
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

int CBlockAnd::BlkTypeIs () const
{
	return BLK_AND;
}