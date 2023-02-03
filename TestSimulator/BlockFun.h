#pragma once

//Block�l�����O�GBlock Fun

#include "BlockBasis.h"

class CBlockFun : public CBlockBasis
{
private:
	//Block rect(�s�b��ӿ�JPin�A�@�ӿ�XPin)
	CRect m_rcPinIn1;
	CRect m_rcPinIn2;
	CRect m_rcPinOut;

	//�s������(�s�b��ӳs������)
	CBlockBasis* m_pBlkHead1;
	CBlockBasis* m_pBlkHead2;

	//�s��Block�s��(�s�b��ӽs��)
	int m_iBlkHead1Num;
	int m_iBlkHead2Num;

	//�B�⤸flag
	int m_iOperFlag;

public:
	//�غc�B�Ѻc�l
	CBlockFun ();
	virtual ~CBlockFun ();

	//���o�B�]�wrect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinIn1Rect () const;
	virtual CRect GetPinIn2Rect () const;
	virtual CRect GetPinOutRect () const;

	//���o�B�]�w�s����Block������
	virtual CBlockBasis* GetBlkHead1Ptr () const;
	virtual CBlockBasis* GetBlkHead2Ptr () const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);
	virtual void SetBlkHead2Ptr (CBlockBasis*);

	//���o�B�]�w�s����Block���s��
	virtual int GetBlkHead1Num () const;
	virtual int GetBlkHead2Num () const;
	virtual void SetBlkHead1Num (int) ;
	virtual void SetBlkHead2Num (int) ;

	//���o�B�]�w�B�⤸flag
	virtual int GetOperFlag () const;
	virtual void SetOperFlag (int);
	
	virtual double GetBlkValue () const;	//���oBlock��
	virtual int GetValueFlag () const;		//���oValue�X��

	virtual void Move (CPoint);		//����Block
	virtual void Draw (CDC*);		//ø�sBlock
	virtual void DrawLine (CDC*);	//ø�sBlock�s���u

	//�P�_Block����
	virtual int BlkTypeIs () const;
};