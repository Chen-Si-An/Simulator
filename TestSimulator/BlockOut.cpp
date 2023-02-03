#include "stdafx.h"

#include "BlockOut.h"

#define PIN_RADIUS 8			//Pin�b�|

#define GOLD RGB (255, 215, 0)	//Pin�}�C��
#define BLACK RGB (0, 0, 0)		//�s�u�C��

#define NO_VALUE -10			//���s�b��J�T��
#define NO_BLOCK_HEAD -14		//���s�b�s��Block

#define BLK_OUT 2				//Block�����s��

CBlockOut::CBlockOut ()
{
	m_rcPinIn = CRect ();

	m_iBlkHeadSize = (int) m_vcBlkHeadPtr.size ();
}

CBlockOut::~CBlockOut ()
{
}

void CBlockOut::SetBlkRect (CPoint ptCenter, int iWidth, int iHeight)
{
	CBlockBasis::SetBlkRect (ptCenter, iWidth, iHeight);

	m_rcPinIn.left   = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinIn.top    = m_rcBlk.top - PIN_RADIUS;
	m_rcPinIn.right  = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinIn.bottom = m_rcBlk.top + PIN_RADIUS;
}

CRect CBlockOut::GetPinIn1Rect () const
{
	return m_rcPinIn;
}

CBlockBasis* CBlockOut::GetBlkHead1Ptr (int iIndex) const
{
	//�^�ǫ��windex���s������
	if (iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadPtr[iIndex];
	else 
		return NULL;
}

void CBlockOut::SetBlkHead1Ptr (CBlockBasis* pBlkHead)
{
	m_vcBlkHeadPtr.push_back (pBlkHead);	//�s�W�s������
}

void CBlockOut::DeleteBlkHead1Ptr (int iIndex)
{
	m_vcBlkHeadPtr.erase (m_vcBlkHeadPtr.begin () + iIndex);	//�M�����windex���s������
}

int CBlockOut::GetBlkHead1Num (int iIndex) const 
{
	//�^�ǫ��windex���s��Block�s��
	if (iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadNum[iIndex];
	else
		return NO_BLOCK_HEAD;
}

void CBlockOut::SetBlkHead1Num (int iHeadBlkNum)  
{
	m_vcBlkHeadNum.push_back (iHeadBlkNum);		//�s�W�s��Block�s��
}

void CBlockOut::ClearBlkHead1Num ()
{
	std::vector <int> ().swap (m_vcBlkHeadNum);	//�M�ųs���s��vector
}

int CBlockOut::GetBlkHeadSize () const 
{
	return m_iBlkHeadSize;						//�^�ǿ�Jchannel�ƥ�
}

void CBlockOut::SetBlkHeadSize (int iFlag) 
{
	//�]�w��Jchannel�ƥ�
	if (iFlag == FALSE)									//��flag��FALSE�ɡA�̳s���s��vector�j�p�]�w
		m_iBlkHeadSize = (int) m_vcBlkHeadNum.size ();
	else if (iFlag == TRUE)								//��flag��TRUE�ɡA�̳s������vector�j�p�]�w
		m_iBlkHeadSize = (int) m_vcBlkHeadPtr.size ();
}

double CBlockOut::GetBlkValue (int iIndex) const
{
	//���o���wchannel��Block��
	if (!m_vcBlkHeadPtr.empty () && iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadPtr[iIndex]->GetBlkValue ();
	else 
		return NO_VALUE;
}

int CBlockOut::GetValueFlag (int iIndex) const
{
	//���o���wchannel��Value�X��
	if (!m_vcBlkHeadPtr.empty () && iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadPtr[iIndex]->GetValueFlag ();
	else 
		return NO_VALUE;
}

void CBlockOut::Move (CPoint ptCursor)
{
	CBlockBasis::Move (ptCursor);

	m_rcPinIn.left   = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinIn.top    = m_rcBlk.top - PIN_RADIUS;
	m_rcPinIn.right  = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinIn.bottom = m_rcBlk.top + PIN_RADIUS;
}

void CBlockOut::Draw (CDC* pDC)
{
	CBrush brushPin (GOLD), * pbrushOld;
	pbrushOld = pDC->SelectObject (&brushPin);
	pDC->Ellipse (m_rcPinIn);
	pDC->SelectObject (pbrushOld);
	
	CBlockBasis::Draw (pDC);
	pDC->DrawTextA (_T ("OUT"), -1, &m_rcBlk,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CBlockOut::DrawLine (CDC* pDC)
{
	if (!m_vcBlkHeadPtr.empty ())
	{
		CPen penLine (PS_SOLID, 5, BLACK);
		CPen* pOldPen;

		pOldPen = pDC->SelectObject (&penLine);

		for (int i = 0; i < m_iBlkHeadSize; i++)
		{
			CPoint ptStart = m_vcBlkHeadPtr[i]->GetPinOutRect ().CenterPoint ();				//�s�u�_�I����JBlock����XPin�}�����I
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
		}
		
		pDC->SelectObject (pOldPen);
	}	
}

int CBlockOut::BlkTypeIs () const
{
	return BLK_OUT;
}