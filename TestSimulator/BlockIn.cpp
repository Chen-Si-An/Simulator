#include "stdafx.h"

#include "BlockIn.h"

#include "math.h"

#define PIN_RADIUS 8			//Pin�b�|

#define GOLD RGB (255, 215, 0)	//Pin�}�C��

#define NO_VALUE -10			//���s�b��J�T��
#define NO_INPUT_FLAG -12		//���s�b��J�i��

#define ZERO 0					//FALSE flag
#define ONE  1					//TRUE flag
#define SIN  2					//Sin flag
#define COS  3					//Cos flag

#define DIGITAL_VALUE 4			//�Ʀ�T��flag
#define ANALOG_VALUE 5			//����T��flag

#define BLK_IN 1				//Block�����s��

CBlockIn::CBlockIn ()
{
	m_rcPinOut = CRect ();
	m_iInputFlag = NO_INPUT_FLAG;
	m_dBlkValue = NO_VALUE;
}

CBlockIn::~CBlockIn ()
{

}

void CBlockIn::SetBlkRect (CPoint ptCenter, int iWidth, int iHeight)
{
	CBlockBasis::SetBlkRect (ptCenter, iWidth, iHeight);

	m_rcPinOut.left = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinOut.top = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) - PIN_RADIUS;
	m_rcPinOut.right = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinOut.bottom = m_rcBlk.CenterPoint ().y + int (m_rcBlk.Height () / 2) + PIN_RADIUS;
}

CRect CBlockIn::GetPinOutRect () const
{
	return m_rcPinOut;
}

int CBlockIn::GetInputFlag() const
{
	return m_iInputFlag;
}

void CBlockIn::SetInputFlag(int iInputFlag)
{
	m_iInputFlag = iInputFlag;
}

double CBlockIn::GetBlkValue () const
{
	return m_dBlkValue;
}

//�ΥH�]�w���w�ɶ��I�UBlock�� (Sin, Cos)
void CBlockIn::SetBlkValue (double dTime)
{
	switch (m_iInputFlag)
	{
	case ZERO:
		m_dBlkValue = 0.;
		break;
	case ONE:
		m_dBlkValue = 1.;
		break;
	case SIN:
		m_dBlkValue = sin (dTime);
		break;
	case COS:
		m_dBlkValue = cos (dTime);
		break;
	default:
		m_dBlkValue = NO_VALUE;
		break;
	}
}

//�]�wBlock�� (TRUE, FALSE)
void CBlockIn::SetBlkValue ()
{
	switch (m_iInputFlag)
	{
	case ZERO:
		m_dBlkValue = 0.;
		break;
	case ONE:
		m_dBlkValue = 1.;
		break;
	default:
		m_dBlkValue = NO_VALUE;
		break;
	}
}

int CBlockIn::GetValueFlag () const
{
	switch (m_iInputFlag)
	{
	case ZERO:					//��J�i�ά�TRUE�ɡA�^�ǼƦ�T��
		return DIGITAL_VALUE;
		break;
	case ONE:					//��J�i�ά�FALSE�ɡA�^�ǼƦ�T��
		return DIGITAL_VALUE;
		break;
	case SIN:					//��J�i�ά�SIN�ɡA�^������T��
		return ANALOG_VALUE;
		break;
	case COS:					//��J�i�ά�COS�ɡA�^������T��
		return ANALOG_VALUE;
		break;
	default:
		return NO_VALUE;
		break;
	}
}

void CBlockIn::Move (CPoint ptCursor)
{
	CBlockBasis::Move (ptCursor);

	m_rcPinOut.left   = m_rcBlk.CenterPoint ().x - PIN_RADIUS;
	m_rcPinOut.top    = m_rcBlk.bottom - PIN_RADIUS;
	m_rcPinOut.right  = m_rcBlk.CenterPoint ().x + PIN_RADIUS;
	m_rcPinOut.bottom = m_rcBlk.bottom + PIN_RADIUS;
}

void CBlockIn::Draw (CDC* pDC)
{
	CBrush brushPin (GOLD), * pbrushOld;
	pbrushOld = pDC->SelectObject (&brushPin);
	pDC->Ellipse (m_rcPinOut);
	pDC->SelectObject (pbrushOld);
	
	CBlockBasis::Draw (pDC);
	pDC->DrawTextA (_T ("IN"), -1, &m_rcBlk,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

int CBlockIn::BlkTypeIs () const
{
	return BLK_IN;
}