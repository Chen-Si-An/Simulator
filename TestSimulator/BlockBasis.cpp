#include "stdafx.h"

#include "BlockBasis.h"


#define LIGHTGRAY RGB (211, 211, 211)	//Block顏色

#define NO_VALUE -10		//不存在輸入訊號
#define NO_OPER_FLAG -11	//Block Fun不存在運算元
#define NO_INPUT_FLAG -12	//Block In不存在輸入波形
#define NO_BLOCK_HEAD -14	//不存在連接Block

#define NO_BLK_TYPE 0

CBlockBasis::CBlockBasis ()
{
	m_rcBlk = CRect ();
	m_iBlkNum = 0;
}

CBlockBasis::~CBlockBasis ()
{
}

CRect CBlockBasis::GetBlkRect () const
{
	return m_rcBlk;
}

//以中心點、Block寬度、Block高度設定Block rect
void CBlockBasis::SetBlkRect (CPoint ptCenter, int iWidth, int iHeight)
{
	m_rcBlk.SetRect (CPoint (ptCenter.x - int (iWidth / 2), ptCenter.y - int (iHeight / 2)),
					 CPoint (ptCenter.x + int (iWidth / 2), ptCenter.y + int (iHeight / 2)));
}

CRect CBlockBasis::GetPinIn1Rect () const 
{
	return CRect ();
}

CRect CBlockBasis::GetPinIn2Rect () const 
{
	return CRect ();
}

CRect CBlockBasis::GetPinOutRect () const
{
	return CRect ();
}

int CBlockBasis::GetBlkNum () const
{
	return m_iBlkNum;
}

void CBlockBasis::SetBlkNum (int iNum) 
{
	 m_iBlkNum = iNum;
}

CBlockBasis* CBlockBasis::GetBlkHead1Ptr () const
{
	return NULL;
}

CBlockBasis* CBlockBasis::GetBlkHead1Ptr (int iIndex) const
{
	return NULL;
}

CBlockBasis* CBlockBasis::GetBlkHead2Ptr () const
{
	return NULL;
}

void CBlockBasis::SetBlkHead1Ptr (CBlockBasis* pBlkHead)
{
}

void CBlockBasis::SetBlkHead2Ptr (CBlockBasis* pBlkHead)
{
}

void CBlockBasis::DeleteBlkHead1Ptr (int iIndex)
{
}

int CBlockBasis::GetBlkHead1Num () const 
{
	return NO_BLOCK_HEAD;
}

int CBlockBasis::GetBlkHead1Num (int iIndex) const 
{
	return NO_BLOCK_HEAD;
}

int CBlockBasis::GetBlkHead2Num () const 
{
	return NO_BLOCK_HEAD;
}

void CBlockBasis::SetBlkHead1Num (int iHeadBlkNum)  
{
}

void CBlockBasis::SetBlkHead2Num (int iHeadBlkNum)  
{
}

void CBlockBasis::ClearBlkHead1Num ()
{
}

int CBlockBasis::GetBlkHeadSize () const 
{
	return NO_BLOCK_HEAD;
}

void CBlockBasis::SetBlkHeadSize (int) 
{
}

int CBlockBasis::GetInputFlag () const
{
	return NO_INPUT_FLAG;
}

void CBlockBasis::SetInputFlag (int iInputFlag)
{ 
}

int CBlockBasis::GetOperFlag () const
{
	return NO_OPER_FLAG;
}

void CBlockBasis::SetOperFlag (int iInputFlag)
{ 
}

double CBlockBasis::GetBlkValue () const
{
	return NO_VALUE;
}

double CBlockBasis::GetBlkValue (int iIndex) const
{
	return NO_VALUE;
}

void CBlockBasis::SetBlkValue (double dBlkValue)
{
}

void CBlockBasis::SetBlkValue ()
{
}

int CBlockBasis::GetValueFlag () const
{
	return NO_VALUE;
}

int CBlockBasis::GetValueFlag (int iIndex) const
{
	return NO_VALUE;
}

//移動Block時依鼠標位置設定Block rect
void CBlockBasis::Move (CPoint ptCursor)
{
	m_rcBlk.SetRect (CPoint (ptCursor.x - int (m_rcBlk.Width () / 2.), ptCursor.y - int (m_rcBlk.Height () / 2.)),
		CPoint (ptCursor.x + int (m_rcBlk.Width () / 2.), ptCursor.y + int (m_rcBlk.Height () / 2.)));
}

void CBlockBasis::Draw (CDC* pDC)
{
	pDC->FillSolidRect (m_rcBlk, LIGHTGRAY);
	pDC->DrawEdge (m_rcBlk, EDGE_RAISED, BF_RECT);
}

void CBlockBasis::DrawLine (CDC* pDC)
{
}

int CBlockBasis::BlkTypeIs () const
{
	return NO_BLK_TYPE;
}