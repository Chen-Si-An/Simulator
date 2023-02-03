#include "stdafx.h"

#include "BlockNot.h"

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

#define BLK_NOT 5				//Block種類編號

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
	//Not邏輯 (若輸入為0，則回傳1，反之回傳0)
	if (m_pBlkHead->GetBlkValue () < TOLERANCE)	
		return 1.;
	else								
		return 0.;
}

int CBlockNot::GetValueFlag () const
{
	if (m_pBlkHead == NULL)									//若輸入接點未連接，回傳"無值"								
		return NO_VALUE;
	else if (m_pBlkHead->GetValueFlag () == NO_VALUE)		//若輸入為無值，回傳"無值"
		return NO_VALUE;	
	else if (m_pBlkHead->GetValueFlag () == COMPUTE_ERROR)	//若輸入為計算錯誤，回傳"計算錯誤"	
		return COMPUTE_ERROR;
	else if (m_pBlkHead->GetValueFlag () == ANALOG_VALUE)	//不允許類比輸入
		return COMPUTE_ERROR;
	else if (m_pBlkHead->GetValueFlag () == NO_OPER_FLAG)	//未設定運算元
		return NO_OPER_FLAG;
	else if (m_pBlkHead->GetValueFlag () == DIGITAL_VALUE)	//若輸入為數位訊號，則回傳數位訊號																											                                                                                       
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

		CPoint ptStart = m_pBlkHead->GetPinOutRect ().CenterPoint ();						//連線起點為輸入Block的輸出Pin腳中心點
		CPoint ptEnd = m_rcPinIn.CenterPoint ();											//連線終點為此Block的輸入Pin腳中心點
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

int CBlockNot::BlkTypeIs () const
{
	return BLK_NOT;
}