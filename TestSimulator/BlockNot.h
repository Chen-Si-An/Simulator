#pragma once

//Block�l�����O�GBlock Not

#include "BlockBasis.h"

class CBlockNot : public CBlockBasis
{
private:
	//Block rect(�s�b�@�ӿ�JPin�A�@�ӿ�XPin)
	CRect m_rcPinIn;
	CRect m_rcPinOut;

	//�s������
	CBlockBasis* m_pBlkHead;

	//�s��Block�s��
	int m_iBlkHeadNum;

public:
	//�غc�B�Ѻc�l
	CBlockNot ();
	virtual ~CBlockNot ();

	//���o�B�]�wrect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinIn1Rect () const;
	virtual CRect GetPinOutRect () const;

	//���o�B�]�w�s����Block������
	virtual CBlockBasis* GetBlkHead1Ptr () const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);

	//���o�B�]�w�s����Block���s��
	virtual int GetBlkHead1Num () const;
	virtual void SetBlkHead1Num (int) ;	
	
	virtual double GetBlkValue () const;	//���oBlock��
	virtual int GetValueFlag () const;		//���oValue�X��

	virtual void Move (CPoint);		//����Block
	virtual	void Draw (CDC*);		//ø�sBlock
	virtual	void DrawLine (CDC*);	//ø�sBlock�s���u

	//�P�_Block����
	virtual int BlkTypeIs () const;
};