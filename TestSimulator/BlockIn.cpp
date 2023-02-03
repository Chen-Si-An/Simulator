#include "stdafx.h"

#include "BlockIn.h"

#include "math.h"

#define PIN_RADIUS 8			//Pin半徑

#define GOLD RGB (255, 215, 0)	//Pin腳顏色

#define NO_VALUE -10			//不存在輸入訊號
#define NO_INPUT_FLAG -12		//不存在輸入波形

#define ZERO 0					//FALSE flag
#define ONE  1					//TRUE flag
#define SIN  2					//Sin flag
#define COS  3					//Cos flag

#define DIGITAL_VALUE 4			//數位訊號flag
#define ANALOG_VALUE 5			//類比訊號flag

#define BLK_IN 1				//Block種類編號

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

//用以設定指定時間點下Block值 (Sin, Cos)
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

//設定Block值 (TRUE, FALSE)
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
	case ZERO:					//輸入波形為TRUE時，回傳數位訊號
		return DIGITAL_VALUE;
		break;
	case ONE:					//輸入波形為FALSE時，回傳數位訊號
		return DIGITAL_VALUE;
		break;
	case SIN:					//輸入波形為SIN時，回傳類比訊號
		return ANALOG_VALUE;
		break;
	case COS:					//輸入波形為COS時，回傳類比訊號
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