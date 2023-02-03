#pragma once

//Block�l�����O�GBlock Out

#include "BlockBasis.h"

#include <vector>

class CBlockOut : public CBlockBasis
{
private:
	//Block rect(�s�b�@�ӿ�JPin)
	CRect m_rcPinIn;

	//�s������vector(�s�b�h�ӿ�Jchannel)
	std::vector <CBlockBasis*> m_vcBlkHeadPtr;

	//�s���s��vector(�s�b�h�ӿ�Jchannel)
	std::vector <int> m_vcBlkHeadNum;

	//��Jchannel�ƥ�
	int m_iBlkHeadSize;


public:
	//�غc�B�Ѻc�l
	CBlockOut ();
	virtual ~CBlockOut ();

	//���o�B�]�wrect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinIn1Rect () const;

	//���o�B�]�w�s����Block������
	virtual CBlockBasis* GetBlkHead1Ptr (int) const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);
	virtual void DeleteBlkHead1Ptr (int);

	//���o�B�]�w�s����Block���s��
	virtual int GetBlkHead1Num (int) const;
	virtual void SetBlkHead1Num (int) ;
	virtual void ClearBlkHead1Num ();

	//���o�B�]�w��Jchannel�ƥ�
	virtual int GetBlkHeadSize () const;
	virtual void SetBlkHeadSize (int);

	virtual double GetBlkValue (int) const;	//���oBlock��
	virtual int GetValueFlag (int) const;	//���oValue�X��

	virtual void Move (CPoint);		//����Block
	virtual	void Draw (CDC*);		//ø�sBlock
	virtual void DrawLine (CDC*);	//ø�sBlock�s���u

	//�P�_Block����
	virtual int BlkTypeIs () const;
};