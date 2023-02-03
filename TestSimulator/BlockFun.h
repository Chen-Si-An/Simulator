#pragma once

//Block衍生類別：Block Fun

#include "BlockBasis.h"

class CBlockFun : public CBlockBasis
{
private:
	//Block rect(存在兩個輸入Pin，一個輸出Pin)
	CRect m_rcPinIn1;
	CRect m_rcPinIn2;
	CRect m_rcPinOut;

	//連接指標(存在兩個連接指標)
	CBlockBasis* m_pBlkHead1;
	CBlockBasis* m_pBlkHead2;

	//連接Block編號(存在兩個編號)
	int m_iBlkHead1Num;
	int m_iBlkHead2Num;

	//運算元flag
	int m_iOperFlag;

public:
	//建構、解構子
	CBlockFun ();
	virtual ~CBlockFun ();

	//取得、設定rect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinIn1Rect () const;
	virtual CRect GetPinIn2Rect () const;
	virtual CRect GetPinOutRect () const;

	//取得、設定連接至Block的指標
	virtual CBlockBasis* GetBlkHead1Ptr () const;
	virtual CBlockBasis* GetBlkHead2Ptr () const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);
	virtual void SetBlkHead2Ptr (CBlockBasis*);

	//取得、設定連接至Block的編號
	virtual int GetBlkHead1Num () const;
	virtual int GetBlkHead2Num () const;
	virtual void SetBlkHead1Num (int) ;
	virtual void SetBlkHead2Num (int) ;

	//取得、設定運算元flag
	virtual int GetOperFlag () const;
	virtual void SetOperFlag (int);
	
	virtual double GetBlkValue () const;	//取得Block值
	virtual int GetValueFlag () const;		//取得Value旗標

	virtual void Move (CPoint);		//移動Block
	virtual void Draw (CDC*);		//繪製Block
	virtual void DrawLine (CDC*);	//繪製Block連接線

	//判斷Block種類
	virtual int BlkTypeIs () const;
};