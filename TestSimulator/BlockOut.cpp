#include "stdafx.h"

#include "BlockOut.h"

#define PIN_RADIUS 8			//Pin半徑

#define GOLD RGB (255, 215, 0)	//Pin腳顏色
#define BLACK RGB (0, 0, 0)		//連線顏色

#define NO_VALUE -10			//不存在輸入訊號
#define NO_BLOCK_HEAD -14		//不存在連接Block

#define BLK_OUT 2				//Block種類編號

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
	//回傳指定index的連接指標
	if (iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadPtr[iIndex];
	else 
		return NULL;
}

void CBlockOut::SetBlkHead1Ptr (CBlockBasis* pBlkHead)
{
	m_vcBlkHeadPtr.push_back (pBlkHead);	//新增連接指標
}

void CBlockOut::DeleteBlkHead1Ptr (int iIndex)
{
	m_vcBlkHeadPtr.erase (m_vcBlkHeadPtr.begin () + iIndex);	//清除指定index的連接指標
}

int CBlockOut::GetBlkHead1Num (int iIndex) const 
{
	//回傳指定index的連接Block編號
	if (iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadNum[iIndex];
	else
		return NO_BLOCK_HEAD;
}

void CBlockOut::SetBlkHead1Num (int iHeadBlkNum)  
{
	m_vcBlkHeadNum.push_back (iHeadBlkNum);		//新增連接Block編號
}

void CBlockOut::ClearBlkHead1Num ()
{
	std::vector <int> ().swap (m_vcBlkHeadNum);	//清空連接編號vector
}

int CBlockOut::GetBlkHeadSize () const 
{
	return m_iBlkHeadSize;						//回傳輸入channel數目
}

void CBlockOut::SetBlkHeadSize (int iFlag) 
{
	//設定輸入channel數目
	if (iFlag == FALSE)									//當flag為FALSE時，依連接編號vector大小設定
		m_iBlkHeadSize = (int) m_vcBlkHeadNum.size ();
	else if (iFlag == TRUE)								//當flag為TRUE時，依連接指標vector大小設定
		m_iBlkHeadSize = (int) m_vcBlkHeadPtr.size ();
}

double CBlockOut::GetBlkValue (int iIndex) const
{
	//取得指定channel的Block值
	if (!m_vcBlkHeadPtr.empty () && iIndex < m_iBlkHeadSize)
		return m_vcBlkHeadPtr[iIndex]->GetBlkValue ();
	else 
		return NO_VALUE;
}

int CBlockOut::GetValueFlag (int iIndex) const
{
	//取得指定channel的Value旗標
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
			CPoint ptStart = m_vcBlkHeadPtr[i]->GetPinOutRect ().CenterPoint ();				//連線起點為輸入Block的輸出Pin腳中心點
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
		}
		
		pDC->SelectObject (pOldPen);
	}	
}

int CBlockOut::BlkTypeIs () const
{
	return BLK_OUT;
}