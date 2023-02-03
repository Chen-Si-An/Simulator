#pragma once

//Block衍生類別：Block Out

#include "BlockBasis.h"

#include <vector>

class CBlockOut : public CBlockBasis
{
private:
	//Block rect(存在一個輸入Pin)
	CRect m_rcPinIn;

	//連接指標vector(存在多個輸入channel)
	std::vector <CBlockBasis*> m_vcBlkHeadPtr;

	//連接編號vector(存在多個輸入channel)
	std::vector <int> m_vcBlkHeadNum;

	//輸入channel數目
	int m_iBlkHeadSize;


public:
	//建構、解構子
	CBlockOut ();
	virtual ~CBlockOut ();

	//取得、設定rect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinIn1Rect () const;

	//取得、設定連接至Block的指標
	virtual CBlockBasis* GetBlkHead1Ptr (int) const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);
	virtual void DeleteBlkHead1Ptr (int);

	//取得、設定連接至Block的編號
	virtual int GetBlkHead1Num (int) const;
	virtual void SetBlkHead1Num (int) ;
	virtual void ClearBlkHead1Num ();

	//取得、設定輸入channel數目
	virtual int GetBlkHeadSize () const;
	virtual void SetBlkHeadSize (int);

	virtual double GetBlkValue (int) const;	//取得Block值
	virtual int GetValueFlag (int) const;	//取得Value旗標

	virtual void Move (CPoint);		//移動Block
	virtual	void Draw (CDC*);		//繪製Block
	virtual void DrawLine (CDC*);	//繪製Block連接線

	//判斷Block種類
	virtual int BlkTypeIs () const;
};