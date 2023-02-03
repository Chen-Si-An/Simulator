#include "stdafx.h"

#include "BlockFun.h"

#define PIN_RADIUS 8			//Pin半徑

#define GOLD RGB (255, 215, 0)	//Pin腳顏色
#define BLACK RGB (0, 0, 0)		//連線顏色

#define FALSE_VALUE 10			//FALSE回傳值
#define TRUE_VALUE 11			//TRUE回傳值
#define NO_VALUE -10			//不存在輸入訊號
#define NO_OPER_FLAG -11		//不存在運算元
#define NO_INPUT_FLAG -12		//不存在輸入波形
#define COMPUTE_ERROR -13		//不允許的運算
#define NO_BLOCK_HEAD -14		//不存在連接Block

#define PLUS 1					// + flag
#define MINUS 2					// - flag
#define MULTI 3					// * flag
#define DIV 4					// / flag

#define DIGITAL_VALUE 4			//數位訊號flag
#define ANALOG_VALUE 5			//類比訊號flag

#define BLK_FUN 6				//Block種類編號

CBlockFun::CBlockFun ()
{
	m_rcPinIn1 = CRect ();
	m_rcPinIn2 = CRect ();
	m_rcPinOut = CRect ();

	m_pBlkHead1 = NULL;
	m_pBlkHead2 = NULL;

	m_iBlkHead1Num = NO_BLOCK_HEAD;
	m_iBlkHead2Num = NO_BLOCK_HEAD;

	m_iOperFlag = NO_OPER_FLAG;
}

CBlockFun::~CBlockFun ()
{
}

void CBlockFun::SetBlkRect (CPoint ptCenter, int iWidth, int iHeight)
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

CRect CBlockFun::GetPinIn1Rect () const
{
	return m_rcPinIn1;
}

CRect CBlockFun::GetPinIn2Rect () const
{
	return m_rcPinIn2;
}

CRect CBlockFun::GetPinOutRect () const
{
	return m_rcPinOut;
}

CBlockBasis* CBlockFun::GetBlkHead1Ptr () const
{
	return m_pBlkHead1;
}

CBlockBasis* CBlockFun::GetBlkHead2Ptr () const
{
	return m_pBlkHead2;
}

void CBlockFun::SetBlkHead1Ptr (CBlockBasis* pBlkHead1) 
{
	 m_pBlkHead1 = pBlkHead1;
}

void CBlockFun::SetBlkHead2Ptr (CBlockBasis* pBlkHead2) 
{
	 m_pBlkHead2 = pBlkHead2;
}

int CBlockFun::GetBlkHead1Num () const 
{
	return m_iBlkHead1Num;
}

int CBlockFun::GetBlkHead2Num () const 
{
	return m_iBlkHead2Num;
}

void CBlockFun::SetBlkHead1Num (int iHeadBlkNum)  
{
	m_iBlkHead1Num = iHeadBlkNum;
}

void CBlockFun::SetBlkHead2Num (int iHeadBlkNum)  
{
	m_iBlkHead2Num = iHeadBlkNum;
}

double CBlockFun::GetBlkValue () const
{
	switch (m_iOperFlag)
	{
	case PLUS:																	//將兩輸入值相加
		return m_pBlkHead1->GetBlkValue () + m_pBlkHead2->GetBlkValue ();
		break;
	case MINUS:																	//將兩輸入值相減
		return m_pBlkHead1->GetBlkValue () - m_pBlkHead2->GetBlkValue ();
		break;
	case MULTI:																	//將兩輸入值相乘
		return m_pBlkHead1->GetBlkValue () * m_pBlkHead2->GetBlkValue ();
		break;
	case DIV:																	//將兩輸入值相除
		return m_pBlkHead1->GetBlkValue () / m_pBlkHead2->GetBlkValue ();
		break;
	default:
		return NO_VALUE;
		break;
	}
}

int CBlockFun::GetValueFlag () const
{
	if (m_pBlkHead1 == NULL || m_pBlkHead2 == NULL)																//若存在任一輸入接點未連接，回傳"無值"										
		return NO_VALUE;
	else if (m_pBlkHead1->GetValueFlag () == NO_VALUE || m_pBlkHead2->GetValueFlag () == NO_VALUE)				//若存在任一輸入為無值，回傳"無值"
		return NO_VALUE;
	else if (m_pBlkHead1->GetValueFlag () == COMPUTE_ERROR || m_pBlkHead2->GetValueFlag () == COMPUTE_ERROR)	//若存在任一輸入為計算錯誤，回傳"計算錯誤"	
		return COMPUTE_ERROR;
	else if (m_pBlkHead1->GetValueFlag () == DIGITAL_VALUE || m_pBlkHead2->GetValueFlag () == DIGITAL_VALUE)	//不允許數位輸入
		return COMPUTE_ERROR;
	else if (m_iOperFlag == NO_OPER_FLAG)																		//未設定運算元
		return NO_OPER_FLAG;
	else if (m_pBlkHead1->GetValueFlag () == NO_OPER_FLAG || m_pBlkHead2->GetValueFlag () == NO_OPER_FLAG)		//未設定運算元
		return NO_OPER_FLAG;
	else if (m_pBlkHead1->GetValueFlag () == ANALOG_VALUE && m_pBlkHead2->GetValueFlag () == ANALOG_VALUE)		//若輸入皆為類比訊號，則回傳類比訊號																							                                                                                       
		return ANALOG_VALUE;
	else
		return NO_VALUE;
}

int CBlockFun::GetOperFlag () const
{
	return m_iOperFlag;
}

void CBlockFun::SetOperFlag (int iOperFlag)
{
	m_iOperFlag = iOperFlag;
}

void CBlockFun::Move (CPoint ptCursor)
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

void CBlockFun::Draw (CDC* pDC)
{
	CBrush brushPin (GOLD), * pOldBrush;
	pOldBrush = pDC->SelectObject (&brushPin);
	pDC->Ellipse (m_rcPinIn1);
	pDC->Ellipse (m_rcPinIn2);
	pDC->Ellipse (m_rcPinOut);
	pDC->SelectObject (pOldBrush);

	CBlockBasis::Draw (pDC);
	pDC->DrawTextA (_T ("FUN"), -1, &m_rcBlk,  DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CBlockFun::DrawLine (CDC* pDC)
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

int CBlockFun::BlkTypeIs () const
{
	return BLK_FUN;
}