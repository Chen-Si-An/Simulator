#pragma once

//Block衍生類別：Block In

#include "BlockBasis.h"

class CBlockIn : public CBlockBasis
{
private:
	//Block rect(存在一個輸出Pin)
	CRect m_rcPinOut;

	//輸入波形flag
	int m_iInputFlag;

	//Block值
	double m_dBlkValue;

public:
	//建構、解構子
	CBlockIn ();
	virtual ~CBlockIn ();

	//取得、設定rect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinOutRect () const;

	//取得、設定輸入波形flag
	virtual int GetInputFlag () const;
	virtual void SetInputFlag (int);

	//取得、設定Block值
	virtual double GetBlkValue () const;
	virtual void SetBlkValue (double);
	virtual void SetBlkValue ();
	virtual int GetValueFlag () const;	//取得Value旗標

	virtual void Move (CPoint);	//移動Block
	virtual	void Draw (CDC*);	//繪製Block

	//判斷Block種類
	virtual int BlkTypeIs () const;
};