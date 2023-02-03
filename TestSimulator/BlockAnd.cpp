#include "stdafx.h"

#include "BlockAnd.h"

#include "math.h"

#define PIN_RADIUS 8			//Pin半徑

#define GOLD RGB (255, 215, 0)	//Pin腳顏色
#define BLACK RGB (0, 0, 0)		//連線顏色

#define NO_VALUE -10			//不存在輸入訊號
#define NO_OPER_FLAG -11		//不存在運算元
#define COMPUTE_ERROR -13		//不允許的運算
#define NO_BLOCK_HEAD -14		//不存在連接Block

#define TOLERANCE 0.001			//容忍誤差值

#define DIGITAL_VALUE 4			//數位訊號flag
#define ANALOG_VALUE 5			//類比訊號flag

#define BLK_AND 3				//Block種類編號

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
	//And邏輯 (若兩輸入皆為1，則回傳1，否則回傳0)
	if (abs (m_pBlkHead1->GetBlkValue () - 1) < TOLERANCE && abs (m_pBlkHead2->GetBlkValue () - 1) < TOLERANCE)
		return 1.;	
	else
		return 0.;
}

int CBlockAnd::GetValueFlag () const
{
	if (m_pBlkHead1 == NULL || m_pBlkHead2 == NULL)																//若存在任一輸入接點未連接，回傳"無值"										
		return NO_VALUE;
	else if (m_pBlkHead1->GetValueFlag () == NO_VALUE || m_pBlkHead2->GetValueFlag () == NO_VALUE)				//若存在任一輸入為無值，回傳"無值"
		return NO_VALUE;
	else if (m_pBlkHead1->GetValueFlag () == COMPUTE_ERROR || m_pBlkHead2->GetValueFlag () == COMPUTE_ERROR)	//若存在任一輸入為計算錯誤，回傳"計算錯誤"	
		return COMPUTE_ERROR;
	else if (m_pBlkHead1->GetValueFlag () == ANALOG_VALUE || m_pBlkHead2->GetValueFlag () == ANALOG_VALUE)		//不允許類比輸入
		return COMPUTE_ERROR;
	else if (m_pBlkHead1->GetValueFlag () == NO_OPER_FLAG || m_pBlkHead2->GetValueFlag () == NO_OPER_FLAG)		//未設定運算元
		return NO_OPER_FLAG;
	else if (m_pBlkHead1->GetValueFlag () == DIGITAL_VALUE && m_pBlkHead2->GetValueFlag () == DIGITAL_VALUE)	//若輸入皆為數位訊號，則回傳數位訊號
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

		CPoint ptStart = m_pBlkHead1->GetPinOutRect ().CenterPoint ();						//連線起點為輸入Block的輸出Pin腳中心點
		CPoint ptEnd = m_rcPinIn1.CenterPoint ();											//連線終點為此Block的輸入Pin腳中心點
		int iGrid = (int) (m_rcBlk.Height () / 2. + 0.5);									//垂直格點間隔
		int iCornerGridY = (int) (((ptStart.y + ptEnd.y) / 2. - ptStart.y) / iGrid + 0.5);	//計算連線轉折處距離輸入Block幾個隔點
		int iCornerY = iCornerGridY * iGrid + ptStart.y;									//連線轉折處Y座標位置
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
		
		CPoint ptStart = m_pBlkHead2->GetPinOutRect ().CenterPoint ();						//連線起點為輸入Block的輸出Pin腳中心點				
		CPoint ptEnd = m_rcPinIn2.CenterPoint ();											//連線終點為此Block的輸入Pin腳中心點
		int iGrid = (int) (m_rcBlk.Height () / 2. + 0.5);									//垂直格點間隔
		int iCornerGridY = (int) (((ptStart.y + ptEnd.y) / 2. - ptStart.y) / iGrid + 0.5);	//計算連線轉折處距離輸入Block幾個隔點
		int iCornerY = iCornerGridY * iGrid + ptStart.y;									//連線轉折處Y座標位置
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