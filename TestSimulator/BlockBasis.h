#pragma once

//Block基礎類別

class CBlockBasis
{
protected:
	CRect m_rcBlk;	//Block rect

	int m_iBlkNum;	//Block於vector中的編號

public:
	//建構、解構子
	CBlockBasis ();		
	virtual ~CBlockBasis ();	

	//取得、設定rect
	CRect GetBlkRect () const;	
	virtual void SetBlkRect (CPoint, int, int);	
	virtual CRect GetPinIn1Rect () const;		
	virtual CRect GetPinIn2Rect () const;		
	virtual CRect GetPinOutRect () const;		

	//取得、設定Block編號
	int GetBlkNum () const;	
	void SetBlkNum (int);	

	//取得、設定連接至Block的指標
	virtual CBlockBasis* GetBlkHead1Ptr () const;	
	virtual CBlockBasis* GetBlkHead1Ptr (int) const;
	virtual CBlockBasis* GetBlkHead2Ptr () const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);
	virtual void SetBlkHead2Ptr (CBlockBasis*);
	virtual void DeleteBlkHead1Ptr (int);	//自Block Out連接指標vector中清除指定的指標

	//取得、設定連接至Block的編號
	virtual int GetBlkHead1Num () const;
	virtual int GetBlkHead1Num (int) const;
	virtual int GetBlkHead2Num () const;
	virtual void SetBlkHead1Num (int) ;
	virtual void SetBlkHead2Num (int) ;
	virtual void ClearBlkHead1Num ();		//清空Block Out連接編號vector

	//取得、設定連接Block Out的數目
	virtual int GetBlkHeadSize () const;
	virtual void SetBlkHeadSize (int);

	//取得、設定Block In, Fun的flag
	virtual int GetInputFlag () const;
	virtual void SetInputFlag (int);
	virtual int GetOperFlag () const;
	virtual void SetOperFlag (int);

	//取得、設定Block值
	virtual double GetBlkValue () const;
	virtual double GetBlkValue (int) const;	//取得Block Out指定channel的值
	virtual void SetBlkValue (double);
	virtual void SetBlkValue ();
	virtual int GetValueFlag () const;
	virtual int GetValueFlag (int) const;

	virtual void Move (CPoint);		//移動Block
	virtual void Draw (CDC*);		//繪製Block
	virtual void DrawLine (CDC*);	//繪製Block連接線

	//判斷Block種類
	virtual int BlkTypeIs () const;
};